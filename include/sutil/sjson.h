#ifndef SLIB_SJSON_H
#define SLIB_SJSON_H

#include "sobj/sobject.h"
#include "sio/sfile.h"
#include "sutil/sutil.h"

namespace slib {
    namespace sio {
        class SLIB_DLL SJson : public sobj {
            
        public:
            SJson();
			SJson(sio::SFile &file);
            SJson(const sobj &obj);
            SJson(const SJson &js);
            ~SJson();
            
            static String jsString(const sobj &obj);
            static sobj jsObj(const char *s);
            
            SJson &operator=(const sobj &obj);
            SJson &operator=(const SJson &js);
            
            void load(const char *path);
            void save(const char *path);
            
            void parse(const char *s);
            String toString();
        };
    }
}

#endif
