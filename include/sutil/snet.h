#ifndef SLIB_SNET_H
#define SLIB_SNET_H

// Require libcurl
#include "sobj/sobject.h"
#include "sobj/sstring.h"
#include "sobj/sdate.h"
#include "sobj/sdata.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"
#include "sio/stream.h"
//#include "sutil/scode.h"
#include "sutil/ssys.h"
extern "C" {
#include "curl/curl.h"
}

namespace slib {
    namespace sutil {
        extern SLIB_DLL Response download(const char* url, SObjPtr attribute = snull);
        extern SLIB_DLL Response callapi(const char* url, const SDictionary& query, const SDictionary& header = {});
        extern SLIB_DLL Response upload(const char* url, SObjPtr attribute = snull);
    }

    #define SNET_ERROR 0x0090
        
        #define HTTP_CONNECTION 0x01
        #define FTP_CONNECTION  0x02
    
        #define SSL_CONNECTION  0x10
    
        extern SDictionary STANDARD_HTTP;
        extern SDictionary STANDARD_HTTPS;
        //extern SDictionary STANDARD_FTP;

        extern const SDictionary DEFAULT_DOWNLOAD_ARGS;
        
#define CURL_ERR_TEXT(N, T) (u8"libcurl error #"+std::to_string(N)+u8": "+std::string(T)).c_str()

        class SLIB_DLL SNetException : public Exception {
        public:
			SNetException(const char* f, sint l, const char* func, sint e = 0, const char* target = nullptr, const char* note = nullptr);
            ~SNetException();
        };
        extern inline size_t writeCallback(void *buf, size_t size, size_t nmemb, void *ptr);
		//extern inline size_t writeToFileCallback(void* buf, size_t size, size_t nmemb, void* ptr);
		
        using SNetworkException = SNetException;

        

        class SLIB_DLL SNetWork {
            //struct curl_slist* _list;
            CURL* _curl;
        public:
            Response response;
        private:
            IOStream _stream;
        public:
            SNetWork();
            ~SNetWork();
            
            void http(const char *url, bool ssl = false, const char *cookie = nullptr, const char *post = nullptr);
            void ftp(const char *url, bool ssl = false, const char *info = nullptr);
            Response& connect(const char* url, SObjPtr attribute = snull);
            void init();
            void clear();
			//void upload(const char* path, const char* url);
            //void call(const char* path);
            //void download(const char* path, const char* url, const char *user = nullptr, const char *pswd = nullptr);
        };

}
#endif
