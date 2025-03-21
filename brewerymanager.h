#ifndef BREWERYMANAGER_H
#define BREWERYMANAGER_H

#include <QObject>
#include <QtQml>

#define SOUTH_POLE_LATITUDE -90.0
#define NORTH_POLE_LATITUDE 90.0
#define PRIME_MERIDIAN 0.0

class BreweryManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit BreweryManager(QObject *parent = nullptr);

    Q_INVOKABLE void findLongestName();
    Q_INVOKABLE void findNorthernmostBrewery();
    Q_INVOKABLE void findSouthernmostBrewery();

signals:
    void longestNameFound(QString name);
    void northernmostBreweryFound(QString name, double latitude);
    void southernmostBreweryFound(QString name, double latitude);

private slots:
    void handleLongestNameResponse(QNetworkReply* reply);
    void handleCoordinatesResponse(QNetworkReply* reply);

private:
    QNetworkAccessManager networkManager;

    static constexpr int _perPage = 50;

    int _longestNameLength;
    QStringList _longestNamesList;

    void fetchPage(int page, std::function<void(QNetworkReply*)> responseHandler);
};

#endif
