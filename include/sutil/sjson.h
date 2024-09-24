#ifndef SLIB_SJSON_H
#define SLIB_SJSON_H
#include "sobj/sobjptr.h"
#include "sio/sfile.h"
namespace slib {
    /**
    * @namespace sjson
    * \~english @brief Namescpace fro functions to handle JSON data and file
    * \~japanese @brief JSONデータとファイルを扱うための関数をまとめた名前空間
    */
    namespace sjson {
       extern SLIB_DLL String jsString(const SObjPtr& obj, bool form = false, int layer = 0);
       extern SLIB_DLL SObjPtr jsObj(const char* s);
       extern SLIB_DLL SObjPtr load(const char* path);
       extern SLIB_DLL void save(const SObjPtr& obj, const char* path, bool form = false);
       extern SLIB_DLL SObjPtr parse(const char* s);
       extern SLIB_DLL String toString(const SObjPtr& obj, bool form = false);
    }
}
#endif
