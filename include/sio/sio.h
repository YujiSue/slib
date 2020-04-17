#ifndef SLIB_SIO_H
#define SLIB_SIO_H

#include "sbasic/sbasic.h"
#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sdate.h"
#include "sobj/sdata.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"
#include "sobj/stable.h"
#include "sobj/stext.h"
#include "sutil/ssys.h"
extern "C" {
#include "zlib/zlib.h"
}

namespace slib {
    namespace sio {
        
		constexpr suint FILE_NOT_EXIST_ERROR = 0x0011;
		constexpr suint FILE_OPEN_ERROR = 0x0012;
		constexpr suint DIRECTORY_LOAD_ERROR = 0x0013;

        class SLIB_DLL SIOException : public SException {
        public:
            SIOException(const char* f, sint l, const char* func, sint e = 0, const char* target = nullptr, const char* note = nullptr);
            ~SIOException();
        };

		#ifdef WIN_OS
		#define ROOT_PATH String("C:\\")
		#define ROOT_DRIVE(X) String(X)+":\\"
		#define PATH_SEPARATOR String("\\")
		#else
        #define ROOT_PATH String("/")
        #define PATH_SEPARATOR String("/")
        #endif
        
        #define SIO_BUFFER_SIZE 256
		#ifdef WIN_OS
		#define HOME_PATH String(getenv("USERPROFILE"))
		#else
        #define HOME_PATH String(getenv("HOME"))
		#endif
        #define CURRENT_PATH currentPath()
        
		//File type
		constexpr sushort BINARY_FILE = 0x0000;

		//Text 0x0010
		constexpr sushort TEXT_FILE = 0x0010;
		constexpr sushort RTF_FILE = 0x0011;
		constexpr sushort CSV_FILE = 0x0012;
		constexpr sushort TSV_FILE = 0x0013;

		//Data 0x0020
		constexpr sushort JSON_FILE = 0x0021;

		//Image 0x0040
		constexpr sushort RAW = 0x0041;
		constexpr sushort PICT = 0x0042;
		constexpr sushort BMP = 0x0043;
		constexpr sushort TIFF = 0x0044;
		constexpr sushort JPEG = 0x0045;
		constexpr sushort PNG = 0x0046;

		//Sound 0x0080


		//Movie 0x00C0


		//3D 0x100
		constexpr sushort OBJ_FILE = 0x0101;

		//Archived 0x800
		constexpr sushort ZIP_FILE = 0x0801;


		constexpr sushort EXE_FILE = 0x0821;
		constexpr sushort PKG_FILE = 0x0822;

		//Program 0x1000
		//Script 0x1100
		//Compile 0x1200
		//Interpret 0x1400
		//Hybrid 0x1600
		//ML 0x1800
		constexpr sushort XML_FILE = 0x1801;
		constexpr sushort HTML_FILE = 0x1811;
		constexpr sushort XHTML_FILE = 0x1812;
		constexpr sushort MD_FILE = 0x1821;

		constexpr sushort PLIST_FILE = 0x180A;
		//constexpr sushort SOML_FILE = 0x180F;

		constexpr sushort SVG_FILE = 0x1841;


        class SLIB_DLL SFile;
        #define filearray Array<slib::sio::SFile>
        
        constexpr subyte READ = 0x01;
		constexpr subyte WRITE = 0x02;
		constexpr subyte CREATE = 0x04;
		constexpr subyte APPEND = 0x08;
        constexpr subyte DIRECTORY = 0x10;
        
		constexpr subyte OVERWRITE = 0x01;
        constexpr subyte CANCEL_IFEXIST = 0x02;
        constexpr subyte BACKUP_ORI = 0x04;
        
        extern String currentPath();
        extern bool fileExist(const char *s);
    }

	template<typename T, class M>
	extern inline std::ostream& operator<<(std::ostream& os, const Array<T, M>& array) { return os << toString(array); }
	template<typename T, size_t S, class M>
	extern inline std::ostream& operator<<(std::ostream& os, const FixedArray<T, S, M>& array) { return os << toString(array); }
	template<typename T, class M>
	extern inline std::ostream& operator<<(std::ostream& os, const BiArray<T, M>& array) { return os << toString(array); }
	template<class Key, class Val>
	extern inline std::ostream& operator<<(std::ostream& os, const Map<Key, Val>& map) { return os << toString(map); }

    extern inline std::ostream &operator<<(std::ostream &os, const Char &c) { 
#if defined(WIN32_OS) || defined(WIN64_OS)
		return os << c.toString().localize().cstr();
#else
		return os << c.toString().cstr();
#endif
	}
	extern inline std::ostream &operator<<(std::ostream &os, const String & str) {
#if defined(WIN32_OS) || defined(WIN64_OS)
		return os << str.localize().cstr();
#else
		return os << str.cstr();
#endif
	}
	extern inline std::istream& operator>>(std::istream& is, String& str) {
		is.seekg(0, std::ios::end);
		size_t size = is.tellg();
		is.clear();
		is.seekg(0);
#if defined(WIN32_OS) || defined(WIN64_OS)
		char* buf = new char[size + 1];
		is.read(buf, size);
		buf[size] = 0;
		str = String::toUTF8(buf);
#else
		str.resize(size);
		return is.read(str.ptr(), size);
#endif
		return is;
	}

    extern inline std::ostream &operator<<(std::ostream &os, const SNumber &num) { return os << num.toString(); }
	extern inline std::ostream &operator<<(std::ostream &os, const SChar& c) { return os << c.toString(); }
	extern inline std::ostream &operator<<(std::ostream &os, const SString &str) {
#if defined(WIN32_OS) || defined(WIN64_OS)
		return os << str.localize().cstr();
#else
		return os << str.cstr();
#endif
	}
    extern inline std::ostream &operator<<(std::ostream &os, const SDate &date) { return os << date.toString(); }
    extern inline std::ostream &operator<<(std::ostream &os, const SData &data) { return os << data.toString(); }
    extern inline std::ostream &operator<<(std::ostream &os, const SArray &array) { return os << array.toString(); }
    extern inline std::ostream &operator<<(std::ostream &os, const SDictionary &dict) { return os << toString(dict); }
    extern inline std::ostream &operator<<(std::ostream &os, const SText &text) {
		if (text.attributes().empty()) os << text.cstr();
		else {
			auto& attr = text.attributes();
			if (0 < attr[0].first.begin) os << text.string().substring(srange(0, attr[0].first.begin));
			sforeach(attr) {
				if (E_.second.type & sstyle::PLAIN) os << PLAIN_TXT_TAG;
				if (E_.second.type & sstyle::BOLD) os << BOLD_TXT_TAG;
				if (E_.second.type & sstyle::ITALIC) os << ITALIC_TXT_TAG;
				if (E_.second.type & sstyle::UNDERLINE) os << UNDERLINE_TXT_TAG;
				if (E_.second.color == "black") os << DEFAULT_COLOR_TXT_TAG;
				if (E_.second.color == "red") os << RED_TXT_TAG;
				if (E_.second.color == "green") os << GREEN_TXT_TAG;
				if (E_.second.color == "yellow") os << YELLOW_TXT_TAG;
				if (E_.second.color == "blue") os << BLUE_TXT_TAG;
				if (E_.second.color == "magenta") os << MAGENTA_TXT_TAG;
				if (E_.second.color == "cyan") os << CYAN_TXT_TAG;
				if (E_.second.color == "white") os << WHITE_TXT_TAG;
				if (E_.second.background == "black") os << DEFAULT_COLOR_TXTBG_TAG;
				if (E_.second.background == "red") os << RED_TXTBG_TAG;
				if (E_.second.background == "green") os << GREEN_TXTBG_TAG;
				if (E_.second.background == "yellow") os << YELLOW_TXTBG_TAG;
				if (E_.second.background == "blue") os << BLUE_TXTBG_TAG;
				if (E_.second.background == "magenta") os << MAGENTA_TXTBG_TAG;
				if (E_.second.background == "cyan") os << CYAN_TXTBG_TAG;
				if (E_.second.background == "white") os << WHITE_TXTBG_TAG;
				os << text.string().substring(srange(E_.first.begin, E_.first.end));
				os << DEFAULT_TXT_TAG;
				if (it < attr.end() - 1)
					os << text.string().substring(srange(E_.first.end, (it + 1)->first.begin));
			}
			os << DEFAULT_TXT_TAG;
			if (attr.last().first.end < text.string().length() - 1) os << text.string().substring(attr.last().first.end);
		}
		return os;
	}
	extern inline std::ostream& operator<<(std::ostream& os, const STable& table) { return os << table.toString(); }
    template<class Return, class... Args>
	extern inline std::ostream& operator<<(std::ostream& os, const SFunction<Return(Args...)>& func) { return os << func.toString(); }
	extern inline std::ostream& operator<<(std::ostream& os, const SObjPtr& obj) { return os << obj.toString(); }
}
#endif