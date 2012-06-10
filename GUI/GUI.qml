import QtQuick 1.1



Rectangle //bitte löschen
{
    width: 820;
    height: 435;
    Rectangle
    {
        id: mainwindow
        x: 10
        y: 10
        width: 805
        height: 410
        radius: 5
        border.color: "#00ccff"
        border.width: 2
        z: 1


        Rectangle
        {
            anchors.top: mainwindow.top
            anchors.topMargin: 5
            id: serverlist
            radius: 5
            border.color: "#00ccff"
            border.width: 2
            z:2
            width: 100; height: 370
        }

        Rectangle
        {
            anchors.left: serverlist.right
            anchors.leftMargin: 5
            y:5
            id: output
            radius: 5
            border.color: "#00ccff"
            border.width: 2
            z:2
            width: 600 ; height: 370
        }


        Rectangle
        {
            anchors.left: output.right
            anchors.leftMargin: 2
            y:7
            id: variables
            radius: 5
            z:2
            width: 96; height: 368
        }


        Rectangle
        {
            anchors.top: serverlist.bottom
            anchors.topMargin: 5
            id: input
            border.width: 2;
            radius: 5;
            border.color: "#00CCFF"
            z:2
            width: 700; height: 30
        }

        Rectangle
        {
            anchors.left: input.right
            anchors.top: variables.bottom
            anchors.topMargin: 5
            z:3
            Button { text: "process"}
        }

    }


}
