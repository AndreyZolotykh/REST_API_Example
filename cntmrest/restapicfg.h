#ifndef RESTAPI_H
#define RESTAPI_H

#include <QString>
#include <QMap>
#include <QDir>
#include "dbconfig.h"
#include "../commonheaders/iparameters.h"
class LibApi{
private:
    QString _libName;
    QString _url;
    QString _function;
    EXT_FUNC_COLL_TYPE _ver;
public:
    LibApi(const QString& libName, const QString& url, const QString& function, EXT_FUNC_COLL_TYPE ver);
    ~LibApi();

    const QString& getLibName() const;
    const QString& getUrl() const;
    const QString& getFunction() const;
    EXT_FUNC_COLL_TYPE getVer() const;
    QString toString() const;
};
#define DEFAULT_CFG_PATH "/etc/ncom.cfg.json"
#define ACTUAL_VERSION "0.0.1"
class RestApiCfg{
private:
    QString _pwd;

    QString _host;
    qint16 _port;

    QString _version;
    QString _currentVersion;
    QString _basepath;

    qint32 _maxThreads;
    qint32 _maxQueue;

    QMap<QString,LibApi*> _map;

    DBConfig _dbcfg;
    class Ctrl{
    public:
        Ctrl();
        ~Ctrl();
    };
    friend class Ctrl;

    RestApiCfg(const QString& ver,const QString& pwd);
    ~RestApiCfg();

    static RestApiCfg *_inatance;
    static Ctrl _ctrl;
public:    
    void load();
    void load(const QString& path);

    const LibApi* find(const QString& url) const;

    qint32 getMaxThreads() const;
    qint32 getMaxQueue() const;

    const QString& getVersion() const;
    const QString& getbasePath() const;

    const QString& host() const;
    qint16 port() const;

    const DBConfig& getDBConfig() const;

    static const RestApiCfg* getInstance(const QString& pwd);
    static const RestApiCfg* getInstance();
private:
    void _createDefault(const QString& path);
    int _load(const QString& path);
    void _clear();
};

#endif // RESTAPI_H
