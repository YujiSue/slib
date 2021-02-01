#ifndef SLIB_SNET_H
#define SLIB_SNET_H

// Require libcurl
extern "C" {
#include "curl/curl.h"
}
#include "sobj.h"
#include "sio/sio.h"
#include "sutil/scode.h"

namespace slib {
    #define SNET_ERROR 0x0090
        
        #define HTTP_CONNECTION 0x01
        #define FTP_CONNECTION  0x02
    
        #define SSL_CONNECTION  0x10
    
        extern SDictionary STANDARD_HTTP;
        extern SDictionary STANDARD_HTTPS;
        //extern SDictionary STANDARD_FTP;
        
#define CURL_ERR_TEXT(N, T) (u8"libcurl error #"+std::to_string(N)+u8": "+std::string(T)).c_str()

        class SLIB_DLL SNetException : public SException {
        public:
			SNetException(const char* f, sint l, const char* func, sint e = 0, const char* target = nullptr, const char* note = nullptr);
            ~SNetException();
        };
        extern size_t writeCallback(void *buf, size_t size, size_t nmemb, void *ptr);
		extern size_t writeToFileCallback(void* buf, size_t size, size_t nmemb, void* ptr);
		
        class SLIB_DLL SNetWork {
        private:
            CURL *_curl;
            CURLcode _res;
            
        public:
            SData data;
            
        public:
            SNetWork();
            ~SNetWork();
            
            void http(const char *url, bool ssl = false, const char *cookie = nullptr, const char *post = nullptr);
            void ftp(const char *url, bool ssl = false, const char *info = nullptr);
            void connect(const SDictionary &dict);
			//void upload(const char* path, const char* url);
			void download(const char* path, const char* url, const char *user = nullptr, const char *pswd = nullptr);
        };
}
#endif
