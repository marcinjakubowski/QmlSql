/* In order for this example to work, you will need a database.
   You can create one fitting this example by running the following SQL script on a MySQL instance:

    CREATE USER qmlsql IDENTIFIED BY 'qmlsql';

    CREATE DATABASE qmlsql;

    GRANT ALL PRIVILEGES ON qmlsql.* TO qmlsql;

    USE qmlsql;

    CREATE TABLE qmlsql
    (
        id int primary key auto_increment,
        name text,
        age int
    );

    INSERT INTO qmlsql (name, age)
    VALUES ('John', 2), ('Martin', 35), ('Maddie', 33), ('Peter', 0);
*/

import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

import QmlSql 1.0

Window {
    visible: true
    width: Screen.width / 2
    height: Screen.width / 2
    title: qsTr("QmlSqlQuery Example")

    Column{
        anchors.fill: parent
        anchors.margins: 20
        anchors.centerIn: parent
        spacing: 2

        Label{
            font.bold: true
            text: qsTr("Enter in a query then press enter")
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }
        TextField{
            id: quryString
            width: parent.width / 1.07
            placeholderText: query.queryString
            anchors.horizontalCenter: parent.horizontalCenter
            onAccepted: {
                   query.queryString = text
                   query.exec()
            }

        }
        Button{
            text:qsTr("Run Query")
            width: parent.width / 1.07
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked:{
                query.queryString = quryString.text
                query.exec()
            }
        }


        Label{
            text: qsTr("Output")
            font.bold: true
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }
        TextArea{
            id: queryOut
            width: parent.width / 1.07
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height - (quryString.height *4.3)
        }


        QmlSqlQuery{
            id: query
            // Set the connection name to the qml-sql-database
            connectionName: "mainConnection"
            // Use a query string to fill the model
            queryString: "SELECT * FROM qmlsql"
            onDone: {
                queryOut.text = lastQueryOutPut
            }
            onErrorStringChanged: {
                queryOut.textFormat = Text.RichText
                queryOut.text = "<p style='color:red'>" +errorString +"</p>"
                queryOut.textFormat = Text.AutoText
            }
        }


        QmlSqlDatabase{
            id: mainConnection
            connectionName: "mainConnection"

            // Set the source of the database
            source: "127.0.0.1"

            // set the database Name
            databaseName: "qmlsql"

            // set the User of the connection
            user: "qmlsql"

            // set the password for that User
            password: "qmlsql"

            // set the port for the connection
            port: 3306

            // set the driver to use
            databaseDriver: QmlSqlDatabase.MySql

            // add the database to memory so we can call over and over again
            Component.onCompleted: addDataBase()


            // Run the query to fill the model
            // ! make sure that you are connected first !
            onConnectionOpened: query.exec()

            onErrorStringChanged: {
                queryOut.textFormat = Text.RichText
                queryOut.text = "<p style='color:red'>" +errorString +"</p>"
                queryOut.textFormat = Text.AutoText
            }
        }
    }
}
