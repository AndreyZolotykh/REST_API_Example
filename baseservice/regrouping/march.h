#ifndef MARCH_H
#define MARCH_H

#include <QList>
#include <QUuid>
#include <QDateTime>
#include <QMap>

#include "../commonheaders/templist.h"


class TMarchParam;
class TMarchPath;
class TMarchMilestone;
class TMarchResult;

/**
 * @brief Марш своим ходом
 */
class TMarch: public ISerialize{
public:
    /**
     * @brief состояния объекта
     */
    enum STATUS{
        NEW = 10, ///< не решен
        SOLVED = 20, ///< решен
        ERR = 30, ///< ошибка в процессе решения
        PROC = 40, ///< решается (идет процесс решения)
        UNDEF = 66 ///< не определено
    };

    /**
     * @brief режим движения
     */
    enum MODE{
        MODE_FROM = 1, ///< задано время прохождение ИР
        MODE_TO = 2, ///< задано время прибытия в РР
        MODE_DIRECT = 3 ///< задано оба времени
    };

    /**
     * @brief Коды ошибок
     */
    enum ERR_CODE{
        OK = 0,             ///< ошибок нет
        WRONG_ERR_CODE      ///< неверный код ошибки
    };

    TMarch();
    virtual ~TMarch();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();
    QJsonObject toJson(bool isShort);

    const TMarchParam* getParam(const QUuid& pathid,const QUuid& paramid); ///< поиск параметров маршрута
#ifdef QT_DEBUG
    void printMapPram()const;
#endif

    // getters
    const QUuid& getId()const; ///< идентификатор
    const QUuid& getVarId()const; ///< идентификатор операции
    const QString& getTitle()const; ///< название
    const QUuid& getMainDepId()const; ///< идентификатор формирования
    STATUS getStatus()const; ///< статус
    ERR_CODE getErrCode()const; ///< код ошибки
    QString getErrDescr(); ///< описание ошибки
    const QUuid& getDeployAreaStartId()const; ///< идентификатор района откуда марш
    const QUuid& getDeployAreaFinishId()const; ///< идентификатор района куда марш
    MODE getMode() const; ///< режим расчетов
    const QDateTime& getBegin() const; ///< время начало прохождеия ИР
    const QDateTime& getEnd() const; ///< время развертывания в РР
    const QList<TMarchParam*>& getParam()const; ///< параметры марша
    const QList<TMarchPath*>& getPaths()const; ///< маршруты
    const QList<TMarchMilestone*>& getMilestones()const; ///< рубежи (ИР, РР, РО)
    const TMarchResult * getResult()const; ///< результат расчетов
    const QUuid& getOwnerid()const; ///< владелец
    // setters
    void setId(const QUuid& id); ///< идентификатор
    void setVarId(const QUuid& varid); ///< идентификатор операции
    void setTitle(const QString& title); ///< название
    void setMainDepId(const QUuid& mainDepId); ///< идентификатор формирования
    void setStatus(STATUS status); ///< статус
    void setErrCode(ERR_CODE errCode); ///< код ошибки
//    void setErrDescr(const QString& errDescr); ///< описание ошибки
    void setDeployAreaStartId(const QUuid& deployAreaStartId); ///< идентификатор района откуда марш
    void setDeployAreaFinishId(const QUuid& deployAreaFinishId); ///< идентификатор района куда марш
    void setMode(MODE mode); ///< режим расчетов
    void setBegin(const QDateTime& v); ///< время начало прохождеия ИР
    void setEnd(const QDateTime& v); ///< время развертывания в РР
    void addParam(TMarchParam *param); ///< параметры марша
    void addPath(TMarchPath* paths); ///< маршруты
    void addMilestone(TMarchMilestone* milestones); ///< рубежи (ИР, РР, РО)
    void setResult(TMarchResult *result); ///< результат расчетов
    void setOwnerId(const QUuid& ownerid); ///< владелец


    // атрибуты DB
    static const char* getDBField_id();
    static const char* getDBField_varid();
    static const char* getDBField_title();
    static const char* getDBField_mainDepId();
    static const char* getDBField_status();
    static const char* getDBField_errCode();
    static const char* getDBField_deployAreaStartId();
    static const char* getDBField_deployAreaFinishId();
    static const char* getDBField_content();

private:
    QUuid _id; ///< идентификатор
    QUuid _varid; ///< идентификатор операции
    QString _title; ///< название
    QUuid _mainDepId; ///< идентификатор формирования
    STATUS _status; ///< статус
    ERR_CODE _errCode; ///< код ошибки
//    QString _errDescr; ///< описание ошибки
    QUuid _deployAreaStartId; ///< идентификатор района откуда марш
    QUuid _deployAreaFinishId; ///< идентификатор района куда марш
    MODE _mode; ///< режим
    QDateTime _tbegin; ///< время начало прохождеия ИР
    QDateTime _tend; ///< время развертывания в РР
    QList<TMarchParam*> _param; ///< параметры марша
    QList<TMarchPath*> _paths; ///< маршруты
    QList<TMarchMilestone*> _milestones; ///< рубежи (ИР, РР, РО)
    TMarchResult *_result; ///< результат расчетов
    QUuid _ownerid; ///< владелец


    QMap<QUuid,QMap<QUuid,TMarchParam*>> _mapParam; ///< хешь для быстрого поиска параметров


    // строковые значения статусов
    static const char* stStatus_NEW;
    static const char* stStatus_SOLVED;
    static const char* stStatus_ERR;
    static const char* stStatus_PROC;

    // строковое значение режимов
    static const char* stMODE_FROM;
    static const char* stMODE_TO;
    static const char* stMODE_DIRECT;

    // атрибуты Json
    static const char* stJsonField_id;
    static const char* stJsonField_varid;
    static const char* stJsonField_title;
    static const char* stJsonField_mainDepId;
    static const char* stJsonField_status;
    static const char* stJsonField_errCode;
    static const char* stJsonField_errDescr;
    static const char* stJsonField_deployAreaStartId;
    static const char* stJsonField_deployAreaFinishId;
    static const char* stJsonField_mode;
    static const char* stJsonField_tbegin;
    static const char* stJsonField_tend;
    static const char* stJsonField_param;
    static const char* stJsonField_paths;
    static const char* stJsonField_milestones;
    static const char* stJsonField_result;
    static const char* stJsonField_ownerid;


    // атрибуты DB
    static const char* stDBField_id;
    static const char* stDBField_varid;
    static const char* stDBField_title;
    static const char* stDBField_mainDepId;
    static const char* stDBField_status;
    static const char* stDBField_errCode;
    static const char* stDBField_deployAreaStartId;
    static const char* stDBField_deployAreaFinishId;
    static const char* stDBField_content;


    // преобразование перечислений
    void setStatus(const QString& v);/// < статус из строки
    QString getStatusAsStr() const; ///< статус как строка

    void setMode(const QString& v);/// < режим из строки
    QString getModeAsStr() const; ///< режим как строка


    static const char* errDescriptions[];
    static const quint32 MAX_ERR_CODE;
};


/**
 * @brief Параметры марша
 */
class TMarchParam: public ISerialize{
public:
    TMarchParam();
    virtual ~TMarchParam();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    //getters
    const QUuid& getId() const; ///< идентификатор
    const QUuid& getPathId()const;///< номер маршрута
    const QString& getTitle()const;///< название параметра для визуализации
    double getValue()const;///< значение (в словаре значение по умолчанию)
    //setters
    void setId(const QUuid& id); ///< идентификатор
    void setPathId(const QUuid& pathid);///< номер маршрута
    void setTitle(const QString& title);///< название параметра для визуализации
    void setValue(double value);///< значение (в словаре значение по умолчанию)

    bool isCommon() const;

    static const char* getDBField_id();
    static const char* getDBField_value();
    static const char* getDBField_title();
private:
    QUuid _id;      ///< идентификатор
    QUuid _pathid;  ///< идентификато маршрута
    bool _isCommon; ///< общие данные
    QString _title; ///< название параметра для визуализации
    double _value;  ///< значение (в словаре значение по умолчанию)

    static const char* stJsonField_id;
    static const char* stJsonField_pathid;
    static const char* stJsonField_title;
    static const char* stJsonField_value;

    static const char* stDBField_id;
    static const char* stDBField_value;
    static const char* stDBField_title;
};

/**
 * @brief элемент маршрута
 */
class TMarchPathItem: public ISerialize{
public:
    /**
     * @brief Тип элемента маршрута
     */
    enum TYPE{
        TO_SL = 1, ///< до ИР
        TO_RA = 2, ///< до РО
        TO_RL = 3, ///< до РР
        TO_DA = 4, ///< ДО района развертывания
        TYPE_UNDEF = -1, ///< участок, если появится дробления по классу дорог
    };
    /**
     * @brief класс дороги
     */
    enum CLASS{
        CLASS_UNDEF = -1
    };

    TMarchPathItem();
    virtual ~TMarchPathItem();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    static bool comapre(const TMarchPathItem* o1,const TMarchPathItem* o2); ///< сравнение эл-в
    //getters
    TYPE getType() const; ///< тип элемента
    CLASS getClass() const; ///< класс элемента
    double getSpeed() const; ///< скорость заданная оператором
    int geOrder() const;///< порядковый номер эл-та
    int getNum() const; ///< номер для РР и РО
    int getDist() const;///< длина участка в метрах
    //setters
    void setType(TYPE typeitem); ///< тип элемента
    void setClass(CLASS classitem); ///< класс элемента
    void setSpeed(double speed); ///< скорость заданная оператором
    void setOrder(int order);///< порядковый номер эл-та
    void setNum(int num); ///< номер для РР и РО
    void setDist(int dist);///< длина участка в метрах

    QString getKey() const;
#ifdef QT_DEBUG
    QString toString() const;
#endif

private:

    TYPE _typeitem; ///< тип элемента
    CLASS _classitem; ///< класс элемента
    double _speed; ///< скорость заданная оператором
    int _order;///< порядковый номер эл-та
    int _num; ///< номер для РР и РО
    int _dist;///< длина участка в метрах

    static const char* stTYPE_TO_SL;
    static const char* stTYPE_TO_RL;
    static const char* stTYPE_TO_RA;
    static const char* stTYPE_TO_DA;
    static const char* st_UNDEF;


    static const char* stJsonField_typeitem;
    static const char* stJsonField_class;
    static const char* stJsonField_speed;
    static const char* stJsonField_order;
    static const char* stJsonField_num;
    static const char* stJsonField_dist;

    void setType(const QString& v);/// < тип из строки
    QString getTypeAsStr() const; ///< тип как строка

    void setClass(const QString& v);/// < класс из строки
    QString getClassAsStr() const; ///< класс как строка
};

/**
 * @brief Маршруты
 */
class TMarchPath: public ISerialize{
public:
    /**
     * @brief Элементы ПЭШ
     */
    enum FORMATION{
        UNDEF = -1
    };

    TMarchPath();
    virtual ~TMarchPath();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    void sortItems(); ///< сортировка эл-в
    // getters
    const QUuid& getId() const; ///< идентификатор формирования
    const QUuid& getPathId() const; ///< идентификатор формирования
    FORMATION getFormation() const;///< тип элемента ПЭШ
    int getOrder() const; ///< порядок следования
    const QList<TMarchPathItem*>& getItems()const; ///< участки маршрута
    // setters
    void setId(const QUuid& id); ///< идентификатор формирования
    void setPathId(const QUuid& pathid); ///< идентификатор формирования
    void setFormation(FORMATION frm);///< тип элемента ПЭШ
    void setOrder(int order); ///< порядок следования
    void add(TMarchPathItem* item); ///< участки маршрута
private:
    QUuid _id; ///< идентификатор формирования
    QUuid _pathid; ///< идентификатор маршрутов
    FORMATION _frm;///< тип элемента ПЭШ
    int _order; ///< порядок следования
    QList<TMarchPathItem*> _items; ///< участки маршрута


    static const char* stJsonField_id;
    static const char* stJsonField_pathid;
    static const char* stJsonField_frm;
    static const char* stJsonField_order;
    static const char* stJsonField_items;
};
/**
 * @brief Рубежи
 */
class TMarchMilestone: public ISerialize{
public:
    TMarchMilestone();
    virtual ~TMarchMilestone();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();
};

//---------------------------------------------------------------------------------------------
/**
 * @brief Результаты по рубежам и р-нам
 */
class TMarchResultMilestone: public ISerialize{
public:
    /**
     * @brief тип объекта
     */
    enum TYPE{
        SL = 1, ///< ИР
        RL = 2, ///< РР
        RA = 3, ///< РО
    };
    TMarchResultMilestone();
    virtual ~TMarchResultMilestone();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    //getters
    TYPE getType()const; ///< тип
    int getNum()const; ///< номер для РР и РО
    const QDateTime& getHead()const;///< время прохода головы колонны
    const QDateTime& getTail()const; ///< время прохода хвоста колонны
    const QDateTime& getRestTime()const; ///< время отдыха
    //setters
    void setType(TYPE type); ///< тип
    void setNum(int num); ///< номер для РР и РО
    void setHead(const QDateTime& head);///< время прохода головы колонны
    void setTail(const QDateTime& tail); ///< время прохода хвоста колонны
    void setRestTime(const QDateTime& resttime); ///< время отдыха

    long getIntHead()const;
    long getIntTail()const;
    void setIntHead(long head);
    void setIntTail(long tail);
    void addIntHead(long head);
    void addIntTail(long tail);

    QString getKey()const;
private:
    TYPE _type; ///< тип
    int _num; ///< номер для РР и РО
    QDateTime _head;///< время прохода головы колонны
    QDateTime _tail; ///< время прохода хвоста колонны
    QDateTime _resttime; ///< время отдыха

    long _intHead,_intTail; ///< внутренне время в секундах

    static const char* stTYPE_SL;
    static const char* stTYPE_RL;
    static const char* stTYPE_RA;

    static const char* stJsonField_typeitem;
    static const char* stJsonField_num;
    static const char* stJsonField_head;
    static const char* stJsonField_tail;
    static const char* stJsonField_resttime;

    void setType(const QString& v);/// < тип из строки
    QString getTypeAsStr() const; ///< тип как строка
};
/**
 * @brief Результаты по формированием
 */
class TMarchResultDep: public ISerialize{
public:
    TMarchResultDep();
    TMarchResultDep(const QUuid& depid,const QUuid& pathid);
    virtual ~TMarchResultDep();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    //getters
    const QUuid& getDepid()const;
    int getOrder()const;
    const QUuid& getPathId()const;
    int getUnitCt()const;
    double getUnitLength()const;
    double getPathLength()const;///< длина маршрута
    double getLen2SL()const;
    double getLen2DA()const;
    double getDistBetweenUnit()const; ///< дистанция между машинами
    double getSpeedPath()const;///< скорость на маршруте
    double getSpeed2SL()const;///< скорость до ИР
    double getSpeed2DA()const;///< скорость до РР
    const QDateTime& getBegin()const;
    const QDateTime&  getEnd()const;
    const QList<TMarchResultMilestone*>& getMilestones()const;
    //setters
    void setDepId(const QUuid &depid);
    void setOrder(int order);
    void setPathId(const QUuid& pathid);    
    void setUnitCt(int unitct);
    void setUnitsLength(double unitslength);
    void setPathLength(double pathlegth);///< длина маршрута
    void setLen2SL(double len2SL);
    void setLen2DA(double len2DA);
    void setDistBetweenUnit(double distBetweenUnit); ///< дистанция между машинами
    void setSpeedPath(double speedPath);///< скорость на маршруте
    void setSpeed2SL(double speed2SL);///< скорость до ИР
    void setSpeed2DA(double speed2DA);///< скорость до РР
    void setBegin(const QDateTime &begin);
    void setEnd(const QDateTime  &end);
    void addMilestone(TMarchResultMilestone* milestone);


    long getIntBegin()const;
    long getIntEnd()const;
    void setIntBegin(long begin);
    void setIntEnd(long end);
    void addIntBegin(long v);
    void addIntEnd(long v);

    double getPreDist()const;
    void setPreDist(double v);
private:

    QUuid _depid; ///< идентификатор формирования
    int _order; ///< порядок
    QUuid _pathid; ///< номер маршрута
    int _unitct; ///< кол-во машин
    double _unitslength; ///< длина коллоны
    double _pathlength;///< длина маршрута
    double _len2SL; ///< до ИР
    double _len2DA; ///< до РР
    double _distBetweenUnit; ///< дистанция между машинами
    double _speedPath;///< скорость на маршруте
    double _speed2SL;///< скорость до ИР
    double _speed2DA;///< скорость до РР
    QDateTime _begin; ///< время начала движения
    QDateTime  _end; ///< время прибытия
    QList<TMarchResultMilestone*> _milestones; ///< ИР, РР и РО

    long _intBegin,_intEnd; // внутренне время
    double _preDist; // дистанция до впереди идущего

    static const char* stJsonField_depid;
    static const char* stJsonField_order;
    static const char* stJsonField_pathid;
    static const char* stJsonField_unitct;
    static const char* stJsonField_unitslength;
    static const char* stJsonField_pathlength;
    static const char* stJsonField_len2SL;
    static const char* stJsonField_len2DA;
    static const char* stJsonField_distBetweenUnit;
    static const char* stJsonField_speedPath;
    static const char* stJsonField_speed2SL;
    static const char* stJsonField_speed2DA;
    static const char* stJsonField_begin;
    static const char* stJsonField_end;
    static const char* stJsonField_milestones;
};
/**
 * @brief Результаты расчетов
 */
class TMarchResult: public ISerialize{
public:
    TMarchResult();
    virtual ~TMarchResult();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    //getters
    int getDays()const; ///< кол-во суточных переходов
    int getDeep()const; ///< глубина полосы
    int getWidth()const; ///< ширина полосы
    int getRestCt()const; ///< кол-во суточных отдыхов
    const QList<TMarchResultDep*>& getDeps()const; ///< решения по формированиям
    //setters
    void setDays(int days); ///< кол-во суточных переходов
    void setDeep(int deep); ///< глубина полосы
    void setWidth(int width); ///< ширина полосы
    void setRestCt(int restct); ///< кол-во суточных отдыхов
    void addDeps(TMarchResultDep* deps); ///< решения по формированиям
private:
    int _days; ///< кол-во суточных переходов
    int _deep; ///< глубина полосы
    int _width; ///< ширина полосы
    int _restct; ///< кол-во суточных отдыхов
    QList<TMarchResultDep*> _deps; ///< решения по формированиям

    static const char* stJsonField_days;
    static const char* stJsonField_deep;
    static const char* stJsonField_width;
    static const char* stJsonField_restct;
    static const char* stJsonField_deps;
};



//---------------------------------------------------------------------------------------------
/**
 * @brief Список маршей
 */
class TMarchItemList: public TemplList<TMarch>{
public:
    TMarchItemList(){}
    virtual ~TMarchItemList(){}

    QList<TMarch*>::iterator begin();
    QList<TMarch*>::iterator end();
};

/**
 * @brief список элементов словаря
 */
class TMarchParamDic: public TemplList<TMarchParam>{
public:
    TMarchParamDic(){}
    virtual ~TMarchParamDic(){}

    QList<TMarchParam*>::iterator begin();
    QList<TMarchParam*>::iterator end();
};

#endif // MARCH_H
