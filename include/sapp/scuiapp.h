#ifndef SAPP_SCUIAPP_H
#define SAPP_SCUIAPP_H

#include "sapp/sapp.h"

namespace slib {
    namespace sapp {

#define SCUI_NO_COMMAND 0x00
#define SCUI_MULTI_COMMAND 0x01
//#define SCUI_RESIDUAL 0x02

		class SLIB_DLL SCuiApp : public SApp {

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
