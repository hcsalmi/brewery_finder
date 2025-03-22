import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: secondPage
    anchors.fill: parent

    Image {
        source: "qrc:/beer_hops.png"
        anchors.fill: parent
    }

    FontLoader {
        id: charmFont
        source: "qrc:/Charm-Regular.ttf"
    }

    Button {
        id: backButton
        text: "Back"
        font.pixelSize: parent.height * 0.03
        font.family: charmFont.name
        width: parent.width * 0.12
        height: parent.height * 0.06
        hoverEnabled: false

        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 20
            topMargin: 10
        }

        background: Rectangle {
            id: backButtonBackground
            color: Qt.rgba(243/255, 156/255, 18/255, 0.8)
            radius: 3

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: backButtonBackground.color = "#f39c12"
                onExited: backButtonBackground.color = Qt.rgba(243/255, 156/255, 18/255, 0.8)
            }
        }
        onClicked: pageLoader.source = "FirstPage.qml";
    }


    // Column Layout to stack buttons vertically
   ColumnLayout {
       id: buttonColumn
       anchors {
            top: backButton.bottom
            left: parent.left
            right: parent.right
            margins: 20
            topMargin: 20
       }

        spacing: 5
        height: parent.height * 0.2

       Button {
            id: longestNameButton
            text: "Search for the brewery with the longest name"
            font.pixelSize: parent.height * 0.15
            font.family: charmFont.name
            hoverEnabled: false
            Layout.fillWidth: true
            Layout.fillHeight: true

            background: Rectangle {
                id: longestNameButtonBackground
                color: Qt.rgba(243/255, 156/255, 18/255, 0.8)
                radius: 3

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: longestNameButtonBackground.color = "#f39c12"
                    onExited: longestNameButtonBackground.color = Qt.rgba(243/255, 156/255, 18/255, 0.8)
                }
            }
            onClicked: brewMaster.findLongestName();
       }

       Button {
           id: northernmostButton
           text: "Search for the northernmost brewery"
           font.pixelSize: parent.height * 0.15
           font.family: charmFont.name
           hoverEnabled: false
           Layout.fillWidth: true
           Layout.fillHeight: true

           background: Rectangle {
               id: northernmostButtonBackground
               color: Qt.rgba(243/255, 156/255, 18/255, 0.8)
               radius: 3

               MouseArea {
                   anchors.fill: parent
                   hoverEnabled: true
                   onEntered: northernmostButtonBackground.color = "#f39c12"
                   onExited: northernmostButtonBackground.color = Qt.rgba(243/255, 156/255, 18/255, 0.8)
               }
           }
           onClicked: brewMaster.findNorthernmostBrewery();
       }

       Button {
            id: southernmostButton
            text: "Search for the southernmost brewery"
            font.pixelSize: parent.height * 0.15
            font.family: charmFont.name
            hoverEnabled: false
            Layout.fillWidth: true
            Layout.fillHeight: true

            background: Rectangle {
                id: southernmostButtonBackground
                color: Qt.rgba(243/255, 156/255, 18/255, 0.8)
                radius: 3

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: southernmostButtonBackground.color = "#f39c12"
                    onExited: southernmostButtonBackground.color = Qt.rgba(243/255, 156/255, 18/255, 0.8)
                }
            }
            onClicked: brewMaster.findSouthernmostBrewery();
       }
   }

   Rectangle {
       id: textBox
       color: Qt.rgba(170/255, 198/255, 34/255, 0.9)
       width: buttonColumn.width
       height: parent.height * 0.4
       anchors {
           top: buttonColumn.bottom
           horizontalCenter: parent.horizontalCenter
           topMargin: 35
       }

       Text {
            id: dynamicText
            text: "Select a search option above to display information here."
            color: "black"
            font.pixelSize: Math.min(parent.height * 0.1, parent.width * 0.15)
            font.family: charmFont.name
            width: parent.width * 0.9
            anchors.centerIn: parent
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
       }
   }
   Button {
       id: mapButton
       text: "Show on map"
       font.pixelSize: parent.height * 0.03
       font.family: charmFont.name
       width: parent.width * 0.4
       height: parent.height * 0.06
       hoverEnabled: false

       anchors {
            top: textBox.bottom
            horizontalCenter: textBox.horizontalCenter
            topMargin: 20
       }

       background: Rectangle {
           id: mapButtonBackground
           color: Qt.rgba(243/255, 156/255, 18/255, 0.9)
           radius: 3

           MouseArea {
               anchors.fill: parent
               hoverEnabled: true
               onEntered: mapButtonBackground.color = "#f39c12"
               onExited: mapButtonBackground.color = Qt.rgba(243/255, 156/255, 18/255, 0.9)
           }
       }
       onClicked: pageLoader.source = "MapPage.qml";
   }
    BreweryManager {
        id: brewMaster

        onLongestNameFound: function(name) {
            dynamicText.text = name;
        }
        onNorthernmostBreweryFound: function(name, latitude) {
            dynamicText.text = name + "\n(Lat: " + latitude.toFixed(6) + ")";
        }
        onSouthernmostBreweryFound: function(name, latitude) {
            dynamicText.text = name + "\n(Lat: " + latitude.toFixed(6) + ")";
        }
    }
}
