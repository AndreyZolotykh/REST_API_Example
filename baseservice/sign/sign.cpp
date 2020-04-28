#include "sign.h"

TSign::TSign(){;}
TSign::~TSign(){;}

const char* txtSignId = "id";
const char* txtSignMapId = "mapid";
const char* txtSignLayerId = "layerid";
const char* txtSignVarId = "varid";
const char* txtSignDepId = "depid";
const char* txtSignClassCode = "classcode";
const char* txtSignParam = "param";
const char* txtSignAttr = "attr";
bool TSign::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();

        QJsonValue vId = mainObj.value(QString(txtSignId));
        QJsonValue vMapId = mainObj.value(QString(txtSignMapId));
        QJsonValue vLayerId = mainObj.value(QString(txtSignLayerId));
        QJsonValue vVarId = mainObj.value(QString(txtSignVarId));
        QJsonValue vDepId = mainObj.value(QString(txtSignDepId));
        QJsonValue vClassCode = mainObj.value(QString(txtSignClassCode));
        QJsonValue vParam = mainObj.value(QString(txtSignParam));
        QJsonValue vAttr = mainObj.value(QString(txtSignAttr));


        if(!vId.isNull() && vId.isString()){
            _id = QUuid::fromString(vId.toString());
        }

        if(!vMapId.isNull() && vMapId.isString()){
            _mapId = QUuid::fromString(vMapId.toString());
        }

        if(!vLayerId.isNull() && vLayerId.isString()){
            _layerId = QUuid::fromString(vLayerId.toString());
        }

        if(!vVarId.isNull() && vVarId.isString()){
            _varId = QUuid::fromString(vVarId.toString());
        }

        if(!vDepId.isNull() && vDepId.isString()){
            _depId = QUuid::fromString(vDepId.toString());
        }

        if(!vClassCode.isNull() && vClassCode.isString()){
            _classCode = vClassCode.toString();
        }

        if(!vParam.isNull() && vParam.isObject()){
            _param = vParam.toObject();

        }

        if(!vAttr.isNull() && vAttr.isObject()){
            _attr = vAttr.toObject();
        }
    }
    return ret;

}
QJsonObject TSign::toJson(){
    QJsonObject ret;

    ret.insert(QString(txtSignId),_id.toString());
    ret.insert(QString(txtSignMapId),_mapId.toString());
    ret.insert(QString(txtSignLayerId),_layerId.toString());
    ret.insert(QString(txtSignVarId),_varId.toString());
    ret.insert(QString(txtSignDepId),_depId.toString());
    ret.insert(QString(txtSignClassCode),_classCode);
    ret.insert(QString(txtSignParam),_param);    
    ret.insert(QString(txtSignAttr),_attr);

    return ret;
}

const QUuid& TSign::getId()const{return _id;}
const QUuid& TSign::getMapId()const{return _mapId;}
const QUuid& TSign::getLayerId()const{return _layerId;}
const QUuid& TSign::getVarId()const{return _varId;}
const QUuid& TSign::getDepId()const{return _depId;}
const QString& TSign::getClassCode()const{return _classCode;}
const QJsonObject& TSign::getParam()const{return _param;}
const QJsonObject& TSign::getAttr()const{return _attr;}
const QVariant TSign::getAttr(const QString &key) const{
    if(_attr.contains(key)){
        return _attr[key];
    }
    return QVariant(QVariant::Invalid);
}
void TSign::setId(const QUuid& id){_id = id;}
void TSign::setMapId(const QUuid& mapId){_mapId=mapId;}
void TSign::setLayerId(const QUuid& layerId){_layerId=layerId;}
void TSign::setVarId(const QUuid& varId){_varId=varId;}
void TSign::setDepId(const QUuid& depId){_depId=depId;}
void TSign::setClassCode(const QString& classCode){_classCode = classCode;}
void TSign::setParam(const QJsonObject& param){_param=param;}
void TSign::setAttr(const QString &key, const QString& value){
    check(key);
    _attr.insert(key, value);
}
void TSign::setAttr(const QString &key, int value){
    check(key);
    _attr.insert(key, value);
}
void TSign::setAttr(const QString & key, bool value){
    check(key);
    _attr.insert(key, value);
}
void TSign::check(const QString &key){
    if(_attr.contains(key)){
        _attr.remove(key);
    }
}
void TSign::setAttr(const QJsonObject& attr){
    _attr = attr;
}
void TSign::clearAttr(){
    _attr = QJsonObject();
}
void TSign::removeAttr(const QString & key){
    if(_attr.contains(key)){
        _attr.remove(key);
    }
}
bool TSign::isContainsAttr(const QString & key){return _attr.contains(key);}

//////////////////////////////////
QList<TSign*>::iterator TSignItemList::begin(){return _items.begin();}
QList<TSign*>::iterator TSignItemList::end(){return _items.end();}
