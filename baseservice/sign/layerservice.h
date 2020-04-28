#ifndef LAYERSERVICE_H
#define LAYERSERVICE_H

#include "layer.h"


/**
 * @brief Класс cервисов для атрибутов карты
 */
class LayerService {
public:
    /**
     * @brief получить екземпляр cервисов
     * @return сервис
     */
    static LayerService* getInstance();
    /**
     * @brief получить атрибуты карты
     * @param p - параметры запроса
     * @return список элементов
     */
    TLayerItemList* getLayers(AbstructTransferObject* p);
private:
    LayerService();
    ~LayerService();

    class Ctrl{
    public:
        Ctrl();
        ~Ctrl();
    };
    friend class Ctrl;
    static LayerService *_inatance;
    static Ctrl _ctrl;
};

#endif // LAYERSERVICE_H
