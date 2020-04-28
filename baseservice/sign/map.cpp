#include "map.h"

TMap::TMap(){;}
TMap::~TMap(){;}


const char* txtMapMapId = "mapid";
const char* txtMapLayerId = "layerid";
const char* txtMapVarId = "varid";
const char* txtMapAttr = "attr";
const char* txtMapLayerName = "layername";
bool TMap::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();


        QJsonValue vMapId = mainObj.value(QString(txtMapMapId));
        QJsonValue vLayerId = mainObj.value(QString(txtMapLayerId));
        QJsonValue vVarId = mainObj.value(QString(txtMapVarId));
        QJsonValue vAttr = mainObj.value(QString(txtMapAttr));
        QJsonValue vLayerName = mainObj.value(QString(txtMapLayerName));

        if(!vMapId.isNull() && vMapId.isString()){
            _mapId = QUuid::fromString(vMapId.toString());
        }

        if(!vLayerId.isNull() && vLayerId.isString()){
            _layerId = QUuid::fromString(vLayerId.toString());
        }

        if(!vVarId.isNull() && vVarId.isString()){
            _varId = QUuid::fromString(vVarId.toString());
        }

        if(!vLayerName.isNull() && vLayerName.isString()){
            _layername = vLayerName.toString();
        }

        if(!vAttr.isNull() && vAttr.isObject()){
            _attr = vAttr.toObject();
        }
    }
    return ret;

}
QJsonObject TMap::toJson(){
    QJsonObject ret;

    ret.insert(QString(txtMapMapId),_mapId.toString());
    ret.insert(QString(txtMapLayerId),_layerId.toString());
    ret.insert(QString(txtMapVarId),_varId.toString());
    ret.insert(QString(txtMapAttr),_attr);
    ret.insert(QString(txtMapLayerName),_layername);

    return ret;
}


const QUuid& TMap::getMapId()const{return _mapId;}
const QUuid& TMap::getLayerId()const{return _layerId;}
const QUuid& TMap::getVarId()const{return _varId;}
const QJsonObject& TMap::getAttr()const{return _attr;}
const QString& TMap::getLayerName()const{return _layername;}


void TMap::setMapId(const QUuid& mapId){_mapId=mapId;}
void TMap::setLayerId(const QUuid& layerId){_layerId=layerId;}
void TMap::setVarId(const QUuid& varId){_varId=varId;}
void TMap::setAttr(const QJsonObject& attr){_attr = attr;}
void TMap::setLayerName(const QString& name){_layername = name;}

//////////////////////////////////
QList<TMap*>::iterator TMapItemList::begin(){return _items.begin();}
QList<TMap*>::iterator TMapItemList::end(){return _items.end();}
