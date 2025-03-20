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

void BreweryManager::findSouthernmostBrewery()
{
    QUrl apiUrl("https://api.openbrewerydb.org/v1/breweries?by_country=Ireland&per_page=200");
    QNetworkRequest request(apiUrl);
    QNetworkReply *reply = networkManager.get(request);
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
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if(!jsonDoc.isArray())
            return;

        QJsonArray breweriesArray = jsonDoc.array();
        double southernmostLatitude = NORTH_POLE_LATITUDE;
        QString southernmostName = "Unknown";
        QStringList southernmostNamesList;
        const double TOLERANCE = 0.00001;


        for (auto it = breweriesArray.begin(); it != breweriesArray.end(); it++ )
        {
            QJsonObject brewery = it->toObject();
            double latitude = brewery["latitude"].toString().toDouble();

            if (latitude < southernmostLatitude)
            {
                southernmostLatitude = latitude;
                southernmostName = brewery["name"].toString();
                southernmostNamesList.clear();
                southernmostNamesList.append(southernmostName);
            }
            else if (qAbs(latitude - southernmostLatitude) < _tolerance)
            {
                southernmostNamesList.append(brewery["name"].toString());
            }
        }
        QString southernmostNamesString = southernmostNamesList.join(", ");
        emit southernmostBreweryFound(southernmostNamesString, southernmostLatitude);
    }
    else
    {
        qDebug() << "Error fetching southernmost brewery:" << reply->errorString();
    }

    reply->deleteLater();
}
