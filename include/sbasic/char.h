#ifndef SLIB_CHAR_H
#define SLIB_CHAR_H

#include "sconfig.h"

namespace slib {
	class SLIB_DLL String;
    class SLIB_DLL SChar;
    class SLIB_DLL SString;
    class SLIB_DLL SObjPtr;

	class SLIB_DLL Char {
        friend class String;
        
    public:
        const char *_ptr;
		String *_str;
        
    public:
        static sbyte u8size(const char *c);
        static bool isNarrow(const char *s);
        static bool isWide(const char *s);
        static bool isNumeric(const char *s);
        static bool isNumChar(char c);
        static bool isWideNum(const char *s);
        static bool isWSChar(char c);
        static sbyte toNum(char c);
        static sbyte toNum(const char *s);
        static String wideChar(char c);
        static char narrowChar(const char *s);
        
    public:
        Char();
        Char(String *s, const char *c);
        Char(const Char &c);
        ~Char();
        
        Char &operator=(const char *s);
        Char &operator=(const std::string &s);
        Char &operator=(const String &s);
        Char &operator=(const SString &s);
        Char &operator=(const Char &c);
        Char &operator=(const SChar &c);
        Char &operator=(const SObjPtr &obj);
        Char &operator+=(int p);
        Char &operator-=(int p);
		Char operator+(int p);
		Char operator-(int p);
        Char &operator ++();
        Char &operator --();
        
        size_t index() const;
        size_t length() const;
        void setIndex(size_t s);
        void setOffset(size_t s);
        
        const char *cstr() const;
        std::string toStr() const;
        String toString() const;
        
        bool operator<(const Char &c) const;
        bool operator==(const char &c) const;
        bool operator==(const char *s) const;
        bool operator==(const Char &c) const;
    };
}

#endif
