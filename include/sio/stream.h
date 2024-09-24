#ifndef SLIB_STREAM_H
#define SLIB_STREAM_H
#include "sbasic/string.h"
#include "sbasic/array.h"
#include "sbasic/list.h"
#include "sbasic/map.h"
#include "sbasic/tuple.h"
#include "smath/la.h"
#include "sio/sfile.h"
/**
* @cond
*/
template<class Strm>
inline void _writeout1(Strm strm) {}
template<class First, class... Args>
inline void _writeout1(std::ostream *strm, const First& f, Args&... args) {
	(*strm) << f; strm->flush();
	_writeout1(strm, args...);
}
template<class First, class... Args>
inline void _writeout1(slib::String* strm, const First& f, Args&... args) {
	(*strm) << slib::toString(f);
	_writeout1(strm, args...);
}
template<class First, class... Args>
inline void _writeout1(slib::SFile* strm, const First& f, Args&... args) {
	(*strm) << slib::toString(f); strm->flush();
	_writeout1(strm, args...);
}
inline void _writeout2(std::ostream* strm) { (*strm) << slib::NL << std::flush; }
inline void _writeout2(slib::String *strm) { (*strm) << slib::NL; }
inline void _writeout2(slib::SFile* strm) { (*strm) << slib::NL; strm->flush(); }
template<class... Args>
inline void _writeout2(std::ostream* strm, const bool& b, Args&... args) {
	(*strm) << (b?"TRUE":"FALSE");
	_writeout2(strm, args...);
}
template<class First, class... Args>
inline void _writeout2(std::ostream* strm, const First& f, Args&... args) {
	(*strm) << f;
	_writeout2(strm, args...);
}
template<class First, class... Args>
inline void _writeout2(slib::String* strm, const First& f, Args&... args) {
	(*strm) << slib::toString(f);
	_writeout2(strm, args...);
}
template<class First, class... Args>
inline void _writeout2(slib::SFile* strm, const First& f, Args&... args) {
	(*strm) << slib::toString(f);
	_writeout2(strm, args...);
}
/**
* @endcond
*/
namespace slib {	
	/**
	* @class IOStream
	* \~english @brief Input and output stream class
	* \~japanese @brief IOストリームクラス
	*/
	class SLIB_DLL IOStream {
		union stream {
			std::istream* is;
			std::ostream* os;
			slib::String* ss;
			slib::SFile* fs;
			stream() { os = &std::cout; }
			~stream() {}
		};
		stream _strm;
		subyte _mode;
	public:
		IOStream();
		IOStream(std::istream& s);
		IOStream(std::ostream& s);
		IOStream(slib::String& s, const subyte m);
		IOStream(slib::SFile& s, const subyte m);
		~IOStream();

		void setStdIStream(std::istream& s);
		void setStdOStream(std::ostream& s);
		void setStrIStream(slib::String& s);
		void setStrOStream(slib::String& s);
		void setFileIStream(slib::SFile& s);
		void setFileOStream(slib::SFile& s);

		IOStream& operator>>(String& s);
		void read(String &s);
		void readLine(String& s);
		void readBytes(ubytearray& bytes);

		template<class... Args>
		void write(const Args&... args) {
			if (_mode & sio::STDIO) _writeout1(_strm.os, args...);
			else if (_mode & sio::STRIO) _writeout1(_strm.ss, args...);
			else if (_mode & sio::FILEIO) _writeout1(_strm.fs, args...);
		}
		void writeBytes(const subyte* b, const size_t sz);
		void writeBytes(const ubytearray &bytes);
		IOStream& operator<<(const char* s);
		template<class Cls>
		IOStream& operator<<(const Cls& cls) {
			if (_mode & sio::STDIO) (*_strm.os) << cls;
			else if (_mode & sio::STRIO) (*_strm.ss) << slib::toString(cls);
			else if (_mode & sio::FILEIO) (*_strm.fs) << slib::toString(cls);
			return *this; 
		}
		template<class... Args>
		void print(const Args&... args) { 
			if (_mode & sio::STDIO) _writeout2(_strm.os, args...);
			else if (_mode & sio::STRIO) _writeout2(_strm.ss, args...);
			else if (_mode & sio::FILEIO) _writeout2(_strm.fs, args...);
		}
		void flush();
	};
	/**
	* @cond
	*/
	extern SLIB_DLL IOStream DEFAULT_ISTREAM;
	extern SLIB_DLL IOStream DEFAULT_OSTREAM;

#define SRead(X) (slib::DEFAULT_ISTREAM).read(X)
#define SWrite(...) (slib::DEFAULT_OSTREAM).write(__VA_ARGS__)
#define SPrint(...) (slib::DEFAULT_OSTREAM).print(__VA_ARGS__)
	/**
	* @endcond
	*/

}
#endif