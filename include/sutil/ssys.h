#ifndef SLIB_SSYS_H
#define SLIB_SSYS_H
#include "sbasic/node.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"
namespace slib {
	class SLIB_DLL SImage;
	/**
	* @class Response
	* \~english @brief Response class with return/error code, standard output, and error message.
	*/
	class SLIB_DLL Response {
	public:
		int code;
		String output, error;
		SObjPtr attribute;
	public:
		Response(int c = 0, const char *o = nullptr, const char *e = nullptr);
		Response(const Exception& ex);
		Response(const Response& res);
		virtual ~Response();
		Response& operator=(const Response& res);
		SObjPtr toObj() const;
		SObjPtr json() const;
		void clear();
		operator bool() const;
	};
	/// @cond
	extern inline String& _expandCmd(String& cmd) { return cmd; }
	template<typename Arg, typename... Args>
	extern inline String& _expandCmd(String& cmd, const Arg& arg, const Args&... args) {
		cmd << " " << arg;
		return _expandCmd(cmd, args...);
	}
	/// @endcond
	namespace ssys {
		/**
		* \~english @brief Exec. system command
		* 
		*/
		template<typename... Args>
		extern Response exec(const Args&... args) {
			Response res;
			String cmd;
			_expandCmd(cmd, args...);
			FILE* fp;
			fp = popen(cmd, "r");
			const int BUFFER_SIZE = 1024;
			char buf[BUFFER_SIZE];
			if (fp) {
				while (!feof(fp)) {
					buf[BUFFER_SIZE - 1] = '\0';
					auto ret = fgets(buf, BUFFER_SIZE, fp);
					if (ret) res.output += const_cast<const char*>(buf);
					else if (feof(fp)) break;
					else {
						res.code = ferror(fp);

					}
				}
				res.code = pclose(fp);

			}
			else {
				res.code = 1;
				res.error = "Failed to \"popen\".";
			}
			return res;
		}
		/**
		* \~english @brief Find ID of a running process
		*
		*/
		extern SLIB_DLL size_t getPID(const char* name);
		/**
		* \~english @brief Find IDs of a running process
		*
		*/
		extern SLIB_DLL inline Array<size_t> getPIDs(const char* name);
		/**
		* \~english @brief Kill process
		*
		*/
		extern SLIB_DLL void kill(size_t pid);
		/**
		* \~english @brief Return user name
		*
		*/
		extern SLIB_DLL String userName();
		/**
		* \~english @brief Make sure that the application can be launched (installed and path set).
		* 
		*/
		extern SLIB_DLL bool isInstalled(const char *prog);
		/**
		 * 
		 */
		extern SLIB_DLL String getEnv(const char *key);
		/**
		 * 
		 */
		extern SLIB_DLL void setEnv(const char *key, const char *value);
	}

	class SLIB_DLL SProcess {
	private:
		bool _complete, _wait, _res;
		String _msg;

		#ifdef WIN_OS
		HANDLE _pipe[4]; // parent->child in(0), out(1), child->parent in(2), out(3)
		SECURITY_ATTRIBUTES _saAttr;
		STARTUPINFO _siStartInfo;
		PROCESS_INFORMATION _piProcInfo;
		
		#else
        sint STD_IN, STD_OUT;
        int _status;
		pid_t _pid;
		sint _pipe[4]; // parent->child in(0), out(1), child->parent in(2), out(3)
		#endif

	public:
		SProcess();
		~SProcess();

		void createChild(const char* cmd, bool wait = true);
		void send(const char* cmd, bool receive = true);
		void receive();
		String& message();
		int processID();
		void wait();
		void kill();
	};

	class SLIB_DLL SharedMemory {
	private:
		size_t _size;
		void* _memory;
		String _name;
#ifdef WIN_OS
		HANDLE _map;
#else
		bool _child;
		int _pid, _sid;
#endif
	public:
		SharedMemory();
		SharedMemory(size_t s, const char* n, int i = 0);
		SharedMemory(const SharedMemory &mem);
		~SharedMemory();
		SharedMemory& operator=(const SharedMemory& mem);
		void create();
		void share();
		void* pointer();
		template<class Cls>
		void alloc() { new(_memory) Cls; }
		template<class Cls>
		Cls* convert() {
			return static_cast<Cls*>(_memory);
		}
	};

}

#endif
