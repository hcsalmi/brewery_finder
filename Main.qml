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
            breweryText.text = longestName;
        }
    }

    Text {
          id: breweryText
          text: "Longest brewery name will appear here."
          anchors.centerIn: parent
      }

      Button {
          text: "Find Longest Name"
          anchors.top: mainWindow.top


          onClicked: {
              brewMaster.findLongestName();
          }
      }
}
