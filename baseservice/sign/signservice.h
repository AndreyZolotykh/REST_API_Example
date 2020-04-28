#ifndef SIGNSERVICE_H
#define SIGNSERVICE_H

#include "sign.h"


/**
 * @brief Класс cервисов для знака
 */
class SignService {
public:
    /**
     * @brief получить екземпляр cервисов
     * @return сервис
     */
    static SignService* getInstance();
    /**
     * @brief получить знаки
     * @param p - параметры запроса
     * @return список элементов
     */
    TSignItemList* getSigns(AbstructTransferObject* p);
    /**
     * @brief добовать/обновить знаки
     * @param p - параметры запроса
     * @return список элементов
     */
    TSignItemList* updSigns(AbstructTransferObject* p, bool upd);
    /**
     * @brief удалить знаки
     * @param p - параметры запроса
     * @return код результата
     */
    RETURN_CODE delSigns(AbstructTransferObject* p);
private:
    SignService();
    ~SignService();

    class Ctrl{
    public:
        Ctrl();
        ~Ctrl();
    };
    friend class Ctrl;
    static SignService *_inatance;
    static Ctrl _ctrl;
};

#endif // SIGNSERVICE_H
