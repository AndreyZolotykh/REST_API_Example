#include "speedtest.h"
#include <QDateTime>
#include <QDebug>

#ifdef QT_DEBUG
SpeedTest* SpeedTest::_inst = new SpeedTest() ;
#else
SpeedTest* SpeedTest::_inst = NULL;
#endif
SpeedTest::SpeedTest(){
    _tt = QDateTime::currentMSecsSinceEpoch();
    //qDebug()<<"Speed test begin";
}
SpeedTest::~SpeedTest(){
    qint64 tmp = (QDateTime::currentMSecsSinceEpoch()-_tt);
    qDebug()<<"Processing "<<QString::number(tmp)<<" ms";
}
