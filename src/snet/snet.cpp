#include "snet/snet.h"

using namespace slib;

SDictionary slib::STANDARD_HTTP =
{ kv("url", ""), kv("ssl", false), kv("cookie", snull), kv("post", snull), kv("load", true)};

SDictionary slib::STANDARD_HTTPS =
{ kv("url", ""), kv("ssl", true), kv("cookie", snull), kv("post", snull), kv("load", true)};


SNetException::SNetException(const char* f, sint l, const char* func, sint e, const char* target, const char* note)
	: SException(f, l, func, e, target, note) { 
	prefix = "snet";
	message = "Curl error.";
	description = TARGET_TEXT(target) + u8" process error. [" + std::string(note ? note : "") + "]";
}
SNetException::~SNetException() {}

size_t slib::writeCallback(void *buf, size_t size, size_t nmemb, void *ptr) {
    size_t block = size * nmemb;
    auto dat = static_cast<SData *>(ptr);
    dat->append((subyte *)buf, block);
    return block;
}

SNetWork::SNetWork() {
    curl_global_init(CURL_GLOBAL_ALL);
}

SNetWork::~SNetWork() {
    curl_global_cleanup();
}

void SNetWork::http(const char *url, bool ssl, const char *cookie, const char *post) {
    _curl = curl_easy_init();
    data.clear();
	if (!_curl) throw SNetException(ERR_INFO, SLIB_EXEC_ERROR, "curl_easy_init", EXEC_TEXT("null"));
    curl_easy_setopt(_curl, CURLOPT_URL, url);
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, true);
    if(ssl) {
#ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
#ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
    }
    if(cookie) {
        curl_easy_setopt(_curl, CURLOPT_COOKIEJAR, cookie);
        curl_easy_setopt(_curl, CURLOPT_COOKIEFILE, cookie);
    }
    if (post) {
        curl_easy_setopt(_curl, CURLOPT_POST, true);
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, post);
    }
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, (void *)&data);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeCallback);
    _res = curl_easy_perform(_curl);
    if (_res) throw SNetException(ERR_INFO, SLIB_EXEC_ERROR, "curl_easy_perform", CURL_ERR_TEXT(_res, curl_easy_strerror(_res)));
    curl_easy_cleanup(_curl);
}

void SNetWork::ftp(const char *url, bool ssl, const char *info) {
    // Not supported yet.
}

void SNetWork::connect(const SDictionary &dict) {
    _curl = curl_easy_init();
    data.clear();
	if (!_curl) throw SNetException(ERR_INFO, SLIB_EXEC_ERROR, "curl_easy_init", EXEC_TEXT("null"));
    if(!(dict.hasKey("url"))) return;
    curl_easy_setopt(_curl, CURLOPT_URL, (const char *)dict["url"]);
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, true);
    if(dict.hasKey("ssl")) {
        #ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        #endif
        #ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        #endif
    }
    if(dict["cookie"]) {
        curl_easy_setopt(_curl, CURLOPT_COOKIEJAR, (const char *)dict["cookie"]);
        curl_easy_setopt(_curl, CURLOPT_COOKIEFILE, (const char *)dict["cookie"]);
    }
    if(dict["post"]) {
        curl_easy_setopt(_curl, CURLOPT_POST, true);
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, (const char *)toString(dict["post"].dict(), "=", "&"));
    }
    if(dict["load"]) {
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, (void *)&data);
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeCallback);
    }
    _res = curl_easy_perform(_curl);
	if (_res) throw SNetException(ERR_INFO, SLIB_EXEC_ERROR, "curl_easy_perform", CURL_ERR_TEXT(_res, curl_easy_strerror(_res)));
    curl_easy_cleanup(_curl);
}
