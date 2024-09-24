#ifndef SAPP_EXCEPTION_H
#define SAPP_EXCEPTION_H
#include "sbasic/exception.h"
#include "sapp/appbasic.h"
#define CHECK_(R) if((R)["code"]){throw slib::SAppException();}

extern inline slib::String commandErrTxt(const char* s) { return S_(Command) << " '" << s << "' is not available."; }
extern inline slib::String insufficientArgsErrTxt(const char* s) { return S_(Option) << " '" << s << "' is required."; }
extern inline slib::String undefOptsErrTxt(const char* s) { return S_(Option) << " '" << s << "' is not defined."; }

extern inline slib::String pluginLoadErrText(const char* s) { return S_(Filed to load the plugin) << " '" << s << "'."; }
extern inline slib::String pluginFuncLoadErrText(const char* s1, const char* s2, const char* s3) { return S_(Filed to load the function) << " '" << s2 << "' in the plugin '" << s1 << "'." << slib::NL << s3; }


/*
#define SAPP_ERROR_EXIT 0x0A01
#define SAPP_INIT_ERROR 0x0A02

#define INSUFFICIENT_ARGS_ERROR 0x0A11
#define INSUFFICIENT_OPT_ERROR 0x0A12
#define COMMAND_NOT_EXIST_ERROR 0x0A13
#define OPTION_NOT_EXIST_ERROR 0x0A14

#define SAPP_LAUNCH 0x0AF0
#define SAPP_TERMINATE 0x0AF1
#define SAPP_PROGRESS 0x0AF2

#define SAPP_EXECTED 0x0AFF
*/

namespace slib {
	namespace sapp {
		constexpr int APP_ERROR = 0x0A00;
		constexpr int APP_INIT_ERROR = 0x0A01;
		constexpr int APP_EXEC_ERROR = 0x0A02;

		constexpr int INSUFFICIENT_ARGS_ERROR = 0x0A11;
		constexpr int UNDEFINED_OPT_ERROR = 0x0A12;
		constexpr int COMMAND_ERROR = 0x0A13;

		constexpr int PLUGIN_LOAD_ERROR = 0x0A21;
		constexpr int PLUGIN_FUNC_LOAD_ERROR = 0x0A22;
		constexpr int PLUGIN_EXEC_ERROR = 0x0A23;

		// App state
		constexpr int INITIALIZE = 0x0AE0;
		constexpr int PREPARED = 0x0AE1;
		constexpr int COMPLETE = 0x0AE2;
		constexpr int IN_PROGRESS = 0x0AE4;
		constexpr int TERMINATED = 0x0AE8;
	}



	class SLIB_DLL SAppException : public Exception {
	public:
		SAppException();
		SAppException(int i, const char* msg = nullptr, const char* desc = nullptr);
		SAppException(Exception &ex);
		SAppException(Response &res);


		SAppException(const char* f, sint l, const char* func, sint e = 0, const char* target = nullptr, const char* note = nullptr);
		SAppException(const char* f, sint l, const char* func, const Exception& ex);
		~SAppException();
	};
}
#define AppCommandException(X) SAppException(sapp::COMMAND_ERROR, "Command error.", commandErrTxt(X))
#define InsufficientArgsException(X) SAppException(sapp::INSUFFICIENT_ARGS_ERROR, "Insufficient argments error.", insufficientArgsErrTxt(X))
#define UndefinedOptException(X) SAppException(sapp::UNDEFINED_OPT_ERROR, "Undefined option error.", undefOptsErrTxt(X))

#define PluginLoadException(X) SAppException(sapp::PLUGIN_LOAD_ERROR, "Plugin load error.", pluginLoadErrText(X))
#define PluginLoadFuncException(X,Y,Z) SAppException(sapp::PLUGIN_FUNC_LOAD_ERROR, "Plugin function load error.", pluginFuncLoadErrText(X,Y,Z))

#endif