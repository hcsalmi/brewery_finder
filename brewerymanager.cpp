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

