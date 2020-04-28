#include "attrvvt.h"
#include "../utils/utils.h"

const char* TAttrItem::stJsonField_id = "id";
const char* TAttrItem::stJsonField_title = "title";
const char* TAttrItem::stJsonField_stitle = "stitle";
const char* TAttrItem::stJsonField_gr = "gr";

const char* TAttrItem::stDBField_id = "id";
const char* TAttrItem::stDBField_title = "title";
const char* TAttrItem::stDBField_stitle = "stitle";
const char* TAttrItem::stDBField_gr = "gr";

TAttrItem::TAttrItem(){;}
TAttrItem::~TAttrItem(){;}
bool TAttrItem::fromJson(const QJsonValue& str){
    bool ret = true;
    qWarning()<<"Operation <TAttrItem::fromJson> not impliments";
    return ret;
}
QJsonObject TAttrItem::toJson(){
    QJsonObject ret;

    ret.insert(QString(stJsonField_id), _id);
    ret.insert(QString(stJsonField_title), _title);
    ret.insert(QString(stJsonField_stitle), _stitle);
    ret.insert(QString(stJsonField_gr), _gr);

    return ret;
}

int TAttrItem::getId()const{return _id;}
const QString& TAttrItem::getTitle() const{return _title;}
const QString& TAttrItem::getShortTitle() const{return _stitle;}
int TAttrItem::getGroup() const{return _gr;}

void TAttrItem::setId(int v){ _id = v;}
void TAttrItem::setTitle(const QString& v){ _title = v;}
void TAttrItem::setShortTitle(const QString&  v){ _stitle = v;}
void TAttrItem::setGroup(int v){ _gr = v;}

const char* TAttrItem::getDBField_id(){return stDBField_id;}
const char* TAttrItem::getDBField_title(){return stDBField_title;}
const char* TAttrItem::getDBField_stitle(){return stDBField_stitle;}
const char* TAttrItem::getDBField_gr(){return stDBField_gr;}
//////////////////////////////////
const char* TAttrVVTItem::stJsonField_id = "id";
const char* TAttrVVTItem::stJsonField_attr = "attr";


const char* TAttrVVTItem::stDBField_id = "id";
const char* TAttrVVTItem::stDBField_attr = "attr";

TAttrVVTItem::TAttrVVTItem(){;}
TAttrVVTItem::~TAttrVVTItem(){;}

bool TAttrVVTItem::fromJson(const QJsonValue& str){
    bool ret = true;
    if(str.isObject()){
        QJsonObject o = str.toObject();

        QJsonValue v_id = o.value(QString(stJsonField_id));
        QJsonValue v_attr = o.value(QString(stJsonField_attr));

        ret&=UTILS::parseUUID(v_id,_id);

        _attr = v_attr;
    }
    return ret;
}
QJsonObject TAttrVVTItem::toJson(){
    QJsonObject ret;

    ret.insert(stJsonField_id,_id.toString());
    ret.insert(stJsonField_attr,_attr);

    return ret;
}

const QUuid& TAttrVVTItem::getId()const{return _id;}
const QJsonValue& TAttrVVTItem::getAttr()const{return _attr;}

void TAttrVVTItem::setId(const QUuid& v){ _id = v;}
QJsonValue TAttrVVTItem::setAttr(const QJsonValue& v){ _attr = v;}

const char* TAttrVVTItem::getDBField_id(){return stDBField_id;}
const char* TAttrVVTItem::getDBField_attr(){return stDBField_attr;}
//////////////////////////////////
QList<TAttrVVTItem*>::iterator TAttrVVTItemList::begin(){return _items.begin();}
QList<TAttrVVTItem*>::iterator TAttrVVTItemList::end(){return _items.end();}
//////////////////////////////////
QList<TAttrItem*>::iterator TAttrItemList::begin(){return _items.begin();}
QList<TAttrItem*>::iterator TAttrItemList::end(){return _items.end();}
