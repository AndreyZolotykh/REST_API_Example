#include <QCoreApplication>
#include <QFileInfo>
#include <QLibrary>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QScopedPointer>

#include "restapicfg.h"
#include "server.h"


//#define USE_FILE_LOG_ROTATION


#define LOGFILEPATH QString("/var/log/ncom/")
#define LOGFILENAME QString("ncom.log")
#define LOGFILE LOGFILEPATH+LOGFILENAME


QScopedPointer<QFile> m_logFile;
void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);    
    RestServerApp app;

#ifdef USE_FILE_LOG_ROTATION
    QFile *flog = new QFile(LOGFILE);
    if(flog->exists()){
        if(!flog->rename(LOGFILEPATH+"ncom."+QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss")+".log")){
            qWarning()<<"Error while rename";
        }
    }
    delete flog;
#endif

    m_logFile.reset(new QFile(LOGFILE));

    if(m_logFile.data()->open(QIODevice::WriteOnly | QIODevice::Append)){
        qInstallMessageHandler(logHandler);
    }

    app.setPWD(a.applicationDirPath());
    cout << a.applicationName().toStdString() << " starting..." << endl;

    return app.run(argc, argv);
}


void logHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    QTextStream ts(m_logFile.data());
    switch (type) {
    case QtInfoMsg: ts<<"INF ";break;
    case QtDebugMsg:
#ifdef QT_DEBUG
        ts<<"DBG ";
#else
        return;
#endif
        break;
    case QtWarningMsg: ts<<"WRN ";break;
    case QtCriticalMsg: ts<<"CRT ";break;
    case QtFatalMsg: ts<<"FTL ";break;
    }
    QString txt = QString("%1: %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(msg);

    ts<<context.category<<": "<<txt<<endl;
    ts.flush();
}
