#ifndef DBCONFIG_H
#define DBCONFIG_H


#include "../commonheaders/iparameters.h"

class DBConfig {
private:
    QString _dbname;
    QString _username;
    QString _password;
    QString _host;
    QString _port;
public:
    explicit DBConfig(){;}
    DBConfig(const QString& dbname, const QString& username, const QString& password, const QString& host, const QString& port);
    DBConfig(const DBConfig& obj);
    DBConfig& operator =(const DBConfig& obj);

    ~DBConfig();

    const QString& getDBName()const;
    const QString& getUserName()const;
    const QString& getPassword()const;
    const QString& getDBHost()const;
    const QString& getDBPort()const;

    void setDBName(const QString& v);
    void setUserName(const QString& v);
    void setPassword(const QString& v);
    void setDBHost(const QString& v);
    void setDBPort(const QString& v);
};

#endif // DBCONFIG_H
