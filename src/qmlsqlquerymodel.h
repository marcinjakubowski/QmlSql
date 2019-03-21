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


class QmlSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT

    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)
    Q_PROPERTY(QString queryString READ queryString WRITE setQueryString NOTIFY queryStringChanged)
    Q_PROPERTY(QStringList rolesList READ rolesList  NOTIFY rolesListChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)


public:
    explicit QmlSqlQueryModel(QObject *parent = nullptr);
    QString connectionName() const;
    void setConnectionName(const QString& connectionName);

    QString queryString() const;
    void setQueryString(const QString& queryString);

    QStringList rolesList() const;

    QString errorString() const;

    bool readOnly() const;
    void setReadOnly(bool readOnly);


     Q_INVOKABLE void exec();
     Q_INVOKABLE void clearModel();
     QVariant  data(const QModelIndex& index, int role)const;
     QHash<int, QByteArray>roleNames() const;
     QString parseError(const QSqlError::ErrorType& mError);

signals:
     void compleated();
    void connectionNameChanged();
    void queryStringChanged();
    void rolesListChanged();
    void error(const QString err);
    void errorStringChanged();
    void readOnlyChanged();

protected slots:
    void handelErrorString(const QString& errorString);

private:
    QString m_connectionName;
    QString m_queryString;
    QStringList m_roleList;
    QString m_error;
    bool m_readOnly;
};
#endif // QSQLQUERYMODEL_H
