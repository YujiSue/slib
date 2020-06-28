#ifndef SLIB_SMEDIA_H
#define SLIB_SMEDIA_H

#include "smath/la.h"
#include "smath/geom.h"

namespace slib {
    namespace smedia {
        constexpr int SMEDIA_ERROR = 0x00C0;
        
        class SLIB_DLL SMediaException : public SException {
        public:
            SMediaException(const char* f, sint l, const char* func, sint e = 0, const char* target = nullptr, const char* note = nullptr);
            ~SMediaException();
        };
    }
}
#endif
