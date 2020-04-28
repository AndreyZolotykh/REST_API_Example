#include "baseservice.h"
#include <QJsonDocument>
#include <QDebug>
#include "var/tvar.h"
#include "deployarea/deployareas.h"
#include "sign/signservice.h"
#include "sign/mapservice.h"
#include "sign/layerservice.h"
#include "oshs/oshsservice.h"
#include "regrouping/marchservice.h"
#include "regrouping/regroupingservice.h"
#include "vvt/attrvvtservice.h"
#include "utils/utils.h"


BASESERVICESHARED_EXPORT int dicVVT(AbstructTransferObject** obj){
//    AbstructTransferObject *p = *obj;
    int ret = 0;

    return ret;
}

const char* PARAM_OWNER = "owner";
const char* PARAM_SUBTYPE = "subtype";
const char* PARAM_ITEMID = "itemid";
const char* PARAM_VARID = "varid";
const char* PARAM_FILTERID = "filterid";
const char* PARAM_LEVEL = "level";

BASESERVICESHARED_EXPORT int dicOSHS(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;
    OSHSService* serv = OSHSService::getInstance();

    if(p->getMethod()==GET){
#ifdef QT_DEBUG
        QString paramTest = p->getParam("test");
        QString paramFake = p->getParam("fake");
#endif
        OWNER_TYPE ownerType = owOUR;
        if(p->isParam(PARAM_OWNER)){
            ownerType = UTILS::fromStringOwner(p->getParam(PARAM_OWNER));
        }
        SUB_TYPE subType = stALL;
        if(p->isParam(PARAM_SUBTYPE)){
            subType = UTILS::fromStringSub(p->getParam(PARAM_SUBTYPE));
        }
        QUuid itemId;
        if(p->isParam(PARAM_ITEMID)){
            try{
                itemId = QUuid::fromString(p->getParam(PARAM_ITEMID));
            }catch(...){
                qWarning()<<"Wrong uuid format itemid="<<p->getParam(PARAM_ITEMID);
            }
        }
        QUuid varId;
        if(p->isParam(PARAM_VARID)){
            try{
                varId = QUuid::fromString(p->getParam(PARAM_VARID));
            }catch(...){
                qWarning()<<"Wrong uuid format varid="<<p->getParam(PARAM_VARID);
            }
        }
        QUuid filterId;
        if(p->isParam(PARAM_FILTERID)){
            try{
                filterId = QUuid::fromString(p->getParam(PARAM_FILTERID));
            }catch(...){
                qWarning()<<"Wrong uuid format filterid="<<p->getParam(PARAM_FILTERID);
            }
        }
        int level = 0;
        if(p->isParam(PARAM_LEVEL)){
            try{
                level = atoi(p->getParam(PARAM_LEVEL).toStdString().c_str());
            }catch(...){
                qWarning()<<"Wrong uuid format level="<<p->getParam(PARAM_LEVEL);
            }
        }
#ifdef QT_DEBUG
        if(!paramTest.isEmpty()){
            TOSHSItem* root = serv->getOSHSTEST(p);
            if(root!=NULL){
                p->setContent(serializeObjToString((ISerialize*)root));
                delete root;
            }else{
                p->setReturnCode(INTERNAL_ERROR);
            }
        }else if(!paramFake.isEmpty()){
            TOSHSItem* root = serv->getOSHSFAKE(p);
            if(root!=NULL){
                p->setContent(serializeObjToString((ISerialize*)root));
                delete root;
            }else{
                p->setReturnCode(INTERNAL_ERROR);
            }
        }else
#endif
        {
            TOSHSItem* root = serv->getOSHS(p);
            if(root!=NULL){
                p->setContent(serializeObjToString((ISerialize*)root));
                delete root;
            }else{
                p->setReturnCode(INTERNAL_ERROR);
            }
        }
    }

    return ret;
}

const char* PARAM_SUM = "sum";
BASESERVICESHARED_EXPORT int dicOSHSVVT(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    if(p->getMethod()==GET){
#ifdef QT_DEBUG
        QString paramFake = p->getParam("fake");
#endif
        QUuid itemId;
        if(p->isParam(PARAM_ITEMID)){
            try{
                itemId = QUuid::fromString(p->getParam(PARAM_ITEMID));
            }catch(...){
                qWarning()<<"Wrong uuid format itemid="<<p->getParam(PARAM_ITEMID);
            }
        }else{
            p->setReturnCode(WRONG_REQUEST);
            qWarning()<<"Bad request, not set itemid";
        }
        QUuid varId;
        if(p->isParam(PARAM_VARID)){
            try{
                varId = QUuid::fromString(p->getParam(PARAM_VARID));
            }catch(...){
                qWarning()<<"Wrong uuid format varid="<<p->getParam(PARAM_VARID);
            }
        }else{
            p->setReturnCode(WRONG_REQUEST);
            qWarning()<<"Bad request, not set itemid";
        }
        bool isSum=false;
        if(p->isParam(PARAM_SUM)){
            try{
                isSum =  static_cast<QVariant>(p->getParam(PARAM_SUM)).toBool();
            }catch(...){
                qWarning()<<"Wrong param sum = "<<p->getParam(PARAM_SUM);
            }
        }
     }
    return ret;
}


BASESERVICESHARED_EXPORT int dicVariant(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    if(p->getMethod()==GET){
#ifdef QT_DEBUG
        QString paramTest = p->getParam("test");
        if(!paramTest.isEmpty()){
            p->setContent(VAR::getVariantsTEST());
        }else
#endif
        {
            p->setContent(serializeObjToString((ISerialize*)getVariants(p)));
        }
    }else if(p->getMethod()==DELETE) {
        int ret = delVariants(p);
        if(ret!=0){
            p->setReturnCode(ERR_VERIFY_DATA);
        }
    }else if(p->getMethod()==POST || p->getMethod()==PUT){
        int ret = setVariants(p);
        if(ret!=0){
            p->setReturnCode(ERR_VERIFY_DATA);
        }
    }else if(p->getMethod()==PATCH) {
        VAR::createCurrentVariant(p);
    }else{
        p->setReturnCode(WRONG_REQUEST);
    }


    return ret;
}

int dicDeployAreas(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    DeployAreasCtrl* serv = DeployAreasCtrl::getInstance();
    
    
    if(p->getMethod()==GET){
        TDeployAreasItemList* dic = serv->getDic(p);
        if(dic==NULL){
            p->setReturnCode(INTERNAL_ERROR);
        }else{
            p->setContent(serializeObjToString((ISerialize*)dic));
            delete dic;
        }
    }else if(p->getMethod()==DELETE) {
        p->setReturnCode(UNAVAILABLE);
    }else if(p->getMethod()==POST || p->getMethod()==PUT){
        p->setReturnCode(UNAVAILABLE);
    }else{
        p->setReturnCode(WRONG_REQUEST);
    }


    return ret;    
}

int map(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    MapService* serv = MapService::getInstance();


    if(p->getMethod()==GET){
        TMapItemList* ls = serv->getMaps(p);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else if(p->getMethod()==DELETE) {
        RETURN_CODE sts = serv->delMaps(p);
        p->setReturnCode(sts);
    }else if(p->getMethod()==POST || p->getMethod()==PUT){
        bool mode = p->getMethod()==POST?false:true;
        TMapItemList* ls = serv->updMaps(p, mode);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else{
//        p->setReturnCode(WRONG_REQUEST);
        p->setReturnCode(OK); // FIX FOR FAKE ANSWER
    }


    return ret;
}
int dicAttr(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    AttrVVTService* serv = AttrVVTService::getInstance();


    if(p->getMethod()==GET){
        TAttrItemList* ls = serv->getAttrList(p);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else if(p->getMethod()==DELETE) {
        RETURN_CODE sts = UNAVAILABLE;
        p->setReturnCode(sts);
    }else if(p->getMethod()==POST || p->getMethod()==PUT){
        RETURN_CODE sts = UNAVAILABLE;
        p->setReturnCode(sts);
    }else{
        p->setReturnCode(OK);
    }


    return ret;
}
int dicAttrVVT(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    AttrVVTService* serv = AttrVVTService::getInstance();


    if(p->getMethod()==GET){
        TAttrVVTItemList *ls = serv->getAttrVVT(p);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else if(p->getMethod()==DELETE) {
        RETURN_CODE sts = serv->delAttrVVT(p);
        p->setReturnCode(sts);
    }else if(p->getMethod()==POST || p->getMethod()==PUT){
        bool mode = p->getMethod()==POST?false:true;
        TAttrVVTItem* ls = serv->updAttrVVT(p, mode);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else{
        p->setReturnCode(OK);
    }


    return ret;
}
int dicMarch(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    MarchService* serv = MarchService::getInstance();


    if(p->getMethod()==GET){
        TMarchParamDic* ls = serv->getMarchParamDic(p);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else if(p->getMethod()==DELETE) {
        p->setReturnCode(WRONG_REQUEST);
    }else if(p->getMethod()==POST || p->getMethod()==PUT){
        p->setReturnCode(WRONG_REQUEST);
    }else if(p->getMethod()==PATCH){
        p->setReturnCode(WRONG_REQUEST);
    }else{
        p->setReturnCode(OK);// FIX FOR FAKE ANSWER
    }


    return ret;
}
int dicDicRegroupingParamPt(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    RegroupingService* serv = RegroupingService::getInstance();


    if(p->getMethod()==GET){
        TRegroupingParamPtItemList* ls = serv->getRegroupingParamPtItemList(p);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else if(p->getMethod()==DELETE) {
        RETURN_CODE sts = serv->delRegroupingParamPtItemList(p);
        p->setReturnCode(sts);
    }else if(p->getMethod()==POST || p->getMethod()==PUT){
        bool mode = p->getMethod()==POST?false:true;
        TRegroupingParamPtItem* march = serv->updRegroupingParamPtItem(p, mode);
        if(march!=NULL){
            p->setContent(serializeObjToString((ISerialize*)march));
            delete march;
        }
    }else{
        p->setReturnCode(OK);// FIX FOR FAKE ANSWER
    }


    return ret;
}
int march(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    MarchService* serv = MarchService::getInstance();


    if(p->getMethod()==GET){
        TMarchItemList* ls = serv->getMarchs(p);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else if(p->getMethod()==DELETE) {
        RETURN_CODE sts = serv->delMarch(p);
        p->setReturnCode(sts);
    }else if(p->getMethod()==POST || p->getMethod()==PUT){
        bool mode = p->getMethod()==POST?false:true;
        TMarch* march = serv->updMarch(p, mode);
        if(march!=NULL){
            p->setContent(serializeObjToString((ISerialize*)march));
            delete march;
        }
    }else if(p->getMethod()==PATCH){
        RETURN_CODE sts = serv->solveMarch(p);
        p->setReturnCode(sts);
    }else{
        p->setReturnCode(OK);// FIX FOR FAKE ANSWER
    }


    return ret;
}
int layer(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    LayerService* serv = LayerService::getInstance();


    if(p->getMethod()==GET){
        TLayerItemList* ls = serv->getLayers(p);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else if(p->getMethod()==DELETE) {
        p->setReturnCode(WRONG_REQUEST);
    }else if(p->getMethod()==POST || p->getMethod()==PUT){
        p->setReturnCode(WRONG_REQUEST);
    }else{
        p->setReturnCode(OK);// FIX FOR FAKE ANSWER
    }


    return ret;
}
int sign(AbstructTransferObject** obj){
    AbstructTransferObject *p = *obj;
    int ret = 0;

    SignService* serv = SignService::getInstance();

    if(p->getMethod()==GET){
        TSignItemList* ls = serv->getSigns(p);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else if(p->getMethod()==DELETE) {
        RETURN_CODE sts = serv->delSigns(p);
        p->setReturnCode(sts);
    }else if(p->getMethod()==POST || p->getMethod()==PUT){
        bool mode = p->getMethod()==POST?false:true;
        TSignItemList* ls = serv->updSigns(p, mode);
        if(ls!=NULL){
            p->setContent(serializeObjToString((ISerialize*)ls));
            delete ls;
        }
    }else{
        p->setReturnCode(WRONG_REQUEST);
    }


    return ret;
}

QString serializeObjToString(ISerialize *obj){
    QJsonDocument doc = QJsonDocument(obj->toJson());
    return QString(doc.toJson(QJsonDocument::Compact));
}
