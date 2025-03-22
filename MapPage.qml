import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtLocation 5.15
import QtPositioning 5.15

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
        onClicked: pageLoader.source = "SecondPage.qml";
    }
    Plugin {
        id: mapPlugin
        name: "osm" // OpenStreetMap
    }

    Rectangle {
        id: mapContainer
        anchors {
             top: backButton.bottom
            bottom: parent.bottom
             left: parent.left
             right: parent.right
             margins: 20
        }
        color: "transparent" // No background to avoid hiding the image

        Map {
            id: map
            anchors.fill: parent
            plugin: mapPlugin
            center: QtPositioning.coordinate(53.4129, -7.5030) // Centre of Ireland
            zoomLevel: 8
            minimumZoomLevel: 2
            maximumZoomLevel: 18

            // Pin marker
            MapQuickItem {
                id: marker
                anchorPoint.x: pinImage.width/2
                anchorPoint.y: pinImage.height
                coordinate: QtPositioning.coordinate(53.4129, -7.5030)

                sourceItem: Column {
                    Image {
                        id: pinImage
                        source: "qrc:/pin_marker.png"
                        width: 30
                        height: 30

                        // Fallback if image is missing
                        Rectangle {
                            id: pinFallback
                            visible: pinImage.status !== Image.Ready
                            width: 30
                            height: 30
                            color: "red"
                            radius: width/2
                            border.color: "white"
                            border.width: 2
                        }
                    }
                }
            }
        }

        MouseArea {
            id: dragArea
            anchors.fill: parent
            property bool isDragging: false
            property real lastX: 0
            property real lastY: 0
            property real speedMultiplier: 1

            onPressed: {
                lastX = mouse.x
                lastY = mouse.y
                isDragging = true
            }

            onPositionChanged: {
                if (!isDragging) return

                var deltaX = mouse.x - lastX
                var deltaY = mouse.y - lastY

                // Get current map viewport in coordinates
                var topLeft = map.toCoordinate(Qt.point(0, 0))
                var bottomRight = map.toCoordinate(Qt.point(map.width, map.height))
                var coordWidth = Math.abs(bottomRight.longitude - topLeft.longitude)
                var coordHeight = Math.abs(bottomRight.latitude - topLeft.latitude)

                var moveLongitude = -deltaX * coordWidth / map.width * speedMultiplier
                var moveLatitude = deltaY * coordHeight / map.height * speedMultiplier

                lastX = mouse.x
                lastY = mouse.y

                // Move the map
                map.center = QtPositioning.coordinate(
                    map.center.latitude + moveLatitude,
                    map.center.longitude + moveLongitude
                )
            }

            onReleased: {
                isDragging = false
            }

            // Wheel handler for zooming
            onWheel: {
                if (wheel.angleDelta.y > 0)
                {
                    if (map.zoomLevel < map.maximumZoomLevel)
                        map.zoomLevel += 0.5
                }
                else
                {
                    if (map.zoomLevel > map.minimumZoomLevel)
                        map.zoomLevel -= 0.5
                }
            }
        }
    }

    Rectangle {
        id: zoomInButton
        width: 40
        height: 40
        radius: width/2
        color: "white"
        opacity: 0.8
        border.color: "gray"
        border.width: 1
        anchors {
            right: parent.right
            bottom: zoomOutButton.top
            margins: 10
        }
        z: 1

        Text {
            text: "+"
            anchors.centerIn: parent
            font.pixelSize: 24
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (map.zoomLevel < map.maximumZoomLevel) {
                    map.zoomLevel += 1
                }
            }
        }
    }

    Rectangle {
        id: zoomOutButton
        width: 40
        height: 40
        radius: width/2
        color: "white"
        opacity: 0.8
        border.color: "gray"
        border.width: 1
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 10
        }
        z: 1

        Text {
            text: "-"
            anchors.centerIn: parent
            font.pixelSize: 24
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (map.zoomLevel > map.minimumZoomLevel) {
                    map.zoomLevel -= 1
                }
            }
        }
    }
}
