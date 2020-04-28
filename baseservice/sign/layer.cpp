#include "layer.h"

TLayer::TLayer(){;}
TLayer::~TLayer(){;}

const char* TLayer::stJsonField_MapId = "mapid";
const char* TLayer::stJsonField_LayerId = "layerid";
const char* TLayer::stJsonField_VarId = "varid";
const char* TLayer::stJsonField_LayerName = "layername";

const char* TLayer::stDBField_MapId = "mapid";
const char* TLayer::stDBField_LayerId = "layerid";
const char* TLayer::stDBField_VarId = "varid";
const char* TLayer::stDBField_LayerName = "layername";
//const char* txtLayerMapId = "mapid";
//const char* txtLayerLayerId = "layerid";
//const char* txtLayerVarId = "varid";
//const char* txtLayerLayerName = "layername";
bool TLayer::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();


        QJsonValue vMapId = mainObj.value(QString(TLayer::stJsonField_MapId));
        QJsonValue vLayerId = mainObj.value(QString(TLayer::stJsonField_LayerId));
        QJsonValue vVarId = mainObj.value(QString(TLayer::stJsonField_VarId));
        QJsonValue vLayerName = mainObj.value(QString(TLayer::stJsonField_LayerName));

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

    }
    return ret;

}
QJsonObject TLayer::toJson(){
    QJsonObject ret;

    ret.insert(QString(TLayer::stJsonField_MapId),_mapId.toString());
    ret.insert(QString(TLayer::stJsonField_LayerId),_layerId.toString());
    ret.insert(QString(TLayer::stJsonField_VarId),_varId.toString());
    ret.insert(QString(TLayer::stJsonField_LayerName),_layername);

    return ret;
}


const QUuid& TLayer::getMapId()const{return _mapId;}
const QUuid& TLayer::getLayerId()const{return _layerId;}
const QUuid& TLayer::getVarId()const{return _varId;}
const QString& TLayer::getLayerName()const{return _layername;}


void TLayer::setMapId(const QUuid& mapId){_mapId=mapId;}
void TLayer::setLayerId(const QUuid& layerId){_layerId=layerId;}
void TLayer::setVarId(const QUuid& varId){_varId=varId;}
void TLayer::setLayerName(const QString& name){_layername = name;}

//////////////////////////////////
QList<TLayer*>::iterator TLayerItemList::begin(){return _items.begin();}
QList<TLayer*>::iterator TLayerItemList::end(){return _items.end();}
