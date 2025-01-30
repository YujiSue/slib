#ifndef SLIB_SNUMBER_H
#define SLIB_SNUMBER_H
#include "sobj/sobject.h"
#include "smath/fraction.h"
#include "smath/complex.h"
#define N(X) slib::SNumber(X)
namespace slib {
    /**
    * @class SNumber
    * \~english Numeric value class
    */
    class SLIB_DLL SNumber : public SObject {
    private:
        subyte _type;
        union numvalue {
            sinteger _i;
            suinteger _ui;
            double _r;
            bool _b;
            sint _f[2];
            float _c[2];
        };
        numvalue _value;

    public:
		static String toBinary(subyte b, bool le = true);
		static String toOct(subyte b);
		static String toHex(subyte b);
        //static SNumber toNumber(const char *s);
        
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
#ifndef LINUX_OS
        SNumber(long i);
#ifndef MAC_OS
        SNumber(unsigned long ui);
#endif
#endif
        SNumber(long long i);
#ifdef MAC_OS
        SNumber(unsigned long long ui);
#endif
#ifdef LINUX_OS
        SNumber(sinteger i);
#endif
        SNumber(float f);
        SNumber(double d);
        SNumber(sint n, sint d);
        SNumber(const sfrac &frac);
        SNumber(float r, float i);
        SNumber(const Complex<float> &comp);
        SNumber(const char *s);
        SNumber(const SNumber &num);
        ~SNumber();

        SNumber& operator=(const SNumber& num);
		SNumber& operator+=(const SNumber& num);
        SNumber& operator-=(const SNumber& num);

        SNumber& operator++();
        SNumber operator++(int) const;
        SNumber& operator--();
        SNumber operator--(int) const;
        SNumber operator-() const;
        SNumber absolute() const;

        SNumber operator+(const SNumber& num) const;
		SNumber operator-(const SNumber& num) const;
		SNumber& operator*=(const SNumber& num);
        SNumber operator*(const SNumber& num) const;
        SNumber& operator/=(const SNumber& num);
        SNumber operator/(const SNumber& num) const;
        SNumber reciprocal() const;
        SNumber& operator%=(const SNumber& num);
        SNumber operator%(const SNumber& num) const;
        sinteger quot(const SNumber& num) const;

		subyte type() const;
        void *ptr() const;
        void setType(subyte type);

        bool isNaN() const;
		bool isInf() const;
		bool isNInf() const;
        
        
        //String precised(size_t s, smath::ROUND round = DEFAULT) const;



		
		String getClass() const;
		String toString(const char* format = nullptr) const;
        SObject *clone() const;
        
		
		
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
        Complex<float> complex() const;
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
        operator sfrac() const;
        operator Complex<float>() const;
        operator const char* () const;
        //bool operator<(const sobj &obj) const;
        bool operator<(const SNumber &sn) const;
        template<typename T>
        bool operator<(const T &val) const { return *this < SNumber(val); }
        //bool operator>(const sobj &obj) const;
        bool operator>(const SNumber& sn) const;
        template<typename T>
        bool operator>(const T &val) const { return *this > SNumber(val); }
        //bool operator<=(const sobj &obj) const;
        bool operator<=(const SNumber &sn) const;
        template<typename T>
        bool operator<=(const T &val) const { return *this <= SNumber(val); }
        //bool operator>=(const sobj &obj) const;
        bool operator>=(const SNumber &sn) const;
        template<typename T>
        bool operator>=(const T &val) const { return *this >= SNumber(val); }
        //bool operator==(const sobj &obj) const;
        bool operator==(const SNumber &sn) const;
        template<typename T>
        bool operator==(const T &val) const { return *this == SNumber(val); }
        //bool operator!=(const sobj &obj) const;
        bool operator!=(const SNumber &sn) const;
        template<typename T>
        bool operator!=(const T &val) const { return *this != SNumber(val); }
    };
    /**
    * @cond
    */
	extern inline SNumber operator+(sbyte num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(subyte num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(sshort num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(sushort num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(int num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(unsigned int num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(size_t num1, const SNumber& num2) { return SNumber(num1) += num2; };
#ifdef WIN64_OS
	extern inline SNumber operator+(long num1, const SNumber& num2) { return SNumber(num1) += num2; };
#ifndef MAC_OS
	extern inline SNumber operator+(unsigned long num1, const SNumber& num2) { return SNumber(num1) += num2; };
#endif
#endif
	extern inline SNumber operator+(long long num1, const SNumber& num2) { return SNumber(num1) += num2; };
#ifdef MAC_OS
	extern inline SNumber operator+(unsigned long long num1, const SNumber& num2) { return SNumber(num1) += num2; };
#endif
#ifdef LINUX_OS
	extern inline SNumber operator+(sinteger num1, const SNumber& num2) { return SNumber(num1) += num2; };
#endif
	extern inline SNumber operator+(float num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator+(double num1, const SNumber& num2) { return SNumber(num1) += num2; };
	extern inline SNumber operator-(sbyte num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(subyte num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(sshort num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(sushort num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(int num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(unsigned int num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(size_t num1, const SNumber& num2) { return SNumber(num1) -= num2; };
#ifdef WIN64_OS
	extern inline SNumber operator-(long num1, const SNumber& num2) { return SNumber(num1) -= num2; };
#ifndef MAC_OS
	extern inline SNumber operator-(unsigned long num1, const SNumber& num2) { return SNumber(num1) -= num2; };
#endif
#endif
	extern inline SNumber operator-(long long num1, const SNumber& num2) { return SNumber(num1) -= num2; };
#ifdef MAC_OS
	extern inline SNumber operator-(unsigned long long num1, const SNumber& num2) { return SNumber(num1) -= num2; };
#endif
#ifdef LINUX_OS
	extern inline SNumber operator-(sinteger num1, const SNumber& num2) { return SNumber(num1) -= num2; };
#endif
	extern inline SNumber operator-(float num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator-(double num1, const SNumber& num2) { return SNumber(num1) -= num2; };
	extern inline SNumber operator*(sbyte& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(subyte& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(sshort& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(sushort& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(int& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(unsigned int& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(size_t& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
#ifdef WIN64_OS
	extern inline SNumber operator*(long& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
#ifndef MAC_OS
	extern inline SNumber operator*(unsigned long& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
#endif
#endif
	extern inline SNumber operator*(long long& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
#ifdef MAC_OS
	extern inline SNumber operator*(unsigned long long& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
#endif
#ifdef LINUX_OS
	extern inline SNumber operator*(sinteger& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
#endif
	extern inline SNumber operator*(float& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator*(double& num1, const SNumber& num2) { return SNumber(num1) *= num2; };
	extern inline SNumber operator/(sbyte& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(subyte& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(sshort& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(sushort& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(int& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(unsigned int& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(size_t& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
#ifdef WIN64_OS
	extern inline SNumber operator/(long& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
#ifndef MAC_OS
	extern inline SNumber operator/(unsigned long& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
#endif
#endif
	extern inline SNumber operator/(long long& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
#ifdef MAC_OS
	extern inline SNumber operator/(unsigned long long& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
#endif
#ifdef LINUX_OS
	extern inline SNumber operator/(sinteger& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
#endif
	extern inline SNumber operator/(float& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator/(double& num1, const SNumber& num2) { return SNumber(num1) /= num2; };
	extern inline SNumber operator%(sbyte& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(subyte& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(sshort& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(sushort& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(int& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(unsigned int& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(size_t& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
#ifdef WIN64_OS
	extern inline SNumber operator%(long& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
#ifndef MAC_OS
	extern inline SNumber operator%(unsigned long& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
#endif
#endif
	extern inline SNumber operator%(long long& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
#ifdef MAC_OS
	extern inline SNumber operator%(unsigned long long& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
#endif
#ifdef LINUX_OS
	extern inline SNumber operator%(sinteger& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
#endif
	extern inline SNumber operator%(float& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
	extern inline SNumber operator%(double& num1, const SNumber& num2) { return SNumber(num1) %= num2; };
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

    /**
    * @endcond
    */
}
/**
* @cond
*/
extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::SNumber& num);
/**
* @endcond
*/
#endif
