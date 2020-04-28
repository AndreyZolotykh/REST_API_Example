#ifndef SIGN_H
#define SIGN_H
#include <QUuid>
#include <QJsonObject>
#include <QVariant>
#include <QMap>

#include "../commonheaders/iparameters.h"
#include "../commonheaders/templist.h"



/**
 * @brief Сущность знака
 */
class TSign: public ISerialize{
public:
    TSign();
    ~TSign();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    // getters
    const QUuid& getId()const; ///< получить идентификатор знака
    const QUuid& getMapId()const;///< получить идентификатор карты
    const QUuid& getLayerId()const;///< получить идентификатор слоя
    const QUuid& getVarId()const;///< получить идентификатор операции
    const QUuid& getDepId()const;///< задать идентификатор формирования
    const QString& getClassCode()const;///< получить класскод
    const QJsonObject& getParam()const;///< получить системные параметры
    const QJsonObject& getAttr()const;///< получить системные параметры
    const QVariant getAttr(const QString& key) const;///< получить атрибут по ключу

    // setters
    void setId(const QUuid& id);///< задать идентификатор знака
    void setMapId(const QUuid& mapId);///< задать идентификатор карты
    void setLayerId(const QUuid& layerId);///< задать идентификатор слоя
    void setVarId(const QUuid& varId);///< задать идентификатор операции
    void setDepId(const QUuid& depId);///< задать идентификатор формирования
    void setClassCode(const QString& classCode);///< задать класскод
    void setParam(const QJsonObject& param);///< задать системные параметры
    void setAttr(const QString &key, const QString& value);///< задать атрибуты
    void setAttr(const QString &key,int value);///< задать атрибуты
    void setAttr(const QString &key, bool value);///< задать атрибуты
    void setAttr(const QJsonObject& attr);///< задать атрибуты

    void clearAttr();///< очистить атрибуты
    void removeAttr(const QString &key);///< удалить атрибут по ключу
    bool isContainsAttr(const QString &key);///< существование атрибута

private:
    QUuid _id;
    QUuid _mapId;
    QUuid _layerId;
    QUuid _varId;
    QUuid _depId;
    QString _classCode;
    QJsonObject _param;
    QJsonObject _attr;


    void check(const QString &key);///< удаление обновляемого элемента
};


////////////////////////////////////////////

class TSignItemList: public TemplList<TSign>{
public:
    TSignItemList(){}
    virtual ~TSignItemList(){}

    QList<TSign*>::iterator begin();
    QList<TSign*>::iterator end();
};

#endif // SIGN_H
