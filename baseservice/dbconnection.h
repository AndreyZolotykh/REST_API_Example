#ifndef DBCONNECTION_H
#define DBCONNECTION_H
#include <QSqlDatabase>
#include <QMap>
#include "../commonheaders/iparameters.h"


QSqlDatabase getConnection(const QString& connectionname,const AbstructTransferObject *p);
QSqlDatabase getConnection(const QString& connectionname,const QMap<QString,QString>& p);
void conv(const AbstructTransferObject *p,QMap<QString,QString>& mp);
QSqlQuery * createQuery(QSqlDatabase& conn, bool &bOpen);
#endif // DBCONNECTION_H
