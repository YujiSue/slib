#ifndef SBIO_SBIEXCEPTION_H
#define SBIO_SBIEXCEPTION_H
#include "sbasic/exception.h"

#define BioFileFormatException(X,Y) slib::sbio::SBioInfoException(slib::FORMAT_ERROR,X,Y)

#define RefMismatchException(X) slib::sbio::SBioInfoException(slib::sbio::REF_MISMATCH_ERROR,"Reference size mismatch error.",X)

namespace slib {
	namespace sbio {
        constexpr int BIOINFO_ERROR = 0xB000;


        constexpr int REF_MISMATCH_ERROR = 0xB001;

        /**
        */
        class SLIB_DLL SBioInfoException : public Exception {
        public:
            SBioInfoException();
            SBioInfoException(int c, const char *msg, const char *desc);
            ~SBioInfoException();
        };

        //
        extern SLIB_DLL String refMisErrorText(const char* s1, int n1, int l1, const char* s2, int n2, int l2);

	}
}

#endif 