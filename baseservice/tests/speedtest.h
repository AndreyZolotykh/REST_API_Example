#ifndef SPEEDTEST_H
#define SPEEDTEST_H

#include <QtCore/qglobal.h>

class SpeedTest{
    qint64 _tt;
    static SpeedTest *_inst;
public:
    SpeedTest();
    ~SpeedTest();
};

#endif // SPEEDTEST_H
