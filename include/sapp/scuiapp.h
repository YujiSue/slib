#ifndef SAPP_SCUIAPP_H
#define SAPP_SCUIAPP_H

#include "sapp/sapp.h"

namespace slib {
    namespace sapp {

#ifdef STANDARD_OUT_STREAM
#define OUT_ std::cout
#define RETURN_ std::endl;
#elif defined(FILE_OUT_STREAM)
#define OUT_ FILE_
#define RETURN_ NEW_LINE;FILE_.flush();
#endif

		class SAPP_DLL SCuiApp : public SApp {

        public:
            SCuiApp();
            SCuiApp(const char *path);
            SCuiApp(SDictionary &&prof);
            virtual ~SCuiApp();
            
			int init(int argc, const char** argv);
            virtual int exec() = 0;
			int run(int argc, const char** argv);
        };
    }
}

#endif
