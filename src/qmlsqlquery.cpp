#include "qmlsqlquery.h"
#include <QStringBuilder>

#include "qmlsqldatabase.h"


/*!
   \qmltype QQmlSqlQuery
   \inqmlmodule QmlSql 1.0
   \ingroup QmlSql
   \inherits QObject
   \brief The QmlSqlQuery class provides a means of executing and manipulating SQL statements.

QmlSqlQuery encapsulates the functionality involved in creating, navigating and retrieving data from SQL queries which are executed on a QmlSqlDatabase. It can be used to execute DML (data manipulation language) statements, such as SELECT, INSERT, UPDATE and DELETE, as well as DDL (data definition language) statements, such as CREATE TABLE. It can also be used to execute database-specific commands which are not standard SQL (e.g. SET DATESTYLE=ISO for PostgreSQL).

Example:  say that you have a QmlSqlDatabase with the connectionName of "master-connection"

\code
    QmlSqlQuery{
        connectionName: "master-connection"
        queryString: "SELECT * FROM SOMETABLE"
        onDone: {
            console.log(lastQueryOutPut)
        }
    }

\endcode


\b{Warning:} You must load the SQL driver via QmlSqlDatabase and open the connection before a
\c QmlSqlQuery is created. Also, the connection must remain open while the query exists.
Otherwise, the behavior of \c QmlSqlQuery is undefined.


\sa QmlSqlDatabase , QmlSqlQueryModel
*/

QmlSqlQuery::QmlSqlQuery(QObject *parent)
    : QObject(parent), m_database(nullptr)
{
    connect(this, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
}

int QmlSqlQuery::rowsAffected() const {
    return m_rowsAffected;
}

QmlSqlDatabase* QmlSqlQuery::database() const {
    return m_database;
}

void QmlSqlQuery::setDatabase(QmlSqlDatabase *database) {
    if (database == m_database)
        return;

    if (m_database != nullptr)
        disconnect(m_database, SIGNAL(connected()), this, SLOT(exec()));

    m_database = database;
    connect(m_database, SIGNAL(connected()), this, SLOT(exec()));

    if (m_database->isConnected())
        exec();

    emit databaseChanged();
}

void QmlSqlQuery::setRowsAffected(int rowsAffected) {
    if (m_rowsAffected == rowsAffected)
        return;
    m_rowsAffected = rowsAffected ;
    emit rowsAffectedChanged();
}

/*!
  \qmlproperty string QQmlSqlQuery::queryString
    Sets the query to queryString that will be run on the connected QmlSqlDatabase via a connectionName.
 It is highly suggested to use a connectionName when running a query,

    \sa connectionName , QmlSqlDatabase
 */
QString QmlSqlQuery::queryString() const {
    return m_queryString;
}

void QmlSqlQuery::setQueryString(const QString& queryString) {
    if (m_queryString == queryString)
        return;
    m_queryString = queryString ;
    emit queryStringChanged();
}

/*!
  \qmlproperty string QQmlSqlQuery::lastQuery
    Returns the text of the current query (queryString) being used, or an empty string if there is no current query text.

\sa queryString , lastQueryOutPut
*/
QString QmlSqlQuery::lastQuery() const {
    return m_lastQuery;
}

void QmlSqlQuery::setLastQuery(const QString& lastQuery) {
    if (m_lastQuery == lastQuery)
        return;
    m_lastQuery = lastQuery ;
    emit lastQueryChanged();
}

/*!
  \qmlproperty string QQmlSqlQuery::lastQueryOutPut
    Returns the text from the last query that was ran. If there was a error one can use errorString to retrive more information about the error,
\sa errorString
*/
QString QmlSqlQuery::lastQueryOutput() const {
    return m_lastQueryOutput;
}

void QmlSqlQuery::setLastQueryOutput(const QString& lastQueryOutput) {
    if (m_lastQueryOutput == lastQueryOutput)
        return;
    m_lastQueryOutput = lastQueryOutput ;
    emit lastQueryOutputChanged();
}

/*!
  \qmlproperty string QQmlSqlQuery::errorString
Returns error information about the last error (if any) that occurred with this query.
 */
QString QmlSqlQuery::errorString() const {
    return m_errorString;
}

void QmlSqlQuery::setErrorString(const QString& errorString) {
    if (m_errorString == errorString)
        return;
    m_errorString = errorString ;
    emit errorStringChanged();
}


/*!
  \qmlmethod void QQmlSqlQuery::exec()
  Executes a previously prepared SQL query (queryString). on a connected QmlSqlDatabase via connectionName.

  \b{Note} That the last error for this query is not reset when exec() is called.
 */
void QmlSqlQuery::exec() {
    execWithQuery(m_database->connectionName(), m_queryString);
}

/*!
  \qmlmethod void QQmlSqlQuery::execWithQuery(const QString& connectionName, const QString& query)
  Method that is used to execute SQL query. This method takes in two strings the first one being
the \c connectionName and the second one being the SQL \c queryString that you wish to run.

Example:

\code

    Column {
        anchors.fill: parent
        spacing: 1
        TextArea{
            id: output
            width:parent.width
            height : parent.height - (queryTrigger.height * 2 + 4)
        }
        Button{
            text: "Clear"
            width:parent.width
            onClicked{
                output.text = ""
            }
        }
        Button{
            id: queryTrigger
            width:parent.width
            text: "Run SELECT * FROM SOMETABLE"
            onClicked{
                dbQueryRunner("master-connection",  "SELECT * FROM SOMETABLE")
            }
        }
    }

    QmlSqlQuery{
        id: dbQueryRunner
        onErrorStringChanged:{
            output.color = red
            output.text = errorString
            output.color = black
        }
        onDone: {
            output.text += lastQueryOutPut
        }
    }

\endcode

  \b{Note} The last error for this query is not reset when execWithQuery() is called.

 \sa QmlSqlDatabase, exec(), connectionName

*/
void QmlSqlQuery::execWithQuery(const QString& connectionName, const QString& query) {
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QSqlQuery db_query(db);
    db_query.prepare(query);

    if (!db_query.exec())
    {
        QString er = QString("could not run query of %1 Reason: %2").arg(query).arg(db_query.lastError().text());
        error(er);
        return ;
    }

    if (db_query.lastError().type() != QSqlError::NoError) {
        error(db_query.lastError().text());
        return;
    }

    if (db_query.isSelect()) {
        QString output;
        QSqlRecord rec = db_query.record();
        const int columnCount = rec.count() - 1 ;
        while (db_query.next()) {
            for (int i=0; i<=columnCount; i++) {
                output.append(db_query.value(i).toString() % '\t');
            }
            output.append('\n');
        }
        setLastQueryOutput(output);
    }
    else {
        setRowsAffected(db_query.numRowsAffected());
    }
    setErrorString(QString());
    emit done();
}

void QmlSqlQuery::handleError(const QString& err) {
    setErrorString(err);
}
