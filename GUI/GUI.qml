import QtQuick 1.1



Rectangle //bitte löschen
{
    width: 820;
    height: 420;
    Rectangle
    {
        id: mainwindow
        x: 10
        y: 10
        width: 800
        height: 400
        radius: 5
        border.color: "#00ccff"
        border.width: 3
        z: 1


        Row
        {
            id: row1
            Column
            {
                Rectangle
                {
                id: serverlist
                radius: 5
                border.color: "#00ccff"
                border.width: 3
                z:2
                width: 100; height: 370
                }
            }

            Column
            {

                Rectangle
                {
                    id: output
                    radius: 5
                    border.color: "#00ccff"
                    border.width: 3
                    z:2
                    width: 600 ; height: 370
                }
            }

            Column
            {
                Rectangle
                {
                    id: variables
                    radius: 5
                    border.color: "#00CCFF"
                    border.width: 3
                    z:2
                    width: 100; height: 370
                }
            }
        }

        Row
        {
            id: row2

            y: row1.height
            Column
            {
                Rectangle
                {
                    id: input
                    border.width: 3;
                    radius: 5;
                    border.color: "#00CCFF"
                    z:2
                    width: 700; height: 30
                }
            }

            Column
            {

                Button { text: "process" }
            }
        }
    }


}
