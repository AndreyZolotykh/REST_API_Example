#include "utils.h"

bool UTILS::parseUUID(const QJsonValue& v, QUuid& res){
    bool ret = true;
    if(!v.isNull() && v.isString()){
        res = QUuid::fromString(v.toString());
    }else{
        ret = false;
    }
    return ret;
}


////////////////////////////////////////////////////////////////////
const char* OW_OUR = "owOur";
const char* OW_ENEMY = "owEnemy";
const char* OW_CUSTOM = "owCustom";
const char* OW_UNDEF = "owUndef";
OWNER_TYPE UTILS::fromStringOwner(const QString& s){
    OWNER_TYPE ret = owUNDEF;
    if(s.compare(OW_OUR)==0){
        ret = owOUR;
    }else if(s.compare(OW_ENEMY)==0){
        ret = owENEMY;
    }else if(s.compare(OW_CUSTOM)==0){
        ret = owCUSTOM;
    }
    return ret;
}
QString UTILS::toStringOwner(OWNER_TYPE t){
    QString ret;
    switch(t){
    case owOUR:
        ret = OW_OUR;
        break;
    case owENEMY:
        ret = OW_ENEMY;
        break;
    case owCUSTOM:
        ret = OW_CUSTOM;
        break;
    default:
        ret = OW_UNDEF;
    }
    return ret;
}
////////////////////////////////////////////////////////////////////
const char* ST_DIRECTLY = "stDirectly";
const char* ST_ATTACHED = "stAttached";
const char* ST_STRENGTHENING = "stStrengthening";
const char* ST_SUPPORT = "stSupport";
const char* ST_ALL = "stAll";
const char* ST_UNDEF = "stUndef";
SUB_TYPE UTILS::fromStringSub(const QString& s){
    SUB_TYPE ret = stUNDEF;
    if(s.compare(ST_DIRECTLY)==0){
        ret = stDIRECTLY;
    }else if(s.compare(ST_ATTACHED)==0){
        ret = stATTACHED;
    }else if(s.compare(ST_STRENGTHENING)==0){
        ret = stSTRENGTHENING;
    }else if(s.compare(ST_SUPPORT)==0){
        ret = stSUPPORT;
    }else if(s.compare(ST_ALL)==0){
        ret = stALL;
    }
    return ret;
}
QString UTILS::toStringSub(SUB_TYPE t){
    QString ret;
    switch(t){
    case stDIRECTLY:
        ret = ST_DIRECTLY;
        break;
    case stATTACHED:
        ret = ST_ATTACHED;
        break;
    case stSTRENGTHENING:
        ret = ST_STRENGTHENING;
        break;
    case stSUPPORT:
        ret = ST_SUPPORT;
        break;
    case stALL:
        ret = ST_ALL;
        break;
    default:
        ret = ST_UNDEF;
    }
    return ret;
}

