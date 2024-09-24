#include "sio/stream.h"
#ifdef WIN_OS
extern "C" {
	extern auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
	extern DWORD mode = 0;
	extern auto console_get = GetConsoleMode(handle, &mode);
	extern auto console_set = SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif
slib::IOStream::IOStream() { setStdOStream(std::cout); }
slib::IOStream::IOStream(std::istream& s) { setStdIStream(s); }
slib::IOStream::IOStream(std::ostream& s) { setStdOStream(s); }
slib::IOStream::IOStream(slib::String& s, const subyte m) {
	if (m & sio::ISTREAM) setStrIStream(s);
	else setStrOStream(s);
}
slib::IOStream::IOStream(slib::SFile& s, const subyte m) {
	if (m & sio::ISTREAM) setFileIStream(s);
	else setFileOStream(s);
}
slib::IOStream::~IOStream() {}
void slib::IOStream::setStdIStream(std::istream& s) { _mode = sio::ISTREAM | sio::STDIO; _strm.is = &s; }
void slib::IOStream::setStdOStream(std::ostream& s) { _mode = sio::OSTREAM | sio::STDIO; _strm.os = &s; }
void slib::IOStream::setStrIStream(slib::String& s) { _mode = sio::ISTREAM | sio::STRIO; _strm.ss = &s; }
void slib::IOStream::setStrOStream(slib::String& s) { _mode = sio::OSTREAM | sio::STRIO; _strm.ss = &s; }
void slib::IOStream::setFileIStream(slib::SFile& s) { _mode = sio::ISTREAM | sio::FILEIO; _strm.fs = &s; }
void slib::IOStream::setFileOStream(slib::SFile& s) { _mode = sio::OSTREAM | sio::FILEIO; _strm.fs = &s; }

slib::IOStream& slib::IOStream::operator>>(slib::String& s) {
	if (_mode & sio::ISTREAM) {
		if (_mode & sio::STDIO) { (*_strm.is) >> s; }
		else if (_mode & sio::STRIO) { s << (*_strm.ss); }
		else if (_mode & sio::FILEIO) { (*_strm.fs) >> s; }
	}
	else throw UndefinedException(String("IOStream class operator '>>' is defined for only input stream. But the current mode is '") << _mode << "'.");
	return *this;
}
void slib::IOStream::read(slib::String& s) { *this >> s; }
void slib::IOStream::readLine(String& s) {
	if (_mode & sio::ISTREAM) {
		if (_mode & sio::STDIO) { (*_strm.is) >> s; }
		else if (_mode & sio::STRIO) { 
			auto c = _strm.ss->cstr();
			while (true) {
				if (*c == '\r' || *c == '\n') break;
				s << *c;
				++c;
			}
			_strm.ss->clip(c - _strm.ss->cstr());
		}
		else if (_mode & sio::FILEIO) { _strm.fs->readLine(s, true); }
	}
	else throw UndefinedException(String("IOStream class function 'readLine' is defined for only input stream. But the current mode is '") << _mode << "'.");
}
void slib::IOStream::readBytes(ubytearray& bytes){}
slib::IOStream& slib::IOStream::operator<<(const char* s) {
	if (_mode == (sio::OSTREAM | sio::STDIO)) {
		(*_strm.os) << s;
	}
	else if (_mode == (sio::OSTREAM | sio::STRIO)) {
		(* _strm.ss) << s;
	}
	else if (_mode == (sio::OSTREAM | sio::FILEIO)) {
		(*_strm.fs) << s;
	}
	return *this;
}
void slib::IOStream::writeBytes(const subyte* b, const size_t sz) {
	if (_mode == (sio::OSTREAM | sio::STDIO)) { 
		String str(sz, '\0');
		Memory<char>::copy(&str[0], (const char*)b, sz);
		(*_strm.os) << str;
	}
	else if (_mode == (sio::OSTREAM | sio::STRIO)) { 
		auto sz_ = _strm.ss->size();
		_strm.ss->resize(sz_ + sz);
		Memory<char>::copy(&_strm.ss->at((int)sz_), (const char*)b, sz);
	}
	else if (_mode == (sio::OSTREAM | sio::FILEIO)) { 
		_strm.fs->writeBytes(b, sz);
	}
}
void slib::IOStream::writeBytes(const ubytearray& bytes) { writeBytes(bytes.data(), bytes.size()); }
void slib::IOStream::flush() {
	if (_mode == (sio::OSTREAM | sio::STDIO)) { (*_strm.os) << std::flush; }
	else if (_mode == (sio::OSTREAM | sio::FILEIO)) { (*_strm.fs).flush(); }
}
slib::IOStream slib::DEFAULT_ISTREAM = slib::IOStream(std::cin);
slib::IOStream slib::DEFAULT_OSTREAM = slib::IOStream(std::cout);