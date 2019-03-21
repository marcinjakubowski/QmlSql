#ifndef QMLSQLDATABASE_H
#define QMLSQLDATABASE_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

class QmlSqlDatabase : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged )
    Q_PROPERTY(QString databaseName READ databaseName WRITE setdatabaseName NOTIFY databaseNameChanged)
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)
    Q_PROPERTY(QString  errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(DataBaseDriver databaseDriver READ databaseDriver WRITE setDatabaseDriver NOTIFY databaseDriverChanged)
    Q_PROPERTY(QStringList databaseDriverList READ databaseDriverList NOTIFY databaseDriverListChanged)
    Q_ENUMS(DataBaseDriver)
    Q_ENUMS(TableTypes)

public:
    explicit QmlSqlDatabase(QObject *parent = nullptr);

    enum TableType{ Tables, SystemTables, Views, AllTables };
    enum DataBaseDriver{ Postgres, MySql, OCI, ODBC, DB2, TDS, SQLite, SQLite2, IBase };
    enum CloseReason{ Error, Requested, Unknown  };

    DataBaseDriver databaseDriver()const;
    void setDatabaseDriver(const DataBaseDriver& databaseDriver);
    QStringList databaseDriverList()const;
    void setDatabaseDriverList();

    QString errorString() const;
    QString source() const;
    void setSource(const QString& source);

    QString databaseName() const;
    void setdatabaseName(const QString& databaseName);

    QString user() const;
    void setUser(const QString& user);

    QString password() const;
    void setPassword(const QString& password);

    int port() const;
    void setPort(int port);

    QString connectionName() const;
    void setConnectionName(const QString& connectionName);

    Q_INVOKABLE void addDataBase();
    Q_INVOKABLE QStringList connectionNames();
    Q_INVOKABLE void removeDatabase(const QString& connectionName);
    Q_INVOKABLE void closeAllConnections();
    Q_INVOKABLE QStringList tables(const QString& connectionName,const TableType& tableType);

signals:
    void sourceChanged();
    void databaseNameChanged();
    void userChanged();
    void passwordChanged();
    void portChanged();
    void connectionNameChanged();
    void databaseDriverChanged();
    void databaseDriverListChanged();
    void errorStringChanged();

    //INTERNAL
    void error(QString);
    //passing the conenction name
    void connectionOpened(QSqlDatabase, QString);
    void closeRequested(CloseReason, QString);
    void sqlError(QSqlError);

public slots:
    void handleError(const QString& err);
    void handleOpened(QSqlDatabase database, const QString& connectionName);
    void handleCloseRequested(const CloseReason& reason, const QString& connectionName);
    void handleSqlError(const QSqlError& err);

private:
    QSqlDatabase db;
    QString m_source;
    QString m_dbName;
    QString m_user;
    QString m_password;
    QString m_databaseType;
    int m_port;
    QString m_connectionName;
    DataBaseDriver m_databaseDriver;
    QString m_databaseDriverString;
    QStringList m_databaseDriverList;

    QString m_errorString;

    QSql::TableType setTableType(const QmlSqlDatabase::TableType& type);
    QString closeReasonToString(const CloseReason& cR);
};
#endif //  // QMLSQLDATABASE
