#ifndef SMATH_COMPLEX_H
#define SMATH_COMPLEX_H

#include "smath/mathbasic.h"
#include "sbasic/string.h"

namespace slib {
    namespace smath {
                #define scomp Complex<float>
                template<typename T>
        struct Complex {
            T real, imaginary;

            Complex();
            Complex(T r, T i = 0.0);
            Complex(const char *s);
            Complex(const Complex &comp);
            ~Complex();
            Complex &operator=(const Complex &comp);
            Complex &operator+=(const Complex &comp);
            Complex &operator+=(T t);
            Complex &operator-=(const Complex &comp);
            Complex &operator-=(T t);
            Complex &operator*=(const Complex &comp);
            Complex &operator*=(T t);
            Complex &operator/=(const Complex &comp);
            Complex &operator/=(T t);
                        Complex operator-() const;
            Complex operator+(const Complex &comp) const;
            Complex operator+(T t) const;
            Complex operator-(const Complex &comp) const;
            Complex operator-(T t) const;
            Complex operator*(const Complex &comp) const;
            Complex operator*(T t) const;
            Complex operator/(const Complex &comp) const;
            Complex operator/(T t) const;
            void conjugate();
            Complex conjugated() const;
            double absolute() const;
            double argument() const;
            bool isImaginary() const;
            bool isReal() const;
            String rounded(size_t size, smath::ROUND rnd = smath::DEFAULT) const;
            String toString() const;
            bool operator < (const Complex &c) const;
            bool operator == (const Complex &c) const;
            bool operator != (const Complex &c) const;
        };
        template<typename T>
        extern inline Complex<T> operator+(T t, const Complex<T> &comp) { return Complex<T>(t) + comp; }
        template<typename T>
        extern inline Complex<T> operator-(T t, const Complex<T> &comp) { return Complex<T>(t) - comp; }
        template<typename T>
        extern inline Complex<T> operator*(T t, const Complex<T> &comp) { return Complex<T>(t) * comp; }
        template<typename T>
        extern inline Complex<T> operator/(T t, const Complex<T> &comp) { return Complex<T>(t) / comp; }
        template<typename T>
        extern inline bool operator<(T t, const Complex<T> &comp) { return Complex<T>(t) < comp; }
        template<typename T>
        extern inline bool operator==(T t, const Complex<T> &comp) { return Complex<T>(t) < comp; }

        /*============================================================*/

        template<typename T>
        Complex<T>::Complex() : real(0), imaginary(0) {}
        template<typename T>
        Complex<T>::Complex(T r, T i) : real(r), imaginary(i) {}
        template<typename T>
        Complex<T>::Complex(const char *s) {
            auto tmp = String::trim(s);
            if (tmp.last() == 'i') {
                auto nums = tmp.matched(R(/[+-]*[0-9.]+/g));
                if (nums.size() == 1) {
                    real = 0.f;
                    imaginary = nums[0].floatValue();
                }
                else {
                    real = nums[0].floatValue();
                    imaginary = nums[1].floatValue();
                }
            }
            else {
                real = tmp.floatValue();
                imaginary = 0.f;
            }
        }
        template<typename T>
        Complex<T>::Complex(const Complex<T> &comp) : real(comp.real), imaginary(comp.imaginary) {}
        template<typename T>
        Complex<T>::~Complex() {}

        template<typename T>
        Complex<T> &Complex<T>::operator=(const Complex<T> &comp) {
            real = comp.real;
            imaginary = comp.imaginary;
            return *this;
        }
        template<typename T>
                Complex<T> &Complex<T>::operator+=(const Complex<T> &comp) {
            real += comp.real;
            imaginary += comp.imaginary;
            return *this;
        }
        template<typename T>
                Complex<T> &Complex<T>::operator+=(T t) {
            real += t;
            return *this;
        }
        template<typename T>
                Complex<T> &Complex<T>::operator-=(const Complex<T> &comp) {
            real -= comp.real;
            imaginary -= comp.imaginary;
            return *this;
        }
        template<typename T>
                Complex<T> &Complex<T>::operator-=(T t) {
            real -= t;
            return *this;
        }
        template<typename T>
                Complex<T> &Complex<T>::operator*=(const Complex<T> &comp) {
            float tmp_r = real, tmp_i = imaginary;
            real = tmp_r*comp.real-tmp_i*comp.imaginary;
            imaginary = tmp_r*comp.imaginary+tmp_i*comp.real;
            return *this;
        }
        template<typename T>
                Complex<T> &Complex<T>::operator*=(T t) {
            real *= t;
            imaginary *= t;
            return *this;
        }
        template<typename T>
                Complex<T> &Complex<T>::operator/=(const Complex<T> &comp) {
            if (comp.real == 0.f && comp.imaginary == 0.f)
                throw SMathException(ERR_INFO, DIV_ZERO_ERR, "comp");
            (*this) *= comp.conjugated();
            (*this) /= absolute();
            return *this;
        }
        template<typename T>
                Complex<T> &Complex<T>::operator/=(T t) {
            if (t == 0.f) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "t");
            real /= t;
            imaginary /= t;
            return *this;
        }
                template<typename T>
                Complex<T> Complex<T>::operator-() const { return Complex<T>(-real, -imaginary); }
        template<typename T>
                Complex<T> Complex<T>::operator+(const Complex<T> &comp) const {
                        Complex<T> c(*this);
            return c+=comp;
        }
        template<typename T>
                Complex<T> Complex<T>::operator+(T t) const {
            Complex<T> c(*this);
            return c+=t;
        }
        template<typename T>
                Complex<T> Complex<T>::operator-(const Complex<T> &comp) const {
            Complex<T> c(*this);
            return c-=comp;
        }
        template<typename T>
                Complex<T> Complex<T>::operator-(T t) const {
            Complex<T> c(*this);
            return c-=t;
        }
        template<typename T>
                Complex<T> Complex<T>::operator*(const Complex<T> &comp) const {
            Complex<T> c(*this);
            return c*=comp;
        }
        template<typename T>
                Complex<T> Complex<T>::operator*(T t) const {
            Complex<T> c(*this);
            return c*=t;
        }
        template<typename T>
                Complex<T> Complex<T>::operator/(const Complex<T> &comp) const {
            Complex c(*this);
            return c/=comp;
        }
        template<typename T>
                Complex<T> Complex<T>::operator/(T t) const {
            Complex c(*this);
            return c/=t;
        }
        template<typename T>
                void Complex<T>::conjugate() {
            imaginary *= -1;
        }
        template<typename T>
                Complex<T> Complex<T>::conjugated() const {
            Complex comp = *this;
            comp.conjugate();
            return comp;
        }
        template<typename T>
                double Complex<T>::absolute() const { return sqrt((double)real*real+(double)imaginary*imaginary); }
        template<typename T>
                double Complex<T>::argument() const {
                        auto len = absolute();
                        if (D_EPS < len) return imaginary < 0 ? 2 * smath::PI - acos((double)real / len) : acos((double)real / len);
                        throw SMathException(ERR_INFO, DIV_ZERO_ERR);
                }
        template<typename T>
                bool Complex<T>::isImaginary() const { return real == 0.0f; }
        template<typename T>
                bool Complex<T>::isReal() const { return imaginary == 0.0f; }
        template<typename T>
                String Complex<T>::rounded(size_t size, smath::ROUND rnd) const {
            String str1, str2;
            double tmp1 = real*smath::power(10, size), tmp2 = imaginary*smath::power(10, size);
            switch (rnd) {
                case FLOOR:
                    str1 = std::to_string((int)tmp1);
                    str2 = std::to_string((int)tmp2);
                    break;
                case CEIL:
                {
                    if (tmp1 < 0) str1 = std::to_string((int)tmp1-1);
                    else str1 = std::to_string((int)tmp1+1);
                    if (tmp2 < 0) str2 = std::to_string((int)tmp2-1);
                    else str2 = std::to_string((int)tmp2+1);
                    break;
                }
                default:
                {
                    auto remain1 = abs(tmp1-(int)tmp1), remain2 = abs(tmp2-(int)tmp2);
                    if (remain1 < 0.5) str1 = std::to_string((int)tmp1);
                    else {
                        if (tmp1 < 0) str1 = std::to_string((int)tmp1-1);
                        else str1 = std::to_string((int)tmp1+1);
                    }
                    if (remain2 < 0.5) str2 = std::to_string((int)tmp2);
                    else {
                        if (tmp2 < 0) str2 = std::to_string((int)tmp2-1);
                        else str2 = std::to_string((int)tmp2+1);
                    }
                    break;
                }
            }
            if (tmp1 < 0) {
                if (size < str1.length()-1) str1.insert(str1.length()-size, ".");
                else if (size == str1.length()-1) str1.insert(1, "0.");
                else {
                    str1.clip(1);
                    str1.fill(size, '0', true);
                    str1.insert(0, "-0.");
                }
            }
            else {
                if (size < str1.length()) str1.insert(str1.length()-size, ".");
                else if (size == str1.length()) str1.insert(0, "0.");
                else {
                    str1.fill(size, '0', true);
                    str1.insert(0, "0.");
                }
            }
            if (tmp2 < 0) {
                if (size < str2.length()-1) str2.insert(str2.length()-size, ".");
                else if (size == str2.length()-1) str2.insert(1, "0.");
                else {
                    str2.clip(1);
                    str2.fill(size, '0', true);
                    str2.insert(0, "-0.");
                }
            }
            else {
                if (size < str2.length()) str2.insert(str2.length()-size, ".");
                else if (size == str2.length()) str2.insert(0, "0.");
                else {
                    str2.fill(size, '0', true);
                    str2.insert(0, "0.");
                }
            }
            if (real == 0.0f) return str2+"i";
            else if (imaginary == 0.0f) return str1;
            else if(imaginary < 0.0f) return str1+str2+"i";
            else return str1+"+"+str2+"i";
        }
        template<typename T>
                String Complex<T>::toString() const {
            if (imaginary == 0.f) return String(real);
            else if (imaginary < 0.f) return String(real)+String(imaginary)+"i";
            return String(real)+"+"+String(imaginary)+"i";
        }
        template<typename T>
                bool Complex<T>::operator < (const Complex<T> &c) const {
            return real < c.real || (real == c.real && imaginary < c.imaginary);
        }
        template<typename T>
                bool Complex<T>::operator == (const Complex<T> &c) const {
            return real == c.real && imaginary == c.imaginary;
        }
        template<typename T>
                bool Complex<T>::operator != (const Complex<T> &c) const { return !(*this == c); }
    }
}

#endif