#ifndef SLIB_CHAR_H
#define SLIB_CHAR_H
#include "sconfig.h"
namespace slib {
    class SLIB_DLL Utf8Iterator;
    class SLIB_DLL Utf8CIterator;
    class SLIB_DLL String;
    namespace sutf8 {
        extern bool SLIB_DLL check(const char* c);
        extern sbyte SLIB_DLL size(const char* c);
        extern bool SLIB_DLL isNarrow(const char* s);
        extern bool SLIB_DLL isWide(const char* s);
        extern bool SLIB_DLL isWS(char c);
        extern bool SLIB_DLL isNumChar(const char c);
        extern bool SLIB_DLL isWideNum(const char* s);
        extern bool SLIB_DLL isNumeric(const char* s);
        extern char SLIB_DLL toChar(const sbyte n);
        extern sbyte SLIB_DLL toNum(const char c);
        extern sbyte SLIB_DLL toNum(const char* s);
        extern bool SLIB_DLL isUpper(const char c);
        extern bool SLIB_DLL isLower(const char c);
        extern char SLIB_DLL toUpper(const char c);
        extern char SLIB_DLL toLower(const char c);
        extern String SLIB_DLL toWide(char c);
        extern char SLIB_DLL toNarrow(const char* s);
    }
    /**
    * @class Char
    * \~english @brief UTF-8 character class
    * \~japanese @brief UTF-8 文字クラス
    */
    class SLIB_DLL Char {
        friend Utf8Iterator;
        friend Utf8CIterator;
    private:
        char* _ptr;
        String* _base;
    public:
        Char();
        Char(const char* c, const String* s);
        Char(const Char& c);
        ~Char();
        Char& operator=(const char* s);
        Char& operator=(const Char& c);
        Char& operator++();
        Char& operator--();
        /**
        *\~english @brief Get byte size of the character
        *\~japanese @brief 現在の文字のバイト数
        * @return byte size
        */
        size_t size() const;
        size_t index() const;
        /**
        *\~english @brief Check whether the byte(s) of the character is UTF-8
        */
        bool isUTF8() const;
        /**
        *\~english @brief Return the const pointer of the character
        */
        const char* cstr() const;
        /**
        *\~english @brief Return the character as String
        */
        String toString() const;
        bool operator<(const char* s) const;
        bool operator<(const Char& c) const;
        bool operator==(const char* s) const;
        bool operator==(const Char& c) const;
    };
    /*
    * @cond
    */
    extern SLIB_DLL String toString(const Char& ch, const char* format = nullptr);
    /*
    * @endcond
    */
}
#endif
