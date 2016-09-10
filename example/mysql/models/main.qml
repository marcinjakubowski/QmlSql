import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

import QmlSql 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Qml Sql Model Example")

    ListView{
        anchors.fill: parent
        model:queryModel
        delegate: Button{
            // say I have a db and a column is called name like so

            // -----------------------
            // | id | name | address |
            // _______________________
            // | 0  | joe  | 123 lane|

            // the model will know all the columns

            text: name

        }

    }




    QmlSqlQueryModel{
        id: queryModel
        // Set the connection name to the qml-sql-database
        connectionName: "mainConnection"
        // Use a query string to fill the model
        queryString: "SELECT * FROM SOMETABLE"
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
        onConnectionOpened: queryModel.exec()
    }
}
