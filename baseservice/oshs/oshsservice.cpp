#include "oshsservice.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonDocument>
#include <QSqlDatabase>

#include "utils/utils.h"
#include "../dbconnection.h"

OSHSService* OSHSService::_instance = NULL;
OSHSService::Ctrl OSHSService::_ctrl;

OSHSService::Ctrl::Ctrl(){

}
OSHSService::Ctrl::~Ctrl(){
    if(_instance!=NULL){
        delete _instance;
    }
}

OSHSService::OSHSService(){

}

OSHSService *OSHSService::getInstance(){
    if(_instance==NULL){
        _instance = new OSHSService();
    }
    return _instance;
}

const char* CONN_NAME_OSHS = "OshsService";

const char* qINSERT_OSHS_ITEM = "SELECT * FROM v1.oshs_insert_update(?,?,?,?,?,?,?)";
const char* qSELECT_OSHS_ITEM = "SELECT * FROM v1.oshs_item()";
const char* qSELECT_OSHS_VARID = "SELECT * FROM v1.oshs_select_varid(:VARID)";
const char* qSELECT_OSHS_ALL = "select id, fname, sname, id_icon, data, owner, id_parent, type, id_var from oshs_item, oshs_sub where id = id_item";
const char* qDELETE_OSHS_ITEM = "SELECT * FROM v1.oshs_delete(?,false)";


TOSHSItem*  OSHSService::getOSHS(AbstructTransferObject* p) {

    TOSHSItem* ret= NULL;

    QSqlDatabase conn = getConnection(CONN_NAME_OSHS,p);

    bool bOpen = false;
    if(!conn.isOpen()) {
        if(!conn.open()) {
            qWarning()<<"Error while open db";
            return NULL;
        }
        bOpen = true;
    }

    QMap<QUuid,TOSHSItem*> mTmp;
    QUuid id, parid;

    QUuid varid = QUuid::fromString(p->getParam("varid"));
    int deep = p->getParam("deep").toInt();
    QUuid itemid = QUuid::fromString(p->getParam("itemid"));

    TOSHSItem *newItem;
    TOSHSItem *root = NULL;

    QSqlQuery *q = new QSqlQuery(conn);
    q->prepare(qSELECT_OSHS_VARID);
    q->bindValue(":VARID", varid);
    if(q->exec()) {
        while(q->next()) {
            id = QUuid::fromString(q->value("id").toString());
            parid = (QUuid::fromString(q->value("id_parent").toString()));
            if(mTmp.contains(id)) {
                newItem = mTmp[id];
            } else {
                    newItem = new TOSHSItem();
                    mTmp[id] = newItem;
                   }
            // заполняем поля newItem
             newItem->setId(QUuid::fromString(q->value("id").toString()));
             newItem->setFname(q->value("fname").toString());
             newItem->setSname(q->value("sname").toString());
             newItem->setIdIcon(QUuid::fromString(q->value("id_icon").toString()));
             newItem->setData(QJsonDocument::fromJson(q->value("data").toString().toUtf8()).object());
             newItem->setOwner((OWNER_TYPE)(q->value("owner").toInt()));
             newItem->setSubtype((SUB_TYPE)(q->value("type").toInt()));

            // проверяем на корень
            if(parid.isNull()) {
                    root = newItem;
            } else {
                    parid = QUuid::fromString(q->value("id_parent").toString());
                    if(parid==id) {
                        root = newItem;
                    } else { //если не корневой элемент
                        TOSHSItem *par;
                        if(mTmp.contains(parid)){
                         par = mTmp[parid];
                        } else {
                           par = new TOSHSItem(parid); // ВАЖНО!!! Создаем родителя, заполним его на другой итерации когда будут данные
                           mTmp[parid] = par;
                          }
                        // привязка иерархии, если глубже чем задано, то не добавляем в контейнер
                        if ((deep == -1) || (par->getLevel() <= deep))
                        {
                            newItem->setParent(par);
                            par->addChild(newItem);
                        }
                       }
                    }
            }
    } else {
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            p->setReturnCode(INTERNAL_ERROR);
           }
    delete q;

    if(bOpen) { conn.close(); }

    QSqlDatabase::removeDatabase(CONN_NAME_OSHS);

    if (itemid != NULL) root = mTmp[itemid];

    ret = root;

    return ret;
}

TOSHSItem*  OSHSService::updOSHS     (AbstructTransferObject* p, bool upd) {}
RETURN_CODE OSHSService::delOSHS    (AbstructTransferObject* p) {};

TOSHSItem*  OSHSService::getOSHSTEST (AbstructTransferObject* p) {

        TOSHSItem* ret = new TOSHSItem(
                    QUuid::createUuid(),
                    "LVL 1:1",
                    QUuid::createUuid(),
                    stDIRECTLY,
                    owOUR
                    );

        TOSHSItem* lvl1_1 = new TOSHSItem(
                    QUuid::createUuid(),
                    "LVL 1:1",
                    QUuid::createUuid(),
                    stDIRECTLY,
                    owOUR
                    );
        ret->addChild(lvl1_1);

        TOSHSItem* lvl1_2 = new TOSHSItem(
                    QUuid::createUuid(),
                    "LVL 1:2",
                    QUuid::createUuid(),
                    stDIRECTLY,
                    owOUR
                    );
        ret->addChild(lvl1_2);

        TOSHSItem* lvl2_1 = new TOSHSItem(
                    QUuid::createUuid(),
                    "LVL 2:1",
                    QUuid::createUuid(),
                    stDIRECTLY,
                    owOUR
                    );
        lvl1_1->addChild(lvl2_1);

        TOSHSItem* lvl2_2 = new TOSHSItem(
                    QUuid::createUuid(),
                    "LVL 2:2",
                    QUuid::createUuid(),
                    stSUPPORT,
                    owOUR
                    );
        lvl1_1->addChild(lvl2_2);

        TOSHSItem* lvl2_3 = new TOSHSItem(
                    QUuid::createUuid(),
                    "LVL 2:3",
                    QUuid::createUuid(),
                    stDIRECTLY,
                    owOUR
                    );
        lvl1_2->addChild(lvl2_3);

        TOSHSItem* lvl2_4 = new TOSHSItem(
                    QUuid::createUuid(),
                    "LVL 2:4",
                    QUuid::createUuid(),
                    stATTACHED,
                    owOUR
                    );
        lvl1_2->addChild(lvl2_4);

    //    std::cout<<("TREE:\r\n"+ret->testPrint().toStdString())<<std::endl;

        return ret;
}
TOSHSItem*  OSHSService::getOSHSFAKE (AbstructTransferObject* p){
     TOSHSItem* ret = NULL;

     ret = new TOSHSItem(
                QUuid("10000000-1111-0000-1111-000000000000"),
                "66 рбр",
                QUuid::createUuid(),
                stDIRECTLY,
                owOUR
                );

    TOSHSItem* lvl1_1 = new TOSHSItem(
                QUuid("20000000-1111-0000-1111-000000000001"),
                "1 рдн",
                QUuid::createUuid(),
                stDIRECTLY,
                owOUR
                );
    ret->addChild(lvl1_1);

    TOSHSItem* lvl1_2 = new TOSHSItem(
                QUuid("20000000-1111-0000-1111-000000000002"),
                "2 рдн",
                QUuid::createUuid(),
                stDIRECTLY,
                owOUR
                );
    ret->addChild(lvl1_2);

    TOSHSItem* lvl1_3 = new TOSHSItem(
                QUuid("20000000-1111-0000-1111-000000000003"),
                "3 рдн",
                QUuid::createUuid(),
                stDIRECTLY,
                owOUR
                );
    ret->addChild(lvl1_3);
////////////////////////////////////////////////////////////////////////

    TOSHSItem* lvl2_1 = new TOSHSItem(
                QUuid("30000000-1111-0000-1111-000000000001"),
                "1 спу",
                QUuid::createUuid(),
                stDIRECTLY,
                owOUR
                );
    lvl1_1->addChild(lvl2_1);

    TOSHSItem* lvl2_2 = new TOSHSItem(
                QUuid("30000000-1111-0000-1111-000000000002"),
                "2 спу",
                QUuid::createUuid(),
                stSUPPORT,
                owOUR
                );
    lvl1_1->addChild(lvl2_2);

    TOSHSItem* lvl2_3 = new TOSHSItem(
                QUuid("30000000-1111-0000-1111-000000000003"),
                "3 спу",
                QUuid::createUuid(),
                stDIRECTLY,
                owOUR
                );
    lvl1_1->addChild(lvl2_3);

    TOSHSItem* lvl2_4 = new TOSHSItem(
                QUuid("30000000-1111-0000-1111-000000000004"),
                "4 спу",
                QUuid::createUuid(),
                stATTACHED,
                owOUR
                );
    lvl1_1->addChild(lvl2_4);

    ////////////////////////////////////////////////////////////////////////

        TOSHSItem* lvl2_5 = new TOSHSItem(
                    QUuid("30000000-1111-0000-1111-000000000005"),
                    "5 спу",
                    QUuid::createUuid(),
                    stDIRECTLY,
                    owOUR
                    );
        lvl1_2->addChild(lvl2_5);

        TOSHSItem* lvl2_6 = new TOSHSItem(
                    QUuid("30000000-1111-0000-1111-000000000006"),
                    "6 спу",
                    QUuid::createUuid(),
                    stSUPPORT,
                    owOUR
                    );
        lvl1_2->addChild(lvl2_6);

        TOSHSItem* lvl2_7 = new TOSHSItem(
                    QUuid("30000000-1111-0000-1111-000000000007"),
                    "7 спу",
                    QUuid::createUuid(),
                    stDIRECTLY,
                    owOUR
                    );
        lvl1_2->addChild(lvl2_7);

        TOSHSItem* lvl2_8 = new TOSHSItem(
                    QUuid("30000000-1111-0000-1111-000000000008"),
                    "8 спу",
                    QUuid::createUuid(),
                    stATTACHED,
                    owOUR
                    );
        lvl1_2->addChild(lvl2_8);
        ////////////////////////////////////////////////////////////////////////

            TOSHSItem* lvl2_9 = new TOSHSItem(
                        QUuid("30000000-1111-0000-1111-000000000009"),
                        "9 спу",
                        QUuid::createUuid(),
                        stDIRECTLY,
                        owOUR
                        );
            lvl1_3->addChild(lvl2_9);

            TOSHSItem* lvl2_10 = new TOSHSItem(
                        QUuid("30000000-1111-0000-1111-00000000000A"),
                        "10 спу",
                        QUuid::createUuid(),
                        stSUPPORT,
                        owOUR
                        );
            lvl1_3->addChild(lvl2_10);

            TOSHSItem* lvl2_11 = new TOSHSItem(
                        QUuid("30000000-1111-0000-1111-00000000000B"),
                        "11 спу",
                        QUuid::createUuid(),
                        stDIRECTLY,
                        owOUR
                        );
            lvl1_3->addChild(lvl2_11);

            TOSHSItem* lvl2_12 = new TOSHSItem(
                        QUuid("30000000-1111-0000-1111-00000000000C"),
                        "12 спу",
                        QUuid::createUuid(),
                        stATTACHED,
                        owOUR
                        );
            lvl1_3->addChild(lvl2_12);
    return ret;
}
