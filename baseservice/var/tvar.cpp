#include "tvar.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QtSql>
#include "dbconnection.h"


const char* txtId = "id";
const char* txtOwnerId = "owner";
const char* txtTitle = "title";
const char* txtBegin = "begin";
const char* txtEnd = "end";
const char* txtState = "state";
const char* txtItems = "items";

VAR::TVar::TVar(){
    _state = stsUndef;
}
VAR::TVar::~TVar(){}

const QUuid& VAR::TVar::getId() const{return _id;}
const QUuid& VAR::TVar::getOwnerId() const{return _owner;}
const QString& VAR::TVar::getTitle() const{return _title;}
const QDateTime& VAR::TVar::getBegin() const{return _tbegin;}
const QDateTime& VAR::TVar::getEnd() const{return _tend;}
VAR::VAR_STATE VAR::TVar::getState() const{return _state;}

void VAR::TVar::setId(const QUuid& v){_id=v;}
void VAR::TVar::setOwnerId(const QUuid& v){_owner=v;}
void VAR::TVar::setTitle(const QString& v){_title=v;}
void VAR::TVar::setBegin(const QDateTime& v){_tbegin=v;}
void VAR::TVar::setEnd(const QDateTime &v){_tend=v;}
void VAR::TVar::setState(VAR_STATE v){_state=v;}

VAR::VAR_STATE VAR::TVar::convState(const QString& v)const{
    VAR_STATE ret = stsUndef;
    if(v.compare("stsDeleted")==0){
        ret = stsDeleted;
    }else if(v.compare("stsPlanning")==0){
        ret = stsPlanning;
    }else if(v.compare("stsComplited")==0){
        ret = stsComplited;
    }
    return ret;
}

QString VAR::TVar::convState()const{
    QString ret;
    switch (_state) {
    case stsDeleted:
        ret = "stsDeleted";
        break;
    case stsPlanning:
        ret = "stsPlanning";
        break;
    case stsComplited:
        ret = "stsComplited";
        break;
    default:
        ret = "stsUndef";
        break;
    }
    return ret;
}

bool VAR::TVar::fromJson(const QJsonValue &str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject mainObj = str.toObject();

        QJsonValue vId = mainObj.value(QString(txtId));
        QJsonValue vOwnerId = mainObj.value(QString(txtOwnerId));
        QJsonValue vTitle = mainObj.value(QString(txtTitle));
        QJsonValue vBegin = mainObj.value(QString(txtBegin));
        QJsonValue vEnd = mainObj.value(QString(txtEnd));
        QJsonValue vState = mainObj.value(QString(txtState));

        if(!vId.isNull() && vId.isString()){
            _id = QUuid::fromString(vId.toString());
        }

        if(!vOwnerId.isNull() && vOwnerId.isString()){
            _owner = QUuid::fromString(vOwnerId.toString());
        }

        if(!vTitle.isNull() && vTitle.isString()){
            _title = vTitle.toString();
        }

        if(!vBegin.isNull() && vBegin.isString()){
            _tbegin = QDateTime::fromString(vBegin.toString(),Qt::ISODate);
        }

        if(!vEnd.isNull() && vEnd.isString()){
            _tend = QDateTime::fromString(vEnd.toString(),Qt::ISODate);
        }

        if(!vState.isNull() && vState.isString()){
            _state = convState(vState.toString());
        }
    }
    return ret;
}
QJsonObject VAR::TVar::toJson(){
    QJsonObject ret;

    ret.insert(QString(txtId),_id.toString());
    ret.insert(QString(txtOwnerId),_owner.isNull()?QString():_owner.toString());
    ret.insert(QString(txtTitle),_title);
    ret.insert(QString(txtBegin),_tbegin.isValid()?_tbegin.toString(Qt::ISODate):QString());
    ret.insert(QString(txtEnd),_tend.isValid()?_tend.toString(Qt::ISODate):QString());
    ret.insert(QString(txtState),convState());

    return ret;
}
/////////////////////////////////////////////////////////////////////////////
VAR::TListVar::TListVar(){;}
VAR::TListVar::~TListVar(){
    foreach (TVar* it, _items) {
       if(it!=NULL) delete it;
    }
    _items.clear();
}

void VAR::TListVar::add(TVar *v){
    if(v!=NULL){
        _items<<v;
    }
}
bool VAR::TListVar::isEmpty(){
    return _items.isEmpty();
}

bool VAR::TListVar::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject obj = str.toObject();

        QJsonValue items = obj.value(txtItems);
        if(!items.isNull() && items.isArray()){
            QJsonArray a = items.toArray();
            foreach (QJsonValue it, a) {
                TVar *newItem = new TVar();
                if(newItem->fromJson(it)){
                    _items<<newItem;
                }else{
                    delete newItem;
                }

            }
        }
    }

    return ret;
}
QJsonObject VAR::TListVar::toJson(){
    QJsonObject ret;

    QJsonArray a;
    foreach (TVar* it, _items) {
        a.append(it->toJson());
    }

    ret.insert(QString(txtItems),a);

    return ret;
}
////////////////////////////////////////////////////////////////////////////////////////

const char* qSELECT_VARIANTS   =    "SELECT * FROM v1.variant_select()";
const char* qSELECT_VARIANTS_COND = "SELECT * FROM v1.variant_select_cond(:ID)";

const char* qDELETE_VARIANTS   =    "SELECT * FROM v1.variant_delete(); ";
const char* qDELETE_VARIANTS_COND = "SELECT * FROM v1.variant_delete_cond(:ID); ";

const char* qINSERT_VARIANTS =       "SELECT * FROM v1.variant_insert( :OWNER, :TITLE, :STATE, :TBEGIN, :TEND)";
const char* qUPDATE_VARIANTS =      "SELECT * FROM v1.variant_update(:ID, :OWNER, :TITLE, :STATE, :TBEGIN, :TEND)";

///
const char * CONN_NAME = "VARIANTS";
int VAR::createCurrentVariant(AbstructTransferObject* p){
    int ret = 0;
    {
        QSqlDatabase conn = getConnection(CONN_NAME,p);
        if(conn.open()){
            QSqlQuery * q = new QSqlQuery(conn);
            q->prepare("SELECT FROM v1.create_current_variant()");
            if(q->exec()){
                p->setReturnCode(CREATE);
                p->setContent("{\"id\":\"66666666-0000-0000-0000-000000000000\"}");
            }else{
                ret = -3;
            }
            delete q;
            conn.close();
        }else{
            qWarning()<<"Error while open db connect";
            ret = -2;
        }
    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    return ret;
}

#ifdef QT_DEBUG
QString VAR::getVariantsTEST(){
    QString testData = "{\"items\":["
                       "{"
                       "\"id\":\""+QUuid::createUuid().toString()+"\","
                       "\"owner\":\""+QUuid::createUuid().toString()+"\","
                       "\"title\":\""+QString("XZ-1")+"\","
                       "\"begin\":\""+QDateTime::currentDateTime().toString(Qt::ISODate)+"\","
                       "\"end\":\""+QDateTime::currentDateTime().toString(Qt::ISODate)+"\","
                       "\"state\":\""+QString("stsDeleted")+"\""
                       "},{"
                       "\"id\":\""+QUuid::createUuid().toString()+"\","
                       "\"owner\":"+QString("null")+","
                       "\"title\":\""+QString("XZ-2")+"\","
                       "\"begin\":\""+QDateTime::currentDateTime().toString(Qt::ISODate)+"\","
                       "\"end\":"+QString("null")+","
                       "\"state\":\""+QString("stsComplited")+"\""
                       "}"
                       "]}";
    QJsonDocument doc = QJsonDocument::fromJson(testData.toUtf8());
    QJsonObject mainObj = doc.object();
    TListVar *lsVar = new TListVar();
    lsVar->fromJson(mainObj);


    QJsonDocument doc1 = QJsonDocument(lsVar->toJson());
    return QString(doc1.toJson(QJsonDocument::Compact));
}
#endif

VAR::TListVar* getVariants(AbstructTransferObject *p){
    VAR::TListVar* ret = NULL;

    QString param_id = p->getParam("id");
    QUuid id;
    if(!param_id.isNull() && !param_id.isEmpty()){
        try{
            id = QUuid::fromString(param_id);
        }catch(...){
            qWarning()<<"Wrong param id: "<<param_id;
        }
    }

//    QMap<QString,QString> dbparam;
//    conv(p,dbparam);
//    ret = getVariantsParam(id,dbparam);
    {
        QSqlDatabase conn = getConnection(CONN_NAME,p);
        ret = getVariantsConn(id,conn);
        if(ret==NULL || ret->isEmpty()){
            p->setReturnCode(INTERNAL_ERROR);
        }
    }
    QSqlDatabase::removeDatabase(CONN_NAME);
    return ret;
}
VARSHARED_EXPORT VAR::TListVar* getVariantsConn(const QUuid& id, QSqlDatabase& conn){
    VAR::TListVar* ret = NULL;

    {
        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return ret;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);

        q->prepare(id.isNull()?qSELECT_VARIANTS:qSELECT_VARIANTS_COND);
        if(!id.isNull()){
            q->bindValue(":ID",id.toString());
        }

        ret = new  VAR::TListVar();
        if(q->exec()){
            while(q->next()){
                VAR::TVar* newItem = new VAR::TVar();
                newItem->setId(QUuid::fromString(q->value("id").toString()));
                newItem->setOwnerId(QUuid::fromString(q->value("owner").toString()));
                newItem->setTitle(q->value("title").toString());
                newItem->setState( (VAR::VAR_STATE)q->value("state").toInt());
                newItem->setBegin(q->value("tbegin").toDateTime());
                newItem->setEnd(q->value("tend").toDateTime());
                ret->add(newItem);
            }
        }else{
            qWarning()<<"Error while exec query: "<<q->lastError().text();
        }
        delete q;
        if(bOpen){conn.close();}
    }

    return ret;
}

int delVariants(AbstructTransferObject *p){
    int ret = 0;

    QString param_id = p->getParam("id");
    QUuid id;
    if(!param_id.isNull() && !param_id.isEmpty()){
        try{
            id = QUuid::fromString(param_id);
        }catch(...){
            qWarning()<<"Wrong param id: "<<param_id;
        }
    }

    {
        QSqlDatabase conn = getConnection(CONN_NAME,p);
        ret = delVariantsConn(id,conn);
    }
    QSqlDatabase::removeDatabase(CONN_NAME);
    return ret;
}
int delVariantsConn(const QUuid& id, QSqlDatabase& conn){
    int ret = -1;

    {
        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return ret;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);

        q->prepare(id.isNull()?qDELETE_VARIANTS:qDELETE_VARIANTS_COND);
        if(!id.isNull()){
            q->bindValue(":ID",id.toString());
        }


        if(!q->exec()){
            qWarning()<<"Error while exec query: "<<q->lastError().text();
        }
        delete q;
        if(bOpen){conn.close();}
    }

    return ret;
}
int setVariants(AbstructTransferObject *p){
    int ret = 0;

    VAR::TVar *var = new VAR::TVar();
    QJsonDocument doc = QJsonDocument::fromJson(p->getInputJSON().toUtf8());
    QJsonObject mainObj = doc.object();
    if(var->fromJson(mainObj)){
        QSqlDatabase conn = getConnection(CONN_NAME,p);
        ret = setVariantsConn(var, conn, p->getMethod()==PUT );
    }else{
        ret = -1;
        p->setReturnCode(ERR_VERIFY_DATA);
    }

    QSqlDatabase::removeDatabase(CONN_NAME);
    return ret;
}
int setVariantsConn(VAR::TVar* var, QSqlDatabase& conn, bool upd){
    int ret = -1;
    if(var==NULL){
        qWarning()<<"Wrong input param";
        return ret;
    }

    bool bOpen = false;
    if(!conn.isOpen()){
        if(!conn.open()){
            qWarning()<<"Error while open db";
            return ret;
        }
        bOpen = true;
    }

    QSqlQuery *q = new QSqlQuery(conn);

    q->prepare(!upd?qINSERT_VARIANTS:qUPDATE_VARIANTS);
    if(upd){
        q->bindValue(":ID",var->getId().toString());
    }

    q->bindValue(":OWNER",var->getOwnerId().toString());
    q->bindValue(":TITLE",var->getTitle());
    q->bindValue(":STATE",var->getState());
    q->bindValue(":TBEGIN",var->getBegin());
    q->bindValue(":TEND",var->getEnd());

    if(q->exec()){
        if(!upd){
            QString id;
            while(q->next()){
                id = q->value(0).toString();
            }
            if(!id.isNull() && !id.isEmpty()){
                var->setId(QUuid::fromString(id));
            }
        }
        ret = 0;
    }else{
        qWarning()<<"Error while exec query: "<<q->lastError().text();
    }

    delete q;

    if(bOpen){conn.close();}
    return ret;
}
///
/// \brief getVariantsParam
/// \param id
/// \param dbparam
/// \return
/// \deprecated
///
VAR::TListVar* getVariantsParam(const QUuid& id,const QMap<QString,QString>& dbparam ){
    VAR::TListVar* ret = NULL;

    qDebug()<<"XZ2: "<<dbparam[DB_NAME];
    {
        QSqlDatabase conn = getConnection(CONN_NAME,dbparam);
        if(conn.open()){
            QSqlQuery *q = new QSqlQuery(conn);

            q->prepare(id.isNull()?qSELECT_VARIANTS:qSELECT_VARIANTS_COND);
            if(!id.isNull()){
                q->bindValue(":ID",id.toString());
            }

            ret = new  VAR::TListVar();
            if(q->exec()){
                while(q->next()){
                    VAR::TVar* newItem = new VAR::TVar();
                    newItem->setId(QUuid::fromString(q->value("id").toString()));
                    newItem->setOwnerId(QUuid::fromString(q->value("owner").toString()));
                    newItem->setTitle(q->value("title").toString());
                    newItem->setState( (VAR::VAR_STATE)q->value("state").toInt());
                    newItem->setBegin(q->value("tbegin").toDateTime());
                    newItem->setEnd(q->value("tend").toDateTime());
                    ret->add(newItem);
                }
            }else{
                qWarning()<<"Error while exec query: "<<q->lastError().text();
            }
            delete q;
            conn.close();
        }else{
            qWarning()<<"Error while open db";
        }
    }
    QSqlDatabase::removeDatabase(CONN_NAME);
    return ret;
}
