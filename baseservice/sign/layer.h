#ifndef LAYER_H
#define LAYER_H
#include <QUuid>

#include "../commonheaders/iparameters.h"
#include "../commonheaders/templist.h"



/**
 * @brief Сущность атрибутов карты
 */
class TLayer: public ISerialize{
public:
    TLayer();
    virtual ~TLayer();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    // getters
    const QUuid& getMapId()const;///< получить идентификатор карты
    const QUuid& getLayerId()const;///< получить идентификатор слоя
    const QUuid& getVarId()const;///< получить идентификатор операции
    const QString& getLayerName()const;///< получить название слоя

    // setters
    void setMapId(const QUuid& mapId);///< задать идентификатор карты
    void setLayerId(const QUuid& layerId);///< задать идентификатор слоя
    void setVarId(const QUuid& varId);///< задать идентификатор операции
    void setLayerName(const QString& name);///< задать название слоя

private:
    QUuid _mapId;
    QUuid _layerId;
    QUuid _varId;
    QString _layername;


public:
    static const char* stJsonField_MapId;// = "mapid";
    static const char* stJsonField_LayerId;// = "layerid";
    static const char* stJsonField_VarId;// = "varid";
    static const char* stJsonField_LayerName;// = "layername";

    static const char* stDBField_MapId;// = "mapid";
    static const char* stDBField_LayerId;// = "layerid";
    static const char* stDBField_VarId;// = "varid";
    static const char* stDBField_LayerName;// = "layername";
};


////////////////////////////////////////////

class TLayerItemList: public TemplList<TLayer>{
public:
    TLayerItemList(){}
    virtual ~TLayerItemList(){}

    QList<TLayer*>::iterator begin();
    QList<TLayer*>::iterator end();
};

#endif // LAYER_H
