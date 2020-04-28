#ifndef ATTRVVT_H
#define ATTRVVT_H
#include <QUuid>


#include "../commonheaders/templist.h"
#include "../commonheaders/iparameters.h"

class TAttrVVTItem:public ISerialize{
public:
    TAttrVVTItem();
    ~TAttrVVTItem();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    const QUuid& getId()const;
    const QJsonValue& getAttr()const;

    void setId(const QUuid& v);
    QJsonValue setAttr(const QJsonValue& v);

    static const char* getDBField_id();
    static const char* getDBField_attr();
private:
    QUuid _id;
    QJsonValue _attr;

    static const char* stJsonField_id;
    static const char* stJsonField_attr;


    static const char* stDBField_id;
    static const char* stDBField_attr;
};

class TAttrVVTItemList: public TemplList<TAttrVVTItem>{
public:
    TAttrVVTItemList(){;}
    virtual ~TAttrVVTItemList(){;}

    QList<TAttrVVTItem*>::iterator begin();
    QList<TAttrVVTItem*>::iterator end();
};


class TAttrItem:public ISerialize{
public:
    TAttrItem();
    ~TAttrItem();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    int getId()const;
    const QString& getTitle() const;
    const QString& getShortTitle() const;
    int getGroup() const;

    void setId(int v);
    void setTitle(const QString& v);
    void setShortTitle(const QString&  v);
    void setGroup(int v);

    static const char* getDBField_id();
    static const char* getDBField_title();
    static const char* getDBField_stitle();
    static const char* getDBField_gr();
private:
    int _id;    ///< идентификатор
    QString _title; ///< наименование
    QString _stitle; ///< сокращение
    int _gr; ///< группа

    static const char* stJsonField_id;
    static const char* stJsonField_title;
    static const char* stJsonField_stitle;
    static const char* stJsonField_gr;

    static const char* stDBField_id;
    static const char* stDBField_title;
    static const char* stDBField_stitle;
    static const char* stDBField_gr;
};

class TAttrItemList: public TemplList<TAttrItem>{
public:
    TAttrItemList(){;}
    virtual ~TAttrItemList(){;}

    QList<TAttrItem*>::iterator begin();
    QList<TAttrItem*>::iterator end();
};

#endif // ATTRVVT_H
