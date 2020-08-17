#include "sapp/sapp.h"

using namespace slib;
using namespace slib::sapp;

SAppException::SAppException(const char* f, sint l, const char* func, sint e, const char* target, const char* note) 
	: SException(f, l, func, e, target, note) { 
	prefix = "sapp"; 
	switch (e) {
	case INSUFFICIENT_ARGS_ERROR:
	{
		message = "Insufficient argment error.";
		description = TARGET_TEXT(target) + u8" requires " + String(note);
		break;
	}
	case INSUFFICIENT_OPT_ERROR:
	{
		message = "Insufficient option error.";
		description = TARGET_TEXT(target) + u8" requires " + String(note);
		break;
	}
	case COMMAND_NOT_EXIST_ERROR:
	{
		message = "Command error.";
		description = TARGET_TEXT(target) + u8" is not defined. ";
		break;
	}
	case OPTION_NOT_EXIST_ERROR:
	{
		message = "Option error.";
		description = TARGET_TEXT(target) + u8" is not defined. ";
		break;
	}
	default:
		break;
	}

}
SAppException::~SAppException() {}

inline String codeStr(sint code) {
	/*
	if (code & SLIB_LOG_CODE) return "log";
	else if (code & SLIB_LAUNCH_CODE) return "launched";
	else if (code & SLIB_TERMINATE_CODE) return "terminated";
	else if (code & SLIB_ERROR_CODE) return "error";
	else if (code & SLIB_WARNING_CODE) return "warning";
	*/
	return "";
}
log_data::log_data(sint c, const char* s) {
	date = SDate(slib::sstyle::YMDHMS);
	code = c;
	msg = s;
}
log_data::~log_data() {}
SLogger::SLogger(const char* path) {
	if (path) open(path);
}
SLogger::~SLogger() { if (_file.isOpened()) _file.close(); }
void SLogger::open(const char* path) {
	if (sio::fileExist(path)) _file = sio::SFile(path, sio::APPEND);
	else _file = sio::SFile(path, sio::CREATE);
}
void SLogger::close() {
	_file.close();
}
void SLogger::log(sint code, const char* msg) {
	_lock.lock();
	SDate date(slib::sstyle::YMDHMS);
	if (_file.isOpened()) {
		_file << date.toString() << "[" << codeStr(code) << "]" << TAB << msg << NEW_LINE;
		_file.flush();
	}
	else _data.add(log_data(code, msg));
	_lock.unlock();
}

SApp::SApp() {}
SApp::SApp(const char *path) : SApp() {
    try { profile.load(path); }
	catch (sio::SIOException ie) { ie.print(); }
}
SApp::SApp(SDictionary &&prof) : SApp() {
    profile.swap(prof);
}
SApp::~SApp() {}
