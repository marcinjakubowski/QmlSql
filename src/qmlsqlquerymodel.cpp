#include "qmlsqlquerymodel.h"



/*!
   \qmltype QmlSqlQueryModel
   \inqmlmodule QmlSql 1.0
   \ingroup QmlSql
   \inherits QObject
   \brief The QmlSqlQueryModel object provides a read-only data model for SQL result sets.

   \b{Note:} When you destroy your object this also destroys the the instance of QmlSqlQueryModel, thus everytime it gets recreated. Some people may wish to put this object in a qml file that never gets destoryed till the application is destroyed.

QmlSqlQueryModel is a high-level interface for executing SQL statements and traversing the result set. It is built on top of the lower-level QSqlQuery and can be used to provide data to views such as ListView. For example
Say that we have a QmlSqlDatabase open with te connectionName of "master-connection"
\code
    ListView{
        model: sqlModel
    }
    QmlSqlQueryModel{
        id: sqlModel
        connectionName: "master-connection"
        queryString: "SELECT name, salary FROM employee"
        Component.onCompleted:{
            exec()
        }
    }

\endcode


The model is read-only by default. To make it writable you can use QmlSqlQuery but in the near future there will be a option to set it as writable.

The querymodel example illustrates how to use QmlSqlQueryModel to display the result of a query.

 */


QmlSqlQueryModel::QmlSqlQueryModel(QObject *parent) :
    QSqlQueryModel(parent),
    m_readOnly(true)
{

}


/*!
  \qmlproperty string QmlSqlQueryModel::connectionName
   Set the connectionName to connectionName. This is used on a open QmlSqlDatabase,
 */
QString QmlSqlQueryModel::connectionName() const
{
    return m_connectionName;
}

void QmlSqlQueryModel::setConnectionName(const QString &connectionName)
{
    if ( m_connectionName == connectionName )
        return;
    m_connectionName = connectionName ;
    emit connectionNameChanged();
}

/*!
  \qmlproperty string QmlSqlQueryModel::queryString
  Resets the model and sets the data provider to be the given query.

  errorString can be used to retrieve verbose information if there was an error setting the query.

\sa errorString
*/
QString QmlSqlQueryModel::queryString() const
{
    return  m_queryString;
}

void QmlSqlQueryModel::setQueryString(const QString &queryString)
{
    if ( m_queryString == queryString )
        return;
    m_queryString = queryString ;
    emit queryStringChanged();
}


QStringList QmlSqlQueryModel::rolesList() const
{
    return m_roleList;
}


/*!
 \qmlproperty string QmlSqlQueryModel::errorString
    Returns information about the last error that occurred on the database.
\sa exec()
*/
QString QmlSqlQueryModel::errorString() const
{
    return m_error;
}

// this is not yet implemented
bool QmlSqlQueryModel::readOnly() const
{
    return m_readOnly;
}
// this is not yet implemented
void QmlSqlQueryModel::setReadOnly(const bool &readOnly)
{
    if(m_readOnly == readOnly)
        return;
    m_readOnly = readOnly;
    emit readOnlyChanged();
}

/*!
 \qmlmethod void QmlSqlQueryModel::exec()
 Fills or refils the model based on the queryString that one sets. If there is a error one can use errorString or its signal onErrorStringChaned to gather information about that error

 \sa queryString , errorString
*/
void QmlSqlQueryModel::exec()
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    QSqlQueryModel::setQuery(m_queryString, db);
    if ( this->lastError().isValid() )
    {
        error(  parseError( this->lastError().type() ) );
        return ;
    }
    else
    {
        roleNames();
    }
}

void QmlSqlQueryModel::clearModel()
{
    this->clear();
}

QHash<int, QByteArray>QmlSqlQueryModel::roleNames() const
{
    QHash<int,QByteArray> hash;
    for( int i = 0; i < record().count(); i++)
    {
        hash.insert(Qt::UserRole + i + 1, QByteArray(record().fieldName(i).toLatin1()));
    }
    return hash;

}

QString QmlSqlQueryModel::parseError(const QSqlError::ErrorType &mError)
{
    if ( mError == QSqlError::NoError ){ return "No Error"; }
    else if ( mError == QSqlError::ConnectionError){ return "Connection Error"; }
    else if ( mError == QSqlError::StatementError){ return "Statement Error "; }
    else if ( mError == QSqlError::TransactionError){ return "TransactionError"; }
    else if ( mError == QSqlError::UnknownError){ return "Unknown Error"; }
    else {return QString();}
}

void QmlSqlQueryModel::handelErrorString(const QString &errorString)
{
    if(m_error == errorString)
        return;
    m_error = errorString;
    emit errorStringChanged();
}


// set up the model
QVariant QmlSqlQueryModel::data(const QModelIndex &index, int role)const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole)
    {
        value = QSqlQueryModel::data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}
