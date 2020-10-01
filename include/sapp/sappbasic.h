#ifndef SAPP_SAPPBASIC_H
#define SAPP_SAPPBASIC_H

#define SAPP_VERSION 1.0.0

#include "sobj.h"
#ifdef WIN_OS
#else
#include <dlfcn.h>
#endif

#ifndef SAPP_DLL
#if defined(_WINDLL)
#define SAPP_DLL __declspec(dllexport)
#else
#define SAPP_DLL
#endif
#endif

namespace slib {
	namespace sapp {

		constexpr suint SCUI_APP = 0x0000;
		constexpr suint SGUI_APP = 0x0001;

		constexpr suint SINGLE_COMMAND = 0x0010;
		constexpr suint MULTI_COMMAND = 0x0020;
		constexpr suint BACKGROUND_PROCESS = 0x0040;

		constexpr suint SINGLE_VIEW_UI = 0x0010;
		constexpr suint MULTI_VIEW_UI = 0x0020;

		typedef enum {
			FILE_OUT = 1,
			STD_OUT = 2,
			ALERT = 4,
			ERROR_LOG = 8,
		} LOGGER_MODE;


#define SAPP_ERROR 0x0A00
#define SAPP_ERROR_EXIT 0x0A01
#define SAPP_INIT_ERROR 0x0A02

#define INSUFFICIENT_ARGS_ERROR 0x0A11
#define INSUFFICIENT_OPT_ERROR 0x0A12
#define COMMAND_NOT_EXIST_ERROR 0x0A13
#define OPTION_NOT_EXIST_ERROR 0x0A14

#define PLUGIN_LOAD_ERROR 0x0A21
#define PLUGIN_FUNC_LOAD_ERROR 0x0A22
#define PLUGIN_EXEC_ERROR 0x0A23

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
			SApp(const char* path);
			SApp(SDictionary&& prof);
			virtual ~SApp();
		};

#ifndef splugin
#ifdef _WINDLL
#define splugin extern __declspec(dllexport) int
#else
#define splugin extern int
#endif
#endif

		template<typename... Args>
		class SPlugIn {
#ifdef WIN_OS
			typedef int(CALLBACK* Func)(Args...);
#else
			typedef int(*Func)(Args...);
#endif
		private:
			Func _func;

#ifdef WIN_OS
			HMODULE _dll;
			FARPROC _proc;
#else
			void* _lib;
#endif

		public:
			SPlugIn() {
#ifdef WIN_OS
				_dll = NULL;
				_proc = NULL;
#else
				_lib = nullptr;
#endif
			}
			SPlugIn(const char* lib) {
#ifdef WIN_OS
#ifdef UNICODE
				wchar_t wtemp[256];
				auto upath = String(lib).unicode();
				memcpy(wtemp, upath.c_str(), upath.length() * sizeof(wchar_t));
				wtemp[upath.length()] = '\0';
				_dll = LoadLibrary(wtemp);
#else
				char ctemp[256];
				auto upath = String(lib).localize();
				memcpy(ctemp, upath.cstr(), upath.length() * sizeof(wchar_t));
				ctemp[upath.length()] = '\0';
				_dll = LoadLibrary(ctemp);
#endif

				if (_dll == NULL) throw SAppException(ERR_INFO, PLUGIN_LOAD_ERROR);
#else
				_lib = dlopen(lib, RTLD_LAZY);
				if (_lib == NULL) throw SAppException(ERR_INFO, PLUGIN_LOAD_ERROR);
#endif
			}
			SPlugIn(const char* lib, const char* f) {
#ifdef WIN_OS
#ifdef UNICODE
				wchar_t wtemp[256];
				auto upath = String(lib).unicode();
				memcpy(wtemp, upath.c_str(), upath.length() * sizeof(wchar_t));
				wtemp[upath.length()] = '\0';
				_dll = LoadLibrary(wtemp);
#else
				char ctemp[256];
				auto upath = String(lib).localize();
				memcpy(ctemp, upath.cstr(), upath.length() * sizeof(wchar_t));
				ctemp[upath.length()] = '\0';
				_dll = LoadLibrary(ctemp);
#endif

				if (_dll == NULL) throw SAppException(ERR_INFO, PLUGIN_LOAD_ERROR);
				_proc = GetProcAddress(_dll, f);
				if (_proc == NULL) throw SAppException(ERR_INFO, PLUGIN_FUNC_LOAD_ERROR);
				_func = reinterpret_cast<Func>(_proc);
#else
				_lib = dlopen(lib, RTLD_LAZY);
				if (_lib == NULL) throw SAppException(ERR_INFO, PLUGIN_LOAD_ERROR);
				_func = (int(*)(Args...))dlsym(_lib, f);
				if (_func == NULL) throw SAppException(ERR_INFO, PLUGIN_FUNC_LOAD_ERROR);
#endif
			}
			~SPlugIn() {
#ifdef WIN_OS
				if (_dll != NULL) FreeLibrary(_dll);
#else
				if (_lib) dlclose(_lib);
#endif
			}
			void call(const char* f) {
#ifdef WIN_OS
				_proc = GetProcAddress(_dll, f);
				if (_proc == NULL) throw SAppException(ERR_INFO, PLUGIN_FUNC_LOAD_ERROR);
				_func = reinterpret_cast<Func>(_proc);
#else
				_func = (int(*)(Args...))dlsym(_lib, f);
				if (_func == NULL) throw SAppException(ERR_INFO, PLUGIN_FUNC_LOAD_ERROR);
#endif
			}
			int exec(Args... args) { return (*_func)(args...); }
			int operator()(Args... args) { return (*_func)(args...); }
		};
	}
}

#endif