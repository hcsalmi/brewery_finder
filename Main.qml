import QtQuick
import QtQuick.Controls
import BreweryFinder 1.0

Window {
    visible: true
    width: 400
    height: 600
    title: qsTr("Find Your Brew")

    Loader {
        id: pageLoader
        anchors.fill: parent
        source: "FirstPage.qml"
    }
}
