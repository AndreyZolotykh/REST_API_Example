#pragma once

#include <Poco/Mutex.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Path.h>
#include <Poco/ScopedLock.h>
#include <Poco/StringTokenizer.h>
#include <Poco/URI.h>
#include <Poco/Util/ServerApplication.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <map>          // std::ifstream
#include <QString>
#include "../commonheaders/enums.h"
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

/**
 * @brief The ServiceHandler class
 */
class ServiceHandler : public HTTPRequestHandler{
private:
    uint64_t _tt;
    uint64_t currentMillisec();
    REQUEST_TYPE getMethod(const QString& str) const;
    QString readBody(istream& in);
public:
    ServiceHandler();
    ~ServiceHandler();
    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp);
};
/**
 * @brief The FileHandler class
 */
class FileHandler : public HTTPRequestHandler{
private:
    typedef std::map<const std::string, const std::string> TStrStrMap;
    TStrStrMap CONTENT_TYPE = {
        #include "../commonheaders/mimetypes.h"
    };
    string getPath(string& path);
    string getContentType(string& path);
public:
    FileHandler();
    ~FileHandler();
    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp);
};
/**
 * @brief The RequestHandlerFactory class
 */
class RequestHandlerFactory : public HTTPRequestHandlerFactory{
public:
    virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest & request);
};
/**
    Сервер
*/
class RestServerApp : public ServerApplication{
public:
    void setPWD(const QString& pwd);
    const QString& getPWD() const;
private:
    QString _pwd;
protected:
    int main(const vector<string> &);    
};
