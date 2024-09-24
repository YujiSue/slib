#ifndef SAPP_SCUIAPP_H
#define SAPP_SCUIAPP_H
#include "sapp/sapp.h"
/**
* @cond
*/
#define RUN_CUI_APP(A) int main(int argc,const char* argv[]){A app;return app.run(argc, argv);}
/**
* @endcond
*/
namespace slib {
	namespace sapp {
		/**
		* @class SCuiApp
		* \~english @brief Command line application class
		* \~japanese @brief コマンドラインアプリケーションクラス
		*/
		class SLIB_DLL SCuiApp : public SApp {
		public:
			SCuiApp();
			SCuiApp(const char* prof, const char* format = "json");
			SCuiApp(SDictionary&& prof);
			virtual ~SCuiApp();
			/**
			* \~english @brief Set command and options to preference
			* \~english @param[in]  argc  Count of argments
			* \~english @param[in]  argv  Argments
			* \~english @return 0 if OK else error code 
			*
			* \~japanese @brief コマンドとオプションのセット
			* \~japanese @param[in]  argc  引数のカウント
			* \~japanese @param[in]  argv  引数
			* \~japanese @return 0（成功時）またはエラーコード
			*/
			int init(int argc, const char** argv);
			virtual int exec() = 0;
			/**
			* \~english @brief Run the app's program
			* \~english @param[in]  argc  Count of argments
			* \~english @param[in]  argv  Argments
			* \~english @return 0 if OK else error code
			*
			* \~japanese @brief アプリケーションプログラムの実行
			* \~japanese @param[in]  argc  引数のカウント
			* \~japanese @param[in]  argv  引数
			* \~japanese @return 0（成功時）またはエラーコード
			*/
			int run(int argc, const char** argv);
		};
	}
}
#endif