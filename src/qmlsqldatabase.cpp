#include "qmlsqldatabase.h"


/*!
   \qmltype QmlSqlDatabase
   \inqmlmodule QmlSql 1.0
   \ingroup QmlSql
   \inherits QObject
   \brief The QmlSqlDatabase class represents a connection to a database.
The QmlSqlDatabase provides an interface for accessing a database through a connection. An instance of
QmlSqlDatabase represents the connection. The connection provides access to the database via one of the
supported \c databaseDriver , which are derived from \c{QSqlDriver (c++)}.
Alternatively, one can subclass your own database driver from \c{QSqlDriver (c++)}. See How to Write Your Own Database Driver for more information.
Create a connection (i.e., an instance of QSqlDatabase) by calling one of the method addDatabase() , where you specify the databaseDriver  (i.e., what kind of database will you access) and a connectionName.
A connection is known by its own name, Not by the name of the database it connects to. You can have \b{multiple connections to one database}. QSqlDatabase also supports the concept of a default connection (not recommended),
 which is the unnamed connection. To create the default connection, don't pass the connection name argument before you call addDatabase(). Subsequently, when you call any method function that takes the connection name argument,
 if you don't pass the connectionName argument, the default connection is assumed. The following snippet shows how to create and open a default connection to a PostgreSQL database:

\code
      QmlSqlDatabase{
        id: db
        hostName:"acidalia";
        databaseName: "customdb";
        userName: "mojito";
        password("J0a1m8"

        databaseDriver: QmlSqlDatabase.QPSQL
        onOpen{
            //do something
        }
        Componet.onCompleted:{
            db.addDatabase()
        }
      }
\endcode


Once the QmlSqlDatabase object has been created, set the connection parameters with databaseName, userName, password, hostName and port. Then call addDatabase() to activate the physical connection to the database. The connection is not usable until you open it.
The connection defined above will be the default connection, because we didn't give a connection name to the object with the id of db.

To Add a \c connectionName just add
\code
    connectionName: "customdb-connection"
\endcode

It is a good idea to add a connectionName to the QmlSqlDatabase as that way one can use the other Objects that are in the QmlSql plugin to call over and over again from any page at any time.

Example:

\code
      QmlSqlQuery{
        id: db
        connectionName: "customdb-connection"
      }
\endcode


The code example above will connect to the database with the id of  "customdb-connection" and one does not need to create Objects over and over again.

If an error occurs, errorString will return information about it.
Get the names of the available SQL drivers with databaseDriverList.



\b{Warning : } There are still some wrapping that needs to be done to this class and it is subject to change in the near future.
Some of the things are close open check validation of Database types ect.
*/


QmlSqlDatabase::QmlSqlDatabase(QObject *parent)
    : QObject(parent), m_isConnected(false)
{
    setDatabaseDriverList();
    connect(this, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
    connect(this, SIGNAL(connectionOpened(QSqlDatabase,QString)), this, SLOT(handleOpened(QSqlDatabase,QString)));
    connect(this, SIGNAL(closeRequested(CloseReason,QString)), this, SLOT(handleCloseRequested(CloseReason,QString)));
    connect(this, SIGNAL(sqlError(QSqlError)),this, SLOT(handleSqlError(QSqlError)));
}

/*!
  \qmlproperty enum QmlSqlDatabase::databaseDriver
  Returns the database driver used to access the database connection.
  \sa addDataBase()

 */
QmlSqlDatabase::DataBaseDriver QmlSqlDatabase::databaseDriver() const {
    return m_databaseDriver;
}

void QmlSqlDatabase::setDatabaseDriver(const QmlSqlDatabase::DataBaseDriver& databaseDriver)
{
    if(m_databaseDriver == databaseDriver) {
        return;
    }

    switch (databaseDriver) {
        case Postgres:
            m_databaseDriver = Postgres;
            m_databaseDriverString =  "QPSQL";
            break;
        case MySql:
            m_databaseDriver = MySql;
            m_databaseDriverString =  "QMYSQL";
            break;
        case OCI:
            m_databaseDriver = OCI;
            m_databaseDriverString =  "QOCI";
            break;
        case ODBC:
            m_databaseDriver = ODBC;
            m_databaseDriverString =  "QODBC";
            break;
        case DB2:
            m_databaseDriver = DB2;
            m_databaseDriverString =  "QDB2";
            break;
        case TDS:
            m_databaseDriver = TDS;
            m_databaseDriverString =  "QTDS";
            break;
        case SQLite:
            m_databaseDriver = SQLite;
            m_databaseDriverString =  "QSQLITE";

            break;
        case SQLite2:
            m_databaseDriver = SQLite2;
            m_databaseDriverString =  "QSQLITE2";
            break;
        case IBase:
            m_databaseDriver = IBase;
            m_databaseDriverString =  "QIBASE";
            break;
    }
    emit databaseDriverChanged();
}

/*!
  \qmlproperty string QmlSqlDatabase::databaseDriverList

  Returns a list of all the available database drivers that one can use
*/
QStringList QmlSqlDatabase::databaseDriverList() const
{
    return m_databaseDriverList;
}


void QmlSqlDatabase::setDatabaseDriverList() {
    m_databaseDriverList.clear();
    QList<QString> li;
    foreach(QString l, QSqlDatabase::drivers()) {
       if (QSqlDatabase::isDriverAvailable(l)) {
           li << l;
       }
    }
    m_databaseDriverList << li;
}

/*!
  \qmlproperty string QmlSqlDatabase::errorString
    Returns information about the last error that occurred on the database.
 */
QString QmlSqlDatabase::errorString() const {
    return m_errorString;
}

/*!
  \qmlproperty string QmlSqlDatabase::source
Sets the connection's host name to \c source. To have effect, the source must be set before the connection is opened.

There is no default value.

 */
QString QmlSqlDatabase::source() const {
    return m_source;
}
void QmlSqlDatabase::setSource(const QString& source) {
    if (m_source == source)
        return;

    m_source = source ;
    emit sourceChanged();
}

/*!
 \qmlproperty string QmlSqlDatabase::databaseName
Sets the connection's \c databaseName. To have effect, the \c databaseName must be set before the connection is opened.

\b{Note:} The \c databaseName is not the connectionName. The connectionName must be added before  addDatabase() method is called.

For the QOCI (Oracle) driver, the \c databaseName is the TNS Service Name.

There is no default value.

 */

QString QmlSqlDatabase::databaseName() const {
    return m_dbName;
}

void QmlSqlDatabase::setDatabaseName(const QString& databaseName) {
    if (m_dbName == databaseName)
        return;

    m_dbName = databaseName ;
    emit databaseNameChanged() ;
}

/*!
 \qmlproperty string QmlSqlDatabase::user
Sets the connection's user name to user. To have effect, the \c user name must be set before the connection is opened.

There is no default value
 */
QString QmlSqlDatabase::user() const {
    return m_user;
}

void QmlSqlDatabase::setUser(const QString& user) {
    if (m_user == user)
        return;

    m_user = user ;
    emit userChanged();
}

/*!
 \qmlproperty string QmlSqlDatabase::password
Sets the connection's password to password. To have effect, The \c password must be set before the connection is opened.


There is no default value.

\b{Warning:} This stores the password in plain text within Qt. Use the open() call that takes a password as parameter to avoid this behavior.


 */
QString QmlSqlDatabase::password() const {
    return m_password;
}

void QmlSqlDatabase::setPassword(const QString& password) {
    if (m_password == password)
        return;

    m_password = password ;
    emit passwordChanged();
}


/*!
 \qmlproperty string QmlSqlDatabase::port
Sets the connection's port number to \c port. To have effect, the port number must be set before the connection is opened.

There is no default value.

 */
int QmlSqlDatabase::port() const {
    return m_port;
}

void QmlSqlDatabase::setPort(int port) {
    if (m_port == port)
        return;

    m_port = port ;
    emit portChanged();
}



/*!
  \qmlproperty string QmlSqlDatabase::connectionName
Sets the connectionName to connectionName
*/
QString QmlSqlDatabase::connectionName() const {
    return m_connectionName;
}

void QmlSqlDatabase::setConnectionName(const QString& connectionName) {
    if (m_connectionName == connectionName)
        return;

    m_connectionName = connectionName ;
    emit connectionNameChanged();
}

/*!
 \qmlmethod QmlSqlDatabase::addDataBase()
Adds a database to the list of database connections using the driver type and the connection name connectionName.
 If there already exists a database connection called connectionName, that connection is removed.
The database connection is referred to by connectionName. The newly added database connection is returned.

If connectionName is not specified, the new connection becomes the default connection for the application,
 and subsequent calls to databaseName without the connection name argument will return the default connection.
If a connectionName is provided here, use database(connectionName) to retrieve the connection.

\b{Warning:} If you add a connection with the same name as an existing connection, the new connection replaces the old one.
If you call this function more than once without specifying connectionName, the default connection will be
the one replaced.


\b{Note:} This function is thread-safe


*/
void QmlSqlDatabase::open() {
    db = QSqlDatabase::addDatabase(m_databaseDriverString, m_connectionName);
    db.setHostName(m_source);
    db.setDatabaseName(m_dbName);
    db.setUserName(m_user);
    db.setPassword(m_password);
    db.setPort(m_port);
    if (!db.open()) {
        sqlError(db.lastError());
        closeRequested(Error, m_connectionName);
        disconnected();
    }
    else {
        connectionOpened(db, m_connectionName);
        m_isConnected = true;
        connected();
    }
}

void QmlSqlDatabase::close() {
    db.close();
    QSqlDatabase::removeDatabase(m_connectionName);
    m_isConnected = false;
    disconnected();
}

bool QmlSqlDatabase::isConnected() const {
    return m_isConnected;
}

/*!
 \qmlproperty string QmlSqlDatabase::connectionNames
returns a list of connection Names that are set on this db

for more info on this please see the source code.
 */
QStringList QmlSqlDatabase::connectionNames() {
    return db.connectionNames();
}


/*!
 \qmlmethod QmlSqlDatabase::removeDatabase()
    Removes the database connection \c connectionName from the list of database connections.

    \b{Warning:} There should be no open queries on the database connection when this method is called, otherwise a resource leak will occur.

    To remove the default connection, which may have been created with a call to addDatabase() not specifying a connection name, you can
    retrieve the default connection name by calling connectionName() on the database returned by database(). Note that if a default database
    hasn't been created an invalid database will be returned.

    \b{Note:} This function is thread-safe
*/
void QmlSqlDatabase::removeDatabase(const QString& connectionName) {
    foreach (QString l, db.connectionNames()) {
        if (l == connectionName) {
            QSqlDatabase::removeDatabase(connectionName);
        }
    }

}

void QmlSqlDatabase::closeAllConnections() {
    foreach (QString l, db.connectionNames()) {
        QSqlDatabase::removeDatabase(l);
    }
}

/*!
 \qmlmethod variant QmlSqlDatabase::tables(string connectionName)

 This returns a list of all the table names on a connection point by there connecitonName

 \b{Note:} This is in alpha and subject to change.

 */
QStringList QmlSqlDatabase::tables(const QString& connectionName,const TableType& tableType) {
    QStringList li;

    for(int i = 0 ; i < db.connectionNames().length(); i++) {
        if (db.connectionNames().at(i) == connectionName) {
            li = db.tables(setTableType(tableType));
        }
        else {
            li = QStringList();
            error(QString("could not find database connection with the  connectionName of %1").arg(connectionName)) ;
        }
    }

    return li;
}

void QmlSqlDatabase::componentComplete() {
    open();
}

void QmlSqlDatabase::handleError(const QString& err) {
    if (m_errorString == err)
        return;

    m_errorString = err;
    emit errorStringChanged();
}

void QmlSqlDatabase::handleOpened(QSqlDatabase database, const QString& connectionName) {
    qDebug() << connectionName  << " Is Open " << database.connectionName();
}

void QmlSqlDatabase::handleCloseRequested(const CloseReason& reason, const QString& connectionName) {
    qDebug() << "connection name has been requested to close " << connectionName << " Reason: " << closeReasonToString(reason) ;
}

void QmlSqlDatabase::handleSqlError(const QSqlError& err) {
    QString er = QString("Error"
                         "\nFull reason: %1"
                         "\nDriver Error Text: %3"
                         "\nDataBase Text Error: %4"
                         "\nNative Code Reason: %2"
                        )
            .arg(err.text())
            .arg(err.nativeErrorCode())
            .arg(err.driverText())
            .arg(err.databaseText())
                 ;
    error(er) ;
}

QSql::TableType QmlSqlDatabase::setTableType(const QmlSqlDatabase::TableType& type) {
    if (type == Tables) {
        return QSql::Tables;
    }
    else if (type == SystemTables) {
        return QSql::SystemTables;
    }
    else if (type == Views) {
        return QSql::Views;
    }
    else if (type == AllTables) {
        return QSql::AllTables;
    }

    error("Warning: Not a vaild Table Type trying with AllTypes");
    return  QSql::AllTables;
}



QString QmlSqlDatabase::closeReasonToString(const QmlSqlDatabase::CloseReason& cR)
{
    if (cR == Error) {
        return "Error";
    }
    else if (cR == Requested) {
        return "User Requested";
    }
    else if (cR == Unknown) {
        return "Unknown";
    }

    return "Unknown and not good";

}


