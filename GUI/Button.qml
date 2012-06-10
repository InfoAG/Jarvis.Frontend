import QtQuick 1.0



Rectangle
{

    id:  buttonrec
    property alias text: textItem.text

    width: 100; height: 30
    border.width: 1
    radius: 5
    smooth: true

    gradient: Gradient {
        GradientStop { id: gr1; position: 0.0; color: "darkGray" }
        GradientStop { id: gr2; position: 0.5; color: "black" }
        GradientStop { id: gr3; position: 1.0; color: "darkGray" }
    }

    Text {
        id: textItem
        anchors.centerIn: parent
        font.pointSize: 10
        font.bold: true
        color: "white"
    }

    MouseArea { id: mouseArea; hoverEnabled: false; anchors.fill: parent }


    states: State {
                 name: "down"; when: mouseArea.pressed == true
                 PropertyChanges{ target: gr1; color: "black"}
                 PropertyChanges{ target: gr2; color: "darkGray"}
                 PropertyChanges{ target: gr3; color: "black"}
    }


    /*transitions: Transition {
                 from: ""; to: "down"; reversible: true

    }*/

}
