#ifndef MARCHSERVICE_H
#define MARCHSERVICE_H

#include <QMap>

#include "march.h"


//#define TEST_RESULT
/**
 * @brief Сервис для работы с маршем
 */
class MarchService{
public:
    /**
     * @brief получить экземпляр cервисов
     * @return сервис
     */
    static MarchService* getInstance();
    /**
     * @brief получить марши
     * @param p - параметры запроса
     * @return список элементов
     */
    TMarchItemList* getMarchs(AbstructTransferObject* p);
    /**
     * @brief получить словарь параметров
     * @param p - параметры запроса
     * @return список элементов
     */
    TMarchParamDic* getMarchParamDic(AbstructTransferObject* p);
    /**
     * @brief добовать/обновить марш
     * @param p - параметры запроса
     * @return список элементов
     */
    TMarch* updMarch(AbstructTransferObject* p, bool upd);
    /**
     * @brief удалить марш
     * @param p - параметры запроса
     * @return код результата
     */
    RETURN_CODE delMarch(AbstructTransferObject* p);
    /**
     * @brief решить марш
     * @param p - параметры запроса
     * @return код результата
     */
    RETURN_CODE solveMarch(AbstructTransferObject* p);
private:
    MarchService();
    ~MarchService();
    /**
     * @brief изменения данных по маршу в БД
     * @param item - марш
     * @param mp - параметры БД
     * @param upd - добавление/изменение
     * @param isNew - новый или результат решения
     * @return результат операции
     */
    RETURN_CODE updMarch(TMarch* item, const QMap<QString, QString> &mp, bool upd, bool isNew);
    /**
     * @brief получение данных из БД
     * @param lsMarch - результирующий список
     * @param mp - параметры БД
     * @param id - идентификатор (марша/операции)
     * @param isFullMode - режим получения данных
     * @return
     */
    RETURN_CODE getMarchs(TMarchItemList* lsMarch, const QMap<QString, QString> &mp, const QUuid& id, bool isFullMode);
    /**
     * @brief решение задачи марша
     * @param item - марш
     */
    void solve(TMarch* item);

    /**
     * @brief получение параметра
     * @param item - текущее решение
     * @param pathid - идентификатор маршрута
     * @param paramid - идентификатор параметра
     * @return значение
     */
    double getParam(TMarch *item, const QUuid& pathid, const QUuid& paramid);
    /**
     * @brief проверка обращения к БД
     * @param res - статус
     * @param p - заполнение результата
     */
    RETURN_CODE checkExecQuery(bool res,AbstructTransferObject* p,const QString& mes);
    /**
     * @brief получение и расчет параметров формирования: кол-во машин длинна коллонны и т.д.
     * @param dep - формирование
     */
    void calcDistDeps(TMarchResultDep* dep, double predist);
    /**
     * @brief создание ключа
     * @param d - формирование
     * @param st - участок
     * @return ключ
     */
    QString createKey(const TMarchPath* d,const TMarchPathItem* st) const;
    QString createKey(const TMarchResultDep* d,const TMarchResultMilestone* st) const;
#ifdef TEST_RESULT
    void solveTest(TMarch* item);
#endif

    QMap<QUuid,double> _mapParamDefault; ///< значение по умолчания

    class Ctrl{
    public:
        Ctrl();
        ~Ctrl();
    };
    friend class Ctrl;
    static MarchService *_inatance;
    static Ctrl _ctrl;


    const static char* CONN_NAME;
    const static char* PARAM_VARID;
    const static char* PARAM_ID;

    const static char* qDELETE;
    const static char* qSELECT;
    const static char* qSELECT_PARAM_DIC;
    const static char* qSELECT_FULL;
    const static char* qINSERT_UPD;

    const static char* qPARAM_ID;
    const static char* qPARAM_VARID;
    const static char* qPARAM_TITLE;
    const static char* qPARAM_STATUS;
    const static char* qPARAM_ERRCODE;
    const static char* qPARAM_MAINDEPID;
    const static char* qPARAM_DASTARTID;
    const static char* qPARAM_DAFINISHID;
    const static char* qPARAM_CONTENT;
    const static char* qPARAM_UPD;
    const static char* qPARAM_ISALL;


    //param id consts
    static const char* MARCH_PARAM_FIRST_HALF_REST;///<	Продолжительность привала в первой половине суточного перехода, час.
    static const char* MARCH_PARAM_SECOND_HALF_REST;///< Продолжительность привала во второй половине суточного перехода, час.
    static const char* MARCH_PARAM_DAY_REST_PERIOD;///< Суточный отдых назначается через, сут. перех.
    static const char* MARCH_PARAM_TIME_BETWEEN_ESHELONS;///< Продолжительность времеменного разрыва между эшелонами, час.
    static const char* MARCH_PARAM_DIST_BETWEEN_UNIT;///< машинами
    static const char* MARCH_PARAM_DIST_BETWEEN_COL;///< колоннами
    static const char* MARCH_PARAM_DIST_BETWEEN_BAT;///< отдельными батальонами(дивизионами)
    static const char* MARCH_PARAM_DIST_BETWEEN_BATR;///< отдельными ротами(батр.)
    static const char* MARCH_PARAM_SPEED;///< на маршруте
    static const char* MARCH_PARAM_SPEED_2_SL;///< до ИР
    static const char* MARCH_PARAM_SPEED_2_DA;///< до План. р-на
};

#endif // MARCHSERVICE_H
