#ifndef SLIB_SSTRING_H
#define SLIB_SSTRING_H

#include "sobj/sobject.h"

namespace slib {
   
	class SOBJ_DLL SChar : public SObject, public Char {
    public:
        SChar();
        SChar(String *s, const char *c);
        SChar(const Char &c);
        SChar(const SChar &c);
        ~SChar();
        
		SChar &operator=(const char* s);
        SChar &operator=(const String &s);
        SChar &operator=(const SString &s);
        SChar &operator=(const Char &c);
        SChar &operator=(const SChar &c);
        
        String getClass() const;
        String toString() const;
        SObject *clone() const;
    };

	class SOBJ_DLL SString : public SObject, public String {
        friend SChar;
    protected:
		SObjPtr _char;
        
    public:
        SString();
        SString(bool b);
        SString(int i);
        SString(unsigned int ui);
        SString(size_t ui);
#ifdef WIN64_OS
        SString(long i);
#ifndef MAC_OS
        SString(unsigned long ui);
#endif
#endif
        SString(long long i);
#ifdef MAC_OS
        SString(unsigned long long ui);
#endif
#ifdef LINUX_OS
        SString(sinteger i);
#endif
        SString(float f);
        SString(double d);
        SString(sbyte i);
        SString(subyte i);
        SString(sshort i);
        SString(sushort i);
        SString(const size_t &s, char c);
        SString(char c);
        SString(const SChar &c);
        SString(const char *s);
        SString(const std::string &s);
        SString(const String &s);
        SString(const sobj &obj);
        SString(const SNumber &sn);
        SString(SString &&s);
        SString(const SString &s);
        ~SString();
        
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
        SString &operator=(const SChar &c);
        SString &operator=(const char *s);
        SString &operator=(const std::string &s);
        SString &operator=(const String &s);
        SString &operator=(const SString &s);
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
        SString &operator+=(const SString &s);
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
        
        //IO
        void load(const char *path);
        void save(const char *path);
        
	sobj& u8char(int idx = 0);
	const sobj& u8char(int idx = 0) const;

        //Override
        String getClass() const;
        String toString() const;
        SObject *clone() const;
        
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
    };
    extern SString operator+(const char &c, const SString &s);
    extern SString operator+(const SChar &c, const SString &s);
    extern SString operator+(const char *s1, const SString &s2);
    extern SString operator+(const std::string &s1, const SString &s2);
    extern SString operator+(const int &i, const SString &s);
    extern SString operator+(const sinteger &i, const SString &s);
    extern SString operator+(const size_t &u, const SString &s);
    extern SString operator+(const float &f, const SString &s);
    extern SString operator+(const sreal &r, const SString &s);
    extern SString operator+(const bool &b, const SString &s);
    
    template<typename T, class M>
    extern String toString(const Array<T, M> &array, const char *sep = ",") {
        String str;
        if (!array.empty()) sforeach(array) str<<E_<<sep;
        if (!str.empty()) str.resize(str.length()-strlen(sep));
        return str;
    }
    template<typename T, size_t S, class M>
    extern String toString(const FixedArray<T, S, M> &array, const char *sep = ",") {
        String str;
        if (!array.empty()) sforeach(array) str<<E_<<sep;
        if (!str.empty()) str.resize(str.length()-strlen(sep));
        return str;
    }
    template<typename T, class M>
    extern String toString(const BiArray<T, M> &array, const char *sep = ",") {
        String str;
        if (!array.empty()) sforeach(array) str<<E_<<sep;
        if (!str.empty()) str.resize(str.length()-strlen(sep));
        return str;
    }
    template<class Key, class Val>
    extern String toString(const Map<Key, Val> &map, const char *sep = ";", const char *part = "=") {
        String str;
        if (!map.empty()) sforeach(map) str<<E_.key<<part<<E_.value<<sep;
        if (!str.empty()) str.resize(str.length()-strlen(sep));
        return str;
    }
}

namespace std {
    template<>
    struct hash<slib::SString> {
        size_t operator()(const char *s) const {
            return hash<std::string>{}(s);
        }
    };
}

#endif
