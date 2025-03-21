#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "brewerymanager.h"

BreweryManager::BreweryManager(QObject *parent): QObject(parent){
}

void BreweryManager::findLongestName()
{
    _longestNameLength = 0;
    _longestNamesList.clear();
    int page = 1;

    fetchPage(page, [this](QNetworkReply* reply) {
        handleLongestNameResponse(reply);
    });
}

void BreweryManager::findNorthernmostBrewery()
{
    QUrl apiUrl("https://api.openbrewerydb.org/v1/breweries");
    QUrlQuery query;

    query.addQueryItem("by_country", "Ireland");
    query.addQueryItem("by_dist", QString("%1,%2").arg(NORTH_POLE_LATITUDE).arg(PRIME_MERIDIAN));
    query.addQueryItem("per_page", "3");

    apiUrl.setQuery(query);

    QNetworkRequest request(apiUrl);
    QNetworkReply *reply = networkManager.get(request);

    reply->setProperty("type", "northernmost");
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        handleCoordinatesResponse(reply);
    });
}

void BreweryManager::findSouthernmostBrewery()
{
    QUrl apiUrl("https://api.openbrewerydb.org/v1/breweries");
    QUrlQuery query;

    query.addQueryItem("by_country", "Ireland");
    query.addQueryItem("by_dist", QString("%1,%2").arg(SOUTH_POLE_LATITUDE).arg(PRIME_MERIDIAN));
    query.addQueryItem("per_page", "3");

    apiUrl.setQuery(query);

    QNetworkRequest request(apiUrl);
    QNetworkReply *reply = networkManager.get(request);

    reply->setProperty("type", "southernmost");
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        handleCoordinatesResponse(reply);
    });
}

void BreweryManager::fetchPage(int page, std::function<void(QNetworkReply*)> responseHandler)
{
    QUrl apiUrl("https://api.openbrewerydb.org/v1/breweries");
    QUrlQuery query;
    query.addQueryItem("by_country", "Ireland");
    query.addQueryItem("per_page", QString::number(_perPage));
    query.addQueryItem("page", QString::number(page));
    apiUrl.setQuery(query);

    QNetworkRequest request(apiUrl);
    QNetworkReply *reply = networkManager.get(request);

    // store the page number as property for later handling in the slot
    reply->setProperty("page", page);

    connect(reply, &QNetworkReply::finished, this, [this, reply, responseHandler]{
        responseHandler(reply);
    });
}

void BreweryManager::handleLongestNameResponse(QNetworkReply* reply)
{
    if (!reply)
        return;

    int page = reply->property("page").toInt();  // retrieve the page number
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (reply->error() == QNetworkReply::NoError && jsonDoc.isArray())
    {
        QJsonArray breweriesArray = jsonDoc.array();

        // process the breweries on this page
        for (auto it = breweriesArray.begin(); it != breweriesArray.end(); it++)
        {
            QJsonObject brewery = it->toObject();
            QString name = brewery["name"].toString();
            int nameLength = name.length();

            if (nameLength > _longestNameLength)
            {
                _longestNameLength = nameLength;
                _longestNamesList.clear();
                _longestNamesList.append(name);
            }
            else if (nameLength == _longestNameLength)
            {
                _longestNamesList.append(name);
            }
        }
        // if we fetched a full page, request the next page
        if (breweriesArray.size() == _perPage)
        {
            fetchPage(page + 1, [this](QNetworkReply* reply) {
                handleLongestNameResponse(reply);
            });
        }
        else
        {
            QString longestNamesString = _longestNamesList.join(",");
            emit longestNameFound(longestNamesString);
        }
    }
    else
    {
        qDebug() << "Error fetching breweries:" << reply->errorString();
    }

    reply->deleteLater();
}

void BreweryManager::handleCoordinatesResponse(QNetworkReply* reply)
{
    if (!reply)
        return;

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if(!jsonDoc.isArray())
            return;

        QJsonArray breweriesArray = jsonDoc.array();

        if (breweriesArray.isEmpty())
        {
            qDebug() << "No breweries found.";
            return;
        }

        QJsonObject brewery = breweriesArray.first().toObject();
        QString name = brewery["name"].toString();
        double latitude = brewery["latitude"].toString().toDouble();

        QStringList northernmostNamesList;
        northernmostNamesList.clear();
        QStringList southernmostNamesList;
        northernmostNamesList.clear();

        QString type = reply->property("type").toString();

        if (type == "northernmost")
        {
            northernmostNamesList.clear();
            northernmostNamesList.append(name);
            emit northernmostBreweryFound(name, latitude);
        }
        else if (type == "southernmost")
        {
            southernmostNamesList.clear();
            southernmostNamesList.append(name);
            emit southernmostBreweryFound(name, latitude);
        }
    }
    else
    {
        qDebug() << "Error fetching brewery:" << reply->errorString();
    }

    reply->deleteLater();
}
