#include "march.h"
#include <algorithm>
#include "../utils/utils.h"

const char* TMarch::stStatus_NEW = "NEW";
const char* TMarch::stStatus_SOLVED = "SOLVED";
const char* TMarch::stStatus_ERR = "ERR";
const char* TMarch::stStatus_PROC = "PROC";

const char* TMarch::stMODE_FROM = "MODE_FROM";
const char* TMarch::stMODE_TO = "MODE_TO";
const char* TMarch::stMODE_DIRECT = "MODE_DIRECT";

const char* TMarch::stJsonField_id = "id";
const char* TMarch::stJsonField_varid = "varid";
const char* TMarch::stJsonField_title = "title";
const char* TMarch::stJsonField_mainDepId = "mainDepId";
const char* TMarch::stJsonField_status = "status";
const char* TMarch::stJsonField_errCode = "errCode";
const char* TMarch::stJsonField_errDescr = "errDescr";
const char* TMarch::stJsonField_deployAreaStartId = "deployAreaStartId";
const char* TMarch::stJsonField_deployAreaFinishId = "deployAreaFinishId";
const char* TMarch::stJsonField_mode = "mode";
const char* TMarch::stJsonField_tbegin = "tbegin";
const char* TMarch::stJsonField_tend = "tend";
const char* TMarch::stJsonField_param = "param";
const char* TMarch::stJsonField_paths = "units";
const char* TMarch::stJsonField_milestones = "milestones";
const char* TMarch::stJsonField_result = "result";
const char* TMarch::stJsonField_ownerid = "ownerid";

const char* TMarch::stDBField_id = "id";
const char* TMarch::stDBField_varid = "varid";
const char* TMarch::stDBField_title = "title";
const char* TMarch::stDBField_mainDepId = "maindepid";
const char* TMarch::stDBField_status = "status";
const char* TMarch::stDBField_errCode = "errcode";
const char* TMarch::stDBField_deployAreaStartId = "deployareastartid";
const char* TMarch::stDBField_deployAreaFinishId = "deployareafinishid";
const char* TMarch::stDBField_content = "content";


const char* TMarch::errDescriptions[] = {
    "OK",                       // - 0
    "Не верный код ошибки"      // - 1
};
const quint32 TMarch::MAX_ERR_CODE = 2;
// ----------------------------------------------------------------------
TMarch::TMarch(){
    _status = UNDEF;
    _mode = MODE_FROM;
    _errCode = OK;
    _result = NULL;    
}
TMarch::~TMarch(){
    foreach (TMarchParam* it, _param) {
        delete it;
    }
    _param.clear();

    foreach (TMarchPath* it, _paths) {
        delete it;
    }
    _paths.clear();

    foreach (TMarchMilestone* it, _milestones){
        delete it;
    }
    _milestones.clear();

    if(_result != NULL) delete _result;
}
void TMarch::setMode(const QString& v){
    if(v.compare(stMODE_FROM)==0){
        _mode = MODE_FROM;
    }else if(v.compare(stMODE_TO)==0){
        _mode = MODE_TO;
    }else if(v.compare(stMODE_DIRECT)==0){
        _mode = MODE_DIRECT;
    }else{
        _mode = MODE_FROM;
    }
}
QString TMarch::getModeAsStr() const{
    QString ret ="";
    switch (_mode) {
    case MODE_FROM:
        ret= stMODE_FROM;
        break;
    case MODE_TO:
        ret = stMODE_TO;
        break;
    case MODE_DIRECT:
        ret = stMODE_DIRECT;
        break;
    default:
        break;
    }
    return ret;
}
QString TMarch::getStatusAsStr() const{
    QString ret ="";
    switch (_status) {
    case NEW:
        ret = stStatus_NEW;
        break;
    case SOLVED:
        ret = stStatus_SOLVED;
        break;
    case ERR:
        ret = stStatus_ERR;
        break;
    case PROC:
        ret = stStatus_PROC;
        break;
    default:
        break;
    }
    return ret;
}
void TMarch::setStatus(const QString& v){
    if(v.compare(stStatus_NEW)==0){
        _status = NEW;
    }else if(v.compare(stStatus_SOLVED)==0){
        _status = SOLVED;
    }else if(v.compare(stStatus_ERR)==0){
        _status = ERR;
    }else if(v.compare(stStatus_PROC)==0){
        _status = PROC;
    }else{
        _status = UNDEF;
    }
}
// ----------------------------------------------------------------------
bool TMarch::fromJson(const QJsonValue& str){
    bool ret = true;
//qDebug()<<"Begin parse";
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();
        QJsonValue v_id = mainObj.value(QString(TMarch::stJsonField_id));        
        QJsonValue v_varid = mainObj.value(QString(TMarch::stJsonField_varid));
//qDebug()<<"VarId: "<<v_varid.toString();
        QJsonValue v_title = mainObj.value(QString(TMarch::stJsonField_title));
        QJsonValue v_mainDepId = mainObj.value(QString(TMarch::stJsonField_mainDepId));
//        QJsonValue v_status = mainObj.value(QString(TMarch::stJsonField_status));
//        QJsonValue v_errCode = mainObj.value(QString(TMarch::stJsonField_errCode));
//        QJsonValue v_errDescr = mainObj.value(QString(TMarch::stJsonField_errDescr));
        QJsonValue v_deployAreaStartId = mainObj.value(QString(TMarch::stJsonField_deployAreaStartId));
        QJsonValue v_deployAreaFinishId = mainObj.value(QString(TMarch::stJsonField_deployAreaFinishId));
        QJsonValue v_mode = mainObj.value(QString(TMarch::stJsonField_mode));
        QJsonValue v_tbegin = mainObj.value(QString(TMarch::stJsonField_tbegin));
        QJsonValue v_tend = mainObj.value(QString(TMarch::stJsonField_tend));
        QJsonValue v_param = mainObj.value(QString(TMarch::stJsonField_param));
        QJsonValue v_paths = mainObj.value(QString(TMarch::stJsonField_paths));
        QJsonValue v_milestones = mainObj.value(QString(TMarch::stJsonField_milestones));
        QJsonValue v_result = mainObj.value(QString(TMarch::stJsonField_result));
        QJsonValue v_ownerid = mainObj.value(QString(TMarch::stJsonField_ownerid));


        UTILS::parseUUID(v_id,_id);
//qDebug()<<"Ret1: "<<ret;
        ret&=UTILS::parseUUID(v_varid,_varid);
//qDebug()<<"Ret2: "<<ret;
//qDebug()<<"VarId: "<<_varid.toString();
        if( !v_title.isNull() && v_title.isString() ){
            _title = v_title.toString();
        }else{
            ret&=false;
        }
//qDebug()<<"Ret3: "<<ret;
        ret&=UTILS::parseUUID(v_mainDepId,_mainDepId);
//qDebug()<<"Ret4: "<<ret;
        ret&=UTILS::parseUUID(v_deployAreaStartId,_deployAreaStartId);
//qDebug()<<"Ret5: "<<ret;
        ret&=UTILS::parseUUID(v_deployAreaFinishId,_deployAreaFinishId);
//qDebug()<<"Ret6: "<<ret;

        if(!v_mode.isNull()&& v_mode.isString()){
            setMode(v_mode.toString());
        }else{
            ret&=false;
        }

        if(!v_tbegin.isNull()&& v_tbegin.isString()){
            _tbegin = QDateTime::fromString(v_tbegin.toString(),Qt::ISODate);
        }

        if(!v_tend.isNull()&& v_tend.isString()){
            _tend = QDateTime::fromString(v_tend.toString(),Qt::ISODate);
        }
//qDebug()<<"Ret71: "<<ret;
        foreach (TMarchParam* it, _param) {
            delete it;
        }
        _mapParam.clear();
        _param.clear();
        if(!v_param.isNull() && v_param.isArray()){
            QJsonArray a = v_param.toArray();
            foreach(QJsonValue it, a){
                TMarchParam* newItem = new TMarchParam();
                if(newItem->fromJson(it)){
                    _param<<newItem;

                    // add to map
                    QMap<QUuid,TMarchParam*> mTmp;
                    if(_mapParam.contains(newItem->getPathId())){
                        mTmp = _mapParam[newItem->getPathId()];
                    }
                    mTmp[newItem->getId()] = newItem;
                    _mapParam[newItem->getPathId()] = mTmp;

                }else{
                    qWarning()<<"Error while parse item param, skip item "<<newItem->getId().toString();
                    delete newItem;
                }
            }
        }
//qDebug()<<"Ret72: "<<ret;

        foreach (TMarchPath* it, _paths) {
            delete it;
        }        
        _paths.clear();
        if(!v_paths.isNull() && v_paths.isArray()){
            QJsonArray a = v_paths.toArray();
            foreach(QJsonValue it, a){
                TMarchPath* newItem = new TMarchPath();
                if(newItem->fromJson(it)){
                    _paths<<newItem;
                }else{
                    qWarning()<<"Error while parse item path, skip item";
                    delete newItem;
                }
            }
        }


        foreach (TMarchMilestone* it, _milestones){
            delete it;
        }
        _milestones.clear();
        if(!v_milestones.isNull() && v_milestones.isArray()){
            QJsonArray a = v_milestones.toArray();
            foreach(QJsonValue it, a){
                TMarchMilestone* newItem = new TMarchMilestone();
                if(newItem->fromJson(it)){
                    _milestones<<newItem;
                }else{
                    qWarning()<<"Error while parse item milestone, skip item";
                    delete newItem;
                }
            }
        }


        if(_result!=NULL) delete _result;
        _result = new TMarchResult();
        _result->fromJson(v_result);


        /*ret&=*/UTILS::parseUUID(v_ownerid,_ownerid);

    }else{
        qWarning()<<"NOT OBJ";
        ret = false;
    }
    return ret;
}
QJsonObject TMarch::toJson(){
    return toJson(false);
}
QJsonObject TMarch::toJson(bool isShort){
    QJsonObject ret;

    ret.insert(QString(TMarch::stJsonField_id),_id.toString());
    ret.insert(QString(TMarch::stJsonField_varid),_varid.toString());
    ret.insert(QString(TMarch::stJsonField_title),_title);
    ret.insert(QString(TMarch::stJsonField_mainDepId),_mainDepId.toString());
    ret.insert(QString(TMarch::stJsonField_status),getStatusAsStr());
    ret.insert(QString(TMarch::stJsonField_errCode),QString::number(_errCode));
    ret.insert(QString(TMarch::stJsonField_errDescr),getErrDescr());
    ret.insert(QString(TMarch::stJsonField_deployAreaStartId),_deployAreaStartId.toString());
    ret.insert(QString(TMarch::stJsonField_deployAreaFinishId),_deployAreaFinishId.toString());
    ret.insert(QString(TMarch::stJsonField_ownerid),_ownerid.toString());
    if(!isShort){
        ret.insert(QString(TMarch::stJsonField_mode),getModeAsStr());

        ret.insert(QString(TMarch::stJsonField_tbegin),_tbegin.isValid()?_tbegin.toString(Qt::ISODate):QString());

        ret.insert(QString(TMarch::stJsonField_tend),_tend.isValid()?_tend.toString(Qt::ISODate):QString());

        QJsonArray aParm;
        foreach (TMarchParam *it, _param) {
            aParm.append(it->toJson());
        }
        ret.insert(QString(TMarch::stJsonField_param),aParm);

        QJsonArray aPath;
        foreach (TMarchPath *it, _paths) {
            aPath.append(it->toJson());
        }
        ret.insert(QString(TMarch::stJsonField_paths),aPath);

        QJsonArray aMilestone;
        foreach (TMarchMilestone *it, _milestones) {
            aMilestone.append(it->toJson());
        }
        ret.insert(QString(TMarch::stJsonField_milestones),aMilestone);

        QJsonObject res;
        if(_result!=NULL){
            res = _result->toJson();
        }
        ret.insert(QString(TMarch::stJsonField_result),res);
    }
    return ret;
}
const TMarchParam* TMarch::getParam(const QUuid& pathid,const QUuid& paramid){
    TMarchParam* ret = NULL;

    if(_mapParam.contains(pathid)){
        QMap<QUuid,TMarchParam*> mTmp = _mapParam[pathid];
        if(mTmp.contains(paramid)){
            ret = mTmp[paramid];
        }
    }

    return ret;
}
#ifdef QT_DEBUG
void TMarch::printMapPram()const{
    foreach(QUuid pathid, _mapParam.keys()){
        qDebug()<<"PathID: "<<pathid.toString();
        QMap<QUuid,TMarchParam*> mTmp = _mapParam[pathid];
        foreach(TMarchParam* v, mTmp.values()){
            qDebug()<<"\t- item: "<<v->getId();
        }
    }
}
#endif
// ----------------------------------------------------------------------
const QUuid& TMarch::getId()const{return _id;}
const QUuid& TMarch::getVarId()const{return _varid;}
const QString& TMarch::getTitle()const{return _title;}
const QUuid& TMarch::getMainDepId()const{return _mainDepId;}
TMarch::STATUS TMarch::getStatus()const{return _status;}
TMarch::ERR_CODE TMarch::getErrCode()const{return _errCode;}
QString TMarch::getErrDescr(){
    if(_errCode>=MAX_ERR_CODE){
        _errCode = WRONG_ERR_CODE;
    }
    return QString(errDescriptions[_errCode]);
}
const QUuid& TMarch::getDeployAreaStartId()const{return _deployAreaStartId;}
const QUuid& TMarch::getDeployAreaFinishId()const{return _deployAreaFinishId;}
TMarch::MODE TMarch::getMode() const{return _mode;}
const QDateTime& TMarch::getBegin() const{return _tbegin;}
const QDateTime& TMarch::getEnd() const{return _tend;}
const QList<TMarchParam*> &TMarch::getParam()const{return _param;}
const QList<TMarchPath*>& TMarch::getPaths()const{return _paths;}
const QList<TMarchMilestone*>& TMarch::getMilestones()const{return _milestones;}
const TMarchResult * TMarch::getResult()const{return _result;}
const QUuid& TMarch::getOwnerid()const{return _ownerid;}
// setters
void TMarch::setId(const QUuid& id){ _id = id; }
void TMarch::setVarId(const QUuid& varid){ _varid = varid; }
void TMarch::setTitle(const QString& title){ _title = title; }
void TMarch::setMainDepId(const QUuid& mainDepId){ _mainDepId = mainDepId; }
void TMarch::setStatus(STATUS status){ _status = status; }
void TMarch::setErrCode(ERR_CODE errCode){ _errCode = errCode; }
//void TMarch::setErrDescr(const QString& errDescr){ _errDescr = errDescr; }
void TMarch::setDeployAreaStartId(const QUuid& deployAreaStartId){ _deployAreaStartId = deployAreaStartId; }
void TMarch::setDeployAreaFinishId(const QUuid& deployAreaFinishId){ _deployAreaFinishId = deployAreaFinishId; }
void TMarch::setMode(TMarch::MODE mode){_mode = mode;}
void TMarch::setBegin(const QDateTime& v){_tbegin = v;}
void TMarch::setEnd(const QDateTime& v){_tend = v; }
void TMarch::addParam(TMarchParam *param){
    if(param!=NULL){
        _param<<param;
    }
}
void TMarch::addPath(TMarchPath* path){
    if(path!=NULL){
        _paths<<path;
    }
}
void TMarch::addMilestone(TMarchMilestone* milestone){
    if(milestone!=NULL){
        _milestones<<milestone;
    }
}
void TMarch::setResult(TMarchResult *result){
    if(_result!=NULL) delete _result;
    _result = NULL;
    if(result!=NULL){
        _result = result;
    }
}
void TMarch::setOwnerId(const QUuid& ownerid){ _ownerid = ownerid; }
// Fields
const char* TMarch::getDBField_id(){return stDBField_id;}
const char* TMarch::getDBField_varid(){return stDBField_varid;}
const char* TMarch::getDBField_title(){return stDBField_title;}
const char* TMarch::getDBField_mainDepId(){return stDBField_mainDepId;}
const char* TMarch::getDBField_status(){return stDBField_status;}
const char* TMarch::getDBField_errCode(){return stDBField_errCode;}
const char* TMarch::getDBField_deployAreaStartId(){return stDBField_deployAreaStartId;}
const char* TMarch::getDBField_deployAreaFinishId(){return stDBField_deployAreaFinishId;}
const char* TMarch::getDBField_content(){return stDBField_content;}
//////////////////////////////////
const char* TMarchParam::stJsonField_id = "id";
const char* TMarchParam::stJsonField_pathid = "pathid";
const char* TMarchParam::stJsonField_title = "title";
const char* TMarchParam::stJsonField_value = "value";

const char* TMarchParam::stDBField_id = "id";
const char* TMarchParam::stDBField_value = "defvalue";
const char* TMarchParam::stDBField_title = "title";

TMarchParam::TMarchParam(){
}
TMarchParam::~TMarchParam(){
}
bool TMarchParam::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();

        QJsonValue v_id = mainObj.value(QString(stJsonField_id));
        QJsonValue v_pathid = mainObj.value(QString(stJsonField_pathid));
        QJsonValue v_title = mainObj.value(QString(stJsonField_title));
        QJsonValue v_value = mainObj.value(QString(stJsonField_value));

        ret&=UTILS::parseUUID(v_id,_id);

        if(!v_pathid.isNull()){
            _isCommon = false;
            ret&=UTILS::parseUUID(v_pathid, _pathid);
        }else{
            _isCommon = true;
        }

        if(!v_title.isNull() && v_title.isString()){
            _title = v_title.toString();
        }else{
            ret&=false;
        }

        if(!v_value.isNull() && v_value.isDouble()){
            _value = v_value.toDouble();
        }else{
            ret&=false;
        }
    }else{
        ret = false;
    }
    return ret;
}
QJsonObject TMarchParam::toJson(){
    QJsonObject ret;

    ret.insert(QString(stJsonField_id),_id.toString());
    ret.insert(QString(stJsonField_pathid),_isCommon?NULL:_pathid.toString());
    ret.insert(QString(stJsonField_title),_title);
    ret.insert(QString(stJsonField_value),_value);

    return ret;
}
//getters
const QUuid& TMarchParam::getId() const{return _id;}
const QUuid& TMarchParam::getPathId()const{return _pathid;}
const QString& TMarchParam::getTitle()const{return _title;}
double TMarchParam::getValue()const{return _value;}
//setters
void TMarchParam::setId(const QUuid& id){_id = id;}
void TMarchParam::setPathId(const QUuid& pathid){_pathid = pathid;}
void TMarchParam::setTitle(const QString& title){_title = title;}
void TMarchParam::setValue(double value){_value = value;}

bool TMarchParam::isCommon() const{return _isCommon;}

const char* TMarchParam::getDBField_id(){return stDBField_id;}
const char* TMarchParam::getDBField_value(){return stDBField_value;}
const char* TMarchParam::getDBField_title(){return stDBField_title;}
//////////////////////////////////
const char* TMarchPathItem::stJsonField_typeitem = "typeitem";
const char* TMarchPathItem::stJsonField_class = "class";
const char* TMarchPathItem::stJsonField_speed = "speed";
const char* TMarchPathItem::stJsonField_order = "order";
const char* TMarchPathItem::stJsonField_num = "num";
const char* TMarchPathItem::stJsonField_dist = "dist";

const char* TMarchPathItem::stTYPE_TO_SL = "TO_SL";
const char* TMarchPathItem::stTYPE_TO_RL = "TO_RL";
const char* TMarchPathItem::stTYPE_TO_RA = "TO_RA";
const char* TMarchPathItem::stTYPE_TO_DA = "TO_DA";
const char* TMarchPathItem::st_UNDEF = "UNDEF";

TMarchPathItem::TMarchPathItem(){
    _typeitem = TYPE_UNDEF;
    _classitem = CLASS_UNDEF;
    _speed = -1.0;
    _order = -1;
    _num = -1;
    _dist = -1;
}
TMarchPathItem::~TMarchPathItem(){

}

bool TMarchPathItem::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();

        QJsonValue v_typeitem = mainObj.value(QString(stJsonField_typeitem));
        QJsonValue v_classitem = mainObj.value(QString(stJsonField_class));
        QJsonValue v_speed = mainObj.value(QString(stJsonField_speed));
        QJsonValue v_order = mainObj.value(QString(stJsonField_order));
        QJsonValue v_num = mainObj.value(QString(stJsonField_num));
        QJsonValue v_dist = mainObj.value(QString(stJsonField_dist));

        if(!v_typeitem.isNull() && v_typeitem.isString()){
            setType(v_typeitem.toString());
        }else{
            ret&=false;
        }

        if(!v_classitem.isNull() && v_classitem.isString()){
            setClass(v_classitem.toString());
        }else{
            ret&=false;
        }

        if(!v_speed.isNull() && v_speed.isDouble()){
            _speed = v_speed.toDouble();
        }else{
            ret&=false;
        }

        if(!v_order.isNull() && v_order.isDouble()){
            _order= v_order.toInt();
        }else{
            ret&=false;
        }

        if(!v_num.isNull() && v_num.isDouble()){
            _num= v_num.toInt();
        }else{
            ret&=false;
        }

        if(!v_dist.isNull() && v_dist.isDouble()){
            _dist= v_dist.toInt();
        }else{
            ret&=false;
        }
    }else{
        ret = false;
    }
    return ret;
}
QJsonObject TMarchPathItem::toJson(){
    QJsonObject ret;

    ret.insert(QString(stJsonField_typeitem),getTypeAsStr());
    ret.insert(QString(stJsonField_class),getClassAsStr());
    ret.insert(QString(stJsonField_speed),_speed);
    ret.insert(QString(stJsonField_order),_order);
    ret.insert(QString(stJsonField_num),_num);
    ret.insert(QString(stJsonField_dist),_dist);

    return ret;
}
bool TMarchPathItem::comapre(const TMarchPathItem* o1,const TMarchPathItem* o2){
    bool ret = false;
    if(o1!=NULL && o2!=NULL){
        if( (o1->getType()==o2->getType()) && (o1->getType()==TO_RL || o1->getType()==TO_RA)){
            ret = o1->getNum()<o2->getNum();
        }else{
            ret = o1->getType()<o2->getType();
        }
    }
    return ret;
}
void TMarchPathItem::setType(const QString& v){
    if(v.compare(stTYPE_TO_SL)==0){
        _typeitem = TO_SL;
    }else if(v.compare(stTYPE_TO_RL)==0){
        _typeitem = TO_RL;
    }else if(v.compare(stTYPE_TO_RA)==0){
        _typeitem = TO_RA;
    }else if(v.compare(stTYPE_TO_DA)==0){
        _typeitem = TO_DA;
    }else{
        _typeitem = TYPE_UNDEF;
    }
}
QString TMarchPathItem::getTypeAsStr() const{
    QString ret ="";
    switch (_typeitem) {
    case TO_SL:
        ret = stTYPE_TO_SL;
        break;
    case TO_RL:
        ret = stTYPE_TO_RL;
        break;
    case TO_RA:
        ret = stTYPE_TO_RA;
        break;
    case TO_DA:
        ret = stTYPE_TO_DA;
        break;
    default:
        ret = st_UNDEF;
        break;
    }
    return ret;
}

void TMarchPathItem::setClass(const QString& v){
    /*if(v.compare(stTYPE_TO_SL)==0){
        _typeitem = TO_SL;
    }else if(v.compare(stTYPE_TO_RL)==0){
        _typeitem = TO_RL;
    }else if(v.compare(stTYPE_TO_RA)==0){
        _typeitem = TO_RA;
    }else if(v.compare(stTYPE_TO_DA)==0){
        _typeitem = TO_DA;
    }else*/{
        _classitem = CLASS_UNDEF;
    }
}
QString TMarchPathItem::getClassAsStr() const{
    QString ret ="";
    switch (_classitem) {
    default:
        ret = st_UNDEF;
        break;
    }
    return ret;
}
//getters
TMarchPathItem::TYPE TMarchPathItem::getType() const{return _typeitem;}
TMarchPathItem::CLASS TMarchPathItem::getClass() const{return _classitem;}
double TMarchPathItem::getSpeed() const{return _speed;}
int TMarchPathItem::geOrder() const{return _order;}
int TMarchPathItem::getNum() const{return _num;}
int TMarchPathItem::getDist() const{return _dist;}
//setters
void TMarchPathItem::setType(TMarchPathItem::TYPE typeitem){ _typeitem = typeitem;}
void TMarchPathItem::setClass(TMarchPathItem::CLASS classitem){ _classitem = classitem;}
void TMarchPathItem::setSpeed(double speed){ _speed = speed;}
void TMarchPathItem::setOrder(int order){ _order = order;}
void TMarchPathItem::setNum(int num){ _num = num;}
void TMarchPathItem::setDist(int dist){ _dist = dist;}
QString TMarchPathItem::getKey() const{
    return getTypeAsStr()+"_"+QString::number(_num);
}
#ifdef QT_DEBUG
QString TMarchPathItem::toString() const{
    return getTypeAsStr()+((_typeitem==TO_RL || _typeitem==TO_RA)?" ("+QString::number(_num)+") - ":" - ")+QString::number(_dist);
}
#endif
//////////////////////////////////
const char* TMarchPath::stJsonField_id = "id";
const char* TMarchPath::stJsonField_pathid = "pathid";
const char* TMarchPath::stJsonField_frm = "formation";
const char* TMarchPath::stJsonField_order = "order";
const char* TMarchPath::stJsonField_items = "pathitems";

TMarchPath::TMarchPath(){
    _frm = UNDEF;
    _order = -1;
}
TMarchPath::~TMarchPath(){
    foreach (TMarchPathItem* it, _items) {
        delete it;
    }
    _items.clear();
}
bool TMarchPath::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();
        QJsonValue v_id = mainObj.value(QString(stJsonField_id));
        QJsonValue v_pathid = mainObj.value(QString(stJsonField_pathid));
        //QJsonValue v_frm = mainObj.value(QString(stJsonField_frm));
        QJsonValue v_order = mainObj.value(QString(stJsonField_order));
        QJsonValue v_items= mainObj.value(QString(stJsonField_items));

        ret&=UTILS::parseUUID(v_id,_id);

        ret&=UTILS::parseUUID(v_pathid,_pathid);

        if(!v_order.isNull() && v_order.isDouble()){
            _order = (int)v_order.toDouble();
        }else{
            ret&=false;
        }

        if(!v_items.isNull() && v_items.isArray()){
            QJsonArray array = v_items.toArray();
            foreach(QJsonValue av, array){
                TMarchPathItem *newItem = new TMarchPathItem();
                if(newItem->fromJson(av)){
                    _items<<newItem;
                }else{
                    qWarning()<<"Error while parse TMarchPathItem item - skipping";
                    delete newItem;
                }
            }
            sortItems();
        }else{
            ret&=false;
        }

    }else{
        ret = false;
    }
    return ret;
}
QJsonObject TMarchPath::toJson(){
    QJsonObject ret;

    ret.insert(QString(stJsonField_id),_id.toString());
    ret.insert(QString(stJsonField_pathid),_pathid.toString());
    ret.insert(QString(stJsonField_frm), "UNDEF"); // TODO:
    ret.insert(QString(stJsonField_order),_order);
    QJsonArray a;
    foreach(TMarchPathItem *it, _items){
        a.append(it->toJson());
    }
    ret.insert(QString(stJsonField_items),a);

    return ret;
}
void TMarchPath::sortItems(){
    std::sort(_items.begin(),_items.end(),TMarchPathItem::comapre);
}
// getters
const QUuid& TMarchPath::getId() const{return _id;}
const QUuid& TMarchPath::getPathId() const{return _pathid;}
TMarchPath::FORMATION TMarchPath::getFormation() const{return _frm;}
int TMarchPath::getOrder() const{return _order;}
const QList<TMarchPathItem*>& TMarchPath::getItems()const{return _items;}
// setters
void TMarchPath::setId(const QUuid& id){ _id = id;}
void TMarchPath::setPathId(const QUuid& pathid){ _pathid = pathid;}
void TMarchPath::setFormation(FORMATION frm){ _frm = frm;}
void TMarchPath::setOrder(int order){ _order = order;}
void TMarchPath::add(TMarchPathItem* item){
    if(item!=NULL){
        _items<<item;
    }
}

//////////////////////////////////
TMarchMilestone::TMarchMilestone(){;}
TMarchMilestone::~TMarchMilestone(){;}
bool TMarchMilestone::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){

    }else{
        ret = false;
    }
    return ret;
}
QJsonObject TMarchMilestone::toJson(){
    QJsonObject ret;


    return ret;
}

//////////////////////////////////
const char* TMarchResultMilestone::stJsonField_typeitem = "typeitem";
const char* TMarchResultMilestone::stJsonField_num = "num";
const char* TMarchResultMilestone::stJsonField_head = "tbegin";
const char* TMarchResultMilestone::stJsonField_tail = "tend";
const char* TMarchResultMilestone::stJsonField_resttime = "resttime";

const char* TMarchResultMilestone::stTYPE_SL = "SL";
const char* TMarchResultMilestone::stTYPE_RL = "RL";
const char* TMarchResultMilestone::stTYPE_RA = "RA";

TMarchResultMilestone::TMarchResultMilestone(){
    _type = SL;
    _num = 0;
    _intHead = 0;
    _intTail = 0;
}
TMarchResultMilestone::~TMarchResultMilestone(){;}

bool TMarchResultMilestone::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();

        QJsonValue v_type = mainObj.value(QString(stJsonField_typeitem));
        QJsonValue v_num = mainObj.value(QString(stJsonField_num));
        QJsonValue v_head = mainObj.value(QString(stJsonField_head));
        QJsonValue v_tail = mainObj.value(QString(stJsonField_tail));
        QJsonValue v_resttime = mainObj.value(QString(stJsonField_resttime));

        if(!v_type.isNull() && v_type.isString()){
            setType(v_type.toString());
        }else{
            ret&=false;
        }

        if(!v_num.isNull() && v_num.isDouble()){
            _num = v_num.toDouble();
        }else{
            ret&=false;
        }

        if(!v_head.isNull()&& v_head.isString()){
            _head = QDateTime::fromString(v_head.toString(),Qt::ISODate);
        }else{
            ret&=false;
        }

        if(!v_tail.isNull()&& v_tail.isString()){
            _tail = QDateTime::fromString(v_tail.toString(),Qt::ISODate);
        }else{
            ret&=false;
        }

        if(!v_resttime.isNull()&& v_resttime.isString()){
            _resttime = QDateTime::fromString(v_resttime.toString(),Qt::ISODate);
        }else{
            ret&=false;
        }
    }else{
        ret = false;
    }
    return ret;
}
QJsonObject TMarchResultMilestone::toJson(){
    QJsonObject ret;

    ret.insert(QString(stJsonField_typeitem),getTypeAsStr());
    ret.insert(QString(stJsonField_num),_num);
    ret.insert(QString(stJsonField_head),_head.isValid()?_head.toString(Qt::ISODate):QString());
    ret.insert(QString(stJsonField_tail),_tail.isValid()?_tail.toString(Qt::ISODate):QString());
    ret.insert(QString(stJsonField_resttime),_resttime.isValid()?_resttime.toString(Qt::ISODate):QString());

    return ret;
}

//getters
TMarchResultMilestone::TYPE TMarchResultMilestone::getType()const{return _type;}
int TMarchResultMilestone::getNum()const{return _num;}
const QDateTime& TMarchResultMilestone::getHead()const{return _head;}
const QDateTime& TMarchResultMilestone::getTail()const{return _tail;}
const QDateTime& TMarchResultMilestone::getRestTime()const{return _resttime;}
//setters
void TMarchResultMilestone::setType(TMarchResultMilestone::TYPE type){ _type = type;}
void TMarchResultMilestone::setNum(int num){ _num = num;}
void TMarchResultMilestone::setHead(const QDateTime& head){ _head = head;}
void TMarchResultMilestone::setTail(const QDateTime& tail){ _tail = tail;}
void TMarchResultMilestone::setRestTime(const QDateTime& resttime){ _resttime = resttime;}

long TMarchResultMilestone::getIntHead()const{return _intHead;}
long TMarchResultMilestone::getIntTail()const{return _intTail;}
void TMarchResultMilestone::setIntHead(long head){ _intHead = head;}
void TMarchResultMilestone::setIntTail(long tail){ _intTail = tail;}
void TMarchResultMilestone::addIntHead(long head){ _intHead += head;}
void TMarchResultMilestone::addIntTail(long tail){ _intTail += tail;}

QString TMarchResultMilestone::getKey() const{
    return getTypeAsStr()+"_"+QString::number(_num);
}

void TMarchResultMilestone::setType(const QString& v){
    if(v.compare(stTYPE_SL)==0){
        _type = SL;
    }else if(v.compare(stTYPE_RL)==0){
        _type = RL;
    }else if(v.compare(stTYPE_RA)==0){
        _type = RA;
    }else{
        _type = RL;
    }
}
QString TMarchResultMilestone::getTypeAsStr() const{
    QString ret = "";
    switch (_type) {
    case SL:
        ret = stTYPE_SL;
        break;
    case RL:
        ret = stTYPE_RL;
        break;
    case RA:
        ret = stTYPE_RA;
        break;
    default:
        break;
    }
    return ret;
}
//////////////////////////////////
const char* TMarchResultDep::stJsonField_depid = "depid";
const char* TMarchResultDep::stJsonField_order = "order";
const char* TMarchResultDep::stJsonField_pathid = "pathid";
const char* TMarchResultDep::stJsonField_unitct = "unitct";
const char* TMarchResultDep::stJsonField_unitslength = "unitslength";
const char* TMarchResultDep::stJsonField_pathlength = "pathlength";
const char* TMarchResultDep::stJsonField_len2SL = "len2SL";
const char* TMarchResultDep::stJsonField_len2DA = "len2DA";
const char* TMarchResultDep::stJsonField_distBetweenUnit = "distbetweenunit";
const char* TMarchResultDep::stJsonField_speedPath = "speedpath";
const char* TMarchResultDep::stJsonField_speed2SL = "speed2SL";
const char* TMarchResultDep::stJsonField_speed2DA = "speed2DL";
const char* TMarchResultDep::stJsonField_begin = "tbegin";
const char* TMarchResultDep::stJsonField_end = "tend";
const char* TMarchResultDep::stJsonField_milestones = "milestones";

TMarchResultDep::TMarchResultDep(){
    _order = -1;
    _unitct = -1;
    _unitslength = -1;
    _pathlength = -2;
    _len2SL = -1;
    _len2DA = -1;
    _distBetweenUnit = -2;
    _speedPath = -3;
    _speed2SL = -3;
    _speed2DA = -3;
}
TMarchResultDep::TMarchResultDep(const QUuid& depid, const QUuid &pathid){
    _depid = depid;
    _pathid = pathid;
    _order = -1;
    _unitct = -1;
    _unitslength = -1;
    _pathlength = -2;
    _len2SL = -1;
    _len2DA = -1;
    _distBetweenUnit = -2;
    _speedPath = -3;
    _speed2SL = -3;
    _speed2DA = -3;
}
TMarchResultDep::~TMarchResultDep(){
    foreach(TMarchResultMilestone* it, _milestones){
        delete it;
    }
    _milestones.clear();
}

bool TMarchResultDep::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();

        QJsonValue v_depid = mainObj.value(QString(stJsonField_depid));
        QJsonValue v_order = mainObj.value(QString(stJsonField_order));
        QJsonValue v_pathid = mainObj.value(QString(stJsonField_pathid));
        QJsonValue v_unitct = mainObj.value(QString(stJsonField_unitct));
        QJsonValue v_unitslength = mainObj.value(QString(stJsonField_unitslength));
        QJsonValue v_pathlength = mainObj.value(QString(stJsonField_pathlength));
        QJsonValue v_len2SL = mainObj.value(QString(stJsonField_len2SL));
        QJsonValue v_len2DA = mainObj.value(QString(stJsonField_len2DA));
        QJsonValue v_distBetweenUnit = mainObj.value(QString(stJsonField_distBetweenUnit ));
        QJsonValue v_speedPath = mainObj.value(QString(stJsonField_speedPath));
        QJsonValue v_speed2SL = mainObj.value(QString(stJsonField_speed2SL));
        QJsonValue v_speed2DA = mainObj.value(QString(stJsonField_speed2DA));
        QJsonValue v_begin = mainObj.value(QString(stJsonField_begin));
        QJsonValue v_end = mainObj.value(QString(stJsonField_end));
        QJsonValue v_milestones = mainObj.value(QString(stJsonField_milestones));

        ret&=UTILS::parseUUID(v_depid,_depid);

        if(!v_order.isNull() && v_order.isDouble()){
            _order = v_order.toInt();
        }else{
            ret&=false;
        }

        ret&=UTILS::parseUUID(v_pathid,_pathid);

        if(!v_unitct.isNull() && v_unitct.isDouble()){
            _unitct = v_unitct.toInt();
        }else{
            ret&=false;
        }

        if(!v_unitslength.isNull() && v_unitslength.isDouble()){
            _unitslength = v_unitslength.toDouble();
        }else{
            ret&=false;
        }

        if(!v_pathlength.isNull() && v_pathlength.isDouble()){
            _pathlength = v_pathlength.toDouble();
        }else{
            ret&=false;
        }

        if(!v_len2SL.isNull() && v_len2SL.isDouble()){
            _len2SL = v_len2SL.toDouble();
        }else{
            ret&=false;
        }

        if(!v_len2DA.isNull() && v_len2DA.isDouble()){
            _len2DA = v_len2DA.toDouble();
        }else{
            ret&=false;
        }

        if(!v_distBetweenUnit.isNull() && v_distBetweenUnit.isDouble()){
            _distBetweenUnit = v_distBetweenUnit.toDouble();
        }else{
            ret&=false;
        }

        if(!v_speedPath.isNull() && v_speedPath.isDouble()){
            _speedPath = v_speedPath.toDouble();
        }else{
            ret&=false;
        }

        if(!v_speed2SL.isNull() && v_speed2SL.isDouble()){
            _speed2SL = v_speed2SL.toDouble();
        }else{
            ret&=false;
        }

        if(!v_speed2DA.isNull() && v_speed2DA.isDouble()){
            _speed2DA = v_speed2DA.toDouble();
        }else{
            ret&=false;
        }

        if(!v_begin.isNull()&& v_begin.isString()){
            _begin = QDateTime::fromString(v_begin.toString(),Qt::ISODate);
        }else{
            ret&=false;
        }

        if(!v_end.isNull()&& v_end.isString()){
            _end = QDateTime::fromString(v_end.toString(),Qt::ISODate);
        }else{
            ret&=false;
        }

        if(!v_milestones.isNull() && v_milestones.isArray()){
            QJsonArray array = v_milestones.toArray();
            foreach(QJsonValue av, array){
                TMarchResultMilestone *newItem = new TMarchResultMilestone();
                if(newItem->fromJson(av)){
                    _milestones<<newItem;
                }else{
                    qWarning()<<"Error while parse TMarchPathItem item - skipping";
                    delete newItem;
                }
            }
        }else{
            ret&=false;
        }
    }else{
        ret = false;
    }
    return ret;
}
QJsonObject TMarchResultDep::toJson(){
    QJsonObject ret;

    ret.insert(QString(stJsonField_depid),_depid.toString());
    ret.insert(QString(stJsonField_order),_order);
    ret.insert(QString(stJsonField_pathid),_pathid.toString());
    ret.insert(QString(stJsonField_unitct),_unitct);
    ret.insert(QString(stJsonField_unitslength),_unitslength);
    ret.insert(QString(stJsonField_pathlength),_pathlength);
    ret.insert(QString(stJsonField_len2SL),_len2SL);
    ret.insert(QString(stJsonField_len2DA),_len2DA);
    ret.insert(QString(stJsonField_distBetweenUnit),_distBetweenUnit);
    ret.insert(QString(stJsonField_speedPath),_speedPath);
    ret.insert(QString(stJsonField_speed2SL),_speed2SL);
    ret.insert(QString(stJsonField_speed2DA),_speed2DA);
    ret.insert(QString(stJsonField_begin),_begin.isValid()?_begin.toString(Qt::ISODate):QString());
    ret.insert(QString(stJsonField_end),_end.isValid()?_end.toString(Qt::ISODate):QString());

    QJsonArray a;
    foreach (TMarchResultMilestone * it, _milestones) {
        a.append(it->toJson());
    }

    ret.insert(QString(stJsonField_milestones),a);


    return ret;
}

//getters
const QUuid& TMarchResultDep::getDepid()const{return _depid;}
int TMarchResultDep::getOrder()const{return _order;}
const QUuid &TMarchResultDep::getPathId()const{return _pathid;}
int TMarchResultDep::getUnitCt()const{return _unitct;}
double TMarchResultDep::getUnitLength()const{return _unitslength;}
double TMarchResultDep::getPathLength()const{return _pathlength;}
double TMarchResultDep::getLen2SL()const{return _len2SL;}
double TMarchResultDep::getLen2DA()const{return _len2DA;}
double TMarchResultDep::getDistBetweenUnit()const{return _distBetweenUnit;}
double TMarchResultDep::getSpeedPath()const{return _speedPath;}
double TMarchResultDep::getSpeed2SL()const{return _speed2SL;}
double TMarchResultDep::getSpeed2DA()const{return _speed2DA;}
const QDateTime& TMarchResultDep::getBegin()const{return _begin;}
const QDateTime&  TMarchResultDep::getEnd()const{return _end;}
const QList<TMarchResultMilestone*>& TMarchResultDep::getMilestones()const{return _milestones;}
//setters
void TMarchResultDep::setDepId(const QUuid &depid){ _depid = depid;}
void TMarchResultDep::setOrder(int order){ _order = order;}
void TMarchResultDep::setPathId(const QUuid &pathid){ _pathid = pathid;}
void TMarchResultDep::setUnitCt(int unitct){ _unitct = unitct;}
void TMarchResultDep::setUnitsLength(double unitslength){ _unitslength = unitslength;}
void TMarchResultDep::setPathLength(double pathlegth){_pathlength = pathlegth;}
void TMarchResultDep::setLen2SL(double len2SL){ _len2SL = len2SL;}
void TMarchResultDep::setLen2DA(double len2DA){ _len2DA = len2DA;}
void TMarchResultDep::setDistBetweenUnit(double distBetweenUnit){_distBetweenUnit = distBetweenUnit;}
void TMarchResultDep::setSpeedPath(double speedPath){_speedPath = speedPath;}
void TMarchResultDep::setSpeed2SL(double speed2SL){_speed2SL = speed2SL;}
void TMarchResultDep::setSpeed2DA(double speed2DA){_speed2DA = speed2DA;}
void TMarchResultDep::setBegin(const QDateTime &begin){ _begin = begin;}
void TMarchResultDep::setEnd(const QDateTime  &end){ _end = end;}
void TMarchResultDep::addMilestone(TMarchResultMilestone* milestone){
    if(milestone!=NULL){
        _milestones<<milestone;
    }
}
long TMarchResultDep::getIntBegin()const{return _intBegin;}
long  TMarchResultDep::getIntEnd()const{return _intEnd;}
void TMarchResultDep::setIntBegin(long begin){ _intBegin = begin;}
void TMarchResultDep::setIntEnd(long end){ _intEnd = end;}
void TMarchResultDep::addIntBegin(long v){ _intBegin += v;}
void TMarchResultDep::addIntEnd(long v){ _intEnd += v;}
double TMarchResultDep::getPreDist()const{return _preDist;}
void TMarchResultDep::setPreDist(double v){_preDist = v;}
//////////////////////////////////
const char* TMarchResult::stJsonField_days = "days";
const char* TMarchResult::stJsonField_deep = "deep";
const char* TMarchResult::stJsonField_width = "width";
const char* TMarchResult::stJsonField_restct = "restct";
const char* TMarchResult::stJsonField_deps = "deps";

TMarchResult::TMarchResult(){
    _days = -1;
    _deep = -1;
    _width= -1;
    _restct = -1;
}
TMarchResult::~TMarchResult(){
    foreach(TMarchResultDep* it, _deps){
        delete it;
    }
    _deps.clear();
}
bool TMarchResult::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();

        QJsonValue v_days = mainObj.value(QString(stJsonField_days));
        QJsonValue v_deep = mainObj.value(QString(stJsonField_deep));
        QJsonValue v_width = mainObj.value(QString(stJsonField_width));
        QJsonValue v_restct = mainObj.value(QString(stJsonField_restct));
        QJsonValue v_deps = mainObj.value(QString(stJsonField_deps));

        if(!v_days.isNull() && v_days.isDouble()){
            _days = v_days.toInt();
        }else{
            ret&=false;
        }

        if(!v_deep.isNull() && v_deep.isDouble()){
            _deep = v_deep.toInt();
        }else{
            ret&=false;
        }

        if(!v_width.isNull() && v_width.isDouble()){
            _width = v_width.toInt();
        }else{
            ret&=false;
        }

        if(!v_restct.isNull() && v_restct.isDouble()){
            _restct = v_restct.toInt();
        }else{
            ret&=false;
        }

        if(!v_deps.isNull() && v_deps.isArray()){
            QJsonArray array = v_deps.toArray();
            foreach(QJsonValue av, array){
                TMarchResultDep *newItem = new TMarchResultDep();
                if(newItem->fromJson(av)){
                    _deps<<newItem;
                }else{
                    qWarning()<<"Error while parse TMarchPathItem item - skipping";
                    delete newItem;
                }
            }
        }else{
            ret&=false;
        }

    }else{
        ret = false;
    }
    return ret;
}
QJsonObject TMarchResult::toJson(){
    QJsonObject ret;

    ret.insert(QString(stJsonField_days),_days);
    ret.insert(QString(stJsonField_deep),_deep);
    ret.insert(QString(stJsonField_width),_width);
    ret.insert(QString(stJsonField_restct),_restct);

    QJsonArray a;
    foreach (TMarchResultDep * it, _deps) {
        a.append(it->toJson());
    }
    ret.insert(QString(stJsonField_deps),a);

    return ret;
}

//getters
int TMarchResult::getDays()const{return _days;}
int TMarchResult::getDeep()const{return _deep;}
int TMarchResult::getWidth()const{return _width;}
int TMarchResult::getRestCt()const{return _restct;}
const QList<TMarchResultDep*>& TMarchResult::getDeps()const{return _deps;}
//setters
void TMarchResult::setDays(int days){ _days = days;}
void TMarchResult::setDeep(int deep){ _deep = deep;}
void TMarchResult::setWidth(int width){ _width = width;}
void TMarchResult::setRestCt(int restct){ _restct = restct;}
void TMarchResult::addDeps(TMarchResultDep* deps){
    if(deps!=NULL){
        _deps<<deps;
    }
}

//////////////////////////////////
QList<TMarchParam*>::iterator TMarchParamDic::begin(){return _items.begin();}
QList<TMarchParam*>::iterator TMarchParamDic::end(){return _items.end();}

//////////////////////////////////
QList<TMarch*>::iterator TMarchItemList::begin(){return _items.begin();}
QList<TMarch*>::iterator TMarchItemList::end(){return _items.end();}
