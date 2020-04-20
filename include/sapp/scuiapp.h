#ifndef SAPP_SCUIAPP_H
#define SAPP_SCUIAPP_H

#include "sapp/sapp.h"

namespace slib {
    namespace sapp {

#define SCUI_SCMD_APP 0x0000
#define SCUI_MCMD_APP 0x0001
#define SCUI_LOOP_APP 0x8001

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
