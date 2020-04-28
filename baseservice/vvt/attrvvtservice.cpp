#include "attrvvtservice.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonDocument>

#include "../dbconnection.h"


const char* AttrVVTService::CONN_NAME = "AttrVVTService";

const char* AttrVVTService::PARAM_GR = "gr";
const char* AttrVVTService::PARAM_ID = "id";

const char* AttrVVTService::qSELECT_ATTR_DIC = "SELECT * FROM v1.attr_select(:GR)";
const char* AttrVVTService::qSELECT_ATTR_VVT_DIC = "SELECT * FROM v1.attr_vvt_select(:ID)";
const char* AttrVVTService::qINSERT_UPDATE_ATTR_VVT_DIC = "SELECT * FROM v1.attr_vvt_update(:ID,:ATTR,:UPD)";
const char* AttrVVTService::qDELETE_ATTR_VVT_DIC = "SELECT * FROM v1.attr_vvt_delete(:ID,:ISALL)";

const char* AttrVVTService::qPARAM_GR = ":GR";
const char* AttrVVTService::qPARAM_ID = ":ID";
const char* AttrVVTService::qPARAM_ATTR = ":ATTR";
const char* AttrVVTService::qPARAM_ISALL = ":ISALL";
const char* AttrVVTService::qPARAM_UPD = ":UPD";



TAttrVVTItemList* AttrVVTService::getAttrVVT(AbstructTransferObject* p){
    TAttrVVTItemList* ret = new TAttrVVTItemList();

    QStringList ids;
    if(p->isParam(PARAM_ID)){
        ids = p->getParam(PARAM_ID).split(",");
    }else{
        p->setReturnCode(WRONG_REQUEST);
        return ret;
    }

    {
        QSqlDatabase conn = getConnection(CONN_NAME,p);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return ret;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);
        q->prepare(qSELECT_ATTR_VVT_DIC);
        q->addBindValue(ids);


        if( q->execBatch() ){
            while(q->next()){
                TAttrVVTItem* newItem = new TAttrVVTItem();

                newItem->fromJson(QJsonDocument::fromJson(q->value(TAttrVVTItem::getDBField_attr()).toString().toUtf8()).object());
                newItem->setId(QUuid(q->value(TAttrVVTItem::getDBField_id()).toString()));

                ret->add(newItem);
            }
        }else{
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            p->setReturnCode(INTERNAL_ERROR);
        }

        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    return ret;
}

TAttrVVTItem* AttrVVTService::updAttrVVT(AbstructTransferObject* p, bool upd){
    TAttrVVTItem* ret = NULL;

    TAttrVVTItem* item = new TAttrVVTItem();
    QJsonDocument doc = QJsonDocument::fromJson(p->getInputJSON().toUtf8());

    QJsonObject mainObj = doc.object();
    if(item->fromJson(mainObj)){
        QSqlDatabase conn = getConnection(CONN_NAME,p);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return ret;
            }
            bOpen = true;
        }



        QSqlQuery *q = new QSqlQuery(conn);
        q->prepare(qINSERT_UPDATE_ATTR_VVT_DIC);
        q->bindValue(qPARAM_ID,item->getId().toString());
        q->bindValue(qPARAM_ATTR, QString(QJsonDocument(item->toJson()).toJson(QJsonDocument::Compact)) );
        q->bindValue(qPARAM_UPD,upd);


        if( !q->exec() ){
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            p->setReturnCode(INTERNAL_ERROR);
        }

        delete q;
        if(bOpen){conn.close();}

        ret = item;
    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    return ret;
}

RETURN_CODE AttrVVTService::delAttrVVT(AbstructTransferObject* p){
    RETURN_CODE ret = OK;

    bool isAll = true;
    QStringList ids;
    QVariantList mode;
    if(p->isParam(PARAM_ID)){
        ids = p->getParam(PARAM_ID).split(",");
        foreach(QString it, ids){
            mode << false;
        }
    }
    {
        QSqlDatabase conn = getConnection(CONN_NAME,p);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return ret;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);
        q->prepare(qDELETE_ATTR_VVT_DIC);

        bool res;
        if(isAll){
            q->bindValue(qPARAM_ID,"00000000-0000-0000-0000-000000000000");
            q->bindValue(qPARAM_ISALL,true);
            res = !q->exec();
        }else{
            q->addBindValue(ids);
            q->addBindValue(mode);
            res = q->execBatch();
        }

        if(!res){
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            ret = INTERNAL_ERROR;
        }

        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    return ret;
}


TAttrItemList* AttrVVTService::getAttrList(AbstructTransferObject *p){
    TAttrItemList *ret= new TAttrItemList();

    int gr = -1;
    if(p->isParam(PARAM_GR)){
        gr = p->getParam(PARAM_GR).toInt();
    }
    {
        QSqlDatabase conn = getConnection(CONN_NAME,p);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return ret;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);
        q->prepare(qSELECT_ATTR_DIC);
        q->bindValue(qPARAM_GR,gr);

        if(q->exec()){
            while(q->next()){
                TAttrItem* newItem = new TAttrItem();

                newItem->setId(q->value(TAttrItem::getDBField_id()).toInt());
                newItem->setTitle(q->value(TAttrItem::getDBField_title()).toString());
                newItem->setShortTitle((q->value(TAttrItem::getDBField_stitle()).toString()));
                newItem->setGroup((q->value(TAttrItem::getDBField_gr()).toInt()));

                ret->add(newItem);
            }
        }else{
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            p->setReturnCode(INTERNAL_ERROR);
        }
        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    p->setReturnCode(OK);



    return ret;
}

// Common code -----------------------------------------------------------------------------------
AttrVVTService* AttrVVTService::_inatance = NULL;
AttrVVTService::Ctrl AttrVVTService::_ctrl;

AttrVVTService::Ctrl::Ctrl(){

}
AttrVVTService::Ctrl::~Ctrl(){
    if(_inatance!=NULL){
        delete _inatance;
    }
}
AttrVVTService::AttrVVTService(){

}
AttrVVTService::~AttrVVTService(){

}


AttrVVTService *AttrVVTService::getInstance(){
    if(_inatance==NULL){
        _inatance = new AttrVVTService();
    }
    return _inatance;
}
