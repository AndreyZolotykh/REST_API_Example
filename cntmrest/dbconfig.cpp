#include "dbconfig.h"

DBConfig::DBConfig(const QString& dbname, const QString& username, const QString& password, const QString& host, const QString& port){
    _dbname = dbname;
    _username = username;
    _password = password;
    _host = host;
    _port = port;
}
DBConfig::DBConfig(const DBConfig& obj){
    _dbname = obj._dbname;
    _username = obj._username;
    _password = obj._password;
    _host = obj._host;
    _port = obj._port;
}
DBConfig& DBConfig::operator =(const DBConfig& obj){
    _dbname = obj._dbname;
    _username = obj._username;
    _password = obj._password;
    _host = obj._host;
    _port = obj._port;
    return *this;
}
DBConfig::~DBConfig(){;}

const QString& DBConfig::getDBName()const{return _dbname;}
const QString& DBConfig::getUserName()const{return _username;}
const QString& DBConfig::getPassword()const{return _password;}
const QString& DBConfig::getDBHost()const{return _host;}
const QString& DBConfig::getDBPort()const{return _port;}

void DBConfig::setDBName(const QString& v){_dbname = v;}
void DBConfig::setUserName(const QString& v){_username = v;}
void DBConfig::setPassword(const QString& v){_password = v;}
void DBConfig::setDBHost(const QString& v){_host = v;}
void DBConfig::setDBPort(const QString& v){_port = v;}
