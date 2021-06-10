import QtQuick 2.3
import QtQuick.Controls 1.2

ApplicationWindow {
    id: statisticWindow
    width: 400;
    height: 400
    title: "Статистика"

    Label{
        anchors.horizontalCenter: parent.horizontalCenter
        text: " Ячейка: Скорость, Пробег, Состояние "
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 50

        ListView {
            anchors.fill: parent

            model: statModel

            delegate: Text { text: " " + cell + " : " + speed + " , " + milage + " , " + isAlive }
        }

    }

}
