#ifndef DEPLOYAREAS_H
#define DEPLOYAREAS_H
#include <QList>
#include <QString>
#include "../commonheaders/iparameters.h"
#include "../commonheaders/templist.h"

/**
 * @brief Элемент словаря районов
 */
class TDeployAreasItem :public ISerialize{
public:
    TDeployAreasItem();
    virtual ~TDeployAreasItem();

    virtual bool fromJson(const QJsonValue& str);
    virtual QJsonObject toJson();

    // getters/setters
    int getCode()const;
    int getTypes()const;
    const QString& getFname()const;
    const QString& getSname()const;
    bool isNumber()const;
    int getMinVal()const;
    int getMaxVal()const;
    int getGroup()const;
    int getSubType()const;

    void setCode(int v);
    void setTypes(int v);
    void setFname(const QString& st);
    void setSname(const QString& st);
    void setIsNumber(bool v);
    void setMinVal(int v);
    void setMaxVal(int v);
    void setGroup(int v);
    void setSubType(int v);

private:
    int _code;
    int _types;
    QString _fname;
    QString _sname;
    bool _isNumber;
    int _minval;
    int _maxval;
    int _group;
    int _subtype;
};

/**
 * @brief Список элементов словаря
 */
class TDeployAreasItemList :public TemplList<TDeployAreasItem>{
public:
    TDeployAreasItemList();
    virtual ~TDeployAreasItemList();
};

/**
 * @brief Класс для работы со словарем районов
 */
class DeployAreasCtrl {
public:    
    /**
     * @brief получить екземпляр контроллера словаря
     * @return екземпляр контроллера словаря
     */
    static DeployAreasCtrl* getInstance();
    /**
     * @brief получить словарь типов районов
     * @param p - параметры запроса
     * @return список элементов
     */
    TDeployAreasItemList* getDic(AbstructTransferObject* p);
private:
    DeployAreasCtrl();
    ~DeployAreasCtrl();

    class Ctrl{
    public:
        Ctrl();
        ~Ctrl();
    };
    friend class Ctrl;
    static DeployAreasCtrl *_inatance;
    static Ctrl _ctrl;
};

#endif // DEPLOYAREAS_H
