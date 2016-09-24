#include "qqmlsqlquery.h"



/*!
   \qmltype QQmlSqlQuery
   \inqmlmodule QmlSql 1.0
   \ingroup QmlSql
   \inherits QObject
   \brief The QmlSqlQuery class provides a means of executing and manipulating SQL statements.

QmlSqlQuery encapsulates the functionality involved in creating, navigating and retrieving data from SQL queries which are executed on a QmlSqlDatabase. It can be used to execute DML (data manipulation language) statements, such as SELECT, INSERT, UPDATE and DELETE, as well as DDL (data definition language) statements, such as CREATE TABLE. It can also be used to execute database-specific commands which are not standard SQL (e.g. SET DATESTYLE=ISO for PostgreSQL).

Example:  say that you have a QmlSqlDatabase with the connecionName of "master-connection"

\code
    QmlSqlQuery{
        connectionName: "master-connection"
        queryString: "SELECT * FROM SOMETABLE"
        onDone: {
            console.log(lastQueryOutPut)
        }
    }

\endcode


\b{Warning:} You must load the SQL driver and open the connection before a QmlSqlQuery is created. Also, the connection must remain open while the query exists; otherwise, the behavior of QmlSqlQuery is undefined.


\sa QmlSqlDatabase , QmlSqlQueryModel
*/

QQmlSqlQuery::QQmlSqlQuery(QObject *parent)
    : QObject(parent)
{
    connect(this,SIGNAL(error(QString)),this,SLOT(handelError(QString)));
}

int QQmlSqlQuery::rowsAffected() const
{
    return m_rowsAffected;
}

void QQmlSqlQuery::setRowsAffected(const int &rowsAffected)
{
    if ( m_rowsAffected == rowsAffected )
        return;
    m_rowsAffected = rowsAffected ;
    emit rowsAffectedChanged();
}

/*!
  \qmlproperty string QQmlSqlQuery::queryString
    Sets the query to queryString that will be run on the connected QmlSqlDatabase. It is highly suggested to use a connectionName when running a query,

    \sa connectionName , QmlSqlDatabase
 */
QString QQmlSqlQuery::queryString() const
{
    return m_queryString;
}

void QQmlSqlQuery::setQueryString(const QString &queryString)
{
    if ( m_queryString == queryString )
        return;
    m_queryString = queryString ;
    emit queryStringChanged();
}

/*!
  \qmlproperty string QQmlSqlQuery::lastQuery
    Returns the text of the current query being used, or an empty string if there is no current query text.
 */
QString QQmlSqlQuery::lastQuery() const
{
    return m_lastQuery;
}

void QQmlSqlQuery::setLastQuery(const QString &lastQuery)
{
    if ( m_lastQuery == lastQuery )
        return;
    m_lastQuery = lastQuery ;
    emit lastQueryChanged();
}

/*!
  \qmlproperty string QQmlSqlQuery::lastQueryOutPut
    Returns the text from the last query that was ran. If there was a error one can use errorString to retrive more information about the error,
\sa errorString
*/
QString QQmlSqlQuery::lastQueryOutPut() const
{
    return  m_lastQueryOutPut;
}

void QQmlSqlQuery::setLastQueryOutPut(const QString &lastQueryOutPut)
{
    if ( m_lastQueryOutPut == lastQueryOutPut )
        return;
    m_lastQueryOutPut = lastQueryOutPut ;
    emit lastQueryOutPutChanged();
}

/*!
  \qmlproperty string QQmlSqlQuery::connectionName
Sets up the connectionName to match that of the QmlSqlDatabase. It is highly suggested that one uses the connection name.
 */
QString QQmlSqlQuery::connectionName() const
{
    return m_connectionName;
}

void QQmlSqlQuery::setConnectionName(const QString &connectionName)
{
    if ( m_connectionName == connectionName )
        return;
    m_connectionName =  connectionName;
    emit connectionNameChanged();
}

/*!
  \qmlproperty string QQmlSqlQuery::errorString
Returns error information about the last error (if any) that occurred with this query.
 */
QString QQmlSqlQuery::errorString() const
{
    return  m_errorString;
}

void QQmlSqlQuery::setErrorString(const QString &errorString)
{
    if ( m_errorString == errorString )
        return;
    m_errorString = errorString ;
    emit errorStringChanged();
}


/*!
  \qmlmethod void QQmlSqlQuery::exec()
  Executes a previously prepared SQL query.

  Note that the last error for this query is not reset when exec() is called.
 */
void QQmlSqlQuery::exec()
{

    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    QSqlQuery db_query(db);

    db_query.prepare(m_queryString);

    if (!db_query.exec())
    {
        QString er = QString("could not run query of %1 Reson:  %2").arg(m_queryString).arg( db_query.lastError().text() );
        error(er);
    }
    else
    {
        if (db_query.lastError().type() != QSqlError::NoError)
        {
            error(db_query.lastError().text());
        }
       else if (db_query.isSelect())
        {
            m_lastQueryOutPut.clear();
            QSqlRecord rec = db_query.record();
            const int rowCount = rec.count() - 1 ;
            while (db_query.next()){
                for (int i = 0;i<= rowCount ; i++)
                {
                    m_lastQueryOutPut.append(db_query.value(i).toString() + " "  );
                }
            }
            setLastQueryOutPut(m_lastQueryOutPut);
            emit done();
        }
        //INSERT UPDATE ect
        else
        {
            setRowsAffected(db_query.numRowsAffected());
            emit done();
        }
    }
}

/*!
  \qmlmethod void QQmlSqlQuery::execWithQuery(string string)
  Method that is used to execute SQL query. This method takes in two strings the first one being the connectionName and the second one being the SQL query string that you wish to run.

  Note that the last error for this query is not reset when exec() is called.
 */
void QQmlSqlQuery::execWithQuery(const QString connectionName, const QString &query)
{

    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QSqlQuery db_query(db);
    db_query.prepare(query);

    if (!db_query.exec())
    {
        QString er = QString("could not run query of %1 Reson:  %2").arg(m_queryString).arg( db_query.lastError().text() );
        error(er);
    }
    else
    {
        if (db_query.lastError().type() != QSqlError::NoError)
        {
            error( db_query.lastError().text() );
        }
        else if (db_query.isSelect()){
            m_lastQueryOutPut.clear();
            QSqlRecord rec = db_query.record();
            const int rowCount = rec.count() - 1 ;
            while (db_query.next()){
                for (int i = 0;i<= rowCount ; i++)
                {
                    m_lastQueryOutPut.append(db_query.value(i).toString() + " "  );
                }
            }
            setLastQueryOutPut(m_lastQueryOutPut);
            emit done();
        }
        else
        {
            setRowsAffected(db_query.numRowsAffected());
            emit done();
        }
    }
}

void QQmlSqlQuery::handelError(const QString &err)
{
    setErrorString(err);
}
