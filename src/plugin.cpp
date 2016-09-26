#include "plugin.h"
#include "qmlsqldatabase.h"
#include "qqmlsqlquery.h"
#include "qmlsqlquerymodel.h"
#include "qmlsqlcreatedatabase.h"
#include <qqml.h>

void QQmlSqlPlugin::registerTypes(const char *uri)
{
    // @uri QmlSql
    qmlRegisterType<QmlSqlDatabase>(uri, 1, 0, "QmlSqlDatabase");
    qmlRegisterType<QQmlSqlQuery>(uri,1,0,"QmlSqlQuery");
    qmlRegisterType<QmlSqlQueryModel>(uri,1,0,"QmlSqlQueryModel");
    qmlRegisterType<QmlSqlCreateDatabase>(uri,1,0,"QmlSqlCreateDatabase");
}



