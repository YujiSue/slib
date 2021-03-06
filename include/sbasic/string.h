#ifndef SLIB_STRING_H
#define SLIB_STRING_H

#include "sbasic/memory.h"
#include "sbasic/range.h"
#include "sbasic/sutf8.h"

namespace slib {
	template<typename T>
	class SLIB_DLL SArrayIterator;
	template<typename T>
	class SLIB_DLL SArrayCIterator;
	template<typename T, class M>
	class SLIB_DLL Array;
	template<typename T>
	class SLIB_DLL CArray;
	template<typename T, size_t S, class M>
	class SLIB_DLL FixedArray;
	template<typename T, class M>
	class SLIB_DLL BiArray;
	template<class Key, class Val>
	class SLIB_DLL Map;
	template<typename T>
	class SLIB_DLL Region;
    class SLIB_DLL String;
    class SLIB_DLL SNumber;
    class SLIB_DLL SString;
	class SLIB_DLL SText;
    class SLIB_DLL SObject;
    class SLIB_DLL SObjPtr;
	namespace sio {
		class SLIB_DLL SFile;
	}
    class SLIB_DLL Regex {
    private:
        std::regex _rgx;
        bool _global;
    public:
        Regex();
        Regex(const char *s);
        ~Regex();
        bool match(const char *s) const;
        bool equal(const char *s) const;
        void search(CArray<size_t>&array, const char *s, const char *e) const;
        void search(Array<String, SMemory<String>>&array, const char *s, const char *e) const;
        void split(Array<String, SMemory<String>> &array, const String *str) const;
        String replace(const char *s, const char *alt) const;
        String rearrange(const char *s, const CArray<sint> &order) const;
    };

    class SLIB_DLL String {
        friend Char;
    public:
        static String trim(const char *s);
        static String squot(const char *s);
        static String dquot(const char *s);
        static String dequot(const char *s);
        static String upper(const char *s);
        static String lower(const char *s);
        static String enclose(const char *s, const char *c1, const char *c2);
        static String wide(const char *s);
        static String narrow(const char *s);
#ifdef WIN_OS
		static String toUTF8(const wchar_t* ws);
		static String toUTF8(const char* s);
#endif  
		static const subyte HEAD_PART = 0x01;
		static const subyte TAIL_PART = 0x02;
		static const subyte BOTH_PART = 0x03;
    private:
        struct short_string {
            sbyte size;
            char str[SHORT_STRING_CAPACITY];
        };
        struct long_string {
            size_t capacity;
            size_t size;
            char *str;
        };
        union string {
            short_string _ss;
            long_string _ls;
        };
        
    protected:
        string _str;
        
    protected:
        bool _isLong() const;
        std::pair<char *, size_t> _instance();
        std::pair<const char *, size_t> _cinstance() const;
        void _append(const char *s, size_t l);
        void _insert(sinteger idx, const char *s, size_t l);
        const char *_find(const char *que, size_t s, const char *current, const char *end) const;
        const char *_rfind(const char *que, size_t s, const char *begin, const char *current) const;
        
    public:
        String();
        String(bool b);
        String(int i);
        String(unsigned int ui);
        String(size_t ui);
#ifdef WIN64_OS
        String(long i);
#ifndef MAC_OS
        String(unsigned long ui);
#endif
#endif
        String(long long i);
#ifdef MAC_OS
        String(unsigned long long ui);
#endif
#ifdef LINUX_OS
        String(sinteger i);
#endif
        String(float f);
        String(double d);
        String(sbyte i);
        String(subyte i);
        String(sshort i);
        String(sushort i);
        String(size_t s, const char &c);
        String(char c);
        String(const Char &c);
        String(const char *s);
        String(const std::string &s);
#if defined(WIN32_OS) || defined(WIN64_OS)
		//String(const wchar_t* ws);
		//String(const std::wstring &ws);
#endif
		String(std::initializer_list<char> li);
        String(const String &s);
        String(String &&s);
        String(const SString &s);
        String(const SObjPtr &obj);
        virtual ~String();
        String &operator=(bool b);
        String &operator=(int i);
        String &operator=(unsigned int ui);
        String &operator=(size_t ui);
#ifdef WIN64_OS
        String &operator=(long i);
#ifndef MAC_OS
        String &operator=(unsigned long ui);
#endif
#endif
        String &operator=(long long i);
#ifdef MAC_OS
        String &operator=(unsigned long long ui);
#endif
#ifdef LINUX_OS
        String &operator=(sinteger i);
#endif
        String &operator=(float f);
        String &operator=(double d);
        String &operator=(sbyte i);
        String &operator=(subyte ui);
        String &operator=(sshort i);
        String &operator=(sushort ui);
        String &operator=(char c);
        String &operator=(const char *s);
        String &operator=(const std::string &s);
        String &operator=(const Char &c);
        String &operator=(String &&s);
        String &operator=(const String &s);
        String &operator=(const SString &s);
        String &operator=(SObjPtr obj);
        
        String &operator+=(bool b);
        String &operator+=(int i);
        String &operator+=(unsigned int ui);
        String &operator+=(size_t ui);
#ifdef WIN64_OS
        String &operator+=(long i);
#ifndef MAC_OS
        String &operator+=(unsigned long ui);
#endif
#endif
        String &operator+=(long long i);
#ifdef MAC_OS
        String &operator+=(unsigned long long ui);
#endif
#ifdef LINUX_OS
        String &operator+=(sinteger i);
#endif
        String &operator+=(float f);
        String &operator+=(double d);
        String &operator+=(sbyte i);
        String &operator+=(subyte ui);
        String &operator+=(sshort i);
        String &operator+=(sushort ui);
        String &operator+=(char c);
        String &operator+=(const char *s);
        String &operator+=(const std::string &s);
        String &operator+=(const String &s);
        String &operator+=(const SString &s);
        String &operator+=(SObjPtr obj);
        
        String operator+(bool b) const;
        String operator+(int i) const;
        String operator+(unsigned int ui) const;
        String operator+(size_t ui) const;
#ifdef WIN64_OS
        String operator+(long i) const;
#ifndef MAC_OS
        String operator+(unsigned long ui) const;
#endif
#endif
        String operator+(long long i) const;
#ifdef MAC_OS
        String operator+(unsigned long long ui) const;
#endif
#ifdef LINUX_OS
        String operator+(sinteger i) const;
#endif
        String operator+(float f) const;
        String operator+(double d) const;
        String operator+(sbyte i) const;
        String operator+(subyte ui) const;
        String operator+(sshort i) const;
        String operator+(sushort ui) const;
        String operator+(char c) const;
        String operator+(const char *s) const;
        String operator+(const std::string &s) const;
        String operator+(const String &s) const;
        String operator+(const SString &s) const;
        String operator+(SObjPtr obj) const;
        
        String &operator<<(bool b);
        String &operator<<(int i);
        String &operator<<(unsigned int ui);
        String &operator<<(size_t ui);
#ifdef WIN64_OS
        String &operator<<(long i);
#ifndef MAC_OS
        String &operator<<(unsigned long ui);
#endif
#endif
        String &operator<<(long long i);
#ifdef MAC_OS
        String &operator<<(unsigned long long ui);
#endif
#ifdef LINUX_OS
        String &operator<<(sinteger i);
#endif
		String& operator<<(float f);
		String& operator<<(double d);
		String& operator<<(sbyte i);
		String& operator<<(subyte ui);
		String& operator<<(sshort i);
		String& operator<<(sushort ui);
		String& operator<<(char c);
		String& operator<<(const char* s);
		String& operator<<(const std::string& s);
		String& operator<<(const String& s);
		String& operator<<(const SString& s);
		String& operator<<(const SText& t);
		String& operator<<(const sio::SFile& f);
		String& operator<<(SObjPtr obj);
		String& operator*=(int num);
		String& operator*=(size_t num);
		String operator*(int num) const;
		String operator*(size_t num) const;
        bool isNumeric() const;
        bool isQuoted() const;
        bool empty() const;
        size_t size() const;
        size_t length() const;
        size_t capacity() const;
        char *ptr(size_t idx = 0);
        const char *ptr(size_t idx = 0) const;
        const char *cstr() const;
        std::string toStr() const;
        char &operator[] (sinteger idx);
        const char &operator[] (sinteger idx) const;
        char &at(sinteger idx);
        const char &at(sinteger idx) const;
        char &first();
        const char &first() const;
        char &last();
        const char &last() const;
		void interpret(subyte* bytes, size_t size);
        void copy(const char *dat, size_t size = -1);
		void swap(String& str);
		void reserve(size_t s);
		virtual void resize(size_t s);
		virtual void resize(size_t s, const char& c);
		virtual void clear();
        SArrayIterator<char> begin();
        SArrayCIterator<char> begin() const;
        SArrayIterator<char> end();
        SArrayCIterator<char> end() const;
        void add(const char &c);
        void append(const char *s);
        void append(const std::string &s);
        void append(const String &s);
        void append(const SString &s);
        void insert(sinteger idx, const char *s);
        void insert(sinteger idx, const std::string &s);
        void insert(sinteger idx, const String &s);
        void insert(sinteger idx, const SString &s);
        void removeAt(sinteger idx);
        void remove(size_t off, size_t len = -1);
        void remove(const srange &rng);
		String& replace(size_t off, size_t len, const char* alt);
		String& replace(const srange& rng, const char* alt);
		String& replace(const char* ori, const char* alt);
		String& replace(const Regex& rgx, const char* alt);
		String& rearrange(const Regex& rgx, const CArray<sint>& order);
		String& clip(size_t off, size_t len = -1);
		String& clip(const srange& rng);
		String& fill(size_t s, char fill = ' ', subyte dir = String::TAIL_PART);
		String& trimming();
		String& transform(subyte trans);
        String substring(size_t off, size_t len = -1) const;
        String substring(const srange &range) const;
        String replaced(const char *ori, const char *alt) const;
        String replaced(const Regex &rgx, const char *alt) const;
        String rearranged(const Regex &rgx, const CArray<sint> &order) const;
        String filled(size_t size, char fill = ' ', subyte dir = String::TAIL_PART) const;
        String transformed(subyte trans) const;
        //UTF-8
        size_t charCount() const;
        size_t charIndex(size_t idx) const;
        Char u8charAt(size_t idx) const;
        String strAt(size_t idx) const;
        SUtf8Iterator ubegin();
        SUtf8CIterator ubegin() const;
        SUtf8Iterator uend();
        SUtf8CIterator uend() const;
        size_t count(const char *s, size_t offset = 0) const;
        bool contain(const char *que, size_t offset = 0) const;
        bool match(const Regex &rgx, size_t offset = 0) const;
        bool equal(const Regex &rgx) const;
        size_t find(const char *que, size_t offset = 0) const;
        size_t rfind(const char *que, size_t offset = 0) const;
		slib::CArray<size_t> search(const char *que, size_t offset = 0) const;
		slib::CArray<size_t> search(const Regex &rgx, size_t offset = 0) const;
		slib::Array<slib::String, slib::SMemory<slib::String>> matched(const Regex &rgx, size_t offset = 0) const;
		slib::Array<slib::String, slib::SMemory<slib::String>> split(const char *sep, bool trim = true, bool ignore = false) const;
		slib::Array<slib::String, slib::SMemory<slib::String>> splitline(bool trim = true, bool ignore = false) const;
		slib::Array<slib::String, slib::SMemory<slib::String>> split(const Regex &rgx) const;
		slib::Map<slib::String, slib::String> parse(const char *sep , const char *part, bool trim = true) const;
        bool beginWith(const char *que) const;
        bool endWith(const char *que) const;
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
        SNumber number() const;
#if defined(WIN32_OS) || defined(WIN64_OS)
		std::wstring unicode() const;
		String localize() const;
#endif  
        operator bool() const;
        operator sbyte() const;
        operator subyte() const;
        operator sshort() const;
        operator sushort() const;
        operator int() const;
        operator unsigned int() const;
        operator size_t() const;
#ifdef WIN64_OS
        operator long() const;
#ifndef MAC_OS
        operator unsigned long() const;
#endif
#endif
        operator long long() const;
#ifdef MAC_OS
        operator unsigned long long() const;
#endif
#ifdef LINUX_OS
        operator sinteger() const;
#endif
        operator float() const;
        operator double() const;
        operator const char *() const;
        bool operator < (const char *s) const;
        bool operator < (const std::string &s) const;
        bool operator < (const String &s) const;
        bool operator < (const SString &s) const;
        bool operator == (const char *s) const;
        bool operator == (const std::string &s) const;
        bool operator == (const String &s) const;
        bool operator == (const SString &s) const;
        bool operator != (const char *s) const;
        bool operator != (const std::string &s) const;
        bool operator != (const String &s) const;
        bool operator != (const SString &s) const;
    };
	extern SLIB_DLL String operator+(const char& c, const String& s);
	extern SLIB_DLL String operator+(const char* s1, const String& s2);
	extern SLIB_DLL String operator+(const std::string& s1, const String& s2);
	
	extern inline std::ostream& operator<<(std::ostream& os, const Char& c) {
#if defined(WIN32_OS) || defined(WIN64_OS)
		return os << c.toString().localize().cstr();
#else
		return os << c.toString().cstr();
#endif
	}
	extern inline std::ostream& operator<<(std::ostream& os, const String& str) {
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
}

namespace std {
	template<>
	struct hash<slib::Char> {
		size_t operator()(const slib::Char& c) const {
			return hash<std::string>{}(c.toStr());
		}
	};
	template<>
	struct hash<slib::String> {
		size_t operator()(const slib::String& s) const {
			return hash<std::string>{}(s.toStr());
		}
	};
}
#endif
