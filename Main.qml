import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 2.15
import BreweryFinder 1.0

Window {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Brewery Finder")

    BreweryManager {
        id: brewMaster

        onLongestNameFound: function(longestName) {
            longestNameText.text = longestName;
        }
        onNorthernmostBreweryFound: function(name, latitude) {
            northernmostText.text = "Northernmost brewery: " + name + " (Lat: " + latitude + ")";
        }
        onSouthernmostBreweryFound: function(name, latitude) {
            southernmostText.text = "Southernmost brewery: " + name + " (Lat: " + latitude + ")";
        }
    }

    ColumnLayout {
            id: mainLayout
            anchors.fill: parent
            spacing: 10

        Rectangle {
            id: rec1
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "green"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Button {
                    text: "Find longest name"
                     onClicked: brewMaster.findLongestName() // fetchMetadata will trigger everything
                }

                Text {
                    id: longestNameText
                    text: "No brewery data yet"
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    color: "white"
                }
            }
        }

        Rectangle {
            id: rec2
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "teal"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Button {
                    text: "Find Northernmost"
                    onClicked: brewMaster.findNorthernmostBrewery()
                }
                Text {
                    id: northernmostText
                    text: "No northernmost brewery data yet"
                    color: "white"
                }
            }
        }

        Rectangle {
            id: rec3
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "red"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Button {
                    text: "Find Southernmost"
                    onClicked: brewMaster.findSouthernmostBrewery()
                }
                Text {
                    id: southernmostText
                    text: "No southernmost brewery data yet"
                    color: "white"
                }
            }
        }
    }
}
