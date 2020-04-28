#include "signservice.h"
#include <QJsonDocument>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>

#include "../dbconnection.h"

SignService* SignService::_inatance = NULL;
SignService::Ctrl SignService::_ctrl;

SignService::Ctrl::Ctrl(){

}
SignService::Ctrl::~Ctrl(){
    if(_inatance!=NULL){
        delete _inatance;
    }
}
SignService::SignService(){

}
SignService *SignService::getInstance(){
    if(_inatance==NULL){
        _inatance = new SignService();
    }
    return _inatance;
}
const char* CONN_NAME_SIGN = "SignService";

const char *qINSERT_UPD_SIGN = "SELECT * FROM v1.sign_insert_update(?,?,?,?,?,?,?,?,?)";
const char *qSELECT_SIGN = "SELECT * FROM v1.sign_select()";
const char *qDELETE_SIGN = "SELECT * FROM v1.sign_delete(?,false)";
const char *qDELETE_SIGN_ALL = "SELECT * FROM v1.sign_delete('00000000-0000-0000-0000-000000000000',true)";

const char* PARAM_SIGNSID = "signsid";
const char* PARAM_ISALL = "isall";
RETURN_CODE SignService::delSigns(AbstructTransferObject* p){
    RETURN_CODE ret = OK;

    QVariantList ids;
    QVariantList fake;
    if(p->isParam(PARAM_SIGNSID)){
        QStringList ss = p->getParam(PARAM_SIGNSID).split(",");
        foreach (QString it, ss) {
            try{
                QUuid tmp = QUuid::fromString(it);
                if(tmp.isNull()){
                    ret = WRONG_REQUEST;
                    ids.clear();
                    break;
                }else{
                    ids<<tmp.toString();
                }
            }catch(...){
                ret = WRONG_REQUEST;
            }
            fake<<false;
        }
    }
    bool isAll = false;
    if(p->isParam(PARAM_ISALL)){
        try{
            bool tmp = QVariant(p->getParam(PARAM_ISALL)).toBool();
            isAll = tmp;
        }catch(...){
            ret = WRONG_REQUEST;
        }
    }
    if(!isAll && ids.isEmpty()){
        ret = WRONG_REQUEST;
    }else{

        {
            QSqlDatabase conn = getConnection(CONN_NAME_SIGN,p);

            bool bOpen = false;
            if(!conn.isOpen()){
                if(!conn.open()){
                    qWarning()<<"Error while open db:"<<conn.connectionName();
                    return ret;
                }
                bOpen = true;
            }

            QSqlQuery *q = new QSqlQuery(conn);



            bool res = false;
            if(isAll){
                q->prepare(qDELETE_SIGN_ALL);
                res = q->exec();
            }else{
                q->prepare(qDELETE_SIGN);
                q->addBindValue(ids);
                res = q->execBatch();
            }


            if(!res){
                qWarning()<<"Error while exec query: "<<q->lastError().text();
                p->setReturnCode(INTERNAL_ERROR);
                ret = INTERNAL_ERROR;
            }
            delete q;
            if(bOpen){conn.close();}

        }
        QSqlDatabase::removeDatabase(CONN_NAME_SIGN);

    }

    return ret;
}
TSignItemList* SignService::getSigns(AbstructTransferObject* p){
    TSignItemList* ret = new TSignItemList();

    {
        QSqlDatabase conn = getConnection(CONN_NAME_SIGN,p);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db: "<<conn.connectionName();
                return ret;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);
        q->prepare(qSELECT_SIGN);
        if(q->exec()){
            while(q->next()){
                TSign* newItem = new TSign();

                newItem->setId(QUuid::fromString(q->value("id").toString()));
                newItem->setMapId(QUuid::fromString(q->value("mapid").toString()));
                newItem->setLayerId(QUuid::fromString(q->value("layerid").toString()));
                newItem->setVarId(QUuid::fromString(q->value("varid").toString()));
                newItem->setDepId(QUuid::fromString(q->value("depid").toString()));
                newItem->setClassCode(q->value("classcode").toString());
                newItem->setParam(QJsonDocument::fromJson(q->value("param").toString().toUtf8()).object());
                newItem->setAttr(QJsonDocument::fromJson(q->value("attr").toString().toUtf8()).object());

                ret->add(newItem);
            }
        }else{
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            p->setReturnCode(INTERNAL_ERROR);
        }
        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME_SIGN);


    return ret;

}
TSignItemList* SignService::updSigns(AbstructTransferObject* p,bool upd){
    TSignItemList* ret = NULL;

    TSignItemList *lsItems = new TSignItemList();
    QJsonDocument doc = QJsonDocument::fromJson(p->getInputJSON().toUtf8());

    QJsonObject mainObj = doc.object();
    if(lsItems->fromJson(mainObj)){
        if(!lsItems->isEmpty()){
            {
                QSqlDatabase conn = getConnection(CONN_NAME_SIGN,p);

                bool bOpen = false;
                if(!conn.isOpen()){
                    if(!conn.open()){
                        qWarning()<<"Error while open db: " << conn.connectionName();
                        return ret;
                    }
                    bOpen = true;
                }

                QSqlQuery *q = new QSqlQuery(conn);

                QVariantList ids;
                QVariantList mapids;
                QVariantList layerids;
                QVariantList varids;
                QVariantList depids;
                QVariantList classcodes;
                QVariantList params;
                QVariantList attrs;
                QVariantList modes;

                QList<TSign*>::iterator iter = lsItems->begin();
                for(;iter!=lsItems->end();++iter){
                    TSign* item = *iter;
                    if(!upd){
                        item->setId(QUuid::createUuid());
                    }
                    ids<<item->getId().toString();
                    mapids<<item->getMapId().toString();
                    layerids<<item->getLayerId().toString();
                    varids<<item->getVarId().toString();
                    depids<<item->getDepId().toString();
                    classcodes<<item->getClassCode();
                    params<<QString(QJsonDocument(item->getParam()).toJson(QJsonDocument::Compact));
                    attrs<<QString(QJsonDocument(item->getAttr()).toJson(QJsonDocument::Compact));

                    modes<<upd;

                }

                q->prepare(qINSERT_UPD_SIGN);

                q->addBindValue(ids);
                q->addBindValue(mapids);
                q->addBindValue(layerids);
                q->addBindValue(varids);
                q->addBindValue(depids);
                q->addBindValue(classcodes);
                q->addBindValue(params);
                q->addBindValue(attrs);
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
            QSqlDatabase::removeDatabase(CONN_NAME_SIGN);
        }else{
            qWarning()<<"Empty list!!";
        }



    }else{

        p->setReturnCode(ERR_VERIFY_DATA);
    }

    return ret;
}
