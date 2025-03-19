#ifndef BREWERYMANAGER_H
#define BREWERYMANAGER_H

#include <QObject>
#include <QtQml>

class BreweryManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit BreweryManager(QObject *parent = nullptr);
    Q_INVOKABLE void findLongestName();

signals:
    void longestNameFound(QString longestName);

private slots:
    void handleLongestNameResponse();

private:
    QNetworkAccessManager networkManager;
};

#endif
