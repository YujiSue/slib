#include "sbasic/style.h"
#include "sutil/sjson.h"
#include "sapp/sapp.h"
slib::SAppException::SAppException() : slib::Exception() { prefix = "App"; }
slib::SAppException::SAppException(int i, const char* msg, const char* desc) : slib::Exception(i, msg, desc) { prefix = "App"; }
slib::SAppException::SAppException(slib::Exception& ex) : slib::Exception(ex) { prefix = "App"; }
slib::SAppException::SAppException(slib::Response& res) : slib::Exception(res.code, "Exec error.", res.error) { prefix = "App"; }
slib::SAppException::~SAppException() {}
#ifdef _WINDLL
/*
slib::String slib::SP = { ' ' };
slib::String slib::CR = { '\r' };
slib::String slib::LF = { '\n' };
slib::String slib::CRLF = { '\r', '\n' };
slib::String slib::TAB = { '\t' };
slib::String slib::DEL = { '\b' };
slib::String slib::NL = slib::CRLF;
*/
slib::IOStream slib::DEFAULT_ISTREAM = slib::IOStream(std::cin);
slib::IOStream slib::DEFAULT_OSTREAM = slib::IOStream(std::cout);
#endif
/*
 * SApp class definition
 */
slib::sapp::SApp::SApp() {}
slib::sapp::SApp::SApp(const char* prof, const char* format) : slib::sapp::SApp() {
	auto f = String(format);
	if (f == "json") profile = sjson::parse(prof);
}
slib::sapp::SApp::SApp(SDictionary&& prof) : slib::sapp::SApp() {
	profile.swap(prof);
}
slib::sapp::SApp::~SApp() {}

slib::sapp::SLogger::SLogger(subyte m, const char* path) {
	_mode = m;
	open(path);
}
slib::sapp::SLogger::~SLogger() { if (_file.isOpened()) _file.close(); }
void slib::sapp::SLogger::open(const char* path) {
	if (!path) return;
	_mode |= slib::sio::FILEIO;
	_file.open(path, sio::APPEND);
}
void slib::sapp::SLogger::close() { _file.close(); }
void slib::sapp::SLogger::log(const String& msg) {
	SAutoLock al(_lock);
	SDate date;
	if (_mode & slib::sio::STDIO) SPrint("[", date.toString(sstyle::ISO8601), "]", SP, msg);
	if (_mode & slib::sio::FILEIO) {
		if (_file.isOpened()) {
			_file << "[" << date.toString(sstyle::ISO8601) << "]" << SP << msg << NL;
			_file.flush();
		}
	}
}
void slib::sapp::SLogger::log(const Exception& ex) {
	SAutoLock al(_lock);
	SDate date;
	if (_mode & slib::sio::STDIO) {
		SPrint("[", date.toString(sstyle::ISO8601), "]", SP, stext::RED_TAG, ex.prefix, " error. #", ex.code, stext::DEFAULT_COLOR, SP, ex.message, NL, ex.description);
	}
	if (_mode & slib::sio::FILEIO) {
		if (_file.isOpened()) {
			_file << "[" << date.toString(sstyle::ISO8601) << "]" << SP << ex.prefix << " error. #" << ex.code << SP << ex.message << NL << ex.description << NL;
			_file.flush();
		}
	}
}
