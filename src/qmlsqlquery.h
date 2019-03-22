#ifndef QQMLSQLQUERY_H
#define QQMLSQLQUERY_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QDebug>
#include <QString>

class QmlSqlDatabase;

class QmlSqlQuery : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QmlSqlDatabase* database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(QString queryString READ queryString WRITE setQueryString NOTIFY queryStringChanged)
    Q_PROPERTY(QString lastQuery READ lastQuery WRITE setLastQuery NOTIFY lastQueryChanged)
    Q_PROPERTY(QString lastQueryOutput READ lastQueryOutput WRITE setLastQueryOutput NOTIFY lastQueryOutputChanged)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged)
    Q_PROPERTY(int rowsAffected READ rowsAffected WRITE setRowsAffected NOTIFY rowsAffectedChanged)

public:
    explicit QmlSqlQuery(QObject *parent = nullptr);

    int rowsAffected() const;
    void setRowsAffected(int rowsAffected);

    QString queryString() const;
    void setQueryString(const QString& queryString);

    QString lastQuery() const;
    void setLastQuery(const QString& lastQuery);

    QmlSqlDatabase* database() const;
    void setDatabase(QmlSqlDatabase* database);

    QString lastQueryOutput() const;
    void setLastQueryOutput(const QString& lastQueryOutput);

    QString errorString() const;
    void setErrorString(const QString& errorString);

    Q_INVOKABLE void execWithQuery(const QString& connectionName, const QString& query);
signals:
    void rowsAffectedChanged();
    void queryStringChanged();
    void lastQueryChanged();
    void lastQueryOutputChanged();
    void errorStringChanged();
    void databaseChanged();
    void error(QString);
    void done();

public slots:
    void exec();
    void handleError(const QString& err);

private:
    QmlSqlDatabase* m_database;
    int m_rowsAffected;
    QString m_queryString;
    QString m_lastQuery;
    QString m_lastQueryOutput;
    QString m_connectionName;
    QString m_errorString;
};

#endif // QQMLSQLQUERY_H
