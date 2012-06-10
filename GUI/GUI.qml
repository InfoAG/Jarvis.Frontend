import QtQuick 1.1
import Qt 4.7

/*Rectangle
{
    width: 1000
    height: 1000*/

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
    color: "#003399"
    opacity: 0.8

    Rectangle
    {
        anchors.top: mainrec.top
        anchors.topMargin: 5
        id: serverlist
        border.color: "#30e4d4"
        border.width: 2
        z:2
        width: 100; height: 370
        color: "#003399"
        opacity: 0.8
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
        color: "#003399"
        opacity: 0.8
    }


    Rectangle
    {
        anchors.left: output.right
        anchors.leftMargin: 2
        y:5
        id: variables
        z:2
        width: 96; height: 370
        color: "#003399"
        opacity: 0.8
    }


    Rectangle
    {
        anchors.top: serverlist.bottom
        anchors.topMargin: 5
        id: input
        border.width: 2;
        border.color: "#30e4d4"
        color: "#b0b0b0"
        opacity: 0.8
        z:2
        width: serverlist.width + output.anchors.leftMargin + output.width
        height: 30

        Textarea{textwidth: serverlist.width + output.anchors.leftMargin + output.width; textheight: 30}

    }

    Rectangle
    {
        id: buttonrec
        anchors.left: input.right
        anchors.top: variables.bottom
        anchors.topMargin: 5
        anchors.leftMargin: 2
        color: "#003399"
        z:2
        Button { text: "process"}
    }

    Rectangle //resizerec
    {
        id: resizerec
        anchors.right: mainrec.right
        anchors.bottom: mainrec.bottom
        color: "#003399"
        width: 20; height: 20
        border.color: "#30e4d4"
        border.width: 2

        Image
        {
            source: "images/resize_icon.png"
            anchors.fill: parent

        }

        MouseArea
        {
            anchors.fill: parent
            onPressed: {


            }
        }

    }

    Rectangle //moverec
    {
        id: moverec
        anchors.right: resizerec.left
        anchors.bottom: mainrec.bottom
        color: "#003399"
        width: 20; height: 20
        border.color: "#30e4d4"
        border.width: 2

        Image
        {
            source: "images/move_icon.png"
            anchors.fill: parent

        }

        MouseArea
        {
            id: mouseRegion
            anchors.fill: parent;
            property variant clickPos: "1,1"

            onPressed: {
                clickPos  = Qt.point(mouse.x,mouse.y)
            }

            onPositionChanged: {
                var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
                mainrec.pos = Qt.point(mainrec.pos.x+delta.x,
                                  mainrec.pos.y+delta.y)
            }
        }

    }

}



//}
