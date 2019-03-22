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
    width: 640
    height: 480
    title: qsTr("QmlSqlQueryModel Model Example")

    ListView{
        anchors.fill: parent
        model:queryModel
        delegate: Button{
            width: parent.width / 1.07
            anchors.horizontalCenter: parent.horizontalCenter

            // say I have a database and columns named like so
            // -----------------------
            // | id | name   | age |
            // _______________________
            // | 1  | John   | 2   |
            // | 2  | Martin | 35  |
            // | 3  | Maddie | 33  |
            // | 4  | Peter  | 0   |

            // the model will know all the columns and set the role names

            text: name + ', age ' + age

        }
    }

    QmlSqlQueryModel{
        id: queryModel
        // Set the connection name to the QmlSqlDatabase
        connectionName: "mainConnection"
        // Use a query string to fill the model
        queryString: "SELECT * FROM qmlsql"
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


        // Run the query to fill the model
        // ! make sure that you are connected first !
        onConnected: queryModel.exec()
    }
}
