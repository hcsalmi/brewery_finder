#ifndef BREWERYMANAGER_H
#define BREWERYMANAGER_H

#include <QObject>
#include <QtQml>

#define SOUTH_POLE_LATITUDE -90.0

class BreweryManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit BreweryManager(QObject *parent = nullptr);
    Q_INVOKABLE void findLongestName();
    Q_INVOKABLE void findNorthernmostBrewery();

signals:
    void longestNameFound(QString longestName);
    void northernmostBreweryFound(QString name, double latitude);

private slots:
    void handleLongestNameResponse();
    void handleNorthernmostResponse();


private:
    QNetworkAccessManager networkManager; // Handles API requests
};

#endif
