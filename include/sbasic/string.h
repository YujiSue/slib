#ifndef SLIB_STRING_H
#define SLIB_STRING_H

#include "sbasic/container.h"
#include "sbasic/range.h"
#include "sbasic/area.h"
#include "sbasic/zone.h"
#include "sbasic/u8iter.h"

namespace slib {
    class SLIB_DLL SElement;
    class SLIB_DLL SObjPtr;
    /**
    * @class Regex
    * \~english @brief Regular expression class
    * \~japanese @brief 正規表現クラス
    */
    class SLIB_DLL Regex {
    private:
        std::regex _rgx;
    public:
        bool global;
    public:
        Regex();
        Regex(const char* s);
        ~Regex();
        Regex& operator=(const char *s);
        Regex& operator=(const Regex& reg);
        const std::regex &context() const;
        bool match(const char* s) const;
        bool equal(const char* s) const;
        size_t find(const char* s, size_t offset = 0, bool rev = false) const;
        Array<size_t> findAll(const char* s) const;
        Range<std::cregex_iterator> search(const char* s, size_t offset = 0) const;
    };

#define SHORT_STRING_CAPACITY 23
    /**
    * @class String
    * \~english @brief Text string class
    * \~japanese @brief 文字列クラス
    */
    class SLIB_DLL String {
        friend Regex;
    private:
        /**
        * @cond
        */
        struct SLIB_DLL short_string {
            sbyte size;
            char ptr[SHORT_STRING_CAPACITY];
            short_string();
            ~short_string();
            void copyTo(short_string& dest);
        };
        struct SLIB_DLL long_string {
            size_t capacity;
            size_t size;
            char* ptr;
            long_string();
            ~long_string();
        };
        union SLIB_DLL uni_string {
            short_string ss;
            long_string ls;
            uni_string() : ss() {}
            ~uni_string() {
                if ((reinterpret_cast<subyte*>(this))[0] & 0x01) ls.~long_string();
                else ss.~short_string();
            }
        };
    protected:
        uni_string _str;
    protected:
        bool _isLong() const;
        char* _begin() const;
        char* _end() const;
        /**
        * @endcond
        */
    public:
#ifdef WIN_OS
        static String toUTF8(const wchar_t* ws);
        static String toUTF8(const char* s);
#endif  
    public:
        String();
        String(const bool n);
        String(const int n);
        String(const size_t n);
        //String(const long long n);
        String(const int64_t n);
#ifdef MAC_OS
        //String(const sinteger n);
        String(const suinteger n);
#endif
        String(const float n);
        String(const double n);
        String(const Char& c);
        String(const char* s);
        String(const std::string& s);
#if defined(WIN_OS)
        //String(const wchar_t* ws);
        //String(const std::wstring &ws);
#endif
        String(size_t sz, const char c);
        String(std::initializer_list<char> li);
        String(String&& s) noexcept;
        String(const String& s);
        String(const SElement& elem);
        String(const SObjPtr& obj);
        ~String();

        String& operator=(String&& s) noexcept;
        String& operator=(const String& s);
        String& operator=(const SElement& e);
        String& operator=(const SObjPtr& o);
        String& operator=(const char* s);

        String& operator+=(const char* s);
        String& operator+=(const String& s);
        String& operator+=(const SElement& e);
        String& operator+=(const SObjPtr& o);
        String& operator*=(const int i);

        String& operator<<(const char c);
        String& operator<<(const int n);
        String& operator<<(const int64_t n);
        String& operator<<(const size_t n);
        String& operator<<(const float n);
        String& operator<<(const double n);
        String& operator<<(const char* s);
        String& operator<<(const Char& c);
        String& operator<<(const String& s);
        String& operator<<(const SElement& e);
        String& operator<<(const SObjPtr& o);
        
        String operator+(const char c) const;
        String operator+(const char* s) const;
        String operator+(const String& s) const;
        String operator+(const SElement& e) const;
        String operator+(const SObjPtr& o) const;
        String operator*(const int i) const;
        
        bool empty() const;
        size_t size() const;
        size_t length() const;
        size_t capacity() const;
        void reserve(const size_t s);
        void resize(const size_t s);
        void resize(const size_t s, const char c);

        ArrayIterator<char> iterAt(const int i);
        ArrayCIterator<char> iterAt(const int i) const;
        char& at(const int i);
        const char& at(const int i) const;
        char& operator[](const int i);
        const char& operator[](const int i) const;
        Utf8Iterator u8iterAt(const int i);
        Utf8CIterator u8iterAt(const int i) const;
        Char charAt(const int i) const;
        bool beginWith(const char* que) const;
        bool endWith(const char* que) const;

        ArrayIterator<char> begin();
        ArrayCIterator<char> begin() const;
        ArrayIterator<char> end();
        ArrayCIterator<char> end() const;
        Utf8Iterator u8begin();
        Utf8CIterator u8begin() const;
        Utf8Iterator u8end();
        Utf8CIterator u8end() const;

        void add(const char c);
        void append(const char* s);
        void append(const std::string& s);
        void append(const String& s);
        void insert(const int i, const char* s);
        void insert(const int i, const String& s);
        void removeAt(const int i);
        void remove(const size_t off, const size_t len = -1);
        void remove(const srange& range);
        void clear();
        void swap(String& str);

        String substring(const size_t off, size_t length = -1) const;
        String substring(const srange range) const;
        String& trim();
        String& clip(const size_t off, const size_t len = -1);
        String& clip(const srange &range);
        String& replace(const size_t off, const size_t len, const char* wrd);
        String& replace(const char* ori, const char* wrd);
        String& replace(const Regex& reg, const char* wrd);
        Array<String> split(const char* sep, const bool trim = true, const bool ignore_quot = false, const bool dequote = true) const;
        Array<String> split(const Regex& reg) const;
        Array<String> splitLine(const bool trim = true, const bool ignore_quot = false) const;
        Map<String, String> parse(const char* sep, const char* delim, const bool trim = true, const bool ignore_quot = false, bool dequote = true) const;

        size_t count(const char* que) const;
        size_t find(const char* que, const size_t offset = 0) const;
        size_t find(const Regex& reg, const size_t offset = 0) const;
        size_t rfind(const char* que, const size_t offset = 0) const;
        size_t rfind(const Regex& reg, const size_t offset = 0) const;
        Array<size_t> findAll(const char* que) const;
        Array<size_t> findAll(const Regex& reg) const;
        bool match(const char* que, const size_t offset = 0) const;
        bool match(const Regex& reg) const;
        bool equal(const Regex& reg) const;
        Pair<size_t, String> search(const Regex& reg) const;
        Array<Pair<size_t, String>> searchAll(const Regex& reg) const;

        bool boolean() const;
        sbyte byteValue() const;
        subyte ubyteValue() const;
        sshort shortValue() const;
        sushort ushortValue() const;
        int intValue() const;
        unsigned int uintValue() const;
        size_t sizeValue() const;
        long longValue() const;
        unsigned long ulongValue() const;
        long long llongValue() const;
        unsigned long long ullongValue() const;
        float floatValue() const;
        double doubleValue() const;
        sinteger integer() const;
        suinteger uinteger() const;
        sreal real() const;
        const char* cstr() const;
        const std::string toStr() const;
        bool isUtf8() const;
#if defined(WIN_OS)
        std::wstring unicode() const;
        String localize() const;
#endif  
        operator const char*() const;

        bool operator<(const char* s) const;
        bool operator<(const String& s) const;
        bool operator==(const char* s) const; 
        bool operator==(const String& s) const;
        bool operator==(const SObjPtr& o) const;
        bool operator!=(const char* s) const;
        bool operator!=(const String& s) const;

    };
    /**
    * @cond
    */
    extern SLIB_DLL slib::String operator+(const char* s, const slib::String& str);
    extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::Char& ch);
    extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::String& str);
    extern SLIB_DLL std::istream& operator>>(std::istream& is, slib::String& str);

    template<class C1, class C2>
    extern String toString(const slib::Pair<C1, C2>& pair, const char* format = nullptr) {
        String str;
        str << pair.first << ":" << pair.second;
        return str;
    }
    template<typename T>
    extern String toString(const slib::Range<T>& range, const char* format = nullptr) {
        String str;
        str << "(" << range.begin << "," << range.end << ")";
        return str;
    }
    template<typename T>
    extern String toString(const slib::Area<T>& area, const char* format = nullptr) {
        String str;
        str << "(" << area.ori_x << "," << area.ori_y << "," << area.width << "," << area.height << ")";
        return str;
    }
    template<typename T>
    extern String toString(const slib::Zone<T>& zone, const char* format = nullptr) {
        String str;
        str << "(" << zone.ori_x << "," << zone.ori_y << "," << zone.ori_z << "," << zone.width << "," << zone.height << "," << zone.depth << ")";
        return str;
    }
    extern SLIB_DLL String toString(const char c, const char* format = nullptr);
    extern SLIB_DLL String toString(const char* s, const char* format = nullptr);
    extern SLIB_DLL String toString(const String& str, const char* format = nullptr);
    /**
    * @endcond
    */
}
using stringarray = slib::Array<slib::String>;
/**
* @cond
*/
namespace std {
    template<>
    struct hash<slib::String> {
        size_t operator()(const slib::String& s) const {
            return hash<std::string>{}(s.toStr());
        }
    };
}
/**
* @endcond
*/
#define S(X) slib::String(X)
#define S_(X) slib::String(#X)
#define REG(X) slib::Regex(X)
#define REG_(X) slib::Regex(#X)

namespace slib {
    namespace sstr {
        extern SLIB_DLL bool isQuoted(const char* s);
        extern SLIB_DLL bool isEnclosed(const char* s, const char* bracket);
        extern SLIB_DLL bool isNumeric(const char* s);
    
        extern SLIB_DLL String trim(const char* s);
        extern SLIB_DLL String squote(const char* s);
        extern SLIB_DLL String dquote(const char* s);
        extern SLIB_DLL String dequote(const char* s, bool check = true);
        extern SLIB_DLL String enclose(const char* s, const char* bracket);
        extern SLIB_DLL String fill(const char* s, const char c, const size_t sz, DIRECTION dir);
        extern SLIB_DLL String lfill(const char* s, const char c, const size_t sz);
        extern SLIB_DLL String rfill(const char* s, const char c, const size_t sz);
        extern SLIB_DLL String bfill(const char* s, const char c, const size_t sz);

        extern SLIB_DLL String toWide(const char* s);
        extern SLIB_DLL String toNarrow(const char* s);
        extern SLIB_DLL String toLower(const char* s);
        extern SLIB_DLL String toUpper(const char* s);
        
        constexpr char alphabet[32] = "abcdefghijklmnopqrstuvwxyz";
        constexpr char ALPHABET[32] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        constexpr char arabian[11] = "0123456789";
        constexpr char hex[17] = "0123456789abcdef";
        constexpr char HEX[17] = "0123456789ABCDEF";

        extern SLIB_DLL String greek(const size_t s, bool upper = false);
        extern SLIB_DLL String roman(const size_t s, bool upper = false);
    }
    extern SLIB_DLL String SP;
    extern SLIB_DLL String CR;
    extern SLIB_DLL String LF;
    extern SLIB_DLL String CRLF;
    extern SLIB_DLL String TAB;
    extern SLIB_DLL String DEL;
    extern SLIB_DLL String NL;
}


#endif
