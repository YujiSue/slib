#ifndef SLIB_SNUMBER_H
#define SLIB_SNUMBER_H

#include "smath/frac.h"
#include "smath/comp.h"
#include "smath/geom.h"
#include "sobj/sobject.h"

namespace slib {
    class SLIB_DLL SString;
    using namespace smath;
    
    class SLIB_DLL SNumber : public SObject {
    public:
        static const subyte INTEGER = 1;
        static const subyte UINTEGER = 2;
        static const subyte REAL = 3;
        static const subyte BOOL = 4;
        static const subyte FRAC = 5;
        static const subyte COMPLEX = 6;
        
    private:
		subyte _type;
        union _num {
            sinteger _i;
            suinteger _ui;
            double _r;
            bool _b;
            sint _f[2];
            float _c[2];
        };
        _num _value;

    public:
        static SNumber toNumber(const char *s);
        
    public:
        SNumber();
        SNumber(char c);
        SNumber(bool b);
        SNumber(sbyte i);
        SNumber(subyte ui);
        SNumber(sshort i);
        SNumber(sushort ui);
        SNumber(int i);
        SNumber(unsigned int ui);
        SNumber(size_t ui);
#if defined(WIN64_OS)
        SNumber(long i);
        SNumber(unsigned long ui);
#endif
        SNumber(long long i);
#if defined(MAC_OS)
        SNumber(unsigned long long ui);
#endif
        SNumber(float f);
        SNumber(double d);
        SNumber(sint n, sint d);
        SNumber(const sfrac &frac);
        SNumber(float r, float i);
        SNumber(const scomp &comp);
        SNumber(const sobj &obj);
        SNumber(const SNumber &num);
        ~SNumber();

		SNumber& operator=(const SNumber& num);
		SNumber& operator+=(const SNumber& num);
		SNumber& operator-=(const SNumber& num);
		SNumber& operator*=(const SNumber& num);
		SNumber& operator/=(const SNumber& num);
		SNumber& operator%=(const SNumber& num);
        
        SNumber &operator++();
        SNumber operator++(int) const;
        SNumber &operator--();
        SNumber operator--(int) const;
        SNumber operator-() const;
        SNumber abs() const;
        
		SNumber operator+(const sbyte& num) const;
		SNumber operator+(const subyte& num) const;
		SNumber operator+(const sshort& num) const;
		SNumber operator+(const sushort& num) const;
		SNumber operator+(const int& num) const;
		SNumber operator+(const unsigned int& num) const;
		SNumber operator+(const size_t& num) const;
#if defined(WIN64_OS)
		SNumber operator+(const long& num) const;
		SNumber operator+(const unsigned long& num) const;
#endif
		SNumber operator+(const long long& num) const;
#if defined(MAC_OS)
		SNumber operator+(const unsigned long long& num) const;
#endif
		SNumber operator+(const float& num) const;
		SNumber operator+(const double& num) const;
		SNumber operator+(const sfrac& num) const;
		SNumber operator+(const scomp& num) const;
        SNumber operator+(const SNumber &num) const;
        String operator+(const char *s);
        
		SNumber operator-(const sbyte& num) const;
		SNumber operator-(const subyte& num) const;
		SNumber operator-(const sshort& num) const;
		SNumber operator-(const sushort& num) const;
		SNumber operator-(const int& num) const;
		SNumber operator-(const unsigned int& num) const;
		SNumber operator-(const size_t& num) const;
#if defined(WIN64_OS)
		SNumber operator-(const long& num) const;
		SNumber operator-(const unsigned long& num) const;
#endif
		SNumber operator-(const long long& num) const;
#if defined(MAC_OS)
		SNumber operator-(const unsigned long long& num) const;
#endif
		SNumber operator-(const float& num) const;
		SNumber operator-(const double& num) const;
		SNumber operator-(const sfrac& num) const;
		SNumber operator-(const scomp& num) const;
		SNumber operator-(const SNumber& num) const;
		SNumber operator*(const sbyte& num) const;
		SNumber operator*(const subyte& num) const;
		SNumber operator*(const sshort& num) const;
		SNumber operator*(const sushort& num) const;
		SNumber operator*(const int& num) const;
		SNumber operator*(const unsigned int& num) const;
		SNumber operator*(const size_t& num) const;
#if defined(WIN64_OS)
		SNumber operator*(const long& num) const;
		SNumber operator*(const unsigned long& num) const;
#endif
		SNumber operator*(const long long& num) const;
#if defined(MAC_OS)
		SNumber operator*(const unsigned long long& num) const;
#endif
		SNumber operator*(const float& num) const;
		SNumber operator*(const double& num) const;
		SNumber operator*(const sfrac& num) const;
		SNumber operator*(const scomp& num) const;
		SNumber operator*(const SNumber& num) const;
		SNumber operator/(const sbyte& num) const;
		SNumber operator/(const subyte& num) const;
		SNumber operator/(const sshort& num) const;
		SNumber operator/(const sushort& num) const;
		SNumber operator/(const int& num) const;
		SNumber operator/(const unsigned int& num) const;
		SNumber operator/(const size_t& num) const;
#if defined(WIN64_OS)
		SNumber operator/(const long& num) const;
        SNumber operator/(const unsigned long& num) const;
#endif
		SNumber operator/(const long long& num) const;
#if defined(MAC_OS)
		SNumber operator/(const unsigned long long& num) const;
#endif
		SNumber operator/(const float& num) const;
		SNumber operator/(const double& num) const;
		SNumber operator/(const sfrac& num) const;
		SNumber operator/(const scomp& num) const;
		SNumber operator/(const SNumber& num) const;
		
		sinteger quot(const SNumber& num) const;

		SNumber operator%(const sbyte& num) const;
		SNumber operator%(const subyte& num) const;
		SNumber operator%(const sshort& num) const;
		SNumber operator%(const sushort& num) const;
		SNumber operator%(const int& num) const;
		SNumber operator%(const unsigned int& num) const;
		SNumber operator%(const size_t& num) const;
#if defined(WIN64_OS)
		SNumber operator%(const long& num) const;
		SNumber operator%(const unsigned long& num) const;
#endif
		SNumber operator%(const long long& num) const;
#if defined(MAC_OS)
		SNumber operator%(const unsigned long long& num) const;
#endif
		SNumber operator%(const float& num) const;
		SNumber operator%(const double& num) const;
		SNumber operator%(const sfrac& num) const;
		SNumber operator%(const scomp& num) const;
		SNumber operator%(const SNumber& num) const;
        
        //variable access
        subyte type() const;
        void *ptr() const;
        
        //type conversion
        void setType(subyte type);
        
        //Round
        String precised(size_t s, smath::ROUND round = DEFAULT) const;
        
        //Output
        String toBinary() const;
        String toOct() const;
        String toHex() const;
        
        //override
        String getClass() const;
        String toString() const;
        SObject *clone() const;
        
        //Wide character
        String toWideString() const;
        
        //cast
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
        sfrac fraction() const;
        scomp complex() const;
        
        operator bool() const;
        operator sbyte() const;
        operator subyte() const;
        operator sshort() const;
        operator sushort() const;
        operator int() const;
        operator unsigned int() const;
        operator size_t() const;
#if defined(WIN64_OS)
        operator long() const;
        operator unsigned long() const;
#endif
        operator long long() const;
#if defined(MAC_OS)
        operator unsigned long long() const;
#endif
        operator float() const;
        operator double() const;
        operator sfrac() const;
        operator scomp() const;
        
        //operator const char *() const;
        
        //compare
        bool operator<(const sobj &obj) const;
        bool operator<(const SNumber &sn) const;
        template<typename T>
        bool operator<(const T &val) const { return *this < SNumber(val); }
        bool operator>(const sobj &obj) const;
        bool operator>(const SNumber &sn) const;
        template<typename T>
        bool operator>(const T &val) const { return *this > SNumber(val); }
        bool operator<=(const sobj &obj) const;
        bool operator<=(const SNumber &sn) const;
        template<typename T>
        bool operator<=(const T &val) const { return *this <= SNumber(val); }
        bool operator>=(const sobj &obj) const;
        bool operator>=(const SNumber &sn) const;
        template<typename T>
        bool operator>=(const T &val) const { return *this >= SNumber(val); }
        bool operator==(const sobj &obj) const;
        bool operator==(const SNumber &sn) const;
        template<typename T>
        bool operator==(const T &val) const { return *this == SNumber(val); }
        bool operator!=(const sobj &obj) const;
        bool operator!=(const SNumber &sn) const;
        template<typename T>
        bool operator!=(const T &val) const { return *this != SNumber(val); }
    };
	extern inline SNumber operator+(sbyte& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(subyte& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(sshort& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(sushort& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(int& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(unsigned int& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(size_t& num1, const SNumber& num2) { return SNumber(num1) += num2; };
#if defined(WIN64_OS)
	extern inline SNumber operator+(long& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(unsigned long& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(long long& num1, const SNumber& num2) { return SNumber(num1) += num2; };
#endif
#if defined(MAC_OS)
	extern inline SNumber operator+(unsigned long long& num1, const SNumber& num2) { return SNumber(num1) += num2; };
#endif
	extern inline SNumber operator+(float& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(double& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(sfrac& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(scomp& num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator-(sbyte& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(subyte& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(sshort& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(sushort& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(int& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(unsigned int& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(size_t& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
#if defined(WIN64_OS)
	extern inline SNumber operator-(long& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(unsigned long& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
#endif
	extern inline SNumber operator-(long long& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
#if defined(MAC_OS)
	extern inline SNumber operator-(unsigned long long& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
#endif
	extern inline SNumber operator-(float& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(double& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(sfrac& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(scomp& num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator*(sbyte& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(subyte& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(sshort& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(sushort& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(int& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(unsigned int& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(size_t& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
#if defined(WIN64_OS)
	extern inline SNumber operator*(long& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(unsigned long& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
#endif
	extern inline SNumber operator*(long long& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
#if defined(MAC_OS)
	extern inline SNumber operator*(unsigned long long& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
#endif
	extern inline SNumber operator*(float& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(double& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(sfrac& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(scomp& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator/(sbyte& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(subyte& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(sshort& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(sushort& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(int& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(unsigned int& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(size_t& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
#if defined(WIN64_OS)
	extern inline SNumber operator/(long& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(unsigned long& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
#endif
	extern inline SNumber operator/(long long& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
#if defined(MAC_OS)
	extern inline SNumber operator/(unsigned long long& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
#endif
	extern inline SNumber operator/(float& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(double& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(sfrac& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(scomp& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator%(sbyte& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(subyte& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(sshort& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(sushort& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(int& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(unsigned int& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(size_t& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
#if defined(WIN64_OS)
	extern inline SNumber operator%(long& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(unsigned long& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
#endif
	extern inline SNumber operator%(long long& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
#if defined(MAC_OS)
	extern inline SNumber operator%(unsigned long long& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
#endif
	extern inline SNumber operator%(float& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(double& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(sfrac& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(scomp& num1, const SNumber& num2) { return SNumber(num1) %= num2; };


	extern String operator+(const char *s, const SNumber &num);
   
    template<typename T>
    extern bool operator<(const T &val, const SNumber &num) { return SNumber(val) < num; }
    template<typename T>
    extern bool operator>(const T &val, const SNumber &num) { return SNumber(val) > num; }
    template<typename T>
    extern bool operator<=(const T &val, const SNumber &num) { return SNumber(val) <= num; }
    template<typename T>
    extern bool operator>=(const T &val, const SNumber &num) { return SNumber(val) >= num; }
    template<typename T>
    extern bool operator==(const T &val, const SNumber &num) { return SNumber(val) == num; }
    template<typename T>
    extern bool operator!=(const T &val, const SNumber &num) { return SNumber(val) != num; }
}
#endif
