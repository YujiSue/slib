#ifndef SAPP_SAPP_H
#define SAPP_SAPP_H

#include "sobj.h"
#ifdef WIN_OS
#else
#include <dlfcn.h>
#endif

#ifndef SAPP_DLL
#if defined(USE_SAPP_DLL)
#define SAPP_DLL __declspec(dllimport)
#elif defined(_WINDLL)
#define SAPP_DLL __declspec(dllexport)
#else
#define SAPP_DLL  
#endif
#endif

namespace slib {
    namespace sapp {

        #define SAPP_ERROR 0x0A00
        #define SAPP_ERROR_EXIT 0x0A01
        #define SAPP_INIT_ERROR 0x0A02
        
        #define INSUFFICIENT_ARGS_ERROR 0x0A11
        #define INSUFFICIENT_OPT_ERROR 0x0A12
        #define COMMAND_NOT_EXIST_ERROR 0x0A13
        #define OPTION_NOT_EXIST_ERROR 0x0A14

        
        #define PLUGIN_LOAD_ERROR 0x0A21
        #define PLUGIN_EXEC_ERROR 0x0A22
        #define PLUGIN_NOT_EXIST_ERROR 0x0A23
        
        #define SAPP_LAUNCH 0x0AF0
        #define SAPP_TERMINATE 0x0AF1
        #define SAPP_PROGRESS 0x0AF2
        
        #define SAPP_EXECTED 0x0AFF
        
        
        class SAPP_DLL SAppException : public SException {
        public:
            SAppException(const char* f, sint l, const char* func, sint e = 0, const char* target = nullptr, const char* note = nullptr);
            ~SAppException();
        };
        
        class SAPP_DLL SApp {
        public:
            SDictionary profile, preference;
            
        public:
            SApp();
            SApp(const char *path);
            SApp(SDictionary &&prof);
            virtual ~SApp();
        };
        
    }
}

#endif
