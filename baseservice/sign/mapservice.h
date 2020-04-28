#ifndef MAPSERVICE_H
#define MAPSERVICE_H

#include "map.h"


/**
 * @brief Класс cервисов для атрибутов карты
 */
class MapService {
public:
    /**
     * @brief получить екземпляр cервисов
     * @return сервис
     */
    static MapService* getInstance();
    /**
     * @brief получить атрибуты карты
     * @param p - параметры запроса
     * @return список элементов
     */
    TMapItemList* getMaps(AbstructTransferObject* p);
    /**
     * @brief добовать/обновить атрибуты карты
     * @param p - параметры запроса
     * @return список элементов
     */
    TMapItemList* updMaps(AbstructTransferObject* p, bool upd);
    /**
     * @brief удалить атрибуты карты
     * @param p - параметры запроса
     * @return код результата
     */
    RETURN_CODE delMaps(AbstructTransferObject* p);
private:
    MapService();
    ~MapService();

    class Ctrl{
    public:
        Ctrl();
        ~Ctrl();
    };
    friend class Ctrl;
    static MapService *_inatance;
    static Ctrl _ctrl;
};

#endif // MAPSERVICE_H
