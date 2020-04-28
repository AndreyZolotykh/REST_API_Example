#ifndef REGROUPINGPARAMPT_H
#define REGROUPINGPARAMPT_H

#include <QUuid>

#include "../commonheaders/iparameters.h"
#include "../commonheaders/templist.h"
#include "../commonheaders/enums.h"
/**
 * @brief Районы объектов(точек) погрузки/разгрузки в задачах перегруппировки
 */
class TRegroupingParamPtGroup: public ISerialize{
public:
    TRegroupingParamPtGroup();
    virtual ~TRegroupingParamPtGroup();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    const QUuid& getId() const;
    const QString& getTitle() const;
    REGROUPING_TYPE getType() const;

    void setId(const QUuid& id);
    void setTitle(const QString& title);
    void setType(REGROUPING_TYPE type);

    static const char* getDBField_id();///< поле: идентификатор
    static const char* getDBField_type();///< поле: тип
    static const char* getDBField_title();///< поле: название
private:
    QUuid _id;
    QString _title;
    REGROUPING_TYPE _type;

    static const char* stJsonField_id;///< поле: идентификатор
    static const char* stJsonField_type;///< поле: тип
    static const char* stJsonField_title;///< поле: название

    static const char* stDBField_id;///< поле: идентификатор
    static const char* stDBField_type;///< поле: тип
    static const char* stDBField_title;///< поле: название
};
/**
 * @brief Класс объекта(точки) погрузки/разгрузки в задачах перегруппировки
 */
class TRegroupingParamPtItem:public ISerialize{
public:
    TRegroupingParamPtItem();
    virtual ~TRegroupingParamPtItem();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    const QString& getId()const;///< идентификатор
    REGROUPING_TYPE getType()const;///< тип
    const QString& getTitle()const;///< название
    int getUpload()const;///< время погрузки
    int getDownload()const;///< время разгрузки
    const QJsonValue& getAttr()const;///< атрибуты
    const TRegroupingParamPtGroup* getGroup() const;///< группа

    void setId(const QString& id);///< идентификатор
    void setType(REGROUPING_TYPE type);///< тип
    void setTitle(const QString& title);///< название
    void setUpload(int upload);///< время погрузки
    void setDownload(int download);///< время разгрузки
    void setAttr(const QJsonValue& attr);///< атрибуты
    void setGroup(TRegroupingParamPtGroup* group);///< группа

    static const char* getDBField_id();///< получить поле: идентификатор
    static const char* getDBField_type();///< получить поле: тип
    static const char* getDBField_title();///< получить поле: название
    static const char* getDBField_upload();///< получить поле: время погрузки
    static const char* getDBField_download();///< получить поле: время разгрузки
    static const char* getDBField_attr();///< получить поле: атрибуты
    static const char* getDBField_group();///< получить поле: группа
    static const char* getDBField_gtitle();
private:
    QString _id;///< идентификатор
    REGROUPING_TYPE _type;///< тип
    QString _title;///< название
    int _upload;///< время погрузки
    int _download;///< время разгрузки
    QJsonValue _attr;///< атрибуты
    TRegroupingParamPtGroup *_group;///<группа

    static const char* stJsonField_id;///< поле: идентификатор
    static const char* stJsonField_type;///< поле: тип
    static const char* stJsonField_title;///< поле: название
    static const char* stJsonField_upload;///< поле: время погрузки
    static const char* stJsonField_download;///< поле: время разгрузки
    static const char* stJsonField_attr;///< поле: атрибуты
    static const char* stJsonField_group;///< поле: группа

    static const char* stDBField_id;///< поле: идентификатор
    static const char* stDBField_type;///< поле: тип
    static const char* stDBField_title;///< поле: название
    static const char* stDBField_upload;///< поле: время погрузки
    static const char* stDBField_download;///< поле: время разгрузки
    static const char* stDBField_attr;///< поле: атрибуты
    static const char* stDBField_group;///< поле: группа
    static const char* stDBField_gtitle;
};

class TRegroupingParamPtItemList:public TemplList<TRegroupingParamPtItem>{
public:
    TRegroupingParamPtItemList(){;}
    ~TRegroupingParamPtItemList(){;}

    QList<TRegroupingParamPtItem*>::iterator begin();
    QList<TRegroupingParamPtItem*>::iterator end();
};

class TRegroupingParamPtGroupList:public TemplList<TRegroupingParamPtGroup>{
public:
    TRegroupingParamPtGroupList(){;}
    ~TRegroupingParamPtGroupList(){;}

    QList<TRegroupingParamPtGroup*>::iterator begin();
    QList<TRegroupingParamPtGroup*>::iterator end();
};
#endif // REGROUPINGPARAMPT_H
