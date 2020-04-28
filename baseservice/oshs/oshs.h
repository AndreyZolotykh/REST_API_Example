#ifndef OSHS_H
#define OSHS_H
#include <QUuid>
#include <QJsonObject>
#include <QList>

#include "../commonheaders/iparameters.h"
#include "../commonheaders/basetree.h"


/**
 * @brief Элемент ОШС
 */
class TOSHSItem:public ISerialize, public BaseTreeItem<QUuid,int>{
private:
    QUuid _id;
    QString _sname;
    QString _fname;
    QUuid _id_icon;
    QJsonObject _data;
    OWNER_TYPE _owner;
    SUB_TYPE _subtype;
public:
    TOSHSItem();
    virtual ~TOSHSItem();
    class iterator{
        public:
            iterator(QList<BaseTreeItem*>::iterator el){iter=el;}
            iterator(){;}
            iterator(const iterator &it){iter=it.iter;}
            bool operator ==(const iterator &it) const{return (iter==it.iter);}
            bool operator !=(const iterator &it) const{return !(iter==it.iter);}
            iterator& operator ++(){
                ++iter;
                return *this;
            }
            iterator& operator --(){
                --iter;
                return *this;
            }
            TOSHSItem* operator *() const{return (TOSHSItem*)*iter;}
        private:
            QList<BaseTreeItem*>::iterator iter;
    };

    class const_iterator{
            friend class TOSHSItem;
            const_iterator(QList<BaseTreeItem*>::const_iterator el){iter=el;}
        public:
            const_iterator(){;}
            const_iterator(const const_iterator &it){iter=it.iter;}
            bool operator ==(const const_iterator &it) const{return (iter==it.iter);}
            bool operator !=(const const_iterator &it) const{return !(iter==it.iter);}
            const_iterator& operator ++(){
                ++iter;
                return *this;
            }
            const_iterator& operator --(){
                --iter;
                return *this;
            }
            TOSHSItem* operator *() const{return (TOSHSItem*)*iter;}
        private:
            QList<BaseTreeItem*>::const_iterator iter;
    };

    iterator begin();
    iterator end();

    const_iterator cbegin();
    const_iterator cend();

    void fill(const BaseTreeItem& o);
    void fill(const BaseTreeItem* o);

    TOSHSItem(const TOSHSItem& obj);
    TOSHSItem(const QUuid& id);
    TOSHSItem(const QUuid& id, const QString& sname, const QString& fname, const QUuid& iconid, SUB_TYPE subtype, OWNER_TYPE owner, const QJsonObject& data);
    TOSHSItem(const QUuid& id, const QString& name, const QUuid& iconid, SUB_TYPE subtype, OWNER_TYPE owner, const QJsonObject& data);
    TOSHSItem(const QUuid& id, const QString& name, const QUuid& iconid, SUB_TYPE subtype, OWNER_TYPE owner);

    bool fromJson(const QJsonValue& str);
    QJsonObject toJson();

    const TOSHSItem* getParent() const;
    void addChild(TOSHSItem* item);
    void addChild(const QList<TOSHSItem*>& items);
    void setParent(TOSHSItem* item);
    TOSHSItem* clone() const;

    // getters
    const QUuid& getId()const;              ///< получить идентификатор элемента ОШС
    const QString& getFname() const;        ///< получить полное наименование
    const QString& getSname() const;        ///< получить сокращенное наименование
    const QUuid& getIdIcon() const;         ///< получить идентификатор значка
    OWNER_TYPE getOwner() const;            ///< получить Принадлежность: 1 - свой; 2 - противник; 3 - кастом
    const QJsonObject& getData() const;     ///< получить информацию не для обработки
    SUB_TYPE getSubtype() const;      ///< получить Тип подчинения: 1 - непосредственное; 2 - придан; 3 - усиление

    // setters
    void setId(const QUuid& id);            ///< задать идентификатор элемента ОШС
    void setFname(const QString& fname);    ///< задать полное наименование
    void setSname(const QString& sname);    ///< задать сокращенное наименование
    void setIdIcon(const QUuid& id_icon);   ///< задать идентификатор значка
    void setOwner(OWNER_TYPE owner);        ///< задать Принадлежность: 1 - свой; 2 - противник; 3 - кастом
    void setData(const QJsonObject& data);  ///< задать информацию не для обработки
    void setSubtype(const SUB_TYPE& subtype); ///< задать Тип подчинения: 1 - непосредственное; 2 - придан; 3 - усиление
};

/**
 * @brief Список ОШС
 */
class TOSHSItemList {
public:
    TOSHSItemList(){}
    virtual ~TOSHSItemList(){}

    QList<TOSHSItem*>::iterator begin();
    QList<TOSHSItem*>::iterator end();
};

#endif // OSHS_H
