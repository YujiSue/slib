#include "sio/sfile.h"

slib::sio::SIOException::SIOException(const char* f, sint l, const char* func, sint e, const char* target, const char* note) 
	: slib::SException(f, l, func, e, target, note) { 
	prefix = "sio";
	switch (err) {
	case FILE_NOT_EXIST_ERROR:
	{
		message = "File not exist error.";
		description = TARGET_TEXT(target) + u8" does not exist.";
		break;
	}
	case FILE_OPEN_ERROR:
	{
		message = "File open error.";
		description = TARGET_TEXT(target) + u8" is unable to open.";
		break;
	}
	case DIRECTORY_LOAD_ERROR:
	{
		message = "Directory load error.";
		description = TARGET_TEXT(target) + u8" is unable to load.";
	}
	default:
		break;
	}
}
slib::sio::SIOException::~SIOException() {}

slib::String slib::sio::currentPath() {
	slib::String path;
#if defined(MAC_OS) || defined(UNIX_OS) || defined(LINUX_OS)
    SSystem::exec("pwd", path);
#else
	slib::SSystem::exec("cd", path);
	path = slib::String::toUTF8(path);
#endif
    path.trimming();
    return path;
}
bool slib::sio::fileExist(const char *p) {
    int res;
#if defined(WIN32_OS)
	struct _stat32 buf;
	res = _stat32(p, &buf);
	return !res;
#elif defined(WIN64_OS)
    struct _stat64 buf;
    res = _stat64(p, &buf);
    return !res;
#else
    struct stat buf;
    res = stat(p, &buf);
#endif
    return !res;
}
