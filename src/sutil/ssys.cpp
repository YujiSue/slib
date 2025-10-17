#include "sutil/ssys.h"
#include "sutil/sjson.h"
#include "sio/sio.h"
slib::Response::Response(int c, const char* o, const char* e) : code(c), output(o), error(e) {}
slib::Response::Response(const slib::Exception& ex) {
	code = ex.code; output = ex.prefix + " exception. " + ex.message; error = ex.description;
}
slib::Response::Response(const slib::Response& res) {
	code = res.code; output = res.output; error = res.error; attribute = res.attribute;
}
slib::Response::~Response() {}
slib::Response& slib::Response::operator=(const slib::Response& res) {
	code = res.code; output = res.output; error = res.error; attribute = res.attribute; return *this;
}
slib::SObjPtr slib::Response::toObj() const {
	return {
		D_("code", code),
		D_("out", output),
		D_("err", error),
		D_("attribute", attribute)
	};
}
slib::SObjPtr slib::Response::json() const { return slib::sjson::parse(output); }
void slib::Response::clear() {
	code = 0;
	output.clear();
	error.clear();
	attribute.clear();
}
slib::Response::operator bool() const { return code == 0; }

size_t slib::ssys::getPID(const char* name) {
	String que = sstr::toLower(name);
#ifdef WIN_OS	
	auto res = ssys::exec("tasklist", "/FO", "csv", "|", "find", "/I", sstr::dquote(que));
	auto list = res.output.splitLine();
	sfor(list) {
		auto datas = $_.split(",");
		auto process = sstr::toLower(sstr::dequote(datas[0]));
		if (process == que) return sstr::dequote(datas[1]).sizeValue();
	}
	return NOT_FOUND;
#else
	auto res = exec("ps", "-a", "|", "grep", sstr::dquote(name));
	auto list = res.output.splitLine();
	if (!list.empty() && list[0].size()) {
		sint idcol = 0, namecol = 0;
		auto cols = list[0].split(SP);
		sfori(cols) {
			if (cols[i] == "PID") idcol = i;
			else if (cols[i] == "CMD") namecol = i;
		}
		sforin(it, list.begin() + 1, list.end()) {
			auto datas = $_.split(SP);
			auto process = sstr::toLower(datas[namecol]);
			if (process == name) return datas[idcol].sizeValue();
		}
	}
	return NOT_FOUND;
#endif
}
//Array<size_t> slib::ssys::getPIDs(const char* name);
void slib::ssys::kill(size_t pid) {
	String cmd;
#ifdef WIN_OS
	ssys::exec("taskkill", "/pid", pid, "/F");
#else
	ssys::exec("kill", pid);
#endif
}
slib::String slib::ssys::userName() {
	String name;
#ifdef  WIN_OS
	DWORD size = 256;
	TCHAR buf[256];
	GetUserName(buf, &size);
	name = String::toUTF8(buf);
#else
	char buf[256];
	getlogin_r(buf, 256);
	name = const_cast<const char*>(buf);
#endif
	return name;
}
bool slib::ssys::isInstalled(const char* prog) {
	Response res;
#ifdef WIN_OS
	res = ssys::exec(S_(where.exe ) + prog);
#else
	res = ssys::exec(S_(which ) + prog);
#endif
	return sfs::exist(res.output.splitLine()[0]);
}
slib::String slib::ssys::getEnv(const char *key) { 
	auto val = getenv(key);
	return getenv(key); 
}
void slib::ssys::setEnv(const char *key, const char *value) {
	int res = 0;
#if defined(WIN_OS)
	String str = S(key) << "=" << value;
	res = putenv(str);
#else
	res = setenv(key, value, 1);
#endif
	if (!res) {}
}

slib::SProcess::SProcess() : _complete(false) {
#ifdef WIN_OS
	_saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	_saAttr.bInheritHandle = TRUE;
	_saAttr.lpSecurityDescriptor = NULL;
	_res = CreatePipe(&_pipe[2], &_pipe[3], &_saAttr, 0);
	//if (!_res) throw SException(ERR_INFO, SLIB_CREATE_PIPE_ERR, "Process construction", "Child->Parent");
	_res = SetHandleInformation(_pipe[2], HANDLE_FLAG_INHERIT, 0);
	//if (!_res) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Pipe handle inherit");
	_res = CreatePipe(&_pipe[0], &_pipe[1], &_saAttr, 0);
	//if (!_res) throw SException(ERR_INFO, SLIB_CREATE_PIPE_ERR, "Process construction", "Parent->Child");
	_res = SetHandleInformation(_pipe[1], HANDLE_FLAG_INHERIT, 0);
	//if (!_res) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Pipe handle inherit");

	ZeroMemory(&_piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&_siStartInfo, sizeof(STARTUPINFO));
	_siStartInfo.cb = sizeof(STARTUPINFO);
	_siStartInfo.hStdError = _pipe[3];
	_siStartInfo.hStdOutput = _pipe[3];
	_siStartInfo.hStdInput = _pipe[0];
	_siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
#else
    _res = pipe(&_pipe[2]);
    //if (!_res) throw SException(ERR_INFO, SLIB_CREATE_PIPE_ERR, "Process construction", "Child->Parent");
    _res = pipe(&_pipe[0]);
    //if (!_res) throw SException(ERR_INFO, SLIB_CREATE_PIPE_ERR, "Process construction", "Parent->Child");
    STD_IN = fileno(stdin);
    STD_OUT = fileno(stdout);
#endif
}
slib::SProcess::~SProcess() {
    if (_wait && !_complete) wait();
#ifdef WIN_OS
	CloseHandle(_piProcInfo.hProcess);
	CloseHandle(_piProcInfo.hThread);
	CloseHandle(_pipe[3]);
	CloseHandle(_pipe[0]);
#else
    sforin(i, 0, 4) close(_pipe[i]);
#endif
}
void slib::SProcess::createChild(const char* cmd, bool wait) {
#ifdef WIN_OS
#ifdef UNICODE
	_res = CreateProcess(NULL, &String(cmd).unicode()[0], NULL, NULL, TRUE, 0, NULL, NULL, &_siStartInfo, &_piProcInfo);
#else
	_res = CreateProcess(NULL, &String(cmd).localize()[0], NULL, NULL, TRUE, 0, NULL, NULL, &_siStartInfo, &_piProcInfo);
#endif
	//if (!_res) throw SException(ERR_INFO, SLIB_CREATE_PROCESS_ERR, "Create child process");
#else
    if ((_pid = fork()) < 0) {
        sforin(i, 0, 4) close(_pipe[i]);
        //throw SException(ERR_INFO, SLIB_CREATE_PROCESS_ERR, "Create child process");
    }
    if (_pid == 0) {
        close(_pipe[1]);
        close(_pipe[2]);
        dup2(_pipe[0], 0); close(_pipe[0]);
        dup2(_pipe[3], 1); close(_pipe[3]);
        if (execl(cmd, cmd, NULL) < 0) {
            close(_pipe[0]);
            close(_pipe[3]);
            exit(1);
        }
    }
    close(_pipe[0]);
    close(_pipe[3]);
    STD_IN = _pipe[2];
    STD_OUT = _pipe[1];
#endif
	_wait = wait;
    _complete = false;
}

void slib::SProcess::send(const char *cmd, bool receive) {
    _msg.clear();
#ifdef WIN_OS
	DWORD size;
	_res = WriteFile(_pipe[1], String(cmd).localize().cstr(), (DWORD)strlen(cmd), &size, NULL);
	//if (!_res) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Send to child process", cmd);
	if (receive) {
		CHAR chBuf[4096];
		_res = ReadFile(_pipe[2], chBuf, 4096, &size, NULL);
		if (!_res || !size) {}//throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Receive from child process");
		else {
			chBuf[size] = '\0';
			_msg += String::toUTF8(const_cast<const char*>(chBuf));
		}
	}
#else
    auto res = write(STD_OUT, cmd, strlen(cmd));
    if (receive) {
        int size;
        char buf[4096];
        while (true) {
            size = read(STD_IN, buf, 4096);
			if (size < 0) {}//throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Receive from child process");
            else if (!size) break;
            else {
                buf[size] = '\0';
                _msg += String(buf);
            }
        }
    }
#endif
}
void slib::SProcess::receive() {
    _msg.clear();
#ifdef WIN_OS
	DWORD dwRead;
	CHAR chBuf[4096];
	while (true) {
		_res = ReadFile(_pipe[2], chBuf, 4096, &dwRead, NULL);
		if (!_res || !dwRead) {}//throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Receive from child process");
		else {
			chBuf[dwRead] = '\0';
			_msg += String::toUTF8(const_cast<const char*>(chBuf));
		}
	}
#else
    int size;
    char buf[4096];
    while (true) {
        size = read(STD_IN, buf, 4096);
		if (size < 0) {}// throw SException(ERR_INFO, SLIB_EXEC_ERROR, "Receive from child process");
        else if (!size) break;
        else {
            buf[size] = '\0';
            _msg += String(buf);
        }
    }
#endif
}
slib::String& slib::SProcess::message() { return _msg; }
void slib::SProcess::wait() {
#ifdef WIN_OS
	WaitForSingleObject(_piProcInfo.hProcess, INFINITE);
	_complete = true;
#else
    waitpid(_pid, &_status, 0);
    _complete = true;
#endif
}
int slib::SProcess::processID() {
#ifdef WIN_OS 
	return _piProcInfo.dwProcessId;
#else
	return _pid;
#endif
}
void slib::SProcess::kill() {
#ifdef WIN_OS
	TerminateProcess(_piProcInfo.hProcess, 0);
#else
    ssys::exec(String("kill ")+(sint)_pid);
#endif
}

slib::SharedMemory::SharedMemory(size_t s, const char* n, int i) : _size(s), _name(n) {
#ifndef WIN_OS
	_pid = i;
	_child = false;
	FILE* fp;
	fp = fopen(_name.cstr(), "w");
	fclose(fp);
#endif
}
slib::SharedMemory::~SharedMemory() {
#ifdef WIN_OS
	UnmapViewOfFile(_memory);
	CloseHandle(_map);
#else
	shmdt(_memory);
	if (!_child) shmctl(_sid, IPC_RMID, NULL);
#endif
}
void slib::SharedMemory::create() {
#ifdef WIN_OS
#ifdef UNICODE
	_map = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (DWORD)_size, _name.unicode().c_str());
#else
	_map = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (DWORD)_size, _name.localize().cstr());
#endif
	//if (_map == NULL) throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	_memory = MapViewOfFile(_map, FILE_MAP_ALL_ACCESS, 0, 0, _size);
	if (_memory == NULL) {
		CloseHandle(_map);
		//throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	}
#else
	auto key = ftok(_name.cstr(), _pid);
	//if (key == -1) throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	_sid = shmget(key, _size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	//if (_sid == -1) throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	_memory = shmat(_sid, 0, 0);
	_child = false;
#endif
}
void slib::SharedMemory::share() {
#ifdef WIN_OS
#ifdef UNICODE
	_map = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _name.unicode().c_str());
#else
	_map = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _name.localize().cstr());
#endif
	//if (_map == NULL) throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	_memory = MapViewOfFile(_map, FILE_MAP_ALL_ACCESS, 0, 0, _size);
	if (_memory == NULL) {
		CloseHandle(_map);
		//throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	}
#else
	auto key = ftok(_name.cstr(), _pid);
	_sid = shmget(key, 0, 0);
	//if (_sid == -1) throw SException(ERR_INFO, SLIB_SHARED_MEMORY_ALLOC_ERR);
	_memory = shmat(_sid, 0, 0);
	_child = true;
#endif
}
void* slib::SharedMemory::pointer() { return (void*)_memory; }