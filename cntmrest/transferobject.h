#ifndef TRANSFEROBJECT_H
#define TRANSFEROBJECT_H
#include <QMap>
#include <QString>
#include "dbconfig.h"
#include "../commonheaders/iparameters.h"

class TransferObject : public AbstructTransferObject{
private:
    REQUEST_TYPE _method;
    QMap<QString,QString> _map;
    QString _jsonIn;
    QString _token;

    RETURN_CODE _returnCode;
    QString _jsonOut;


    DBConfig _dbcfg;
public:
    TransferObject(REQUEST_TYPE method, const QString &json, const QString &token, const DBConfig &dbcfg);
    virtual ~TransferObject();

    REQUEST_TYPE getMethod() const;
    QString getParam(const QString& key) const;
    bool isParam(const QString& key) const;
    const QString& getInputJSON() const;
    const QString& getToken() const;

    const QString& getDBName()const;
    const QString& getUserName()const;
    const QString& getPassword()const;
    const QString& getDBHost()const;
    const QString& getDBPort()const;

    RETURN_CODE getReturnCode() const;
    const QString& getJSON() const;    
    size_t getLength() const;


    void setReturnCode(RETURN_CODE code);
    void setContent(const QString& content);

    void put(const QString& key, const QString& value);
};

#endif // TRANSFEROBJECT_H
