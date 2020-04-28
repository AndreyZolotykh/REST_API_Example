#include "regroupingparampt.h"

const char* TRegroupingParamPtGroup::stJsonField_id = "id";
const char* TRegroupingParamPtGroup::stJsonField_type = "type";
const char* TRegroupingParamPtGroup::stJsonField_title = "title";

const char* TRegroupingParamPtGroup::stDBField_id = "id";
const char* TRegroupingParamPtGroup::stDBField_type = "type";
const char* TRegroupingParamPtGroup::stDBField_title = "title";

TRegroupingParamPtGroup::TRegroupingParamPtGroup(){;}
TRegroupingParamPtGroup::~TRegroupingParamPtGroup(){;}

bool TRegroupingParamPtGroup::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject o = str.toObject();

        QJsonValue v_id = o.value(QString(stJsonField_id));
        QJsonValue v_type = o.value(QString(stJsonField_type));
        QJsonValue v_title = o.value(QString(stJsonField_title));

        if(!v_id.isNull() && v_id.isString()){
            _id = QUuid(v_id.toString());
        }else{
            ret&=false;
        }
        if(!v_type.isNull() && v_type.isDouble()){
            _type = (REGROUPING_TYPE)(v_type.toInt());
        }else{
            ret&=false;
        }
        if(!v_title.isNull() && v_title.isString()){
            _title = v_title.toString();
        }else{
            ret&=false;
        }
    }else{
        ret = false;
    }
    return ret;
}
QJsonObject TRegroupingParamPtGroup::toJson(){
    QJsonObject ret;

    ret.insert(QString(stJsonField_id), _id.toString(QUuid::WithoutBraces));
    ret.insert(QString(stJsonField_type), _type);
    ret.insert(QString(stJsonField_title), _title);

    return ret;
}

const QUuid& TRegroupingParamPtGroup::getId() const{return _id;}
const QString& TRegroupingParamPtGroup::getTitle() const{return _title;}
REGROUPING_TYPE TRegroupingParamPtGroup::getType() const{return _type;}

void TRegroupingParamPtGroup::setId(const QUuid& id){_id=id;}
void TRegroupingParamPtGroup::setTitle(const QString& title){_title=title;}
void TRegroupingParamPtGroup::setType(REGROUPING_TYPE type){_type=type;}

const char* TRegroupingParamPtGroup::getDBField_id(){return stDBField_id;}
const char* TRegroupingParamPtGroup::getDBField_type(){return stDBField_type;}
const char* TRegroupingParamPtGroup::getDBField_title(){return stDBField_title;}
//////////////////////////////////////////////////////////////////////////////////
const char* TRegroupingParamPtItem::stJsonField_id = "id";
const char* TRegroupingParamPtItem::stJsonField_type = "type";
const char* TRegroupingParamPtItem::stJsonField_title = "title";
const char* TRegroupingParamPtItem::stJsonField_upload = "upload";
const char* TRegroupingParamPtItem::stJsonField_download = "download";
const char* TRegroupingParamPtItem::stJsonField_attr = "attr";
const char* TRegroupingParamPtItem::stJsonField_group = "group";

const char* TRegroupingParamPtItem::stDBField_id = "id";
const char* TRegroupingParamPtItem::stDBField_type = "type";
const char* TRegroupingParamPtItem::stDBField_title = "title";
const char* TRegroupingParamPtItem::stDBField_upload = "upload";
const char* TRegroupingParamPtItem::stDBField_download = "download";
const char* TRegroupingParamPtItem::stDBField_attr = "attr";
const char* TRegroupingParamPtItem::stDBField_group = "groupid";
const char* TRegroupingParamPtItem::stDBField_gtitle = "gtitle";

TRegroupingParamPtItem::TRegroupingParamPtItem(){
    _group = NULL;
}
TRegroupingParamPtItem::~TRegroupingParamPtItem(){
    if(_group!=NULL){
        delete _group;
    }
}

bool TRegroupingParamPtItem::fromJson(const QJsonValue& str){
    bool ret = true;

    if(str.isObject()){
        QJsonObject o = str.toObject();

        QJsonValue v_id = o.value(QString(stJsonField_id));
        QJsonValue v_type = o.value(QString(stJsonField_type));
        QJsonValue v_title = o.value(QString(stJsonField_title));
        QJsonValue v_upload = o.value(QString(stJsonField_upload));
        QJsonValue v_download = o.value(QString(stJsonField_download));
        QJsonValue v_attr = o.value(QString(stJsonField_attr));
        QJsonValue v_group = o.value(QString(stJsonField_group));

        if(!v_id.isNull() && v_id.isString()){
            _id = v_id.toString();
        }
        if(!v_type.isNull() && v_type.isDouble()){
            _type = (REGROUPING_TYPE)(v_type.toInt());
        }
        if(!v_title.isNull() && v_title.isString()){
            _title = v_title.toString();
        }
        if(!v_upload.isNull() && v_upload.isDouble()){
            _upload = v_upload.toInt();
        }
        if(!v_download.isNull() && v_download.isDouble()){
            _download = v_download.toInt();
        }
        _attr = v_attr;

        if(!v_group.isNull() && v_group.isObject()){
            TRegroupingParamPtGroup* gr = new TRegroupingParamPtGroup();
            if(gr->fromJson(v_group)){
                _group = gr;
            }else{
                qWarning()<<"Error while parse TRegroupingParamPtGroup for item with id "<<_id;
                delete gr;
            }
        }

    }else{
        ret = false;
    }

    return ret;
}
QJsonObject TRegroupingParamPtItem::toJson(){
    QJsonObject ret;

    ret.insert(QString(stJsonField_id), _id);
    ret.insert(QString(stJsonField_type), _type);
    ret.insert(QString(stJsonField_title), _title);
    ret.insert(QString(stJsonField_upload), _upload);
    ret.insert(QString(stJsonField_download), _download);
    ret.insert(QString(stJsonField_attr), _attr);
    if(_group!=NULL){
        ret.insert(QString(stJsonField_group), _group->toJson());
    }else{
        ret.insert(QString(stJsonField_group), QJsonValue::Null);
    }

    return ret;
}

const QString& TRegroupingParamPtItem::getId()const{return _id;}
REGROUPING_TYPE TRegroupingParamPtItem::getType()const{return _type;}
const QString& TRegroupingParamPtItem::getTitle()const{return _title;}
int TRegroupingParamPtItem::getUpload()const{return _upload;}
int TRegroupingParamPtItem::getDownload()const{return _download;}
const QJsonValue& TRegroupingParamPtItem::getAttr()const{return _attr;}
const TRegroupingParamPtGroup* TRegroupingParamPtItem::getGroup()const{return _group;}

void TRegroupingParamPtItem::setId(const QString& id){ _id = id;}
void TRegroupingParamPtItem::setType(REGROUPING_TYPE type){ _type = type;}
void TRegroupingParamPtItem::setTitle(const QString& title){ _title = title;}
void TRegroupingParamPtItem::setUpload(int upload){ _upload = upload;}
void TRegroupingParamPtItem::setDownload(int download){ _download = download;}
void TRegroupingParamPtItem::setAttr(const QJsonValue& attr){ _attr = attr;}
void TRegroupingParamPtItem::setGroup(TRegroupingParamPtGroup* group){ _group = group;}

const char* TRegroupingParamPtItem::getDBField_id(){return stDBField_id;}
const char* TRegroupingParamPtItem::getDBField_type(){return stDBField_type;}
const char* TRegroupingParamPtItem::getDBField_title(){return stDBField_title;}
const char* TRegroupingParamPtItem::getDBField_upload(){return stDBField_upload;}
const char* TRegroupingParamPtItem::getDBField_download(){return stDBField_download;}
const char* TRegroupingParamPtItem::getDBField_attr(){return stDBField_attr;}
const char* TRegroupingParamPtItem::getDBField_group(){return stDBField_group;}
const char* TRegroupingParamPtItem::getDBField_gtitle(){return stDBField_gtitle;}

//////////////////////////////////
QList<TRegroupingParamPtItem*>::iterator TRegroupingParamPtItemList::begin(){return _items.begin();}
QList<TRegroupingParamPtItem*>::iterator TRegroupingParamPtItemList::end(){return _items.end();}

QList<TRegroupingParamPtGroup*>::iterator TRegroupingParamPtGroupList::begin(){return _items.begin();}
QList<TRegroupingParamPtGroup*>::iterator TRegroupingParamPtGroupList::end(){return _items.end();}
