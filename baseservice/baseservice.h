#ifndef BASESERVICE_H
#define BASESERVICE_H

#include <QtCore/qglobal.h>

#if defined(BASESERVICE_LIBRARY)
#  define BASESERVICESHARED_EXPORT Q_DECL_EXPORT
#else
#  define BASESERVICESHARED_EXPORT Q_DECL_IMPORT
#endif

#include "../commonheaders/iparameters.h"

extern "C" {
// Работа с каталогом ВВТ
BASESERVICESHARED_EXPORT int dicVVT(AbstructTransferObject** obj);
// Работа с каталогом операций
BASESERVICESHARED_EXPORT int dicVariant(AbstructTransferObject** obj);
// Работа с ОШС
BASESERVICESHARED_EXPORT int dicOSHS(AbstructTransferObject** obj);
// Обработка ВВТ ОШС
BASESERVICESHARED_EXPORT int dicOSHSVVT(AbstructTransferObject** obj);
// Обработка словаря районов и рубежей
BASESERVICESHARED_EXPORT int dicDeployAreas(AbstructTransferObject** obj);
// Получение/нанисение/удаление знаков ОО
BASESERVICESHARED_EXPORT int sign(AbstructTransferObject** obj);
// Получение/нанисение/удаление атрибутов карты
BASESERVICESHARED_EXPORT int map(AbstructTransferObject** obj);
// Получение/нанисение/удаление атрибутов слоя
BASESERVICESHARED_EXPORT int layer(AbstructTransferObject** obj);
// Работа с маршами
BASESERVICESHARED_EXPORT int march(AbstructTransferObject** obj);
BASESERVICESHARED_EXPORT int dicMarch(AbstructTransferObject** obj);
BASESERVICESHARED_EXPORT int dicDicRegroupingParamPt(AbstructTransferObject** obj);
// работа с атрибутами ВВТ
BASESERVICESHARED_EXPORT int dicAttr(AbstructTransferObject** obj);
BASESERVICESHARED_EXPORT int dicAttrVVT(AbstructTransferObject** obj);
}

QString serializeObjToString(ISerialize *obj);

#endif // BASESERVICE_H
