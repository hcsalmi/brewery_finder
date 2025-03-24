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
    void longestNameFound(const QString &name);
    void breweryCoordinatesFound(const QString &name, double latitude);
    void errorOccurred(const QString &message);

private slots:
    void handleLongestNameResponse(QNetworkReply* reply);
    void handleCoordinatesResponse(QNetworkReply* reply);

private:
    QNetworkAccessManager networkManager;

    static constexpr int _perPage = 50;

    int _longestNameLength;
    QStringList _longestNamesList;

    void fetchPage(int page, const QUrlQuery& baseQuery, std::function<void(QNetworkReply*)> responseHandler);
};

#endif
