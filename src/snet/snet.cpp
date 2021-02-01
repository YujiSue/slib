#include "snet/snet.h"

using namespace slib;

SDictionary slib::STANDARD_HTTP =
{ kv("url", ""), kv("ssl", false), kv("cookie", snull), kv("post", snull), kv("load", true)};

SDictionary slib::STANDARD_HTTPS =
{ kv("url", ""), kv("ssl", true), kv("ignore-ssl", false), kv("cookie", snull), kv("post", snull), kv("load", true)};


SNetException::SNetException(const char* f, sint l, const char* func, sint e, const char* target, const char* note)
	: SException(f, l, func, e, target, note) { 
	prefix = "snet";
	message = "Curl error.";
	description = TARGET_TEXT(target) + u8" process error. [" + std::string(note ? note : "") + "]";
}
SNetException::~SNetException() {}

size_t slib::writeCallback(void *buf, size_t size, size_t nmemb, void *ptr) {
    size_t block = size * nmemb;
	SData* dat = static_cast<SData *>(ptr);
    dat->append((subyte *)buf, block);
    return block;
}
size_t slib::writeToFileCallback(void* buf, size_t size, size_t nmemb, void* ptr) {
	size_t block = size * nmemb;
	sio::SFile *file = static_cast<sio::SFile*>(ptr);
	file->writeBytes((subyte*)buf, block);
	return block;
}

SNetWork::SNetWork() { curl_global_init(CURL_GLOBAL_ALL); }
SNetWork::~SNetWork() { curl_global_cleanup(); }
void SNetWork::http(const char *url, bool ssl, const char *cookie, const char *post) {
    _curl = curl_easy_init();
    data.clear();
	if (!_curl) throw SNetException(ERR_INFO, SLIB_EXEC_ERROR, "curl_easy_init", EXEC_TEXT("null"));
    curl_easy_setopt(_curl, CURLOPT_URL, url);
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, true);
    if(!ssl) {
		curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
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
	curl_global_cleanup();
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
		if (dict["ignore-ssl"]) {
			curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
		}
    }
    if(dict["cookie"]) {
        curl_easy_setopt(_curl, CURLOPT_COOKIEJAR, (const char *)dict["cookie"]);
        curl_easy_setopt(_curl, CURLOPT_COOKIEFILE, (const char *)dict["cookie"]);
    }
    if(dict["post"]) {
        curl_easy_setopt(_curl, CURLOPT_POST, true);
		curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, (const char *)dict["post"]);
		curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, strlen(dict["post"]));
    }
    if(dict["load"]) {
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, (void *)&data);
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeCallback);
    }
    _res = curl_easy_perform(_curl);
	if (_res) throw SNetException(ERR_INFO, SLIB_EXEC_ERROR, "curl_easy_perform", CURL_ERR_TEXT(_res, curl_easy_strerror(_res)));
    curl_easy_cleanup(_curl);
	curl_global_cleanup();
}
void SNetWork::download(const char* path, const char* url, const char* user, const char* pswd) {
	_curl = curl_easy_init();
	if (!_curl) throw SNetException(ERR_INFO, SLIB_EXEC_ERROR);
	sio::SFile file(path, sio::CREATE|sio::WRITE);
	curl_easy_setopt(_curl, CURLOPT_URL,url);
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeToFileCallback);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &file);
	if (user) curl_easy_setopt(_curl, CURLOPT_USERNAME, user);
	if (pswd) curl_easy_setopt(_curl, CURLOPT_PASSWORD, pswd);
	//curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
	_res = curl_easy_perform(_curl);
	curl_easy_cleanup(_curl);
}
