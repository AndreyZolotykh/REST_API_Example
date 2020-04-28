#include "dbconnection.h"
#include <QDebug>
#include <QSqlQuery>
#include "../commonheaders/iparameters.h"


void conv(const AbstructTransferObject *p,QMap<QString,QString>& mp){
    mp[DB_NAME] = p->getDBName();
    mp[DB_USERNAME] = p->getUserName();
    mp[DB_PASSWORD] = p->getPassword();
    mp[DB_HOST] = p->getDBHost();
    mp[DB_PORT] = p->getDBPort();
}
QSqlDatabase getConnection(const QString& connectionname,const AbstructTransferObject *p){
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",connectionname);
    db.setDatabaseName(p->getDBName());
    db.setUserName(p->getUserName());
    db.setPassword(p->getPassword());
    db.setHostName(p->getDBHost());
    db.setPort(p->getDBPort().toInt());
    return db;
}
QSqlDatabase getConnection(const QString& connectionname,const QMap<QString,QString>& p){
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",connectionname);
    db.setDatabaseName(p[DB_NAME]);
    db.setUserName(p[DB_USERNAME]);
    db.setPassword(p[DB_PASSWORD]);
    db.setHostName(p[DB_HOST]);
    db.setPort(p[DB_PORT].toInt());
    return db;
}
QSqlQuery * createQuery(QSqlDatabase& conn, bool &bOpen){
    if(!conn.isOpen()){
        if(!conn.open()){
            qWarning()<<"Error while open db: "<<conn.connectionName();
            return NULL;
        }
        bOpen = true;
    }
    return new QSqlQuery(conn);
}
