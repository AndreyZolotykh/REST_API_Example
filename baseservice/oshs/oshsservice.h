#ifndef OSHSSERVICE_H
#define OSHSSERVICE_H

#include <QMap>

#include "oshs.h"

/**
 * @brief Сервис для работы с ОШС
 */
class OSHSService{
public:
    /**
     * @brief получить экземпляр cервисов
     * @return сервис
     */
    static OSHSService* getInstance();
    /**
     * @brief получить элементы ОШС
     * @param p - параметры запроса
     * @return список элементов
     */
     TOSHSItem* getOSHS(AbstructTransferObject* p);
     /**
      * @brief добавить/обновить элементы ОШС
      * @param p - параметры запроса
      * @return список элементов
      */
     TOSHSItem* updOSHS(AbstructTransferObject* p, bool upd);
     /**
      * @brief удалить элементы ОШС
      * @param p - параметры запроса
      * @return код результата
      */
     RETURN_CODE delOSHS(AbstructTransferObject* p);
     TOSHSItem* getOSHTree(AbstructTransferObject* p);

     TOSHSItem* getOSHSTEST(AbstructTransferObject* p);
     TOSHSItem* getOSHSFAKE(AbstructTransferObject* p);

private:
    OSHSService();
    ~OSHSService();
    /**
     * @brief получение данных из БД
     * @param lsOshs - результирующий список
     * @param mp - параметры БД
     * @param id - идентификатор ОШС
     * @param isFullMode - режим получения данных
     * @return
     */

    QMap<QUuid,double> _mapParamDefault; ///< значение по умолчания

    class Ctrl{
    public:
        Ctrl();
        ~Ctrl();
    };
    friend class Ctrl;
    static OSHSService *_instance;
    static Ctrl _ctrl;

};

#endif // OSHSSERVICE_H
