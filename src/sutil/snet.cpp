#include "sutil/snet.h"
#include "sutil/scode.h"
#include "sio/stream.h"

inline void prepareHeaders(slib::SDictionary& dict) {
    dict["_headers_"] = slib::SArray();
    sfor(dict["header"]) {
        dict["_headers_"].add($_.key() + ": " + $_.value());
    }
}
inline slib::String preparePayload(const slib::SDictionary& dict) {
    if (dict.empty()) return "";
    slib::String payload;
    if (dict.hasKey("_key_order_")) {
        auto &keys = dict["_key_order_"];
        //sfor(keys) payload << slib::sutil::encodeURL($_) << "=" << slib::sutil::encodeURL(dict[$_]) << "&";
        sfor(keys) payload << $_ << "=" << dict[$_] << "&";
    }
    else {
        //sfor(dict) payload << slib::sutil::encodeURL($_.key()) << "=" << slib::sutil::encodeURL($_.value()) << "&";
        sfor(dict) payload << $_.key() << "=" << $_.value() << "&";
    }
    payload.resize(payload.size() - 1);
    return payload;
}
inline size_t slib::writeCallback(void* buf, size_t size, size_t nmemb, void* ptr) {
    size_t block = size * nmemb;
    slib::IOStream* strm = static_cast<slib::IOStream*>(ptr);
    strm->writeBytes((slib::subyte*)buf, block);
    strm->flush();
    return block;
}

slib::Response slib::sutil::download(const char* url, slib::SObjPtr attribute) {
    CURL* curl;
    slib::SFile file;
    slib::IOStream ostream;
    slib::Response res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        res.code = 1;
        return res;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    if (attribute["auto-referer"]) curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    if (attribute["follow-location"]) curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (attribute["ignore-ssl"]) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }
    if (attribute["cookie"]) {
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, (const char*)attribute["cookie"]);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, (const char*)attribute["cookie"]);
    }
    if (attribute["payload"]) {
        auto poststr = preparePayload(attribute["payload"]);
        curl_easy_setopt(curl, CURLOPT_POST, true);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, poststr.cstr());
    }
    if (attribute["auth"]) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, (const char*)attribute["auth"]["user"]);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, (const char*)attribute["auth"]["password"]);
    }
    if (attribute["header"]) {
        struct curl_slist* list = NULL;
        sfor(attribute["header"]) {
            list = curl_slist_append(list, $_.key() + ": " + $_.value());
        }
        if(list) curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    }
    // Set verobose option
    if (attribute["verbose"]) curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    // Make file stream
    if (attribute["output"]) {
        file.open(attribute["output"], sio::MAKE);
    }
    else if (attribute["outdir"]) {
        file.open(sfs::joinPath(attribute["outdir"], sfs::splitPath(url).second), sio::MAKE);
    }
    else file.open(sfs::joinPath(ssys::home(), sfs::splitPath(url).second), sio::MAKE);
    res.output = file.path();
    ostream.setFileOStream(file);
    // Set output call back
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&ostream);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    // Run download
    res.code = curl_easy_perform(curl);
    // Finalize
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    // Return result
    return res;
}
slib::Response slib::sutil::callapi(const char* url, const SDictionary &query, const SDictionary &header) {
    CURL* curl;
    slib::Response res;
    slib::IOStream ostream;
    ostream.setStrOStream(res.output);
　　slib::String payload;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        res.code = 1;
        return res;
    }
    if (!query.empty()) {
	payload = preparePayload(query);
	curl_easy_setopt(curl, CURLOPT_POST, true);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.cstr());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload.size());
    }
    if (!header.empty()) {
        struct curl_slist* list = NULL;
        sfor(header) {
            list = curl_slist_append(list, $_.key() + ": " + $_.value());
        }
        if (list) curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    }
    // Set output call back
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&ostream);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    // Run download
    res.code = curl_easy_perform(curl);
    // Finalize
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    // Return result
    return res;
}


slib::SNetException::SNetException(const char* f, sint l, const char* func, sint e, const char* target, const char* note)
	: slib::Exception() { 
	prefix = "snet";
	message = "Curl error.";
	//description = TARGET_TEXT(target) + u8" process error. [" + std::string(note ? note : "") + "]";
}
slib::SNetException::~SNetException() {}

slib::SNetWork::SNetWork() { 
    curl_global_init(CURL_GLOBAL_ALL);
    _curl = curl_easy_init();
}
slib::SNetWork::~SNetWork() {
    if (_curl) curl_easy_cleanup(_curl);
    curl_global_cleanup(); 
}
void slib::SNetWork::http(const char *url, bool ssl, const char *cookie, const char *post) {
    response.output.clear();
    _stream.setStrOStream(response.output);
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
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, (void *)&_stream);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeCallback);
    response.code = (int)curl_easy_perform(_curl);
    //if (_res) throw SNetException(ERR_INFO, SLIB_EXEC_ERROR, "curl_easy_perform", CURL_ERR_TEXT(_res, curl_easy_strerror(_res)));
    curl_easy_cleanup(_curl);
	curl_global_cleanup();
}
void slib::SNetWork::ftp(const char *url, bool ssl, const char *info) {
    // Not supported yet.
}
slib::Response& slib::SNetWork::connect(const char *url, slib::SObjPtr attribute) {
    struct curl_slist* _list = NULL;
    response.output.clear();
    _stream.setStrOStream(response.output);
    SFile file;
    if (attribute["ignore-ssl"]) {
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }
    if (attribute["auto-referer"]) {
        curl_easy_setopt(_curl, CURLOPT_AUTOREFERER, 1L);
    }
    if (attribute["follow-location"]) {
        curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    }
    if(attribute["cookie"]) {
        curl_easy_setopt(_curl, CURLOPT_COOKIEFILE, (const char *)attribute["cookie"]);
        curl_easy_setopt(_curl, CURLOPT_COOKIEJAR, (const char*)attribute["cookie"]);
    }
    if (attribute["header"]) {
        prepareHeaders(attribute);
        sfor(attribute["_headers_"]) {
            _list = curl_slist_append(_list, (const char*)$_);
        }
        if (_list) curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _list);
    }
    if (attribute["payload"]) {
        attribute["_post_"] = preparePayload(attribute["payload"]);
        curl_easy_setopt(_curl, CURLOPT_POST, true);
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, (const char *)attribute["_post_"]);
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, attribute["_post_"].size());
    }
    if (attribute["auth"]) {
        curl_easy_setopt(_curl, CURLOPT_USERNAME, (const char*)attribute["auth"]["user"]);
        curl_easy_setopt(_curl, CURLOPT_PASSWORD, (const char*)attribute["auth"]["password"]);
    }
    if (attribute["download"]) {
        if (attribute["outdir"]) {
            file.open(sfs::joinPath(attribute["outdir"], sfs::splitPath(url).second), sio::MAKE);
        }
        else if (attribute["output"]) {
            file.open(attribute["output"], sio::MAKE);
        }
        _stream.setFileOStream(file);
    }
    if (attribute["verbose"]) curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(_curl, CURLOPT_URL, url);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, (void*)&_stream);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeCallback);
    response.code = (int)curl_easy_perform(_curl);
    if (response.code) response.error = curl_easy_strerror((CURLcode)response.code);
    return response;
}
void slib::SNetWork::init() {
    _curl = curl_easy_init();
}
void slib::SNetWork::clear() {
    if (_curl) {
        curl_easy_cleanup(_curl);
        _curl = curl_easy_init();
    }
}
