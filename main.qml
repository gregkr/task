import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import FieldModel 1.0
import StatisticModel 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600
    minimumWidth: 800

    color: "#09102B"
    title: qsTr("Fly Game")

    property int fieldSize: 2
    property int cellCapacity: 2
    property int flyStupidity: 1

    property var statModel: StatisticModel{}

    TableView {
        id: tableView
        anchors.fill: parent

        rowSpacing: 1
        columnSpacing: 1

        columnWidthProvider: function (column) { return  25 * cellCapacity; }
        ScrollBar.horizontal: ScrollBar {}
        ScrollBar.vertical: ScrollBar {}

        delegate: Rectangle {
            id: cell
            implicitWidth: 25*cellCapacity
            implicitHeight: 30

            Image {
                anchors.fill: parent
                source: "image://flies/" + model.flies
            }
        }

        model: FieldModel {
            id: fieldModel
        }

    }

    footer: Rectangle {
        id: footer
        height: 120
        color: "#F3F3F4"

        RowLayout {
            id: fieldInputs
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                text: "Размер поля:"
                font.italic: true
                color: "steelblue"
            }

            TextInput{
                id : filedSizeInput
                text: "2"
                validator: IntValidator{bottom: 2; top: 10;}
                focus: true
                onEditingFinished: {
                    fieldSize = text;
                    if ( !fieldModel.running )
                    {
                        fieldModel.createField( fieldSize );
                    }
                }
            }

            Item { width: 20 }

            Label {
                text: "Мухоемкость:"
                font.italic: true
                color: "steelblue"
            }
            TextInput{
                id : cellCapacityInput
                text: "2"
                validator: IntValidator{bottom: 1; top: 5;}
                onEditingFinished: {
                    cellCapacity = text;
                    fieldModel.maxFliesPerCell = cellCapacity;
                    tableView.forceLayout();
                }
            }

        }

        RowLayout {
            id: addFlyInputs
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top : fieldInputs.bottom

            Label {
                text: "Строка:"
                font.italic: true
                color: "steelblue"
            }

            TextInput{
                id : cellRow
                text: "0"
                validator: IntValidator{bottom: 0; top: fieldSize ;}
                focus: true
                onEditingFinished: {
                }
            }

            Item { width: 20 }

            Label {
                text: "Столбец:"
                font.italic: true
                color: "steelblue"
            }
            TextInput{
                id : cellCol
                text: "0"
                validator: IntValidator{bottom: 1; top: fieldSize;}
                onEditingFinished: {
                }
            }

            Item {  width: 20  }

            Label {
                text: "Тупость:"
                font.italic: true
                color: "steelblue"
            }
            TextInput{
                id : stupidity
                text: "0"
                validator: IntValidator{bottom: 1; top: 100;}
                onEditingFinished: {
                }
            }

            Item {  width: 20  }

            Button {
                text: "Добавить муху"
                onClicked: {
                    fieldModel.addFly( cellRow.text, cellCol.text, stupidity.text )
                }
            }
        }

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top : addFlyInputs.bottom

            text: (fieldModel.running == 0) ? "СТАРТ" :
                               (fieldModel.running == 1) ? "СТОП" : "СТАТИСТИКА"
            onClicked: {
                if ( fieldModel.running == 0 )
                {
                    fieldModel.running = 1
                }
                else if ( fieldModel.running == 1 )
                {
                    fieldModel.running = 2
                }
                else if ( fieldModel.running == 2 )
                {
                    fieldModel.setStatModel( statModel )
                    var component = Qt.createComponent("Statistic.qml")
                    var window = component.createObject(root)
                    window.show()
                }

            }
        }
    }

    Component.onCompleted: {
        fieldModel.load();
    }
}
