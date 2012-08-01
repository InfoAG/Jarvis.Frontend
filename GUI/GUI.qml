import QtQuick 1.1
import Qt 4.7


Rectangle
{
    width: 1000
    height: 500


Rectangle
{
    id: mainrec
    x: 10
    y: 10
    width: 807
    height: 430
    border.color: "#30e4d4"
    border.width: 2
    z: 1


    Image
    {
        anchors.fill: parent
        source: "images/Backgroundraster.jpg"
    }

    Rectangle
    {
        anchors.top: mainrec.top
        anchors.topMargin: 5
        id: serverlist
        border.color: "#30e4d4"
        border.width: 2
        z:2
        width: 100; height: 370
        color: "transparent"
    }

    Rectangle
    {
        anchors.left: serverlist.right
        anchors.leftMargin: 5
        y:5
        id: output
        border.color: "#30e4d4"
        border.width: 2
        z:2
        width: 600 ; height: 370
        color: "transparent"
    }


    Rectangle
    {
        anchors.left: output.right
        anchors.leftMargin: 2
        y:5
        id: variables
        z:2
        width: 96; height: 370
        color: "transparent"
    }


    Rectangle
    {
        anchors.top: serverlist.bottom
        anchors.topMargin: 5
        id: input
        border.width: 2;
        border.color: "#30e4d4"
        z:2
        width: serverlist.width + output.anchors.leftMargin + output.width
        height: 30
        color: "transparent"

        Textarea{textwidth: serverlist.width + output.anchors.leftMargin + output.width; textheight: 30}

    }

    Rectangle
    {
        id: buttonrec
        anchors.left: input.right
        anchors.top: variables.bottom
        anchors.topMargin: 5
        anchors.leftMargin: 2
        z:2
        color: "transparent"
        width: mainrec.width-input.width-2*mainrec.border.width
        height: 30

        Image
        {
            id: buttonimage
            anchors.fill: parent
            source: "images/button_0.png"

        }

        MouseArea{id: mouseArea; anchors.fill: parent}

        states: State {
                     name: "down"; when: mouseArea.pressed === true
                     PropertyChanges{ target: buttonimage; source: "images/button_1.png"}
        }
    }

}

}
