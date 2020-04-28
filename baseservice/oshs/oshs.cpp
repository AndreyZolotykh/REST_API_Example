#include "oshs.h"
#include "utils/utils.h"
#include <QDebug>
#include <QJsonArray>


const char* txt_id = "id";
const char* txt_sname = "sname";
const char* txt_fname = "fname";
const char* txt_iconid = "iconid";
const char* txt_subtype = "subtype";
const char* txt_owner = "owner";
const char* txt_data = "data";
const char* txt_child = "child";


TOSHSItem::iterator TOSHSItem::begin(){
    return iterator(((BaseTreeItem*)this)->begin());
}
TOSHSItem::iterator TOSHSItem::end(){
    return iterator(((BaseTreeItem*)this)->end());
}
TOSHSItem::const_iterator TOSHSItem::cbegin(){
    return const_iterator(((BaseTreeItem*)this)->cbegin());
}
TOSHSItem::const_iterator TOSHSItem::cend(){
    return const_iterator(((BaseTreeItem*)this)->cend());
}
void TOSHSItem::fill(const BaseTreeItem& o){
    fill(&o);
}
void TOSHSItem::fill(const BaseTreeItem* o){
    const TOSHSItem* obj = dynamic_cast<const TOSHSItem*>(o);
    _id = obj->_id;
    _sname = obj->_sname;
    _fname = obj->_fname;
    _id_icon = obj->_id_icon;
    _owner = obj->_owner;
    _data = obj->_data;
    _subtype = obj->_subtype;
}
TOSHSItem::~TOSHSItem(){

}

const TOSHSItem* TOSHSItem::getParent() const{
    return (TOSHSItem*)m_Parent;
}
void TOSHSItem::setParent(TOSHSItem* item) {
  m_Parent=item;
}
void TOSHSItem::addChild(TOSHSItem* item){
    if(item!=NULL){
        BaseTreeItem::addChild(item);
    }
}
void TOSHSItem::addChild(const QList<TOSHSItem *> &items){
    foreach (TOSHSItem *it, items) {
        BaseTreeItem::addChild(it);
    }
}
bool TOSHSItem::fromJson(const QJsonValue& str){
    bool ret = true;

    return ret;
}
QJsonObject TOSHSItem::toJson(){
    QJsonObject ret;
    ret.insert(QString(txt_id),_id.toString(QUuid::WithoutBraces));
    ret.insert(QString(txt_sname),_sname);
    ret.insert(QString(txt_fname),_fname);
    ret.insert(QString(txt_iconid),_id_icon.toString(QUuid::WithoutBraces));
    ret.insert(QString(txt_owner),UTILS::toStringOwner(_owner));
    ret.insert(QString(txt_data),_data);
    ret.insert(QString(txt_subtype),UTILS::toStringSub(_subtype));

    QJsonArray child;
    for(TOSHSItem::iterator iter = begin();iter!=end();++iter) {
        child.append((*iter)->toJson());
    }
    ret.insert(txt_child,child);

    return ret;
}

TOSHSItem::TOSHSItem(){
}

TOSHSItem::TOSHSItem(const TOSHSItem& obj):BaseTreeItem(obj.m_id,1){
    _id = obj.m_id;
    _sname = obj._sname;
    _fname = obj._fname;
    _id_icon = obj._id_icon;
    _owner = obj._owner;
    _data = obj._data;
    _subtype = obj._subtype;
}
TOSHSItem::TOSHSItem(const QUuid& id):BaseTreeItem(id,1){
    _sname = "XZ";
    _fname = "XZ";
    _owner = owCUSTOM;
}

TOSHSItem::TOSHSItem(const QUuid& id, const QString& sname, const QString& fname, const QUuid& iconid, SUB_TYPE subtype, OWNER_TYPE owner, const QJsonObject& data):BaseTreeItem(id,1){
        _id = id;
        _sname = sname;
        _fname = fname;
        _id_icon = iconid;
        _subtype = subtype;
        _owner = owner;
        _data = data;
    }
TOSHSItem::TOSHSItem(const QUuid& id, const QString& name, const QUuid& iconid, SUB_TYPE subtype, OWNER_TYPE owner, const QJsonObject& data):BaseTreeItem(id,1){
        _id = id;
        _sname = name;
        _fname = name;
        _id_icon = iconid;
        _subtype = subtype;
        _owner = owner;
        _data = data;
    }
TOSHSItem::TOSHSItem(const QUuid& id, const QString& name, const QUuid& iconid, SUB_TYPE subtype, OWNER_TYPE owner):BaseTreeItem(id,1){
        _id = id;
        _sname = name;
        _fname = name;
        _id_icon = iconid;
        _subtype = subtype;
        _owner = owner;
    }
TOSHSItem* TOSHSItem::clone() const{
    return new TOSHSItem(*this);
}

const QUuid& TOSHSItem::getId() const {return _id;}
const QString& TOSHSItem::getSname() const { return _sname;}
const QString& TOSHSItem::getFname() const { return _fname;}
const QUuid& TOSHSItem::getIdIcon() const { return _id_icon;}
OWNER_TYPE TOSHSItem::getOwner() const{ return _owner;}
const QJsonObject& TOSHSItem::getData() const {return _data;}
SUB_TYPE TOSHSItem::getSubtype() const {return _subtype;};


void TOSHSItem::setId(const QUuid& id) {_id = id;}
void TOSHSItem::setFname(const QString& fname) {_fname = fname;}
void TOSHSItem::setSname(const QString& sname) {_sname = sname;}
void TOSHSItem::setIdIcon(const QUuid& id_icon) {_id_icon = id_icon;}
void TOSHSItem::setOwner(OWNER_TYPE owner){_owner = owner;}
void TOSHSItem::setData(const QJsonObject& data) {_data = data;}
void TOSHSItem::setSubtype(const SUB_TYPE& subtype) {_subtype = subtype;};

////////////////////////////////////////////////////////////////////////////
