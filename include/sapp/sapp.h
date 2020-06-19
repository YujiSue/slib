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

		constexpr suint SCUI_APP = 0x0000;
		constexpr suint SGUI_APP = 0x0001;
		constexpr suint SWEB_APP = 0x0002;

		constexpr suint SINGLE_PROCESS = 0x0010;
		constexpr suint MULTI_COMMAND = 0x0020;
		constexpr suint INTERACTIVE = 0x0040;
		constexpr suint BACKGROUND_PROCESS = 0x0080;

		constexpr suint SINGLE_VIEW_UI = 0x0010;
		constexpr suint MULTI_VIEW_UI = 0x0020;

		constexpr suint USE_CV = 0x0100;
		constexpr suint USE_GL = 0x0200;
		constexpr suint USE_NODE = 0x0400;
		constexpr suint USE_CEF = 0x0800;

		#define SLIB_LOG_CODE 0x01

		#define SLIB_LAUNCH_CODE 0x0A
		#define SLIB_TERMINATE_CODE 0x0B

		#define SLIB_WARNING_CODE 0x0E
		#define SLIB_ERROR_CODE 0x0F

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

		struct log_data {
			SDate date;
			sint code;
			String msg;

			log_data(sint c = 0, const char* s = nullptr);
			~log_data();
		};
		class SAPP_DLL SLogger {
		private:
			SLock _lock;
			sio::SFile _file;
			Array<log_data> _data;

		public:
			SLogger(const char* path = nullptr);
			~SLogger();
			void open(const char* path);
			void close();
			void log(sint code, const char* msg);
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
