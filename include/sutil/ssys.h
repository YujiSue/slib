#ifndef SLIB_SSYS_H
#define SLIB_SSYS_H

#include "sbasic/exception.h"
#include "sbasic/array.h"
#include "sbasic/string.h"

namespace slib {

    namespace smedia {
        class SLIB_DLL SImage;
    }
    
    class SLIB_DLL SSystem {
    protected:
        
    public:
        SSystem();
        SSystem(const char *cd);
        ~SSystem();

		static String charCode();
		static String userName();
        
        static void setCurrent(const char *path);
        static void exec(const char *cmd);
        static int exec(const char *cmd, String &result);
        static size_t getPID(const char *name);
        static void kill(size_t pid);
        //void screenShot(smedia::SImage *img);
        //void copyToBoard(const sobj &obj);
        //sobj pasteFromBoard();
    };

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
