#ifndef MAP_H
#define MAP_H
#include <QUuid>
#include <QJsonObject>
#include <QVariant>
#include <QMap>

#include "../commonheaders/iparameters.h"
#include "../commonheaders/templist.h"



/**
 * @brief Сущность атрибутов карты
 */
class TMap: public ISerialize{
public:
    TMap();
    ~TMap();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    // getters
    const QUuid& getMapId()const;///< получить идентификатор карты
    const QUuid& getLayerId()const;///< получить идентификатор слоя
    const QUuid& getVarId()const;///< получить идентификатор операции
    const QJsonObject& getAttr()const;///< получить системные параметры
    const QString& getLayerName()const;///< получить название слоя

    // setters
    void setMapId(const QUuid& mapId);///< задать идентификатор карты
    void setLayerId(const QUuid& layerId);///< задать идентификатор слоя
    void setVarId(const QUuid& varId);///< задать идентификатор операции
    void setAttr(const QJsonObject& attr);///< задать атрибуты
    void setLayerName(const QString& name);///< задать название слоя

private:
    QUuid _mapId;
    QUuid _layerId;
    QUuid _varId;
    QJsonObject _attr;
    QString _layername;

};


////////////////////////////////////////////

class TMapItemList: public TemplList<TMap>{
public:
    TMapItemList(){}
    virtual ~TMapItemList(){}

    QList<TMap*>::iterator begin();
    QList<TMap*>::iterator end();
};

#endif // MAP_H
