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

slib::sio::siostream::siostream(int i) :_mode(i) {}
slib::sio::siostream::siostream(const char* s) :_mode(1) { _file = sio::SFile(s, sio::READ | sio::CREATE); }
slib::sio::siostream::~siostream() {}
void slib::sio::siostream::setStdMode() { _mode = 0; }
void slib::sio::siostream::setPath(const char* s) { _mode = 1; _file = sio::SFile(s, sio::READ | sio::CREATE); }
void slib::sio::siostream::read(slib::String& s) {
	if (_mode) _file >> s;
	else std::cin >> s;
}

void slib::sio::siostream::input(ubytearray& a) {
	if (_mode) { a.resize(_file.size()); _file.readBytes(a.ptr(), _file.size()); }
	else std::cin >> a;
}
void slib::sio::siostream::_output() {
	if (_mode) { _file << NEW_LINE; _file.flush(); }
	else std::cout << std::endl;
}