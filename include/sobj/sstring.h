#ifndef SLIB_SSTRING_H
#define SLIB_SSTRING_H
#include "sobj/sobject.h"
namespace slib {
    /**
    * @class SString
    * 
    */
	class SLIB_DLL SString : public SObject, public String {
    public:
        SString();
        SString(size_t sz, const char c);
        SString(std::initializer_list<char> li);
        SString(const char *s);
        SString(String&& s) noexcept;
        SString(const String &s);
        SString(SString&& s) noexcept;
        SString(const SString& s);
        ~SString();
        SString& operator=(SString&& s) noexcept;
        SString& operator=(const SString& s);
        
        SString operator+(const char* s) const;
        SString operator+(const String& s) const;
        SString operator+(const SString& s) const;

        SString& operator+=(const char* s);
        SString& operator+=(const String& s);
        SString& operator+=(const SString& s);
        SString& operator<<(const char* s);
        SString& operator<<(const String& s);
        SString& operator<<(const SString& s);
        
        SString& trim();
        SString& clip(const size_t off, const size_t len = -1);
        SString& clip(const srange& range);
        SString& replace(const char* ori, const char* wrd);
        SString& replace(const Regex& reg, const char* wrd);
        SString& replace(const size_t off, const size_t len, const char* wrd);

        void swap(SString& str);

        operator const char* () const;
        //operator String& ();
        //operator const String& () const;

        bool operator==(const char* s) const;
        bool operator==(const std::string& s) const;
        bool operator==(const slib::SObjPtr& s) const;
        bool operator==(const String& s) const;
        bool operator==(const SString& s) const;


/*
        SString &operator=(bool b);
        SString &operator=(int i);
        SString &operator=(unsigned int ui);
        SString &operator=(size_t ui);
#ifdef WIN64_OS
        SString &operator=(long i);
#ifndef MAC_OS
        SString &operator=(unsigned long ui);
#endif
#endif
        SString &operator=(long long i);
#ifdef MAC_OS
        SString &operator=(unsigned long long ui);
#endif
#ifdef LINUX_OS
        SString &operator=(sinteger i);
#endif
        SString &operator=(float f);
        SString &operator=(double d);
        SString &operator=(sbyte i);
        SString &operator=(subyte i);
        SString &operator=(sshort i);
        SString &operator=(sushort i);
        SString &operator=(char c);
        SString &operator=(const char *s);
        SString &operator=(const std::string &s);
        SString &operator=(const String &s);
        
        SString &operator=(const SNumber &n);
        SString &operator=(const sobj &obj);
        
        SString &operator+=(bool b);
        SString &operator+=(int i);
        SString &operator+=(unsigned int ui);
        SString &operator+=(size_t ui);
#ifdef WIN64_OS
        SString &operator+=(long i);
#ifndef MAC_OS
        SString &operator+=(unsigned long ui);
#endif
#endif
        SString &operator+=(long long i);
#ifdef MAC_OS
        SString &operator+=(unsigned long long ui);
#endif
#ifdef LINUX_OS
        SString &operator+=(sinteger i);
#endif
        SString &operator+=(float f);
        SString &operator+=(double d);
        SString &operator+=(sbyte i);
        SString &operator+=(subyte i);
        SString &operator+=(sshort i);
        SString &operator+=(sushort i);
        SString &operator+=(char c);
        SString &operator+=(const Char &c);
        SString &operator+=(const SChar &c);
        SString &operator+=(const char *s);
        SString &operator+=(const std::string &s);
        SString &operator+=(const String &s);
        SString &operator+=(const SNumber &n);
		SString& operator+=(const sio::SFile& n);
        SString &operator+=(const sobj &obj);
        
        SString operator+(bool b) const;
        SString operator+(int i) const;
        SString operator+(unsigned int ui) const;
        SString operator+(size_t ui) const;
#ifdef WIN64_OS
        SString operator+(long i) const;
#ifndef MAC_OS
        SString operator+(unsigned long ui) const;
#endif
#endif
        SString operator+(long long i) const;
#ifdef MAC_OS
        SString operator+(unsigned long long ui) const;
#endif
#ifdef LINUX_OS
        SString operator+(sinteger i) const;
#endif
        SString operator+(float f) const;
        SString operator+(double d) const;
        SString operator+(sbyte i) const;
        SString operator+(subyte i) const;
        SString operator+(sshort i) const;
        SString operator+(sushort i) const;
        SString operator+(char c) const;
        SString operator+(const Char &c) const;
        SString operator+(const SChar &c) const;
        SString operator+(const char *s) const;
        SString operator+(const std::string &s) const;
        SString operator+(const String &s) const;
        SString operator+(const SString &s) const;
        SString operator+(const SNumber &n) const;
		SString operator+(const sio::SFile& n);
        SString operator+(const sobj &obj) const;
        
        SString &operator<<(bool b);
        SString &operator<<(int i);
        SString &operator<<(unsigned int ui);
        SString &operator<<(size_t ui);
#ifdef WIN64_OS
        SString &operator<<(long i);
#ifndef MAC_OS
        SString &operator<<(unsigned long ui);
#endif
#endif
        SString &operator<<(long long i);
#ifdef MAC_OS
        SString &operator<<(unsigned long long ui);
#endif
#ifdef LINUX_OS
        SString &operator<<(sinteger i);
#endif
        SString &operator<<(float f);
        SString &operator<<(double d);
        SString &operator<<(sbyte i);
        SString &operator<<(subyte i);
        SString &operator<<(sshort i);
        SString &operator<<(sushort i);
        SString &operator<<(char c);
        SString &operator<<(const Char &c);
        SString &operator<<(const SChar &c);
        SString &operator<<(const char *s);
        SString &operator<<(const std::string &s);
        SString &operator<<(const String &s);
        SString &operator<<(const SString &s);
        SString &operator<<(const SNumber &n);
		SString& operator<<(const sio::SFile& n);
        SString &operator<<(const sobj &obj);
        SString &operator*=(const int & num);
        SString &operator*=(const size_t & num);
        SString operator*(const int &num) const;
        SString operator*(const size_t & num) const;
        void load(const char *path);
        void save(const char *path);
        */
		//sobj& charAt(int idx = 0);
		//const sobj& charAt(int idx = 0) const;

        String getClass() const;
        String toString(const char *format = nullptr) const;
        SObject *clone() const;
        /*
        bool operator<(const char *s) const;
        bool operator<(const std::string &s) const;
        bool operator<(const sobj &s) const;
        bool operator<(const String &s) const;
        bool operator<(const SString &s) const;
        bool operator==(const char *s) const;
        bool operator==(const std::string &s) const;
        bool operator==(const sobj &s) const;
        bool operator==(const String &s) const;
        bool operator==(const SString &s) const;
        bool operator!=(const char *s) const;
        bool operator!=(const std::string &s) const;
        bool operator!=(const String &s) const;
        bool operator!=(const sobj &s) const;
        bool operator!=(const SString &s) const;
        */
    };
    /*
    extern SLIB_DLL SString operator+(const char &c, const SString &s);
    extern SLIB_DLL SString operator+(const char *s1, const SString &s2);
    extern SLIB_DLL SString operator+(const std::string &s1, const SString &s2);
    extern SLIB_DLL SString operator+(const int &i, const SString &s);
    extern SLIB_DLL SString operator+(const sinteger &i, const SString &s);
    extern SLIB_DLL SString operator+(const size_t &u, const SString &s);
    extern SLIB_DLL SString operator+(const float &f, const SString &s);
    extern SLIB_DLL SString operator+(const sreal &r, const SString &s);
    extern SLIB_DLL SString operator+(const bool &b, const SString &s);
    */
}
/**
* @cond
*/
namespace std {
    template<>
    struct hash<slib::SString> {
        size_t operator()(const char *s) const {
            return hash<std::string>{}(s);
        }
    };
}
/**
* @endcond
*/
#endif
