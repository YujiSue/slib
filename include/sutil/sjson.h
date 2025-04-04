#ifndef SLIB_SJSON_H
#define SLIB_SJSON_H
#include "sobj/sobjptr.h"
#include "sio/sfile.h"
namespace slib {
    /**
    * @namespace sjson
    * \~english @brief Functions to handle JSON data and file
    * \~japanese @brief JSONデータとファイルを扱うための関数をまとめた名前空間
    */
    namespace sjson {
        /**
        * @cond
        */
        extern SLIB_DLL String jsString(const SObjPtr& obj, bool form = false, int layer = 0);
        extern SLIB_DLL SObjPtr jsObj(const char* s);
        /**
        * @condend
        */
        /**
        * \~english @brief Load JSON file as C++ object
        * \~english @param[in]  path  File path
        * \~english @return  C++ object
        */
        extern SLIB_DLL SObjPtr load(const char* path);
        /**
        * \~english @brief Save C++ object as a JSON file
        * \~english @param[in]  obj  C++ object
        * \~english @param[in]  path  File path
        * \~english @param[in]  form  Option to export formatted text
        */
        extern SLIB_DLL void save(const SObjPtr& obj, const char* path, bool form = false);
        /**
        * \~english @brief Read JSON format string as a C++ object
        * \~english @param[in]  str  JSON format string
        * \~english @return  C++ object
        */
        extern SLIB_DLL SObjPtr parse(const char* str);
        /**
        * \~english @brief Stringify object in JSON format
        * \~english @param[in]  obj  Object
        * \~english @param[in]  form  Option to export formatted text
        * \~english @return  JSON format string
        */
        extern SLIB_DLL String toString(const SObjPtr& obj, bool form = false);
    }
}
#endif
