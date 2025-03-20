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

    fetchPageForLongestName(page);
}

void BreweryManager::findNorthernmostBrewery()
{
    _northernmostLatitude = SOUTH_POLE_LATITUDE;
    _northernmostNamesList.clear();
    int page = 1;

    fetchPageForNorthernmost(page);
}

void BreweryManager::findSouthernmostBrewery()
{
    _southernmostLatitude = NORTH_POLE_LATITUDE;
    _southernmostNamesList.clear();
    int page = 1;

    fetchPageForSouthernmost(page);
}

void BreweryManager::fetchPageForLongestName(int page)
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

    connect(reply, &QNetworkReply::finished, this, &BreweryManager::handleLongestNameResponse);
}

void BreweryManager::fetchPageForNorthernmost(int page)
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

    connect(reply, &QNetworkReply::finished, this, &BreweryManager::handleNorthernmostResponse);
}

void BreweryManager::fetchPageForSouthernmost(int page)
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

    connect(reply, &QNetworkReply::finished, this, &BreweryManager::handleSouthernmostResponse);
}

void BreweryManager::handleLongestNameResponse()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
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
            fetchPageForLongestName(page + 1);
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

void BreweryManager::handleNorthernmostResponse()
{
    QNetworkReply *reply;

    reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QNetworkReply::NoError)
    {
        int page = reply->property("page").toInt();  // retrieve the page number
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if(!jsonDoc.isArray())
            return;

        QJsonArray breweriesArray = jsonDoc.array();

        for (auto it = breweriesArray.begin(); it != breweriesArray.end(); it++ )
        {
            QJsonObject brewery = it->toObject();
            QString name = brewery["name"].toString();
            double latitude = brewery["latitude"].toString().toDouble();

            if (latitude > _northernmostLatitude)
            {
                _northernmostLatitude = latitude;
                _northernmostNamesList.clear();
                _northernmostNamesList.append(name);
            }
            else if (qAbs(latitude - _northernmostLatitude) < _tolerance)
            {
                _northernmostNamesList.append(name);
            }
        }
        if (breweriesArray.size() == _perPage)
        {
            fetchPageForNorthernmost(page + 1);
        }
        QString northernmostNamesString = _northernmostNamesList.join(", ");
        emit northernmostBreweryFound(northernmostNamesString, _northernmostLatitude);
    }
    else
    {
        qDebug() << "Error fetching northernmost brewery:" << reply->errorString();
    }

    reply->deleteLater();
}

void BreweryManager::handleSouthernmostResponse()
{
    QNetworkReply *reply;

    reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QNetworkReply::NoError)
    {
        int page = reply->property("page").toInt();  // retrieve the page number
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if(!jsonDoc.isArray())
            return;

        QJsonArray breweriesArray = jsonDoc.array();

        for (auto it = breweriesArray.begin(); it != breweriesArray.end(); it++ )
        {
            QJsonObject brewery = it->toObject();
            QString name = brewery["name"].toString();
            double latitude = brewery["latitude"].toString().toDouble();

            if (latitude < _southernmostLatitude)
            {
                _southernmostLatitude = latitude;
                _southernmostNamesList.clear();
                _southernmostNamesList.append(name);
            }
            else if (qAbs(latitude - _southernmostLatitude) < _tolerance)
            {
                _southernmostNamesList.append(name);
            }
        }
        if (breweriesArray.size() == _perPage)
        {
            fetchPageForSouthernmost(page + 1);
        }
        QString southernmostNamesString = _southernmostNamesList.join(", ");
        emit southernmostBreweryFound(southernmostNamesString, _southernmostLatitude);
    }
    else
    {
        qDebug() << "Error fetching southernmost brewery:" << reply->errorString();
    }

    reply->deleteLater();
}
