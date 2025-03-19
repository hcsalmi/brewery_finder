#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "brewerymanager.h"

BreweryManager::BreweryManager(QObject *parent): QObject(parent){
}

//Pagination not yet implemented
void BreweryManager::findLongestName()
{
    QUrl apiUrl("https://api.openbrewerydb.org/v1/breweries?by_country=Ireland&per_page=200");
    QNetworkRequest request(apiUrl);
    QNetworkReply *reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, &BreweryManager::handleLongestNameResponse);
}

void BreweryManager::findNorthernmostBrewery() //jos useampi, mieti miten kasitellaan
{
    QUrl apiUrl("https://api.openbrewerydb.org/v1/breweries?by_country=Ireland&per_page=200");
    QNetworkRequest request(apiUrl);
    QNetworkReply *reply = networkManager.get(request);
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
    QNetworkReply *reply;

    //sender() allows access to the QNetworkReply that triggered the slot; safety cast
    reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (!jsonDoc.isArray())
            return;

        QJsonArray breweriesArray = jsonDoc.array();

        int longestNameLength = 0;
        QStringList longestNamesList;

        for (auto it = breweriesArray.begin(); it != breweriesArray.end(); it++ )
        {
            QJsonObject brewery = it->toObject();
            QString name = brewery["name"].toString();
            int nameLength = name.length();

            if(nameLength > longestNameLength)
            {
                longestNameLength = nameLength;
                longestNamesList.clear();
                longestNamesList.append(name);
            }
            else if (nameLength == longestNameLength)
                longestNamesList.append(name);
        }
        QString longestNamesString = longestNamesList.join(",");
        emit longestNameFound(longestNamesString);
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
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if(!jsonDoc.isArray())
            return;

        QJsonArray breweriesArray = jsonDoc.array();
        double northernmostLatitude = SOUTH_POLE_LATITUDE;
        QString northernmostName = "Unknown";
        QStringList northernmostNamesList;
        const double TOLERANCE = 0.00001;


        for (auto it = breweriesArray.begin(); it != breweriesArray.end(); it++ )
        {
            QJsonObject brewery = it->toObject();
            double latitude = brewery["latitude"].toString().toDouble();

            if (latitude > northernmostLatitude)
            {
                northernmostLatitude = latitude;
                northernmostName = brewery["name"].toString();
                northernmostNamesList.clear();
                northernmostNamesList.append(northernmostName);
            }
            else if (qAbs(latitude - northernmostLatitude) < TOLERANCE)
            {
                northernmostNamesList.append(brewery["name"].toString());
            }
        }
        QString northernmostNamesString = northernmostNamesList.join(", ");
        emit northernmostBreweryFound(northernmostNamesString, northernmostLatitude);
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
            else if (qAbs(latitude - southernmostLatitude) < TOLERANCE)
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

