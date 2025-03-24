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

    QUrlQuery query;
    query.addQueryItem("by_country", "Ireland");

    fetchPage(page,query,[this](QNetworkReply* reply) {
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

void BreweryManager::fetchPage(int page, const QUrlQuery& baseQuery, std::function<void(QNetworkReply*)> responseHandler)
{
    QUrl apiUrl("https://api.openbrewerydb.org/v1/breweries");

    QUrlQuery query = baseQuery;

    query.addQueryItem("per_page", QString::number(_perPage));
    query.addQueryItem("page", QString::number(page));
    apiUrl.setQuery(query);

    QNetworkRequest request(apiUrl);
    QNetworkReply *reply = networkManager.get(request);

    reply->setProperty("page", page);

    connect(reply, &QNetworkReply::finished, this, [this, reply, responseHandler]{
        responseHandler(reply);
    });
}

void BreweryManager::handleLongestNameResponse(QNetworkReply* reply)
{
    if (!reply)
    {
        emit errorOccurred("Error fetching brewery");
        return;
    }

    int page = reply->property("page").toInt();
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (reply->error() == QNetworkReply::NoError && jsonDoc.isArray())
    {
        QJsonArray breweriesArray = jsonDoc.array();

        if (breweriesArray.isEmpty())
        {
            emit errorOccurred("No breweries found");
            return;
        }
        for (auto it = breweriesArray.begin(); it != breweriesArray.end(); it++)
        {
            QJsonObject brewery = it->toObject();

            QString name = brewery["name"].toString();
            if (name.isEmpty())
                continue;

            int nameLength = name.length();

            if (nameLength > _longestNameLength)
            {
                _longestNameLength = nameLength;
                _longestNamesList.clear();
                _longestNamesList.append(name);
            }
            else if (nameLength == _longestNameLength)
                _longestNamesList.append(name);
        }
        if (breweriesArray.size() == _perPage)
        {
            QUrlQuery nextQuery;
            nextQuery.addQueryItem("by_country", "Ireland");
            fetchPage(page + 1, nextQuery, [this](QNetworkReply* reply) {
                handleLongestNameResponse(reply);
            });
        }
        else
        {
            QString longestNamesString;

            if (_longestNamesList.isEmpty())
                longestNamesString = "Brewery has no name";
            else
                longestNamesString = _longestNamesList.join(",");

            emit longestNameFound(longestNamesString);
        }
    }
    else
        emit errorOccurred("Error fetching brewery name");

    reply->deleteLater();
}

void BreweryManager::handleCoordinatesResponse(QNetworkReply* reply)
{
    if (!reply)
    {
        emit errorOccurred("Error fetching brewery");
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (reply->error() == QNetworkReply::NoError && jsonDoc.isArray())
    {
        QJsonArray breweriesArray = jsonDoc.array();

        if (breweriesArray.isEmpty())
        {
            emit errorOccurred("No breweries found");
            return;
        }

        QJsonObject brewery = breweriesArray.first().toObject();
        QString name = brewery["name"].toString();
        double latitude = brewery["latitude"].toString().toDouble();

        if (name.isEmpty() || qIsNaN(latitude)) {
            emit errorOccurred("Error: Missing or invalid brewery name or latitude data");
            return;
        }

        const double epsilon = 0.0000001;

        if (latitude < (-90.0 - epsilon) || latitude > (90.0 + epsilon))
        {
            emit errorOccurred("Error: Invalid latitude value");
            return;
        }

        emit breweryCoordinatesFound(name, latitude);
    }
    else
        emit errorOccurred("Error fetching brewery");

    reply->deleteLater();
}
