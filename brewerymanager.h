#ifndef BREWERYMANAGER_H
#define BREWERYMANAGER_H

#include <QObject>
#include <QtQml>

#define SOUTH_POLE_LATITUDE -90.0
#define NORTH_POLE_LATITUDE 90.0

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
    void longestNameFound(QString longestName);
    void northernmostBreweryFound(QString name, double latitude);
    void southernmostBreweryFound(QString name, double latitude);

private slots:
    void handleLongestNameResponse();
    void handleNorthernmostResponse();
    void handleSouthernmostResponse();

private:
    QNetworkAccessManager networkManager;

    static const int _perPage = 50;
    const double _tolerance = 0.00001;

    int _longestNameLength;
    QStringList _longestNamesList;

    double _northernmostLatitude;
    QStringList _northernmostNamesList;

    void fetchPageForLongestName(int page);
    void fetchPageForNorthernmost(int page);
};

#endif
