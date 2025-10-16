#ifndef SLIB_SAPP_H
#define SLIB_SAPP_H
#include "sobj/snumber.h"
#include "sobj/sdate.h"
#include "sobj/sdict.h"
#include "sobj/sfunc.h"
#include "sio/stream.h"
#include "sutil/sthread.h"
#include "sutil/ssys.h"
#include "sapp/appexception.h"
namespace slib {
	namespace sapp {
		extern SLIB_DLL String DEFAULT_DATA_DIR;
		extern SLIB_DLL String DEFAULT_CONFIG_DIR;
		extern SLIB_DLL String DEFAULT_LOG_DIR;

	
		/**
		 * @class SApp
		 * \~english @brief Basic application class 
		 * \~japanese @brief アプリケーションクラス
		 */
		class SLIB_DLL SApp {
		public:
			SDictionary profile, preference;

		public:
			SApp();
			SApp(const char* prof, const char* format = "json");
			SApp(SDictionary&& prof);
			virtual ~SApp();
		};

		/**
		 * @class SLogger
		 * \~english @brief Logger class
		 * \~japanese @brief ロガークラス
		 */
		class SLIB_DLL SLogger {
		private:
			subyte _mode;
			SLock _lock;
			SFile _file;
			
		public:
			SLogger(subyte m = slib::sio::STDIO, const char* path = nullptr);
			~SLogger();
			void open(const char* path);
			void close();
			void log(const String& msg);
			void log(const Exception &ex);
		};

#ifndef splugin
#ifdef _WINDLL
#define splugin extern __declspec(dllexport) int
#else
#define splugin extern int
#endif
#endif
		/**
		 * @class SPlugIn
		 * \~english @brief Plugin class
		 * \~japanese @brief プラグインクラス
		 */
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
			String libname;
			String funcname;
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
				libname = String::toUTF8(lib);
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

				if (_dll == NULL) throw PluginLoadException(libname);
#else
				libname = lib;
				_lib = dlopen(lib, RTLD_LAZY);
				if (_lib == NULL) throw PluginLoadException(libname);
#endif
			}
			SPlugIn(const char* lib, const char* f) {
#ifdef WIN_OS
				libname = String::toUTF8(lib);
				funcname = String::toUTF8(f);
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

				if (_dll == NULL) throw PluginLoadException(libname);
				_proc = GetProcAddress(_dll, f);
				if (_proc == NULL) throw PluginLoadFuncException(libname, funcname, "");
				_func = reinterpret_cast<Func>(_proc);
#else
				libname = lib;
				funcname = f;
				_lib = dlopen(lib, RTLD_LAZY);
				if (_lib == NULL) throw PluginLoadException(libname);
				_func = (int(*)(Args...))dlsym(_lib, f);
				if (_func == NULL) throw PluginLoadFuncException(libname, funcname, dlerror());
#endif
			}
			~SPlugIn() {
#ifdef WIN_OS
				if (_dll != NULL) FreeLibrary(_dll);
#else
				if (_lib) dlclose(_lib);
#endif
			}
			void load(const char* lib) {
#ifdef WIN_OS
				libname = String::toUTF8(lib);
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

				if (_dll == NULL) throw PluginLoadException(libname);
#else
				libname = lib;
				_lib = dlopen(lib, RTLD_LAZY);
				if (_lib == NULL) throw PluginLoadException(libname);
#endif
			}
			void call(const char* f) {
#ifdef WIN_OS
				funcname = String::toUTF8(f);
				_proc = GetProcAddress(_dll, f);
				if (_proc == NULL) throw PluginLoadFuncException(libname, funcname, "");
				_func = reinterpret_cast<Func>(_proc);
#else
				funcname = f;
				_func = (int(*)(Args...))dlsym(_lib, f);
				if (_func == NULL) throw PluginLoadFuncException(libname, funcname, dlerror());
#endif
			}
			int exec(Args... args) { return (*_func)(args...); }
			int operator()(Args... args) { return (*_func)(args...); }
		};
	}
}
#endif