#include <QDebug>
#include <QTextStream>
#include <QLibrary>
#include <QDateTime>
#include <iostream>
#include <string>
#include <chrono>
#include "restapicfg.h"
#include "server.h"
#include "transferobject.h"


#define READ_BODY_FIX

//////////////////////////////////////////////////////////////////////////////
ServiceHandler::ServiceHandler(){
    _tt = currentMillisec();
    qDebug()<<"Init service handler";
}
ServiceHandler::~ServiceHandler(){
    qDebug()<<"Destroy service handler "<<QString::number( currentMillisec()-_tt )<<" ms";
}
uint64_t ServiceHandler::currentMillisec(){
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
REQUEST_TYPE ServiceHandler::getMethod(const QString& str) const{
    REQUEST_TYPE ret = UNDEF;
    if(str.compare("GET")==0){
        ret = GET;
    }else if(str.compare("POST")==0){
        ret = POST;
    }else if(str.compare("PUT")==0){
        ret = PUT;
    }else if(str.compare("DELETE")==0){
        ret = DELETE;
    }else if(str.compare("PATCH")==0){
        ret = PATCH;
    }else if(str.compare("OPTIONS")==0){
        ret = OPTIONS;
    }
    return ret;
}

const int BUF_SIZE = 4096;
QString ServiceHandler::readBody(istream& in){
    string str;
    char *buf = new char[BUF_SIZE];
    while(in.read(buf,BUF_SIZE)){
        str.append(buf,BUF_SIZE);
    }
    str.append(buf,in.gcount());
    delete [] buf;

    return QString::fromStdString(str);
}

void ServiceHandler::handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp){
    URI uri(req.getURI());

    QString method = QString::fromStdString(req.getMethod());
    QString url = QString::fromStdString(uri.toString());
    QString query = QString::fromStdString(uri.getQuery());


#ifdef QT_DEBUG
    qDebug() << "URI: " << url;
    qDebug() << "Method: " << method;
    qDebug() << "Query" << query;
#endif

    int idx = url.indexOf("?");
    if(idx>0){
        url = url.left(idx);
    }

    //HTMLForm form(req,req.stream());
    QString reason("");
    QString err;
    int code = 0;

    HTTPResponse::HTTPStatus status = HTTPResponse::HTTP_OK;
    QString answer;

    QString token = QString::fromStdString(req.get("Authorization","XZ"));
#ifdef QT_DEBUG
    qDebug()<<"Authorization: "<<token;
#endif
    const RestApiCfg *cfg = RestApiCfg::getInstance();

    const LibApi* api = cfg->find(url);
    if(api!=NULL){
        QLibrary *lib =  new QLibrary(api->getLibName());
//#ifdef QT_DEBUG
       qDebug()<<"Find service by url "<<url;
//#endif
        if(lib->load()){
//#ifdef QT_DEBUG
//        qDebug()<<"Try resolve func "<<api->getFunction().toUtf8().constData();
//#endif
            LibServiceV3 func = (LibServiceV3)lib->resolve(api->getFunction().toUtf8().constData());
            if(func){
                REQUEST_TYPE _method = getMethod(method);
                QString body = "";
#ifdef READ_BODY_FIX
                if(_method==POST||_method==PUT||_method==PATCH){
#endif
                    body=readBody(req.stream());
#ifdef READ_BODY_FIX
                }
#endif

#ifdef QT_DEBUG
                qDebug()<<"";
                qDebug()<<QDateTime::currentDateTime().toString();
                qDebug()<<"Input:\r\n"<<body; // REMOVE IT
#endif
//                if(form.has("text")){
//                    //body+=QString::fromStdString(form.get("text"));
//                }

                TransferObject *tobj = new TransferObject(_method,body,token,cfg->getDBConfig());
                AbstructTransferObject *obj = (AbstructTransferObject *)tobj;

                if(!query.isEmpty()){
                    QStringList ls = query.split("&");
                    foreach(QString it, ls){
                        QStringList tmp = it.split("=");
                        if(tmp.length()==2){
                            tobj->put(tmp.at(0),tmp.at(1));
                        }else{
#ifdef QT_DEBUG
                            qWarning()<<"Skip wrong argument: "<<it;
#endif
                        }
                    }
                }
                // call function
                try{
                    int ret = func(&obj);
                    if(ret==0){
                        answer = obj->getJSON();
                        if(obj->getReturnCode()!=OK){
                            status = (HTTPResponse::HTTPStatus)obj->getReturnCode();
                            code = status;
                        }
                    }else{
                        err = "Unsupported method "+api->getFunction()+" ret = "+QString::number(ret);
                        code = -1004;
                        reason = "Bad request";
#ifdef QT_DEBUG
                        qWarning()<<err;
#endif
                        status = HTTPResponse::HTTP_BAD_REQUEST;
                    }
                }catch(...){
#ifdef QT_DEBUG
                    qWarning()<<"Lib function crash";
#endif
                }
#ifdef QT_DEBUG
//                qDebug()<<"";
                 qDebug()<<"Output:\r\n"; // REMOVE IT
                qDebug()<<"Output:\r\n"<<obj->getJSON(); // REMOVE IT
#endif
                delete obj;
            }else{
                err = "Error while getting function "+api->getFunction();
                code = -1001;
                reason = "Server inetrnal error";
#ifdef QT_DEBUG
                qWarning()<<err;
#endif
                status = HTTPResponse::HTTP_INTERNAL_SERVER_ERROR;
            }
        }else{
            err = "Error while loading "+api->getLibName()+", "+lib->errorString();
            code = -1002;
            reason = "Server inetrnal error";
#ifdef QT_DEBUG
                qWarning()<<err;
#endif
            status = HTTPResponse::HTTP_INTERNAL_SERVER_ERROR;
        }
        delete lib;
    }else{
        err = "Service not found by url "+url;
        code = -1003;
        reason = "Service not found";
#ifdef QT_DEBUG
        qWarning()<<err;
#endif
        status = HTTPResponse::HTTP_NOT_FOUND;
    }

    if(status!=HTTPResponse::HTTP_OK && status!=HTTPResponse::HTTP_CREATED){
        answer = QString("{\"Code\": "+QString::number(code)+", \"Message\":\""+err+"\"}");
    }

    if(code!=0){
        resp.setStatusAndReason(status,reason.toStdString());
    }else{
        resp.setStatus(status);
    }
    resp.setContentType("application/json");
    resp.set("Access-Control-Allow-Origin","*");
    resp.set("Access-Control-Allow-Headers","Origin, X-Requested-With, Content-Type, Accept");
    resp.set("Access-Control-Allow-Methods","GET, PATCH, PUT, POST, DELETE, OPTIONS");

    ostream& out = resp.send();
    out<<answer.toStdString();

    out.flush();

}
////////////////////////////////////////////////////////////////////////////////
FileHandler::FileHandler(){
}
FileHandler::~FileHandler(){
}
string FileHandler::getPath(string& path){
    if(path == "/"){
        path="/index.html";
    }
    path.insert(0, "./www");
    return path;
}

string FileHandler::getContentType(string& path){
    string contentType("text/plain");
    Poco::Path p(path);

    TStrStrMap::const_iterator i=CONTENT_TYPE.find(p.getExtension());

    if (i != CONTENT_TYPE.end()){ /* Found, i->first is f, i->second is ++-- */
       contentType = i->second;
    }

    if(contentType.find("text/") != std::string::npos){
        contentType+="; charset=utf-8";
    }

    cerr << path << " : " << contentType << endl;

    return contentType;
}
void FileHandler::handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp){
    cerr << "Get static page: ";
    URI uri(req.getURI());
    string path(uri.getPath());

    ifstream ifs (getPath(path).c_str(), ifstream::in);

    ostream& out = resp.send();
    if(ifs){
        resp.setStatus(HTTPResponse::HTTP_OK);
        resp.setContentType(getContentType(path));

        char c = ifs.get();
        while (ifs.good()) {
            out << c;
            c = ifs.get();
        }
    }else{
        resp.setStatus(HTTPResponse::HTTP_NOT_FOUND);
        out << "File not found" << endl;
    }
    out.flush();
    ifs.close();
}
//////////////////////////////////////////////////////////////////////////////////////////
HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const HTTPServerRequest & request){
    const RestApiCfg* cfg = RestApiCfg::getInstance();
//    cerr<<request.getURI()<<endl;
    if (!request.getURI().find(cfg->getbasePath().toStdString())){
        return new ServiceHandler;
    }else{
        return new FileHandler;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void RestServerApp::setPWD(const QString& pwd){
    _pwd = pwd;
#ifdef QT_DEBUG
    qDebug()<<"Set PWD: "<<pwd;
#endif
}
const QString& RestServerApp::getPWD() const{
    return _pwd;
}
int RestServerApp::main(const vector<string> &){
    // Хранилище ссылок на внешние модули
    const RestApiCfg *rapicfg = RestApiCfg::getInstance(_pwd);

    HTTPServerParams* pParams = new HTTPServerParams;


    pParams->setMaxQueued(rapicfg->getMaxQueue());
    pParams->setMaxThreads(rapicfg->getMaxThreads());

    HTTPServer s(new RequestHandlerFactory, ServerSocket(rapicfg->port()), pParams);

    s.start();
    qInfo() << "Server started";
#ifdef QT_DEBUG
    qDebug()<<" with param: MaxThreads = "
            <<QString::number(pParams->getMaxThreads())
            <<", MaxQueue = "
            <<QString::number(pParams->getMaxQueued())
             <<  ", Host = " <<  rapicfg->host() << " , Port = " <<  rapicfg->port()
             << ", Version = " <<  rapicfg->getVersion() << ", BasePath" << rapicfg ->getbasePath();
#endif

    waitForTerminationRequest();  // wait for CTRL-C or kill

    qInfo() << "Shutting down...";
    s.stop();

    return Application::EXIT_OK;
}


