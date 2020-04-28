#ifndef ATTRVVTSERVICE_H
#define ATTRVVTSERVICE_H

#include "../commonheaders/iparameters.h"
#include "attrvvt.h"

class AttrVVTService {
public:
    /**
     * @brief получить экземпляр cервисов
     * @return сервис
     */
    static AttrVVTService* getInstance();

    /**
     * @brief получить атрибуты ВВТ
     * @param p - параметры запроса
     * @return список элементов
     */
    TAttrVVTItemList* getAttrVVT(AbstructTransferObject* p);
    /**
     * @brief добовать/обновить атрибуты ВВТ
     * @param p - параметры запроса
     * @return список элементов
     */
    TAttrVVTItem* updAttrVVT(AbstructTransferObject* p, bool upd);
    /**
     * @brief удалить атрибуты ВВТ
     * @param p - параметры запроса
     * @return код результата
     */
    RETURN_CODE delAttrVVT(AbstructTransferObject* p);

    /**
     * @brief Получить словарь атрибутов ВВТ
     * @param p - параметры
     * @return список атрибутов
     */
    TAttrItemList* getAttrList(AbstructTransferObject *p);
private:
    AttrVVTService();
    virtual ~AttrVVTService();

    class Ctrl{
    public:
        Ctrl();
        ~Ctrl();
    };
    friend class Ctrl;
    static AttrVVTService *_inatance;
    static Ctrl _ctrl;

    const static char* CONN_NAME;

    const static char* PARAM_GR;
    const static char* PARAM_ID;

    const static char* qSELECT_ATTR_DIC;
    const static char* qSELECT_ATTR_VVT_DIC;
    const static char* qINSERT_UPDATE_ATTR_VVT_DIC;
    const static char* qDELETE_ATTR_VVT_DIC;

    const static char* qPARAM_GR;
    const static char* qPARAM_ID;
    const static char* qPARAM_ATTR;
    const static char* qPARAM_ISALL;
    const static char* qPARAM_UPD;
};

#endif // ATTRVVTSERVICE_H
