#include "marchservice.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonDocument>

#include "../dbconnection.h"

const char* MarchService::CONN_NAME = "MarchService";
const char* MarchService::PARAM_VARID = "varid";
const char* MarchService::PARAM_ID = "id";

// query for delete march
const char* MarchService::qDELETE = "SELECT * FROM v1.march_delete(:ID,:VARID,:ISALL)";
const char* MarchService::qSELECT = "SELECT * FROM v1.march_select(:ID)";
const char* MarchService::qSELECT_FULL = "SELECT * FROM v1.march_select_full(:ID)";
const char* MarchService::qSELECT_PARAM_DIC = "SELECT * FROM v1.march_select_dic_param()";
const char* MarchService::qINSERT_UPD = "SELECT * FROM v1.march_insert_update(:ID,:VARID,:TITLE,:STATUS,:ERRCODE,:MAINDEPID,:DASTARTID,:DAFINISHID,:CONTENT,:UPD)";

const char* MarchService::qPARAM_ID = ":ID";
const char* MarchService::qPARAM_VARID = ":VARID";
const char* MarchService::qPARAM_TITLE = ":TITLE";
const char* MarchService::qPARAM_STATUS = ":STATUS";
const char* MarchService::qPARAM_ERRCODE = ":ERRCODE";
const char* MarchService::qPARAM_MAINDEPID = ":MAINDEPID";
const char* MarchService::qPARAM_DASTARTID = ":DASTARTID";
const char* MarchService::qPARAM_DAFINISHID = ":DAFINISHID";
const char* MarchService::qPARAM_CONTENT = ":CONTENT";
const char* MarchService::qPARAM_UPD = ":UPD";
const char* MarchService::qPARAM_ISALL = ":ISALL";



const char* MarchService::MARCH_PARAM_FIRST_HALF_REST = "10001000-0000-2121-0000-000000000001";
const char* MarchService::MARCH_PARAM_SECOND_HALF_REST = "10001000-0000-2121-0000-000000000002";
const char* MarchService::MARCH_PARAM_DAY_REST_PERIOD = "10001000-0000-2121-0000-000000000003";
const char* MarchService::MARCH_PARAM_TIME_BETWEEN_ESHELONS = "10001000-0000-2121-0000-000000000004";
const char* MarchService::MARCH_PARAM_DIST_BETWEEN_UNIT = "10001000-0000-2121-0001-000000000001";
const char* MarchService::MARCH_PARAM_DIST_BETWEEN_COL = "10001000-0000-2121-0001-000000000002";
const char* MarchService::MARCH_PARAM_DIST_BETWEEN_BAT = "10001000-0000-2121-0001-000000000003";
const char* MarchService::MARCH_PARAM_DIST_BETWEEN_BATR = "10001000-0000-2121-0001-000000000004";
const char* MarchService::MARCH_PARAM_SPEED = "10001000-0000-2121-0002-000000000001";
const char* MarchService::MARCH_PARAM_SPEED_2_SL = "10001000-0000-2121-0002-000000000002";
const char* MarchService::MARCH_PARAM_SPEED_2_DA = "10001000-0000-2121-0002-000000000003";
// -----------------------------------------------------------------------------------
RETURN_CODE MarchService::getMarchs(TMarchItemList* lsMarch, const QMap<QString, QString> &mp, const QUuid& id, bool isFullMode){
    RETURN_CODE sts = OK;

    {
        QSqlDatabase conn = getConnection(CONN_NAME,mp);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return INTERNAL_ERROR;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);
        if(isFullMode){
            q->prepare(qSELECT_FULL);
        }else{
            q->prepare(qSELECT);
        }
        q->bindValue(qPARAM_ID,id.toString());


        if(q->exec()){
            while(q->next()){
                TMarch* newItem = new TMarch();
                if(isFullMode){
                    newItem->fromJson(QJsonDocument::fromJson(q->value(TMarch::getDBField_content()).toString().toUtf8()).object());
                }

                newItem->setId(QUuid::fromString(q->value(TMarch::getDBField_id()).toString()));
                newItem->setVarId(QUuid::fromString(q->value(TMarch::getDBField_varid()).toString()));
                newItem->setTitle(q->value(TMarch::getDBField_title()).toString());
                newItem->setStatus((TMarch::STATUS)(q->value(TMarch::getDBField_status()).toDouble()));
                newItem->setErrCode((TMarch::ERR_CODE)(q->value(TMarch::getDBField_errCode()).toDouble()));
                //newItem->setErrDescr((TMarch::STATUS)(q->value(TMarch::getDBField_status()).toDouble()));
                newItem->setMainDepId(QUuid::fromString(q->value(TMarch::getDBField_mainDepId()).toString()));
                newItem->setDeployAreaStartId(QUuid::fromString(q->value(TMarch::getDBField_deployAreaStartId()).toString()));
                newItem->setDeployAreaFinishId(QUuid::fromString(q->value(TMarch::getDBField_deployAreaFinishId()).toString()));

                lsMarch->add(newItem);
            }
        }else{
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            sts = INTERNAL_ERROR;
        }
        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    return sts;
}

TMarchItemList* MarchService::getMarchs(AbstructTransferObject* p){
    TMarchItemList* ret = new TMarchItemList();

    QUuid id;
    bool isFullMode = false;
    RETURN_CODE sts = OK;
    if(p->isParam(PARAM_VARID)){
        id = QUuid::fromString(p->getParam(PARAM_VARID));
        if(p->isParam(PARAM_ID)){
            id = QUuid::fromString(p->getParam(PARAM_ID));
            isFullMode = true;
        }
    }else{
        sts = WRONG_REQUEST;
    }
#ifdef QT_DEBUG
    qDebug()<<"id = "<<id.toString()+" fullMode = "<<isFullMode;
#endif
    if(sts==OK){
        QMap<QString,QString> mp;
        conv(p, mp);
        sts = getMarchs(ret, mp, id, isFullMode);

//        {
//            QSqlDatabase conn = getConnection(CONN_NAME,p);

//            bool bOpen = false;
//            if(!conn.isOpen()){
//                if(!conn.open()){
//                    qWarning()<<"Error while open db";
//                    return ret;
//                }
//                bOpen = true;
//            }

//            QSqlQuery *q = new QSqlQuery(conn);
//            if(isFullMode){
//                q->prepare(qSELECT_FULL);
//            }else{
//                q->prepare(qSELECT);
////#ifdef QT_DEBUG
////                qDebug()<<qSELECT;
////#endif
//            }
//            q->bindValue(":ID",id.toString());


//            if(q->exec()){
//                while(q->next()){
//                    TMarch* newItem = new TMarch();
//                    if(isFullMode){
//                        newItem->fromJson(QJsonDocument::fromJson(q->value(TMarch::getDBField_content()).toString().toUtf8()).object());
//                    }

//                    newItem->setId(QUuid::fromString(q->value(TMarch::getDBField_id()).toString()));
//                    newItem->setVarId(QUuid::fromString(q->value(TMarch::getDBField_varid()).toString()));
//                    newItem->setTitle(q->value(TMarch::getDBField_title()).toString());
//                    newItem->setStatus((TMarch::STATUS)(q->value(TMarch::getDBField_status()).toDouble()));
//                    newItem->setErrCode((quint32)(q->value(TMarch::getDBField_errCode()).toDouble()));
//                    //newItem->setErrDescr((TMarch::STATUS)(q->value(TMarch::getDBField_status()).toDouble()));
//                    newItem->setMainDepId(QUuid::fromString(q->value(TMarch::getDBField_mainDepId()).toString()));
//                    newItem->setDeployAreaStartId(QUuid::fromString(q->value(TMarch::getDBField_deployAreaStartId()).toString()));
//                    newItem->setDeployAreaFinishId(QUuid::fromString(q->value(TMarch::getDBField_deployAreaFinishId()).toString()));

//                    ret->add(newItem);
//                }
//            }else{
//                qWarning()<<"Error while exec query: "<<q->lastError().text();
//                p->setReturnCode(INTERNAL_ERROR);
//            }
//            delete q;
//            if(bOpen){conn.close();}

//        }
//        QSqlDatabase::removeDatabase(CONN_NAME);
    }
    p->setReturnCode(sts);

    return ret;
}
TMarchParamDic* MarchService::getMarchParamDic(AbstructTransferObject* p){
    TMarchParamDic* ret = new TMarchParamDic();


    {
        QSqlDatabase conn = getConnection(CONN_NAME,p);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return ret;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);
        q->prepare(qSELECT_PARAM_DIC);

        if(q->exec()){
            while(q->next()){
                TMarchParam* newItem = new TMarchParam();

                newItem->setId(QUuid::fromString(q->value(TMarchParam::getDBField_id()).toString()));
                newItem->setTitle(q->value(TMarchParam::getDBField_title()).toString());
                newItem->setValue((q->value(TMarchParam::getDBField_value()).toDouble()));
                // newItem->setPathNum(-1);

                ret->add(newItem);
            }
        }else{
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            p->setReturnCode(INTERNAL_ERROR);
        }
        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    p->setReturnCode(OK);



    return ret;
}
RETURN_CODE MarchService::updMarch(TMarch* item, const QMap<QString,QString>& mp, bool upd, bool isNew){
    RETURN_CODE ret = OK;
    {
        QSqlDatabase conn = getConnection(CONN_NAME,mp);

        bool bOpen = false;
        if(!conn.isOpen()){
            if(!conn.open()){
                qWarning()<<"Error while open db";
                return INTERNAL_ERROR;
            }
            bOpen = true;
        }

        QSqlQuery *q = new QSqlQuery(conn);

        q->prepare(qINSERT_UPD);

        q->bindValue(qPARAM_ID,item->getId().toString());
        q->bindValue(qPARAM_VARID,item->getVarId().toString());
        q->bindValue(qPARAM_TITLE,item->getTitle());
        q->bindValue(qPARAM_STATUS,isNew?TMarch::NEW:item->getStatus());
        q->bindValue(qPARAM_ERRCODE,upd?item->getErrCode():0);
        q->bindValue(qPARAM_MAINDEPID,item->getMainDepId().toString());
        q->bindValue(qPARAM_DASTARTID,item->getDeployAreaStartId().toString());
        q->bindValue(qPARAM_DAFINISHID,item->getDeployAreaFinishId().toString());
        q->bindValue(qPARAM_CONTENT, QString(QJsonDocument(item->toJson()).toJson(QJsonDocument::Compact)) );
        q->bindValue(qPARAM_UPD,upd);

        if(!q->exec()){
            qWarning()<<"Error while exec query: "<<q->lastError().text();
            ret= INTERNAL_ERROR;
            item->setStatus(TMarch::ERR);
        }else{
            item->setStatus(TMarch::NEW);
        }
        delete q;
        if(bOpen){conn.close();}

    }
    QSqlDatabase::removeDatabase(CONN_NAME);

    return ret;
}

TMarch* MarchService::updMarch(AbstructTransferObject* p, bool upd){
    TMarch *ret = new TMarch();


    QJsonDocument doc = QJsonDocument::fromJson(p->getInputJSON().toUtf8());

    QJsonObject mainObj = doc.object();
//    ret->fromJson(mainObj);
    if(ret->fromJson(mainObj)){
//        {
//        if(!upd){
//            ret->setId(QUuid::createUuid());
//        }
        QMap<QString,QString> mp;
        conv(p, mp);
        RETURN_CODE sts = updMarch(ret,mp, upd, true);
        p->setReturnCode(sts);



//            QSqlDatabase conn = getConnection(CONN_NAME,p);

//            bool bOpen = false;
//            if(!conn.isOpen()){
//                if(!conn.open()){
//                    qWarning()<<"Error while open db";
//                    return ret;
//                }
//                bOpen = true;
//            }

//            QSqlQuery *q = new QSqlQuery(conn);

//            q->prepare(qINSERT_UPD);

//            q->bindValue(qPARAM_ID,ret->getId().toString());
//            q->bindValue(qPARAM_VARID,ret->getVarId().toString());
//            q->bindValue(qPARAM_TITLE,ret->getTitle());
//            q->bindValue(qPARAM_STATUS,TMarch::NEW);
//            q->bindValue(qPARAM_ERRCODE,0);
//            q->bindValue(qPARAM_MAINDEPID,ret->getMainDepId().toString());
//            q->bindValue(qPARAM_DASTARTID,ret->getDeployAreaStartId().toString());
//            q->bindValue(qPARAM_DAFINISHID,ret->getDeployAreaFinishId().toString());
//            q->bindValue(qPARAM_CONTENT,p->getInputJSON());
//            q->bindValue(qPARAM_UPD,upd);

//            if(!q->exec()){
//                qWarning()<<"Error while exec query: "<<q->lastError().text();
//                p->setReturnCode(INTERNAL_ERROR);
//            }
//            delete q;
//            if(bOpen){conn.close();}

//        }
//        QSqlDatabase::removeDatabase(CONN_NAME);
    }else{
        qWarning()<<"Error while parse json";
        p->setReturnCode(ERR_VERIFY_DATA);
    }


    return ret;
}

RETURN_CODE MarchService::delMarch(AbstructTransferObject* p){
    RETURN_CODE ret = OK;


    QUuid id, varid;

    bool isAll = false;
    if(p->isParam(PARAM_VARID)){
        varid = QUuid::fromString(p->getParam(PARAM_VARID));
        if(p->isParam(PARAM_ID)){
            id = QUuid::fromString(p->getParam(PARAM_ID));
        }else{
            isAll = true;
        }
    }else{
        ret = WRONG_REQUEST;
    }

    if(ret==OK){
        {
            QSqlDatabase conn = getConnection(CONN_NAME,p);

            bool bOpen = false;
            if(!conn.isOpen()){
                if(!conn.open()){
                    qWarning()<<"Error while open db";
                    return ret;
                }
                bOpen = true;
            }

            QSqlQuery *q = new QSqlQuery(conn);

            q->prepare(qDELETE);

            q->bindValue(qPARAM_ID,id.toString());
            q->bindValue(qPARAM_VARID,varid.toString());
            q->bindValue(qPARAM_ISALL,isAll);


//            ret= checkExecQuery(q->exec(),p,q->lastError().text());
            if(!q->exec()){
                qWarning()<<"Error while exec query: "<<q->lastError().text();
                p->setReturnCode(INTERNAL_ERROR);
                ret = INTERNAL_ERROR;
            }
            delete q;
            if(bOpen){conn.close();}

        }
        QSqlDatabase::removeDatabase(CONN_NAME);

    }


    return ret;
}

RETURN_CODE MarchService::checkExecQuery(bool res,AbstructTransferObject* p,const QString& mes){
    RETURN_CODE ret = OK;
    if(!res){
        qWarning()<<"Error while exec query: "<<mes;
        p->setReturnCode(INTERNAL_ERROR);
        p->setContent("{\"Code\": -1013, \"Message\":\""+mes+"\"}");
        ret = INTERNAL_ERROR;
    }
    return ret;
}


RETURN_CODE MarchService::solveMarch(AbstructTransferObject* p){
    RETURN_CODE ret = OK;

    // parse param
    QUuid id,varid;
    if(p->isParam(PARAM_VARID) && p->isParam(PARAM_ID)){
//        qDebug()<<"XZ-1";
        id = QUuid::fromString(p->getParam(PARAM_ID));
//        qDebug()<<"XZ-2";
        varid = QUuid::fromString(p->getParam(PARAM_VARID));
//        qDebug()<<"XZ-3";
    }else{
        ret = WRONG_REQUEST;
    }
//    qDebug()<<"XZ-4: "<<QString::number(ret);
    // load march by id from db
    if(ret==OK){
        QMap<QString,QString> mp;
        conv(p, mp);
        TMarchItemList* ls = new TMarchItemList();
        ret = getMarchs(ls, mp, id, true);
        // try solve
        if(ret==OK){
            if(!ls->isEmpty()){
                QList<TMarch*>::iterator iter = ls->begin();
                TMarch* item = *iter;
                if(item!=NULL){
                    solve( item );
                    updMarch(item,mp,true, false);
                }else{
                    ret = INTERNAL_ERROR;
                    qWarning()<<"Error get march with id "<<id.toString();
                }
            }else{
                ret = NOT_FOUND;
                qWarning()<<"March with id "<<id.toString()<<" not found";
            }
        }
        delete ls;
    }


    return ret;
}



// Solve -------------------------------------------------------------------------------------
double MarchService::getParam(TMarch* item,const QUuid& pathid, const QUuid& paramid){
    double ret = -1;
    const TMarchParam* p = item->getParam(pathid,paramid);
    if(p==NULL){
        ret = _mapParamDefault[paramid];
    }else{
        ret = p->getValue();
    }
    return ret;
}

void MarchService::calcDistDeps(TMarchResultDep* dep,double predist){
    const double unitLen = 10.0;// длина машины
    int unitCt = 60;
    dep->setPreDist(predist);
    dep->setUnitCt(unitCt);
    dep->setUnitsLength(unitLen*unitCt+dep->getDistBetweenUnit()*(unitCt-1));
}

QString MarchService::createKey(const TMarchPath* d,const TMarchPathItem* st) const{
    return d->getPathId().toString()+"_"+st->getKey();
}
QString MarchService::createKey(const TMarchResultDep* d,const TMarchResultMilestone* st) const{
    return d->getPathId().toString()+"_"+st->getKey();
}
void MarchService::solve(TMarch* item){
    const double SPEED_IN_MS = 3.6;
    const double TO_DAYS = 24*60*60;
    const double TO_HOUR = 60*60;
#ifdef QT_DEBUG
    qDebug()<<"Begin solve march with id "<<item->getId().toString()<<" errcode="<<QString::number(item->getErrCode());
    item->printMapPram();
#endif

#ifdef TEST_RESULT
    solveTest(item);
    return;
#endif

    TMarchResult *res = new TMarchResult();

    QMap<QString,long> mTimeOffsetByPath; // смещение времени на маршруте для не головного формирования
    QMap<QString,long> mMaxTimeByItem; // максимальные значения времени на рубежах
    QMap<QString,long> mDeltaByPath; // дельта на маршруте
    QMap<QString,bool> mFirstByItem; // признак
    long maxTime = 0;
    long restTime = 0;
    foreach (TMarchPath *it, item->getPaths()) {
        long pathLength = 0;
        TMarchResultDep* newDep = new TMarchResultDep(it->getId(),it->getPathId());
        newDep->setDistBetweenUnit( getParam(item,it->getPathId(),QUuid(MARCH_PARAM_DIST_BETWEEN_UNIT)) );
        newDep->setOrder(it->getOrder());
        calcDistDeps(newDep,getParam(item,it->getPathId(),QUuid(MARCH_PARAM_DIST_BETWEEN_COL)));


        double speed2SL = getParam(item,it->getPathId(),QUuid(MARCH_PARAM_SPEED_2_SL));
        double speed2DA = getParam(item,it->getPathId(),QUuid(MARCH_PARAM_SPEED_2_DA));
        double speedPath = getParam(item,it->getPathId(),QUuid(MARCH_PARAM_SPEED));

        newDep->setSpeed2SL(speed2SL);
        newDep->setSpeed2DA(speed2DA);
        newDep->setSpeedPath(speedPath);

        long t2SL = 0; // время до ИР
        long t2DA = 0; // время до РР
        long lastRl = 0; // время прохождения последнего РР хвостом
        long localTime = 0; // время движения
        foreach (TMarchPathItem *itt, it->getItems()) {
            TMarchResultMilestone *newMs = new TMarchResultMilestone();

            QString key = createKey(it,itt);
            long offset = 0;
            double preDist = 0;
            if(mTimeOffsetByPath.contains(key)){
                offset = mTimeOffsetByPath[key];
                preDist = newDep->getPreDist();
            }
#ifdef QT_DEBUG
            qDebug()<<key<<", "<<QString::number(offset)<<" - "<<QString::number(preDist);
#endif

            double localSpeed = 0; // скорость на участке
            if(itt->getType()==TMarchPathItem::TO_SL){
                localSpeed = speed2SL/SPEED_IN_MS;

                newDep->setLen2SL(itt->getDist());

                newMs->setType(TMarchResultMilestone::SL);
                newMs->setIntHead(0L+offset+preDist/localSpeed);
                t2SL = newDep->getLen2SL()/localSpeed;

                pathLength+=itt->getDist();
            }else if(itt->getType()==TMarchPathItem::TO_DA){
                localSpeed = speed2DA/SPEED_IN_MS;

                newDep->setLen2DA(itt->getDist());
                t2DA = newDep->getLen2DA()/localSpeed+offset+preDist/localSpeed;

                pathLength+=itt->getDist();
                delete newMs;
                continue;
            }else if(itt->getType()==TMarchPathItem::TO_RL){
                localSpeed = speedPath/SPEED_IN_MS;

                newMs->setType(TMarchResultMilestone::RL);
                newMs->setNum(itt->getNum());
                localTime+=itt->getDist()/localSpeed;
                newMs->setIntHead(localTime+offset+preDist/localSpeed);

                pathLength+=itt->getDist();
            }else{
                newMs->setType(TMarchResultMilestone::RA);
                newMs->setNum(itt->getNum());
                // в зависимости от времени выбираем время отдых
                long localRestTime = 0;
                if((localTime%((long)TO_DAYS))<((long)TO_DAYS)/2){ // первая половина суточного перехода
                    localRestTime = getParam(item,QUuid(),QUuid(MARCH_PARAM_FIRST_HALF_REST))*TO_HOUR;
                }else {
                    localRestTime = getParam(item,QUuid(),QUuid(MARCH_PARAM_SECOND_HALF_REST))*TO_HOUR;
                }
                localTime+=localRestTime;
                newMs->setRestTime(QDateTime::fromSecsSinceEpoch(localRestTime));

            }


            newMs->setIntTail(newMs->getIntHead()+newDep->getUnitLength()/localSpeed);
            if(itt->getType()==TMarchPathItem::TO_RL){
                lastRl = newMs->getIntTail();
            }

            newDep->addMilestone(newMs);

            offset = newMs->getIntTail()-newMs->getIntHead();
            mTimeOffsetByPath[key] = offset;

            // первый проход элемента
            if(!mFirstByItem.contains(key)){
                if(mMaxTimeByItem.contains(newMs->getKey())){
                    if(mMaxTimeByItem[newMs->getKey()]<newMs->getIntHead()){
                        mMaxTimeByItem[newMs->getKey()] = newMs->getIntHead();
                    }
                }else{
                    mMaxTimeByItem[newMs->getKey()] = newMs->getIntHead();
                }
                mFirstByItem[key] = false;
            }
        }

        newDep->setIntBegin(-t2SL);
        newDep->setIntEnd(lastRl+t2DA);
        newDep->setPathLength(pathLength);

        if(maxTime < (newDep->getIntEnd()-newDep->getIntBegin())){
            maxTime = (newDep->getIntEnd()-newDep->getIntBegin());
        }

        res->addDeps(newDep);
    }
    res->setDays(maxTime/TO_DAYS);
    res->setRestCt(restTime/TO_DAYS);
#ifdef QT_DEBUG
    bool isFirst = true;
#endif

    // нормализация
    long offset = 0;
    QDateTime realDate;
    if(item->getMode()==TMarch::MODE_TO){
        realDate = item->getEnd();
    }else{
        realDate = item->getBegin();
    }
    foreach (TMarchResultDep *it, res->getDeps()) {
        if(item->getMode()==TMarch::MODE_TO){
            offset = it->getIntEnd();
        }else{
            offset = 0;
        }
#ifdef QT_DEBUG
        QString info = it->getDepid().toString().replace("{","").replace("}","").replace("-","");
#endif

        double delta = 0;
        foreach (TMarchResultMilestone *itt, it->getMilestones()) {
            QString key = createKey(it,itt);
            if(mDeltaByPath.contains(key)){
                delta = mDeltaByPath[key];
            }else{
                delta = mMaxTimeByItem[itt->getKey()]-itt->getIntHead();
                mDeltaByPath[key] = delta;
            }
            // выравнимваем рубежи
            itt->addIntHead(delta);
            itt->addIntTail(delta);

            // коррекция при обратной схеме
            itt->addIntHead(-offset);
            itt->addIntTail(-offset);

            // формируем от заданной даты
            itt->setHead(realDate.addSecs(itt->getIntHead()));
            itt->setTail(realDate.addSecs(itt->getIntTail()));

#ifdef QT_DEBUG
            info+= " | "+itt->getHead().toString("yyyy-MM-dd hh:mm")+" - "+itt->getTail().toString("yyyy-MM-dd hh:mm");
            if(isFirst){
                qDebug()<<itt->getKey()<<" - "<<realDate.addSecs(mMaxTimeByItem[itt->getKey()]-offset).toString("yyyy-MM-dd hh:mm");
            }
#endif
        }

        // двигаем конечную точку
        it->addIntEnd(delta);

        // коррекция при обратной схеме
        it->addIntBegin(-offset);
        it->addIntEnd(-offset);

        // формируем от заданной даты
        it->setBegin(realDate.addSecs(it->getIntBegin()));
        it->setEnd(realDate.addSecs(it->getIntEnd()));

#ifdef QT_DEBUG
        if(isFirst){
            qDebug()<<"----------------------";
        }
        qDebug()<<info;
        isFirst = false;
#endif
    }


    item->setResult(res);
    item->setStatus(TMarch::SOLVED);
}


#ifdef TEST_RESULT
void MarchService::solveTest(TMarch* item){
    TMarchResult *res = new TMarchResult();

    res->setDays(3);
    res->setRestCt(0);


    int ct = 0;
    int ctc = 0;
    foreach (TMarchPath *it, item->getPaths()) {
//        it->sortItems();
        qDebug()<<"Dep: "<<it->getId().toString();
        TMarchResultDep* newDeps = new TMarchResultDep();
        newDeps->setPathId( it->getPathId() );
        newDeps->setUnitsLength(700.0+ct*100);
        newDeps->setUnitCt(40+ct*5);
        newDeps->setOrder(it->getOrder());
        newDeps->setDepId(it->getId());
        newDeps->setBegin(item->getBegin().addSecs(-ct*6000-600));
        newDeps->setEnd(newDeps->getBegin().addDays(3));
        ctc = 0;
        QDateTime tmp_tt;
        foreach (TMarchPathItem *itt, it->getItems()) {
#ifdef QT_DEBUG
            qDebug()<<"\t- "<<itt->toString();
#endif
            TMarchResultMilestone *newMs = new TMarchResultMilestone();
            if(itt->getType()==TMarchPathItem::TO_SL){
                newDeps->setLen2SL(itt->getDist());

                newMs->setType(TMarchResultMilestone::SL);
                newMs->setHead(item->getBegin().addSecs(ct*12000));
                tmp_tt = newMs->getHead();
            }else if(itt->getType()==TMarchPathItem::TO_DA){
                newDeps->setLen2DA(itt->getDist());
                delete newMs;
                continue;
            }else if(itt->getType()==TMarchPathItem::TO_RL){
                newMs->setType(TMarchResultMilestone::RL);
                newMs->setNum(itt->getNum());
                newMs->setHead(tmp_tt.addDays(ctc));
            }else{
                newMs->setType(TMarchResultMilestone::RA);
                newMs->setNum(itt->getNum());
            }

            newMs->setTail(newMs->getHead().addSecs(16000));



            newDeps->addMilestone(newMs);
            ctc++;
        }

        res->addDeps(newDeps);
        ct++;
    }

    item->setResult(res);
    item->setStatus(TMarch::SOLVED);
}
#endif
// Common code -----------------------------------------------------------------------------------
MarchService* MarchService::_inatance = NULL;
MarchService::Ctrl MarchService::_ctrl;

MarchService::Ctrl::Ctrl(){

}
MarchService::Ctrl::~Ctrl(){
    if(_inatance!=NULL){
        delete _inatance;
    }
}
MarchService::MarchService(){
    // TODO: rewrite - load from DB
    _mapParamDefault[QUuid(MARCH_PARAM_FIRST_HALF_REST)] = 1;
    _mapParamDefault[QUuid(MARCH_PARAM_SECOND_HALF_REST)] = 2;
    _mapParamDefault[QUuid(MARCH_PARAM_DAY_REST_PERIOD)] = 3;
    _mapParamDefault[QUuid(MARCH_PARAM_TIME_BETWEEN_ESHELONS)] = 24;
    _mapParamDefault[QUuid(MARCH_PARAM_DIST_BETWEEN_UNIT)] = 50;
    _mapParamDefault[QUuid(MARCH_PARAM_DIST_BETWEEN_COL)] = 5000;
    _mapParamDefault[QUuid(MARCH_PARAM_DIST_BETWEEN_BAT)] = 3000;
    _mapParamDefault[QUuid(MARCH_PARAM_DIST_BETWEEN_BATR)] = 1000;
    _mapParamDefault[QUuid(MARCH_PARAM_SPEED)] = 25;
    _mapParamDefault[QUuid(MARCH_PARAM_SPEED_2_SL)] = 20;
    _mapParamDefault[QUuid(MARCH_PARAM_SPEED_2_DA)] = 20;
}
MarchService::~MarchService(){

}
MarchService *MarchService::getInstance(){
    if(_inatance==NULL){
        _inatance = new MarchService();
    }
    return _inatance;
}
