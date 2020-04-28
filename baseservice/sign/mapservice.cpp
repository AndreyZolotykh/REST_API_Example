#include "mapservice.h"
#include <QJsonDocument>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>

#include "../dbconnection.h"

MapService* MapService::_inatance = NULL;
MapService::Ctrl MapService::_ctrl;

MapService::Ctrl::Ctrl(){

}
MapService::Ctrl::~Ctrl(){
    if(_inatance!=NULL){
        delete _inatance;
    }
}
MapService::MapService(){

}
MapService::~MapService(){

}
MapService *MapService::getInstance(){
    if(_inatance==NULL){
        _inatance = new MapService();
    }
    return _inatance;
}
const char* CONN_NAME_MAP = "MapService";

const char *qINSERT_UPD_MAP = "SELECT * FROM v1.map_insert_update(?,?,?,?,?,?)";
const char *qSELECT_MAP = "SELECT * FROM v1.map_select(:MAPID,:LAYERID,:VARID)";
const char *qDELETE_MAP = "SELECT * FROM v1.map_delete(:MAPID,:LAYERID,:VARID,false)";
const char *qDELETE_MAP_ALL = "SELECT * FROM v1.map_delete(NULL,NULL,NULL,true)";

const char* PARAM_MAP_MAPID = "mapid";
const char* PARAM_MAP_LAYERID = "layerid";
const char* PARAM_MAP_LAYERNAME = "layername";
const char* PARAM_MAP_VARID = "varid";
const char* PARAM_MAP_ISALL = "isall";

RETURN_CODE parseParam(QUuid& mapid, QUuid& layerid, QUuid& varid, AbstructTransferObject* p){
    RETURN_CODE ret = OK;

    if(p->isParam(PARAM_MAP_MAPID) && p->isParam(PARAM_MAP_LAYERID) && p->isParam(PARAM_MAP_VARID)){
        mapid = QUuid::fromString(p->getParam(PARAM_MAP_MAPID));
        layerid = QUuid::fromString(p->getParam(PARAM_MAP_LAYERID));
        varid = QUuid::fromString(p->getParam(PARAM_MAP_VARID));
    }else{
        ret = WRONG_REQUEST;
    }

    return ret;
}

RETURN_CODE MapService::delMaps(AbstructTransferObject* p){
    RETURN_CODE ret = OK;


    QUuid mapid,layerid, varid;

    bool isAll = false;
    if(p->isParam(PARAM_MAP_ISALL)){
        try{
            bool tmp = QVariant(p->getParam(PARAM_MAP_ISALL)).toBool();
            isAll = tmp;
        }catch(...){
            ret = WRONG_REQUEST;
        }
    }
    if(!isAll){
        ret = parseParam(mapid, layerid, varid, p);
    }


    if(ret==OK){
        {
            QSqlDatabase conn = getConnection(CONN_NAME_MAP,p);

            bool bOpen = false;
            if(!conn.isOpen()){
                if(!conn.open()){
                    qWarning()<<"Error while open db";
                    return ret;
                }
                bOpen = true;
            }

            QSqlQuery *q = new QSqlQuery(conn);


            if(isAll){
                q->prepare(qDELETE_MAP_ALL);
            }else{
                q->prepare(qDELETE_MAP);
                q->bindValue(":MAPID",mapid.toString());
                q->bindValue(":LAYERID",layerid.toString());
                q->bindValue(":VARID",varid.toString());
            }


            if(!q->exec()){
                qWarning()<<"Error while exec query: "<<q->lastError().text();
                p->setReturnCode(INTERNAL_ERROR);
                ret = INTERNAL_ERROR;
            }
            delete q;
            if(bOpen){conn.close();}

        }
        QSqlDatabase::removeDatabase(CONN_NAME_MAP);

    }

    return ret;
}
TMapItemList* MapService::getMaps(AbstructTransferObject* p){
    TMapItemList* ret = new TMapItemList();

    QUuid mapid,layerid, varid;
    RETURN_CODE sts = parseParam(mapid, layerid, varid, p);


    if(sts==OK){

        {
            QSqlDatabase conn = getConnection(CONN_NAME_MAP,p);

            bool bOpen = false;
            if(!conn.isOpen()){
                if(!conn.open()){
                    qWarning()<<"Error while open db";
                    return ret;
                }
                bOpen = true;
            }

            QSqlQuery *q = new QSqlQuery(conn);
            q->prepare(qSELECT_MAP);
            q->bindValue(":MAPID",mapid.toString());
            q->bindValue(":LAYERID",layerid.toString());
            q->bindValue(":VARID",varid.toString());

            if(q->exec()){
                while(q->next()){
                    TMap* newItem = new TMap();

                    newItem->setMapId(QUuid::fromString(q->value("mapid").toString()));
                    newItem->setLayerId(QUuid::fromString(q->value("layerid").toString()));
                    newItem->setVarId(QUuid::fromString(q->value("varid").toString()));
                    newItem->setAttr(QJsonDocument::fromJson(q->value("attr").toString().toUtf8()).object());
                    newItem->setLayerName(q->value("layername").toString());
                    ret->add(newItem);
                }
            }else{
                qWarning()<<"Error while exec query: "<<q->lastError().text();
                p->setReturnCode(INTERNAL_ERROR);
            }
            delete q;
            if(bOpen){conn.close();}

        }
        QSqlDatabase::removeDatabase(CONN_NAME_MAP);
    }else{
        p->setReturnCode(sts);
    }

    return ret;

}
TMapItemList* MapService::updMaps(AbstructTransferObject* p,bool upd){
    TMapItemList* ret = NULL;

    TMapItemList *lsItems = new TMapItemList();
    QJsonDocument doc = QJsonDocument::fromJson(p->getInputJSON().toUtf8());

    QJsonObject mainObj = doc.object();
    if(lsItems->fromJson(mainObj)){
        if(!lsItems->isEmpty()){
            {
                QSqlDatabase conn = getConnection(CONN_NAME_MAP,p);

                bool bOpen = false;
                if(!conn.isOpen()){
                    if(!conn.open()){
                        qWarning()<<"Error while open db";
                        return ret;
                    }
                    bOpen = true;
                }

                QSqlQuery *q = new QSqlQuery(conn);

                QVariantList mapids;
                QVariantList layerids;
                QVariantList varids;
                QVariantList attrs;
                QVariantList layernames;
                QVariantList modes;

                QList<TMap*>::iterator iter = lsItems->begin();
                for(;iter!=lsItems->end();++iter){
                    TMap* item = *iter;

                    mapids<<item->getMapId().toString();
                    layerids<<item->getLayerId().toString();
                    varids<<item->getVarId().toString();
                    attrs<<QString(QJsonDocument(item->getAttr()).toJson(QJsonDocument::Compact));
                    layernames<<item->getLayerName();
                    modes<<upd;

                }

                q->prepare(qINSERT_UPD_MAP);


                q->addBindValue(mapids);
                q->addBindValue(layerids);
                q->addBindValue(varids);
                q->addBindValue(attrs);
                q->addBindValue(layernames);
                q->addBindValue(modes);

                if(!q->execBatch()){
                    qWarning()<<"Error while exec query: "<<q->lastError().text();
                    p->setReturnCode(INTERNAL_ERROR);
                }else{
                    ret = lsItems;

                }
                delete q;
                if(bOpen){conn.close();}

            }
            QSqlDatabase::removeDatabase(CONN_NAME_MAP);
        }else{
            qWarning()<<"Empty list!!";
        }



    }else{

        p->setReturnCode(ERR_VERIFY_DATA);
    }

    return ret;
}
