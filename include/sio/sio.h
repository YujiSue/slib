#ifndef SLIB_SIO_H
#define SLIB_SIO_H
#include "sconfig.h"
#include "sobj/sobjptr.h"

namespace slib {
	class SLIB_DLL SFile;
	/**
	* @class SIOException
	* \~english @brief Exception class for IO process.
	* \~japanese @brief IO関連のプロセスに関する例外クラス
	*/
	class SLIB_DLL SIOException : public Exception {
	public:
		SIOException();
		SIOException(int i, const char* msg, const char* desc);
		~SIOException();
	};
	namespace sio {
		constexpr int FILE_NOT_EXIST_ERROR = 0x0011;
		constexpr int FILE_OPEN_ERROR = 0x0012;
		constexpr int FILE_READ_ERROR = 0x0013;
		constexpr int FILE_WRITE_ERROR = 0x0014;

		constexpr int FILE_SYSTEM_ERROR = 0x0015;


		/**
		* @cond
		*/
		extern SLIB_DLL String nofileErrorText(const char* path);
		extern SLIB_DLL String fileopenErrorText(const char* path);
		extern SLIB_DLL String filereadErrorText(const char* path);
		extern SLIB_DLL String filewriteErrorText(const char* path);

		extern SLIB_DLL String fsErrorMsg(const char* cmd);
		

#define FileNotFoundException(X) SIOException(sio::FILE_NOT_EXIST_ERROR, "File is not exist.", X)
#define FileOpenException(X) SIOException(sio::FILE_OPEN_ERROR, "File open error.", X)
#define FileReadException(X) SIOException(sio::FILE_READ_ERROR, "File read wrror.", X)
#define FileWriteException(X) SIOException(sio::FILE_WRITE_ERROR, "File write error.", X)

#define FileSystemException(X,Y) SIOException(sio::FILE_SYSTEM_ERROR, X, Y)

		/**
		* @endcond
		*/
	}

	namespace sio {
		constexpr subyte READ = 0x01;
		constexpr subyte WRITE = 0x02;
		constexpr subyte MAKE = 0x12;
		constexpr subyte APPEND = 0x22;
		constexpr subyte EDIT = 0x42;
		constexpr subyte DIRECTORY = 0x08;

		constexpr subyte OVERWRITE = 0x01;
		constexpr subyte CANCEL_IFEXIST = 0x02;
		constexpr subyte BACKUP_ORI = 0x04;

		constexpr subyte ISTREAM = 0x10;
		constexpr subyte OSTREAM = 0x20;
		constexpr subyte STDIO = 0x01;
		constexpr subyte STRIO = 0x02;
		constexpr subyte BYTEIO = 0x04;
		constexpr subyte FILEIO = 0x08;
#ifdef WIN_OS
		constexpr char PATH_SEPARATOR[2] = "\\";
		constexpr char PATH_DELIMITER[2] = ";";
#else
		constexpr char PATH_SEPARATOR[2] = "/";
		constexpr char PATH_DELIMITER[2] = ":";
#endif
	}
	namespace ssys {
		/**
		* \~english @brief Return root directory path.
		* \~japanese @brief ルートディレクトリのパスを返す関数
		*/
		extern SLIB_DLL String root();
		/**
		* \~english @brief Return home directory path.
		* \~japanese @brief ホームディレクトリのパスを返す関数
		*/
		extern SLIB_DLL String home();
		/**
		* \~english @brief Return current directory path.
		* \~japanese @brief 現在ディレクトリのパスを返す関数
		*/
		extern SLIB_DLL String current();
		/**
		* \~english @brief Change current directory.
		* \~japanese @brief カレントディレクトリの変更
		*/
		extern SLIB_DLL void setCurrent(const char* path);
	}
}
/**
* @cond
*/
inline void _join_path(slib::String& path) {
	if (path.size()) path.resize(path.size() - strlen(slib::sio::PATH_SEPARATOR));
}
template<typename First, typename... Args>
inline void _join_path(slib::String& path, First& first, Args&... args) {
	path << first << (slib::String(first).endWith(slib::sio::PATH_SEPARATOR) ? "" : slib::sio::PATH_SEPARATOR);
	_join_path(path, args...);
}
/**
* @endcond
*/
namespace slib {
	/**
	* @namespace sfs
	* \~english @brief Namespace for file system
	* \~japanese @brief ファイルシステム関連の名前空間
	*/
	namespace sfs {
		/**
		* \~english @brief Returns the path concatenating all arguments
		* \~japanese @brief 引数を全てつないだパスを返す関数
		*/
		template<typename... Args>
		extern String joinPath(Args... args) {
			String path;
			_join_path(path, args...);
			return path;
		}
		extern SLIB_DLL stringarray pathComponents(const char* path);
		extern SLIB_DLL Pair<slib::String, slib::String> splitPath(const char* path);
		extern SLIB_DLL String fileName(const char* path, bool ext = true);
		extern SLIB_DLL String extension(const char* path);
		extern SLIB_DLL String absolutePath(const char* path);
		extern SLIB_DLL sushort permission(const char* path);
		extern SLIB_DLL bool isDir(const char* path);
		extern SLIB_DLL bool exist(const char* path);
		extern SLIB_DLL int make(const char* path);
		extern SLIB_DLL int makeDir(const char *path, bool recurrent = true, sushort permission = 0x0755);
		extern SLIB_DLL int move(const char* from, const char* to, subyte op = sio::OVERWRITE);
		extern SLIB_DLL int copy(const char* from, const char* to, subyte op = sio::OVERWRITE);
		extern SLIB_DLL int remove(const char* path);
	}
}
#endif