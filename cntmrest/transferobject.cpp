#include "transferobject.h"

TransferObject::TransferObject(REQUEST_TYPE method, const QString &json, const QString &token, const DBConfig &dbcfg){
    _method = method;
    _jsonIn = json;
    _token = token;
    _dbcfg = dbcfg;
    _returnCode = OK;
}
TransferObject::~TransferObject(){

}

REQUEST_TYPE TransferObject::getMethod() const{
    return _method;
}
QString TransferObject::getParam(const QString& key) const{
    return _map[key];
}
bool TransferObject::isParam(const QString& key) const{
    return _map.contains(key);
}
const QString& TransferObject::getInputJSON() const{
    return _jsonIn;
}
const QString& TransferObject::getToken() const{
    return _token;
}
const QString& TransferObject::getDBName()const{return _dbcfg.getDBName();}
const QString& TransferObject::getUserName()const{return _dbcfg.getUserName();}
const QString& TransferObject::getPassword()const{return _dbcfg.getPassword();}
const QString& TransferObject::getDBHost()const{return _dbcfg.getDBHost();}
const QString& TransferObject::getDBPort()const{return _dbcfg.getDBPort();}

void TransferObject::put(const QString& key, const QString& value){
    _map[key]=value;
}

void TransferObject::setReturnCode(RETURN_CODE code){
    _returnCode = code;
}
void TransferObject::setContent(const QString& content){
    _jsonOut = content;
}

RETURN_CODE TransferObject::getReturnCode() const{
    return _returnCode;
}
const QString& TransferObject::getJSON() const {
    return _jsonOut;
}
size_t TransferObject::getLength() const{
    return _jsonOut.length();
}
