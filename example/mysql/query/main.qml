import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4


import QmlSql 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")


    Column{
        anchors.fill: parent
        anchors.margins: 20
        anchors.centerIn: parent
        spacing: 2

        Label{
            text: "Enter in a query then press enter"
        }
        TextField{
            id: quryString
            width: parent.width / 1.07
            placeholderText: "SELECT * FROM SOMETABLE;"
            anchors.horizontalCenter: parent.horizontalCenter
            onAccepted: {
                   query.exec()
            }

        }
        Button{
            text:"Run Query"
            onClicked: query.exec()
        }


        Label{
            text: "Output"
        }
        TextArea{
            id: queryOut
            width: parent.width / 1.07
            anchors.horizontalCenter: parent.horizontalCenter
            height:   parent.height - (quryString.height*2.5)
        }


        QmlSqlQuery{
            id: query
            // Set the connection name to the qml-sql-database
            connectionName: "mainConnection"
            // Use a query string to fill the model
            queryString: "SELECT * FROM SOMETABLE"
            onDone: {
                queryOut.text = lastQueryOutPut
            }
        }


        QmlSqlDatabase{
            id: mainConnection
            connectionName: "mainConnection"

            // Set the source of the database
            source: "127.0.0.1"

            // set the database Name
            databaseName: "SOMEDATABASE"

            // set the User of the connection
            user: ""

            // set the password for that User
            password: ""

            // set the port for the connection
            port: 3306

            // set the driver to use
            databaseDriver: QmlSqlDatabase.MySql

            // add the database to memory so we can call over and over again
            Component.onCompleted: addDataBase()


            // Run the query to fill the model
            // ! make sure that you are connected first !
            onConnectionOpened: query.exec()
        }

    }
}
