#include "deployareas.h"
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "dbconnection.h"

DeployAreasCtrl* DeployAreasCtrl::_inatance = NULL;
DeployAreasCtrl::Ctrl DeployAreasCtrl::_ctrl;

DeployAreasCtrl::Ctrl::Ctrl(){

}
DeployAreasCtrl::Ctrl::~Ctrl(){
    if(_inatance!=NULL){
        delete _inatance;
    }
}
DeployAreasCtrl::DeployAreasCtrl(){

}
DeployAreasCtrl *DeployAreasCtrl::getInstance(){
    if(_inatance==NULL){
        _inatance = new DeployAreasCtrl();
    }
    return _inatance;
}
const char* PARAM_TYPE = "type";
const char* PARAM_GROUP = "group";
const char* CONN_NAME_DA = "DeployAreasCtrl";

const char* qSELECT_DEPLOY_AREAS = "SELECT * FROM v1.deploy_areas_select(:TYPE, :GROUP)";

TDeployAreasItemList* DeployAreasCtrl::getDic(AbstructTransferObject* p){
    int iType=-1, iGr=-1;
    TDeployAreasItemList* ret= NULL;

    // parse param
    if(p->isParam(PARAM_TYPE)){
        bool ok;
        iType = p->getParam(PARAM_TYPE).toInt(&ok);
        if(!ok){
            iType=-1;
            qWarning()<<"Wrong "<<PARAM_TYPE<<" param value ("<<p->getParam(PARAM_TYPE)<<")";
        }
    }
    if(p->isParam(PARAM_GROUP)){
        bool ok;
        iGr = p->getParam(PARAM_GROUP).toInt(&ok);
        if(!ok){
            iGr=-1;
            qWarning()<<"Wrong "<<PARAM_GROUP<<" param value ("<<p->getParam(PARAM_GROUP)<<")";
        }
    }

    // create request
    {
        QSqlDatabase conn = getConnection(CONN_NAME_DA,p);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return ret;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);

        q->prepare(qSELECT_DEPLOY_AREAS);
        q->bindValue(":TYPE",iType);
        q->bindValue(":GROUP",iGr);


        if(q->exec()){
            ret = new TDeployAreasItemList();
            while(q->next()){
                TDeployAreasItem* newItem = new TDeployAreasItem();
                newItem->setCode(q->value("code").toInt());
                newItem->setTypes(q->value("type").toInt());
                newItem->setFname(q->value("fname").toString());
                newItem->setSname(q->value("sname").toString());
                newItem->setIsNumber(q->value("isnum").toBool());
                newItem->setMinVal(q->value("minval").toInt());
                newItem->setMaxVal(q->value("maxval").toInt());
                newItem->setGroup(q->value("group_code").toInt());
                newItem->setSubType(q->value("subtype").toInt());
                ret->add(newItem);
            }
        }else{
            qWarning()<<"Error while exec query: "<<q->lastError().text();
        }
        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME_DA);

    return ret;
}



////////////////////////////////////////////////////////////////////////
TDeployAreasItemList::TDeployAreasItemList(){

}
TDeployAreasItemList::~TDeployAreasItemList(){

}
/////////////////////////////////////////////////////////////////////////
TDeployAreasItem::TDeployAreasItem(){;}
TDeployAreasItem::~TDeployAreasItem(){;}
const char* txtCode = "code";
const char* txtType = "type";
const char* txtFname = "fname";
const char* txtSname = "sname";
const char* txtIsNumber = "isNumber";
const char* txtMinVal = "minval";
const char* txtMaxVal = "maxval";
const char* txtGroup = "group";
const char* txtSubType = "subtype";
bool TDeployAreasItem::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();

        QJsonValue vCode = mainObj.value(QString(txtCode));
        QJsonValue vTypes = mainObj.value(QString(txtType));
        QJsonValue vFname = mainObj.value(QString(txtFname));
        QJsonValue vSname = mainObj.value(QString(txtSname));
        QJsonValue vIsNumber = mainObj.value(QString(txtIsNumber));
        QJsonValue vMinVal = mainObj.value(QString(txtMinVal));
        QJsonValue vMaxVal = mainObj.value(QString(txtMaxVal));
        QJsonValue vGroup = mainObj.value(QString(txtGroup));
        QJsonValue vSubType = mainObj.value(QString(txtSubType));

        if(!vCode.isNull() && vCode.isString()){
            _code = vCode.toInt();
        }

        if(!vTypes.isNull() && vTypes.isString()){
            _types = vTypes.toInt();
        }

        if(!vFname.isNull() && vFname.isString()){
            _fname = vFname.toString();
        }

        if(!vSname.isNull() && vSname.isString()){
            _sname = vSname.toString();
        }

        if(!vIsNumber.isNull() && vIsNumber.isString()){
            _isNumber = vIsNumber.toBool();
        }

        if(!vMinVal.isNull() && vMinVal.isString()){
            _minval = vMinVal.toInt();
        }

        if(!vMaxVal.isNull() && vMaxVal.isString()){
            _maxval = vMaxVal.toInt();
        }

        if(!vGroup.isNull() && vGroup.isString()){
            _group = vGroup.toInt();
        }

        if(!vSubType.isNull() && vSubType.isString()){
            _subtype = vSubType.toInt();
        }
    }
    return ret;
}
QJsonObject TDeployAreasItem::toJson(){
    QJsonObject ret;

    ret.insert(QString(txtCode),QString::number(_code));
    ret.insert(QString(txtType),QString::number(_types));
    ret.insert(QString(txtFname),_fname);
    ret.insert(QString(txtSname),_sname);
    ret.insert(QString(txtIsNumber),QVariant(_isNumber).toString());
    ret.insert(QString(txtMinVal),QString::number(_minval));
    ret.insert(QString(txtMaxVal),QString::number(_maxval));
    ret.insert(QString(txtGroup),QString::number(_group));
    ret.insert(QString(txtSubType),QString::number(_subtype));

    return ret;
}

// getters/setters
int TDeployAreasItem::getCode()const{return _code;}
int TDeployAreasItem::getTypes()const{return _types;}
const QString& TDeployAreasItem::getFname()const{return _fname;}
const QString& TDeployAreasItem::getSname()const{return _sname;}
bool TDeployAreasItem::isNumber()const{return _isNumber;}
int TDeployAreasItem::getMinVal()const{return _minval;}
int TDeployAreasItem::getMaxVal()const{return _maxval;}
int TDeployAreasItem::getGroup()const{return _group;}
int TDeployAreasItem::getSubType()const{return _subtype;}

void TDeployAreasItem::setCode(int v){ _code = v;}
void TDeployAreasItem::setTypes(int v){ _types = v;}
void TDeployAreasItem::setFname(const QString& st){ _fname = st;}
void TDeployAreasItem::setSname(const QString& st){ _sname = st;}
void TDeployAreasItem::setIsNumber(bool v){ _isNumber = v;}
void TDeployAreasItem::setMinVal(int v){ _minval = v;}
void TDeployAreasItem::setMaxVal(int v){ _maxval = v;}
void TDeployAreasItem::setGroup(int v){ _group = v;}
void TDeployAreasItem::setSubType(int v){ _subtype = v;}
