#include "layerservice.h"
#include <QJsonDocument>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>

#include "../dbconnection.h"

LayerService* LayerService::_inatance = NULL;
LayerService::Ctrl LayerService::_ctrl;

LayerService::Ctrl::Ctrl(){

}
LayerService::Ctrl::~Ctrl(){
    if(_inatance!=NULL){
        delete _inatance;
    }
}
LayerService::LayerService(){

}
LayerService::~LayerService(){

}
LayerService *LayerService::getInstance(){
    if(_inatance==NULL){
        _inatance = new LayerService();
    }
    return _inatance;
}
const char* CONN_NAME_LAYER = "LayerService";


const char *qSELECT_LAYER = "SELECT * FROM v1.layer_select(:MAPID,:VARID)";

const char* PARAM_LAYER_MAPID = "mapid";
const char* PARAM_LAYER_VARID = "varid";

RETURN_CODE parseParam(QUuid& mapid, QUuid& varid, AbstructTransferObject* p){
    RETURN_CODE ret = OK;

    if(p->isParam(PARAM_LAYER_MAPID) && p->isParam(PARAM_LAYER_VARID)){
        mapid = QUuid::fromString(p->getParam(PARAM_LAYER_MAPID));
        varid = QUuid::fromString(p->getParam(PARAM_LAYER_VARID));
    }else{
        ret = WRONG_REQUEST;
    }

    return ret;
}

TLayerItemList* LayerService::getLayers(AbstructTransferObject* p){
    TLayerItemList* ret = new TLayerItemList();

    QUuid mapid, varid;
    RETURN_CODE sts = parseParam(mapid, varid, p);


    if(sts==OK){

        {
            QSqlDatabase conn = getConnection(CONN_NAME_LAYER,p);

            bool bOpen = false;
            if(!conn.isOpen()){
                if(!conn.open()){
                    qWarning()<<"Error while open db";
                    return ret;
                }
                bOpen = true;
            }

            QSqlQuery *q = new QSqlQuery(conn);
            q->prepare(qSELECT_LAYER);
            q->bindValue(":MAPID",mapid.toString());
            q->bindValue(":VARID",varid.toString());

            if(q->exec()){
                while(q->next()){
                    TLayer* newItem = new TLayer();

                    newItem->setMapId(QUuid::fromString(q->value(TLayer::stDBField_MapId).toString()));
                    newItem->setLayerId(QUuid::fromString(q->value(TLayer::stDBField_LayerId).toString()));
                    newItem->setVarId(QUuid::fromString(q->value(TLayer::stDBField_VarId).toString()));
                    newItem->setLayerName(q->value(TLayer::stDBField_LayerName).toString());
                    ret->add(newItem);
                }
            }else{
                qWarning()<<"Error while exec query: "<<q->lastError().text();
                p->setReturnCode(INTERNAL_ERROR);
            }
            delete q;
            if(bOpen){conn.close();}

        }
        QSqlDatabase::removeDatabase(CONN_NAME_LAYER);
    }else{
        p->setReturnCode(sts);
    }

    return ret;

}
