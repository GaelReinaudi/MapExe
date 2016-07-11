import QtQuick 1.0

Rectangle {
    width: 400; height: 400; color: "black"

    Grid {
        x: 5; y: 5
        rows: 100; columns: 100; spacing: 20

        Repeater { model: 10000
                   Rectangle { width: 250; height: 170
                               color: "lightgreen"

//                               Text { text: index
  //                                    font.pointSize: 30
    //                                  anchors.centerIn: parent }
                   }
        }
    }
}
