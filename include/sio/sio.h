#ifndef SLIB_SIO_H
#define SLIB_SIO_H

#include "sbasic.h"
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
#define INIT_DEFAULT_IO(X) auto IO_=slib::sio::siostream(X);
#define SET_STDIO_MODE IO_.setStdMode();
#define SET_FILEIO_MODE(X) IO_.setPath(X);
#define SRead(X) IO_.read(X)
#define SReadBytes(X) IO_.input(X)
#define SPrint(...) IO_.output(__VA_ARGS__)

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


                extern inline void _EXPAND_ARGS(SDictionary& params) {}
                template<class... REST>
                extern inline void _EXPAND_ARGS(SDictionary& params, const char *par, REST... rest) {
                        auto str = String(par);
                        auto pos = str.find("=");
                        if (pos == NOT_FOUND) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
                        auto key = str.substring(0, pos);
                        auto val = str.substring(pos + 1);
                        if (val.isQuoted()) params[key] = String::dequot(val);
                        else if (val.isNumeric()) params[key] = SNumber::toNumber(val);
                        else if (val == "null") params[key] = snull;
                        else if (String::lower(val) == "true" || String::lower(val) == "yes") params[key] = SNumber(true);
                        else if (String::lower(val) == "false" || String::lower(val) == "no") params[key] = SNumber(false);
                        else params[key] =val;
                        _EXPAND_ARGS(params, rest...);
                }
                template<class... Args>
                extern inline void EXPAND_ARGS(Args... args) {
                        SDictionary& params;
                        _EXPAND_ARGS(params, args...);
                }

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

                extern inline String currentPath() {
                        String path;
#if defined(MAC_OS) || defined(UNIX_OS) || defined(LINUX_OS)
                        SSystem::exec("pwd", path);
#else
                        SSystem::exec("cd", path);
                        path = String::toUTF8(path);
#endif
                        path.trimming();
                        return path;
                }
                extern inline bool fileExist(const char* s) {
                        int res;
#if defined(WIN32_OS)
                        struct _stat32 buf;
                        res = _stat32(s, &buf);
                        return !res;
#elif defined(WIN64_OS)
                        struct _stat64 buf;
                        res = _stat64(s, &buf);
                        return !res;
#else
                        struct stat buf;
                        res = stat(s, &buf);
                        return !res;
#endif
                }
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