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
        id: lay

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
            onAccepted: run()

            function run() {
                query.queryString = text
                query.exec()
            }

        }



        Button{
            text:qsTr("Run Query")
            width: parent.width * 0.9
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: quryString.run()
        }
        Button {
            text: "Switch connection"
            width: parent.width * 0.1
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: query.database = (query.database == mainConnection ? secondConnection : mainConnection)
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
            textFormat: Text.AutoText
            text: query.errorString.length === 0 ? query.lastQueryOutput : "<p style='color:red'>" + query.errorString + "</p>"
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height - (quryString.height *4.3)
        }


        QmlSqlQuery{
            id: query
            // Set the connection name to the qml-sql-database
            database: mainConnection
            // Use a query string to fill the model
            queryString: "SELECT * FROM qmlsql"
        }

        QmlSqlDatabase{
            id: secondConnection
            connectionName: "secondConnection"

            // Set the source of the database
            source: "127.0.0.1"

            // set the database Name
            databaseName: "qmlsql2"

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

            onErrorStringChanged: {
                queryOut.textFormat = Text.RichText
                queryOut.text = "<p style='color:red'>" +errorString +"</p>"
                queryOut.textFormat = Text.AutoText
            }
        }
    }
}
