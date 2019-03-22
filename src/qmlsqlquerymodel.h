#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>

#include <QDebug>
#include <QVariant>
#include <QStringList>


class QmlSqlDatabase;

class QmlSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT

    Q_PROPERTY(QmlSqlDatabase* database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(QString queryString READ queryString WRITE setQueryString NOTIFY queryStringChanged)
    Q_PROPERTY(QStringList rolesList READ rolesList  NOTIFY rolesListChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)


public:
    explicit QmlSqlQueryModel(QObject *parent = nullptr);

    QString queryString() const;
    void setQueryString(const QString& queryString);

    QmlSqlDatabase* database() const;
    void setDatabase(QmlSqlDatabase* database);

    QStringList rolesList() const;

    QString errorString() const;

    bool readOnly() const;
    void setReadOnly(bool readOnly);

     Q_INVOKABLE void clearModel();
     QVariant data(const QModelIndex& index, int role) const;
     QHash<int, QByteArray>roleNames() const;
     QString parseError(const QSqlError::ErrorType& mError);

public slots:
     void exec();

signals:
    void databaseChanged();
    void queryStringChanged();
    void rolesListChanged();
    void error(const QString err);
    void errorStringChanged();
    void readOnlyChanged();

protected slots:
    void handleErrorString(const QString& errorString);

private:
    QmlSqlDatabase* m_database;
    QString m_queryString;
    QStringList m_roleList;
    QString m_error;
    bool m_readOnly;
};
#endif // QSQLQUERYMODEL_H
