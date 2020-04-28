#include "regroupingservice.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>

#include "../dbconnection.h"

const char* RegroupingService::CONN_NAME = "RegroupingService::";
const char* RegroupingService::PARAM_ID = "id";
const char* RegroupingService::PARAM_TYPE = "type";

const char* RegroupingService::qDELETE = "SELECT * FROM v1.regrouping_param_pt_delete(:ID,:TYPE,:ISALL)";
const char* RegroupingService::qSELECT = "SELECT * FROM v1.regrouping_param_pt_select(:TYPE)";
const char* RegroupingService::qINSERT_UPD= "SELECT * FROM v1.regrouping_param_pt_update(:ID,:TYPE,:TITLE,:UPLOAD,:DOWNLOAD,:ATTR,:GID,:UPD)";

const char* RegroupingService::qDELETE_GR = "SELECT * FROM v1.regrouping_gr_pt_delete(:ID,:TYPE,:ISALL)";
const char* RegroupingService::qSELECT_GR = "SELECT * FROM v1.regrouping_gr_pt_select(:TYPE)";
const char* RegroupingService::qINSERT_UPD_GR= "SELECT * FROM v1.regrouping_gr_pt_update(:ID,:TYPE,:TITLE,:UPD)";


const char* RegroupingService::qPARAM_ID = ":ID";
const char* RegroupingService::qPARAM_TITLE = ":TITLE";
const char* RegroupingService::qPARAM_TYPE = ":TYPE";
const char* RegroupingService::qPARAM_UPLOAD = ":UPLOAD";
const char* RegroupingService::qPARAM_DOWNLOAD = ":DOWNLOAD";
const char* RegroupingService::qPARAM_ATTR = ":ATTR";
const char* RegroupingService::qPARAM_GID = ":GID";
const char* RegroupingService::qPARAM_GTITLE = ":GTITLE";
const char* RegroupingService::qPARAM_UPD = ":UPD";
const char* RegroupingService::qPARAM_ISALL = ":ISALL";


TRegroupingParamPtItemList* RegroupingService::getRegroupingParamPtItemList(AbstructTransferObject* p){
    TRegroupingParamPtItemList* ret = new TRegroupingParamPtItemList();

    int type = 0;
    RETURN_CODE sts = OK;
    if(p->isParam(PARAM_TYPE)){
        type = p->getParam(PARAM_TYPE).toInt();
    }

    if(sts==OK){
        QMap<QString,QString> mp;
        conv(p, mp);
        sts = getRegroupingParamPtItemList(ret, mp, type);
    }
    p->setReturnCode(sts);

    return ret;
}
RETURN_CODE RegroupingService::getRegroupingParamPtItemList(TRegroupingParamPtItemList* lsParam, const QMap<QString, QString> &mp, int type){
    RETURN_CODE sts = OK;

    {
        QSqlDatabase conn = getConnection(CONN_NAME,mp);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return INTERNAL_ERROR;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);
        q->prepare(qSELECT);
        q->bindValue(qPARAM_TYPE,type);


        if(q->exec()){
            while(q->next()){
                TRegroupingParamPtItem* newItem = new TRegroupingParamPtItem();

                newItem->setId(q->value(TRegroupingParamPtItem::getDBField_id()).toString());
                newItem->setType((REGROUPING_TYPE)q->value(TRegroupingParamPtItem::getDBField_type()).toInt());
                newItem->setTitle(q->value(TRegroupingParamPtItem::getDBField_title()).toString());
                newItem->setUpload(q->value(TRegroupingParamPtItem::getDBField_upload()).toDouble());
                newItem->setDownload(q->value(TRegroupingParamPtItem::getDBField_download()).toDouble());

                QVariant gid =  q->value(TRegroupingParamPtItem::getDBField_group());
                QVariant gtitle =  q->value(TRegroupingParamPtItem::getDBField_gtitle());
                if(!gid.isNull() && gid.isValid() ){
                    TRegroupingParamPtGroup *gr = new TRegroupingParamPtGroup();
                    gr->setId( QUuid(gid.toString()) );
                    gr->setTitle( gtitle.toString() );
                    gr->setType( newItem->getType() );
                    newItem->setGroup(gr);
                }

                newItem->setAttr(QJsonDocument::fromJson(q->value(TRegroupingParamPtItem::getDBField_attr()).toString().toUtf8()).object());
                lsParam->add(newItem);
            }
        }else{
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            sts = INTERNAL_ERROR;
        }
        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    return sts;
}

RETURN_CODE RegroupingService::updRegroupingParamPtItem(TRegroupingParamPtItem* item, const QMap<QString,QString>& mp, bool upd){
    RETURN_CODE ret = OK;
    {
        QSqlDatabase conn = getConnection(CONN_NAME,mp);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return INTERNAL_ERROR;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);

        q->prepare(qINSERT_UPD);

        q->bindValue(qPARAM_ID,item->getId());
        q->bindValue(qPARAM_TYPE,item->getType());
        q->bindValue(qPARAM_TITLE,item->getTitle());
        q->bindValue(qPARAM_UPLOAD,item->getUpload());
        q->bindValue(qPARAM_DOWNLOAD,item->getDownload());
        q->bindValue(qPARAM_GID,item->getGroup()!=NULL?item->getGroup()->getId().toString(QUuid::WithoutBraces):QVariant(QVariant::Uuid));
        q->bindValue(qPARAM_ATTR, QString(QJsonDocument(item->getAttr().toObject()).toJson(QJsonDocument::Compact)) );
        q->bindValue(qPARAM_UPD,upd);

        if(!q->exec()){
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            ret= INTERNAL_ERROR;
        }
        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    return ret;
}
TRegroupingParamPtItem* RegroupingService::updRegroupingParamPtItem(AbstructTransferObject* p, bool upd){
    TRegroupingParamPtItem *ret = new TRegroupingParamPtItem();

    QJsonDocument doc = QJsonDocument::fromJson(p->getInputJSON().toUtf8());

    QJsonObject mainObj = doc.object();

    if(ret->fromJson(mainObj)){
        QMap<QString,QString> mp;
        conv(p, mp);
        RETURN_CODE sts = updRegroupingParamPtItem(ret,mp, upd);
        p->setReturnCode(sts);
    }else{
        qWarning()<<"Error while parse json";
        p->setReturnCode(ERR_VERIFY_DATA);
    }


    return ret;
}
RETURN_CODE RegroupingService::delRegroupingParamPtItemList(AbstructTransferObject* p){
    RETURN_CODE ret = OK;


    QString id;
    int type =-1;
    bool isAll = true;
    if(p->isParam(PARAM_ID)){
        id = p->getParam(PARAM_ID);
    }else{
        isAll = true;
    }

    if(ret==OK){
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

            q->prepare(qDELETE);

            q->bindValue(qPARAM_ID,id);
            q->bindValue(qPARAM_TYPE,type);
            q->bindValue(qPARAM_ISALL,isAll);

            if(!q->exec()){
                qWarning()<<"Error while exec query: "<<q->lastError().text();
                p->setReturnCode(INTERNAL_ERROR);
                ret = INTERNAL_ERROR;
            }
            delete q;
            if(bOpen){conn.close();}

        }
        QSqlDatabase::removeDatabase(CONN_NAME);

    }


    return ret;
}





TRegroupingParamPtGroupList* RegroupingService::getRegroupingGrPtItemList(AbstructTransferObject* p){
    TRegroupingParamPtGroupList* ret = new TRegroupingParamPtGroupList();

    int type = 0;
    RETURN_CODE sts = OK;
    if(p->isParam(PARAM_TYPE)){
        type = p->getParam(PARAM_TYPE).toInt();
    }

    if(sts==OK){
        QMap<QString,QString> mp;
        conv(p, mp);
        sts = getRegroupingGrPtItemList(ret, mp, type);
    }
    p->setReturnCode(sts);

    return ret;
}

RETURN_CODE RegroupingService::getRegroupingGrPtItemList(TRegroupingParamPtGroupList* lsGroup, const QMap<QString, QString> &mp, int type){
    RETURN_CODE sts = OK;

    {
        QSqlDatabase conn = getConnection(CONN_NAME,mp);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return INTERNAL_ERROR;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);
        q->prepare(qSELECT_GR);
        q->bindValue(qPARAM_TYPE,type);


        if(q->exec()){
            while(q->next()){
                TRegroupingParamPtGroup* newItem = new TRegroupingParamPtGroup();

                newItem->setId(QUuid(q->value(TRegroupingParamPtItem::getDBField_id()).toString()));
                newItem->setType((REGROUPING_TYPE)q->value(TRegroupingParamPtItem::getDBField_type()).toInt());
                newItem->setTitle(q->value(TRegroupingParamPtItem::getDBField_title()).toString());

                lsGroup->add(newItem);
            }
        }else{
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            sts = INTERNAL_ERROR;
        }
        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    return sts;
}
TRegroupingParamPtGroup *RegroupingService::updRegroupingGrPtItem(AbstructTransferObject* p, bool upd){
    TRegroupingParamPtGroup *ret = new TRegroupingParamPtGroup();

    QJsonDocument doc = QJsonDocument::fromJson(p->getInputJSON().toUtf8());

    QJsonObject mainObj = doc.object();

    if(ret->fromJson(mainObj)){
        QMap<QString,QString> mp;
        conv(p, mp);
        RETURN_CODE sts = updRegroupingGrPtItem(ret,mp, upd);
        p->setReturnCode(sts);
    }else{
        qWarning()<<"Error while parse json";
        p->setReturnCode(ERR_VERIFY_DATA);
    }


    return ret;
}
RETURN_CODE RegroupingService::updRegroupingGrPtItem(TRegroupingParamPtGroup* item, const QMap<QString,QString>& mp, bool upd){
    RETURN_CODE ret = OK;
    {
        QSqlDatabase conn = getConnection(CONN_NAME,mp);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return INTERNAL_ERROR;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);

        q->prepare(qINSERT_UPD_GR);

        q->bindValue(qPARAM_ID,item->getId().toString());
        q->bindValue(qPARAM_TYPE,item->getType());
        q->bindValue(qPARAM_TITLE,item->getTitle());
        q->bindValue(qPARAM_UPD,upd);

        if(!q->exec()){
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            ret= INTERNAL_ERROR;
        }
        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    return ret;
}
RETURN_CODE RegroupingService::delRegroupingGrPtItemList(AbstructTransferObject* p){
    RETURN_CODE ret = OK;


    QString id;
    int type =-1;
    bool isAll = true;
    if(p->isParam(PARAM_ID)){
        id = p->getParam(PARAM_ID);
    }else{
        isAll = true;
    }

    if(ret==OK){
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

            q->prepare(qDELETE_GR);

            q->bindValue(qPARAM_ID,id);
            q->bindValue(qPARAM_TYPE,type);
            q->bindValue(qPARAM_ISALL,isAll);

            if(!q->exec()){
                qWarning()<<"Error while exec query: "<<q->lastError().text();
                p->setReturnCode(INTERNAL_ERROR);
                ret = INTERNAL_ERROR;
            }
            delete q;
            if(bOpen){conn.close();}

        }
        QSqlDatabase::removeDatabase(CONN_NAME);

    }


    return ret;
}




// Common code -----------------------------------------------------------------------------------
RegroupingService* RegroupingService::_inatance = NULL;
RegroupingService::Ctrl RegroupingService::_ctrl;

RegroupingService::Ctrl::Ctrl(){

}
RegroupingService::Ctrl::~Ctrl(){
    if(_inatance!=NULL){
        delete _inatance;
    }
}
RegroupingService::RegroupingService(){

}
RegroupingService::~RegroupingService(){

}
RegroupingService *RegroupingService::getInstance(){
    if(_inatance==NULL){
        _inatance = new RegroupingService();
    }
    return _inatance;
}
