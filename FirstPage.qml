import QtQuick
import QtQuick.Controls

Item {
    anchors.fill: parent

    Image {
        source: "qrc:/beer_hops.png"
        anchors.fill: parent
    }

    FontLoader {
        id: charmFont
        source: "qrc:/Charm-Regular.ttf"
    }

    Rectangle {
        id: welcomeBox
        color: Qt.rgba(170/255, 198/255, 34/255, 0.8)
        radius: 3
        width: parent.width * 0.80
        height: parent.height * 0.15
        anchors {
            top: parent.top
            left: parent.left
            margins: 20
        }

        Item {
            width: parent.width * 0.9
            height: parent.height * 0.9
            anchors.centerIn: parent

            Text {
                id: welcomeText
                text: "Welcome to Find Your Brew!"
                color: "black"
                font.pixelSize: Math.min(parent.height * 0.4, parent.width * 0.1)
                font.family: charmFont.name
                wrapMode: Text.WordWrap
                anchors.centerIn: parent
            }
        }
    }

    Button {
        id: searchButton
        text: "Search for Breweries in Ireland"
        font.pixelSize: Math.min(welcomeText.height * 0.40, welcomeText.width * 0.15)
        font.family: charmFont.name
        hoverEnabled: false
        width: welcomeBox.width
        height: welcomeBox.height * 0.4
        anchors {
            top: welcomeBox.bottom
            horizontalCenter: welcomeBox.horizontalCenter
            topMargin: 20
        }

        background: Rectangle {
            id: buttonBackground
            color: Qt.rgba(243/255, 156/255, 18/255, 0.8)
            radius: 3

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: buttonBackground.color = "#f39c12"
                onExited: buttonBackground.color = Qt.rgba(243/255, 156/255, 18/255, 0.8)
            }
        }

        onClicked: {
            pageLoader.source = "SecondPage.qml";
        }
    }
}
