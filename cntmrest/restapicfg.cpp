#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>
#include <QLibrary>
#include "restapicfg.h"

////////////////////////////////////////////////////////////////////
LibApi::LibApi(const QString& libName, const QString& url, const QString& function, EXT_FUNC_COLL_TYPE ver){
    _libName = libName;
    _url = url;
    _function = function;
    _ver = ver;
}
LibApi::~LibApi(){
    ;
}
const QString& LibApi::getLibName() const{
    return _libName;
}
const QString& LibApi::getUrl() const{
    return _url;
}
const QString& LibApi::getFunction() const{
    return _function;
}
EXT_FUNC_COLL_TYPE LibApi::getVer() const{
    return _ver;
}
QString LibApi::toString() const{
    return "Lib: "+_libName+": "+_url+" -> "+_function;
}
////////////////////////////////////////////////////////////////////

#define HOST_CFG "HostConfig"
#define SERVER_HOST "ServerHost"
#define SERVER_PORT "ServerPort"
#define REST_API "RestAPI"
#define VERSION "Version"
#define BASE_PATH "BasePath"
#define MAX_THREADS "MaxThreads"
#define MAX_QUEUE "MaxQueue"
#define LIB_NAME "LibName"
#define LIB_URL "LibUrl"
#define LIB_FUNC "LibFunc"
#define LIB_VER "LibVer"

#define DB           "DataBase"
//#define DB_NAME      "DBName"
//#define DB_USERNAME  "UserName"
//#define DB_PASSWORD  "Password"
//#define DB_HOST      "DBHost"
//#define DB_PORT      "DBPort"

#define DEF_HOST "ncom"
#define DEF_PORT 9001
#define DEF_MAX_THREADS 16
#define DEF_MAX_QUEUE 100

#define DEF_DB_NAME      "ncomdb"
#define DEF_DB_USERNAME  "postgres"
#define DEF_DB_PASSWORD  "postgres"
#define DEF_DB_HOST      "ncom"
#define DEF_DB_PORT      "5432"

////////////////////////////////////////////////////////////////////
RestApiCfg::RestApiCfg(const QString &ver, const QString& pwd){
    _pwd = pwd;
    _currentVersion = ver;
    _host = DEF_HOST;
    _port = DEF_PORT;
    _maxThreads = DEF_MAX_THREADS;
    _maxQueue = DEF_MAX_QUEUE;
    _basepath ="/api/"+_currentVersion;

    _dbcfg.setDBName(DEF_DB_NAME);
    _dbcfg.setUserName(DEF_DB_USERNAME);
    _dbcfg.setPassword(DEF_DB_PASSWORD);
    _dbcfg.setDBHost(DEF_DB_HOST);
    _dbcfg.setDBPort(DEF_DB_PORT);
#ifdef QT_DEBUG
    qDebug()<<"Create RestApiCfg";
#endif
}
RestApiCfg::~RestApiCfg(){    
    _clear();
#ifdef QT_DEBUG
    qDebug()<<"Delete RestApiCfg";
#endif
}
const DBConfig& RestApiCfg::getDBConfig() const{
    return _dbcfg;
}
void RestApiCfg::_clear(){
    foreach (LibApi *it, _map.values()) {
        delete it;
        it = NULL;
    }
    _map.clear();
}
void RestApiCfg::load(){
    load(DEFAULT_CFG_PATH);
}
void RestApiCfg::load(const QString& path){
    if(_load(path)<0){
        _createDefault(path);
    }
}

const LibApi* RestApiCfg::find(const QString& url) const{
    LibApi* ret = NULL;
    if(_map.contains(url)){
        ret = _map[url];
    }
    return ret;
}

const QString& RestApiCfg::getVersion() const{
    return _version;
}

const QString& RestApiCfg::host() const{
    return _host;
}
qint16 RestApiCfg::port() const{
    return _port;
}
const QString& RestApiCfg::getbasePath() const{
    return _basepath;
}
void RestApiCfg::_createDefault(const QString& path){
    QJsonObject objMain;

    // Create item api lib
    QJsonArray api;

    QJsonObject item;
    item.insert(QString(LIB_NAME),"./libcntminternal");
    item.insert(QString(LIB_URL),"/version");
    item.insert(QString(LIB_FUNC),"getServiceInfoV3");
    item.insert(QString(LIB_VER),V3);
    api.append(item);

    objMain.insert(QString(REST_API),api);

    // DB HOST
    QJsonObject dbCfg;
    dbCfg.insert(QString(DB_NAME),DEF_DB_NAME);
    dbCfg.insert(QString(DB_USERNAME),DEF_DB_USERNAME);
    dbCfg.insert(QString(DB_PASSWORD),DEF_DB_PASSWORD);
    dbCfg.insert(QString(DB_HOST),DEF_DB_HOST);
    dbCfg.insert(QString(DB_PORT),DEF_DB_PORT);
    objMain.insert(QString(DB),dbCfg);

    // CFG HOST
    QJsonObject hostCfg;
    hostCfg.insert(QString(SERVER_HOST),DEF_HOST);
    hostCfg.insert(QString(SERVER_PORT),DEF_PORT);
    objMain.insert(QString(HOST_CFG),hostCfg);

    // SET VERSION
    objMain.insert(QString(VERSION),_currentVersion);

    // SET BASE PATH
    objMain.insert(QString(BASE_PATH),"/api/"+_currentVersion);

    // SET LIMITS
    objMain.insert(QString(MAX_THREADS),_maxThreads);
    objMain.insert(QString(MAX_QUEUE),_maxQueue);

    QJsonDocument doc = QJsonDocument(objMain);
    QFile jsonFile(path);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());
#ifdef QT_DEBUG
    qDebug()<<"Create default config: "<<path;
#endif
}
qint32 RestApiCfg::getMaxThreads() const{
    return _maxThreads;
}
qint32 RestApiCfg::getMaxQueue() const{
    return _maxQueue;
}
const RestApiCfg* RestApiCfg::getInstance(const QString &pwd){
    if(_inatance==NULL){
        _inatance = new RestApiCfg(ACTUAL_VERSION,pwd);
        _inatance->load();
    }
    return getInstance();
}
const RestApiCfg* RestApiCfg::getInstance(){
    return _inatance;
}
int RestApiCfg::_load(const QString& path){
    QString val;
    QFile file;

    _clear();

    file.setFileName(path);
    if(file.exists()){
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        if( !doc.isNull() && !doc.isEmpty() ){
            QJsonObject objMain = doc.object();

            QJsonValue value = objMain.value(QString(VERSION));
            if(value.isString()){
                _version = value.toString();
                if(_version.compare(_currentVersion)!=0){
                    qWarning()<<"Wrong Config Version";
                    return -1;
                }
            }else{
                qWarning()<<"Version not found";
                return -1;
            }

            value = objMain.value(QString(BASE_PATH));
            if(value.isString()){
                _basepath = value.toString();
            }else{
                qWarning()<<"Param "<<BASE_PATH<<" not found, use default value <"<<_basepath<<">";
            }

            value = objMain.value(QString(MAX_THREADS));
            if(value.isDouble()){
                _maxThreads = value.toInt();
            }else{
                qWarning()<<"Param "<<MAX_THREADS<<" not found, use default value <"<<_maxThreads<<">";
            }

            value = objMain.value(QString(MAX_QUEUE));
            if(value.isDouble()){
                _maxQueue = value.toInt();
            }else{
                qWarning()<<"Param "<<MAX_QUEUE<<" not found, use default value <"<<_maxQueue<<">";
            }

            value = objMain.value(QString(DB));
            if(value.isObject()){
                QJsonObject hostConfig = value.toObject();
                QJsonValue vDBNane = hostConfig.value(QString(DB_NAME));
                QJsonValue vUserName = hostConfig.value(QString(DB_USERNAME));
                QJsonValue vPassword = hostConfig.value(QString(DB_PASSWORD));
                QJsonValue vDBHost = hostConfig.value(QString(DB_HOST));
                QJsonValue vDBPort = hostConfig.value(QString(DB_PORT));
                try{
                    _dbcfg.setDBName(vDBNane.toString());
                    _dbcfg.setUserName(vUserName.toString());
                    _dbcfg.setPassword(vPassword.toString());
                    _dbcfg.setDBHost(vDBHost.toString());
                    _dbcfg.setDBPort(vDBPort.toString());
                }catch(...){
                    qWarning()<<"Error db config data";
                }
            }else{
                qWarning()<<"Error db config data";
            }

            value = objMain.value(QString(HOST_CFG));
            if(value.isObject()){
                QJsonObject hostConfig = value.toObject();
                QJsonValue valueServerHost = hostConfig.value(QString(SERVER_HOST));
                QJsonValue valueServerPort = hostConfig.value(QString(SERVER_PORT));
                if(!valueServerHost.isNull() && !valueServerPort.isNull()){
                    if(valueServerHost.isString()){
                        _host = valueServerHost.toString();
                    }
                    if(valueServerPort.isDouble()){
                        _port = valueServerPort.toInt();
                    }
                }
            }else{
                qWarning()<<"Error host config data";
            }




            value = objMain.value(QString(REST_API));
            if(value.isArray()){
                QJsonArray array = value.toArray();
                qInfo()<<"Load:";
                foreach (QJsonValue it, array) {
                    if(it.isObject()){
                        QJsonObject lib = it.toObject();
                        QJsonValue name = lib.value(QString(LIB_NAME));
                        QJsonValue url = lib.value(QString(LIB_URL));
                        QJsonValue func = lib.value(QString(LIB_FUNC));
                        QJsonValue ver = lib.value(QString(LIB_VER));
                        if(name.isString() && url.isString() && func.isString()){
                            EXT_FUNC_COLL_TYPE v = V3;
                            try{
                                if(ver.isDouble()){
                                    v = (EXT_FUNC_COLL_TYPE)(ver.toInt());
                                }
                            }catch(...){
                                 qWarning()<<"Wrong function version set default "<<QString::number(v);
                            }
                            QString lname = _pwd+QDir::separator()+name.toString();
                            QFile f(lname+".so");
                            if(f.exists()){
                                LibApi* newItem = new LibApi(lname,url.toString(),func.toString(),v);

                                QLibrary *lib = new QLibrary(newItem->getLibName());
                                if(!lib->load()){
                                    qWarning()<<"\tError while load "<<name.toString()<<", "<<lib->errorString();
                                    delete newItem;
                                }else{
                                    _map[_basepath+newItem->getUrl()] = newItem;
                                    qInfo()<<"\t"<<newItem->toString();
                                }
                                delete lib;
                            }else{
                                qWarning()<<"\tLib "<<lname<<" not exists";
                            }
                        }else{
                            qWarning()<<"\tWrong item: "<<it;
                        }
                    }else{
                        qWarning()<<"Wrong format config: "<<it;
                    }
                }
                qInfo()<<"BasePath: "<<_basepath;
            }
        }else{
            return -1;
        }
    }else{
        return -1;
    }
    qInfo()<<"Load configuration";
    return 0;
}
/////////////////////////////////////////////
RestApiCfg* RestApiCfg::_inatance = NULL;
RestApiCfg::Ctrl RestApiCfg::_ctrl;

RestApiCfg::Ctrl::Ctrl(){
//    _inatance = new RestApiCfg(ACTUAL_VERSION);
//    _inatance->load();
}
RestApiCfg::Ctrl::~Ctrl(){
    if(_inatance!=NULL){
        delete _inatance;
    }
}
