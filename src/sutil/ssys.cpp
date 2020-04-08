#include "sutil/ssys.h"

using namespace slib;

String SSystem::charCode() {
#ifdef  WIN_OS
	String code;
	SSystem::exec("chcp", code);
	code.clip(code.find(":")+1);
	code.trimming();
	return code;
#else
	return "UTF-8";
#endif
}
String SSystem::userName() {
	String name;
#ifdef  WIN_OS
#ifdef _MANAGED
	//System::Environment::UserName
#else
	DWORD size = 4096;
	TCHAR buf[4096];
	GetUserName(buf, &size);
	name = String::toUTF8(buf);
#endif
#else
	char buf[4096];
	getlogin_r(buf, 4096);
	name = const_cast<const char*>(buf);
#endif
	return name;
}
void SSystem::setCurrent(const char *path) {
	int res;
#ifdef  WIN_OS
	String cur(path);
	res = chdir(cur.localize());
#else
	res = chdir(path);
#endif
    if (res) {
        switch (errno) {
            default:
                break;
        }
        throw SException(ERR_INFO, SLIB_EXEC_ERROR, "chdir", EXEC_TEXT(std::to_string(errno)));
    }
}
void SSystem::exec(const char *cmd) {
    if (!cmd) return;
    FILE *fp;
    fp = popen(cmd, "r");
    if (fp) {
        while(!feof(fp)) {}
        pclose(fp);
    }
    else throw SException(ERR_INFO, SLIB_EXEC_ERROR, "SSystem::exec", cmd);
}
int SSystem::exec(const char *cmd, String &result) {
    result.clear();
    if (!cmd) return 1;
    FILE *fp;
    fp = popen(cmd, "r");
    char buf[4096];
    if (fp) {
        while(!feof(fp)) {
            if(fgets(buf, 4096, fp)) result += buf;
        }
        return pclose(fp);
    }
    else throw SException(ERR_INFO, SLIB_EXEC_ERROR, "SSystem::exec", cmd);
}
size_t SSystem::getProcessID(const char* name) {
#ifdef WIN_OS
	String res;
	exec("tasklist /FO csv", res);
	auto list = res.splitline();
	sforeach(list) {
		auto datas = E_.split(",");
		if (String::dequot(datas[0]).beginWith(name)) 
			return String::dequot(datas[1]).sizeValue();
	}
	return NOT_FOUND;
#else
	String res;
	exec("ps -a", res);
    

	/*
	 */
    return NOT_FOUND;

#endif
}
void SSystem::killProcess(size_t pid) {
	String cmd;
#ifdef WIN_OS
	cmd = "taskkill";
	cmd << " /pid " << pid << " /F";
#else
	cmd = "kill";
	cmd << " " << pid;
#endif
	exec(cmd);
}


SProcess::SProcess() : _complete(false) {
#ifdef WIN_OS

	_saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	_saAttr.bInheritHandle = TRUE;
	_saAttr.lpSecurityDescriptor = NULL;
	_res = CreatePipe(&_pipe[2], &_pipe[3], &_saAttr, 0);
	if (!_res) throw SException(ERR_INFO, SLIB_CREATE_PIPE_ERR, "Process construction", "Child->Parent");
	_res = SetHandleInformation(_pipe[2], HANDLE_FLAG_INHERIT, 0);
	if (!_res) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Pipe handle inherit");
	_res = CreatePipe(&_pipe[0], &_pipe[1], &_saAttr, 0);
	if (!_res) throw SException(ERR_INFO, SLIB_CREATE_PIPE_ERR, "Process construction", "Parent->Child");
	_res = SetHandleInformation(_pipe[1], HANDLE_FLAG_INHERIT, 0);
	if (!_res) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Pipe handle inherit");

	ZeroMemory(&_piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&_siStartInfo, sizeof(STARTUPINFO));
	_siStartInfo.cb = sizeof(STARTUPINFO);
	_siStartInfo.hStdError = _pipe[3];
	_siStartInfo.hStdOutput = _pipe[3];
	_siStartInfo.hStdInput = _pipe[0];
	_siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	
#else

#endif
}
SProcess::~SProcess() {
#ifdef WIN_OS
	if (_wait && !_complete) wait();
	CloseHandle(_piProcInfo.hProcess);
	CloseHandle(_piProcInfo.hThread);
	CloseHandle(_pipe[3]);
	CloseHandle(_pipe[0]);
#else

#endif
}
void SProcess::createChild(const char* cmd, bool wait) {
#ifdef WIN_OS
#ifdef UNICODE
	_res = CreateProcess(NULL, &String(cmd).unicode()[0], NULL, NULL, TRUE, 0, NULL, NULL, &_siStartInfo, &_piProcInfo);
#else
	_res = CreateProcess(NULL, &String(cmd).localize()[0], NULL, NULL, TRUE, 0, NULL, NULL, &_siStartInfo, &_piProcInfo);
#endif
	if (!_res) throw SException(ERR_INFO, SLIB_CREATE_PROCESS_ERR, "Create child process");
#else

#endif
	_wait = wait;
}

void SProcess::send(const char *cmd, bool receive) {
#ifdef WIN_OS
	DWORD size;
	_res = WriteFile(_pipe[1], String(cmd).localize().cstr(), strlen(cmd), &size, NULL);
	if (!_res) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Send to child process", cmd);
	if (receive) {
		CHAR chBuf[4096];
		_res = ReadFile(_pipe[2], chBuf, 4096, &size, NULL);
		if (!_res || !size) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Receive from child process");
		else {
			chBuf[size] = '\0';
			_msg += String::toUTF8(const_cast<const char*>(chBuf));
		}
	}
#else

#endif
}
void SProcess::receive() {
#ifdef WIN_OS
	DWORD dwRead;
	CHAR chBuf[4096];
	while (true) {
		_res = ReadFile(_pipe[2], chBuf, 4096, &dwRead, NULL);
		if (!_res || !dwRead) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Receive from child process");
		else {
			chBuf[dwRead] = '\0';
			_msg += String::toUTF8(const_cast<const char*>(chBuf));
		}
	}
#else

#endif
}
String& SProcess::message() { return _msg; }
void SProcess::wait() {
#ifdef WIN_OS
	WaitForSingleObject(_piProcInfo.hProcess, INFINITE);
	_complete = true;
#else

#endif
}
int SProcess::processID() {
#ifdef WIN_OS 
	return _piProcInfo.dwProcessId;
#else
	return _pid;
#endif
}
void SProcess::kill() {
#ifdef WIN_OS
	TerminateProcess(_piProcInfo.hProcess, 0);
#else

#endif
}

SharedMemory::SharedMemory(size_t s, const char* n, int i) : _size(s), _name(n) {
#ifndef WIN_OS
	_pid = i;
	_child = false;
	FILE* fp;
	fp = fopen(_name.cstr(), "w");
	fclose(fp);
#endif
}
SharedMemory::~SharedMemory() {
#ifdef WIN_OS
	UnmapViewOfFile(_memory);
	CloseHandle(_map);
#else
	shmdt(_memory);
	if (!_child) shmctl(_sid, IPC_RMID, NULL);
#endif
}
void SharedMemory::create() {
#ifdef WIN_OS
#ifdef UNICODE
	_map = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, _size, _name.unicode().c_str());
#else
	_map = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, _size, _name.localize().cstr());
#endif
	if (_map == NULL) throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	_memory = MapViewOfFile(_map, FILE_MAP_ALL_ACCESS, 0, 0, _size);
	if (_memory == NULL) {
		CloseHandle(_map);
		throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	}
#else
	auto key = ftok(_name.cstr(), _pid);
	if (key == -1) throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	_sid = shmget(key, _size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if (_sid == -1) throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	_memory = shmat(_sid, 0, 0);
	_child = false;
#endif
}
void SharedMemory::share() {
#ifdef WIN_OS
#ifdef UNICODE
	_map = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _name.unicode().c_str());
#else
	_map = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _name.localize().cstr());
#endif
	if (_map == NULL) throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	_memory = MapViewOfFile(_map, FILE_MAP_ALL_ACCESS, 0, 0, _size);
	if (_memory == NULL) {
		CloseHandle(_map);
		throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	}
#else
	auto key = ftok(_name.cstr(), _pid);
	_sid = shmget(key, 0, 0);
	if (_sid == -1) throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	_memory = shmat(_sid, 0, 0);
	_child = true;
#endif
}
void* SharedMemory::pointer() { return (void*)_memory; }