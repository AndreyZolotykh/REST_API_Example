#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QJsonValue>
#include <QUuid>


#include "../commonheaders/enums.h"

namespace UTILS{
OWNER_TYPE fromStringOwner(const QString& s);
QString toStringOwner(OWNER_TYPE t);

SUB_TYPE fromStringSub(const QString& s);
QString toStringSub(SUB_TYPE t);

bool parseUUID(const QJsonValue& v, QUuid& res);

}

#endif // UTILS_H
