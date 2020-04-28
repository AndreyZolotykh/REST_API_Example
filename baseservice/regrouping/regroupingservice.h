#ifndef REGROUPINGSERVICE_H
#define REGROUPINGSERVICE_H

#include "regroupingparampt.h"

class RegroupingService{
public:
    /**
     * @brief получить экземпляр cервисов
     * @return сервис
     */
    static RegroupingService* getInstance();
    /**
     * @brief получить словарь
     * @param p - параметры запроса
     * @return список элементов
     */
    TRegroupingParamPtItemList* getRegroupingParamPtItemList(AbstructTransferObject* p);
    /**
     * @brief получение данных из БД
     * @param lsParam - результирующий список
     * @param mp - параметры БД
     * @param type - тип данных
     * @return
     */
    RETURN_CODE getRegroupingParamPtItemList(TRegroupingParamPtItemList* lsParam, const QMap<QString, QString> &mp, int type);
    /**
     * @brief добовать/обновить элемент словаря
     * @param p - параметры запроса
     * @return список элементов
     */
    TRegroupingParamPtItem* updRegroupingParamPtItem(AbstructTransferObject* p, bool upd);
    RETURN_CODE updRegroupingParamPtItem(TRegroupingParamPtItem* item, const QMap<QString,QString>& mp, bool upd);
    /**
     * @brief удалить элемент словаря
     * @param p - параметры запроса
     * @return код результата
     */
    RETURN_CODE delRegroupingParamPtItemList(AbstructTransferObject* p);
    //
    /**
     * @brief получить словарь
     * @param p - параметры запроса
     * @return список элементов
     */
    TRegroupingParamPtGroupList* getRegroupingGrPtItemList(AbstructTransferObject* p);
    /**
     * @brief получение данных из БД
     * @param lsParam - результирующий список
     * @param mp - параметры БД
     * @param type - тип данных
     * @return
     */
    RETURN_CODE getRegroupingGrPtItemList(TRegroupingParamPtGroupList* lsGroup, const QMap<QString, QString> &mp, int type);
    /**
     * @brief добовать/обновить элемент словаря
     * @param p - параметры запроса
     * @return список элементов
     */
    TRegroupingParamPtGroup* updRegroupingGrPtItem(AbstructTransferObject* p, bool upd);
    RETURN_CODE updRegroupingGrPtItem(TRegroupingParamPtGroup* item, const QMap<QString,QString>& mp, bool upd);
    /**
     * @brief удалить элемент словаря
     * @param p - параметры запроса
     * @return код результата
     */
    RETURN_CODE delRegroupingGrPtItemList(AbstructTransferObject* p);
private:
    RegroupingService();
    ~RegroupingService();




    class Ctrl{
    public:
        Ctrl();
        ~Ctrl();
    };
    friend class Ctrl;
    static RegroupingService *_inatance;
    static Ctrl _ctrl;


    const static char* CONN_NAME;
    const static char* PARAM_ID;
    const static char* PARAM_TYPE;

    const static char* qDELETE;
    const static char* qSELECT;
    const static char* qINSERT_UPD;

    const static char* qDELETE_GR;
    const static char* qSELECT_GR;
    const static char* qINSERT_UPD_GR;

    const static char* qPARAM_ID;
    const static char* qPARAM_TITLE;
    const static char* qPARAM_TYPE;
    const static char* qPARAM_UPLOAD;
    const static char* qPARAM_DOWNLOAD;
    const static char* qPARAM_ATTR;
    static const char* qPARAM_GID;
    static const char* qPARAM_GTITLE;
    const static char* qPARAM_UPD;
    const static char* qPARAM_ISALL;

};

#endif // REGROUPINGSERVICE_H
