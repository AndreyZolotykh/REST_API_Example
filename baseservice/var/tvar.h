#ifndef TVAR_H
#define TVAR_H
#include <QUuid>
#include <QString>
#include <QDateTime>
#include <QSqlDatabase>
#include "../commonheaders/iparameters.h"

#include <QtCore/qglobal.h>


namespace VAR {

/*!
 * \brief Возможные состояния операции
 */
enum VAR_STATE{
    stsDeleted,     ///< Удалена
    stsPlanning,    ///< Планируется
    stsComplited,   ///< Завершена
    stsUndef = 66   ///< Неопределено
};


/*!
 * \brief Сущност: операция
 */
class TVar :public ISerialize{

private:
    QUuid _id;          ///< Идентификатор
    QUuid _owner;       ///< Владелец
    QString _title;     ///< Название
    QDateTime _tbegin;  ///< Начало
    QDateTime _tend;    ///< Завершение
    VAR_STATE _state;   ///< Состояние
public:
    TVar();
    virtual ~TVar();

    // GET
    const QUuid& getId() const;
    const QUuid& getOwnerId() const;
    const QString& getTitle() const;
    const QDateTime& getBegin() const;
    const QDateTime& getEnd() const;
    VAR_STATE getState() const;
    // SET
    void setId(const QUuid& v);
    void setOwnerId(const QUuid& v);
    void setTitle(const QString&v);
    void setBegin(const QDateTime& v);
    void setEnd(const QDateTime& v);
    void setState(VAR_STATE v);

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();
private:
    VAR_STATE convState(const QString& v)const;
    QString convState() const;
};

/*!
 * \brief Список орераций
 */
class TListVar: ISerialize{

private:
    QList<TVar*> _items;    ///< Операции
public:
    TListVar();
    virtual ~TListVar();

    void add(TVar *);   ///< Добавления элемента
    bool isEmpty();     ///< Проверка на пустой список

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();
};
#ifdef QT_DEBUG
QString getVariantsTEST();
#endif
int createCurrentVariant(AbstructTransferObject* p);

}


#if defined(BASESERVICE_LIBRARY)
#  define VARSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VARSHARED_EXPORT Q_DECL_IMPORT
#endif
extern "C" {
VARSHARED_EXPORT VAR::TListVar* getVariants(AbstructTransferObject *p);
VARSHARED_EXPORT VAR::TListVar* getVariantsConn(const QUuid& id, QSqlDatabase& conn);

VARSHARED_EXPORT int delVariants(AbstructTransferObject *p);
VARSHARED_EXPORT int delVariantsConn(const QUuid& id, QSqlDatabase& conn);

VARSHARED_EXPORT int setVariants(AbstructTransferObject *p);
VARSHARED_EXPORT int setVariantsConn(VAR::TVar* var, QSqlDatabase& conn, bool upd);
}

#endif // TVAR_H
