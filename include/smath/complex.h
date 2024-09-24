#ifndef SMATH_COMPLEX_H
#define SMATH_COMPLEX_H
#include "smath/mbasic.h"
#include "sbasic/array.h"
namespace slib {
    /**
    * @class Complex
    * \~english @brief Complex number class
    * \~japanese @brief 複素数クラス
    */
    template<typename T>
    class Complex {
    public:
        T real, imaginary;
    public:
        Complex() : real(0), imaginary(0) {}
        Complex(const T r, const T i = 0.0) : real(r), imaginary(i) {}
        Complex(const char* s) {
            auto tmp = String(s).trim();
            if (tmp[-1] == 'i') {
                auto nums = tmp.searchAll(REG_(/[+-]*[0-9.]+/g));
                if (nums.size() == 1) {
                    real = 0.f;
                    imaginary = nums[0].second.floatValue();
                }
                else {
                    real = nums[0].second.floatValue();
                    imaginary = nums[1].second.floatValue();
                }
            }
            else {
                real = tmp.floatValue();
                imaginary = 0.f;
            }
        }
        Complex(const Complex& comp) : real(comp.real), imaginary(comp.imaginary) {}
        ~Complex() {}
        Complex& operator=(const Complex& comp) {
            real = comp.real;
            imaginary = comp.imaginary;
            return *this;
        }
        Complex& operator+=(const Complex& comp) {
            real += comp.real;
            imaginary += comp.imaginary;
            return *this;
        }
        Complex& operator-=(const Complex& comp) {
            real -= comp.real;
            imaginary -= comp.imaginary;
            return *this;
        }
        Complex& operator*=(const Complex& comp) {
            T tmp_r = real, tmp_i = imaginary;
            real = tmp_r * comp.real - tmp_i * comp.imaginary;
            imaginary = tmp_r * comp.imaginary + tmp_i * comp.real;
            return *this;
        }
        Complex& operator/=(const T val) {
            real /= val; imaginary /= val;
            return *this;
        }
        Complex& operator/=(const Complex& comp) {
            if (comp.real == 0.f && comp.imaginary == 0.f) throw DivZeroException("Divider complex is zero at 'Complex::operator /='.");
            auto tmp = comp;
            tmp.conjugate();
            (*this) *= tmp;
            (*this) /= absolute();
            return *this;
        }
        Complex operator-() const { return Complex<T>(-real, -imaginary); }
        Complex operator+(const Complex& comp) const {
            Complex<T> c(*this);
            return c += comp;
        }
        Complex operator-(const Complex& comp) const {
            Complex<T> c(*this);
            return c -= comp;
        }
        Complex operator*(const Complex& comp) const {
            Complex<T> c(*this);
            return c *= comp;
        }
        Complex operator/(const Complex& comp) const {
            Complex c(*this);
            return c /= comp;
        }
        void conjugate() { imaginary *= -1; }
        double absolute() const { return sqrt(real * real + imaginary * imaginary); }
        double argument() const {
            auto len = absolute();
            if (snum::D_EPS < len) return imaginary < 0 ? (2 * snum::PI - acos(real / len)) : acos(real / len);
            else throw DivZeroException(divZeroErrorText("Euclidean norm"));
        }
        bool isImaginary() const { return real == 0.0f; }
        bool isReal() const { return imaginary == 0.0f; }

        String toString(const char* format = nullptr) const {
            String f(format);
            if (f.beginWith("trig")) {
              auto abs = String(absolute()), arg = String(argument());
              return abs + "*(cos(" + arg + ") + sin(" + arg + ")i)";
            }
            else {
              if (imaginary == 0.f) return String(real);
              else if (imaginary < 0.f) return String(real) + String(imaginary) + "i";
              return String(real) + "+" + String(imaginary) + "i";
            }
        }
        bool operator < (const Complex& c) const {
            return real < c.real || (real == c.real && imaginary < c.imaginary);
        }
        bool operator == (const Complex& c) const {
            return real == c.real && imaginary == c.imaginary;
        }
    };

    template<typename T>
    Complex<T> conjugate(const Complex<T>& comp) {
        auto c = comp;
        c.conjugate();
        return c;
    }
    template<typename T>
    extern inline Complex<T> operator+(T t, const Complex<T>& comp) { return Complex<T>(t) + comp; }
    template<typename T>
    extern inline Complex<T> operator-(T t, const Complex<T>& comp) { return Complex<T>(t) - comp; }
    template<typename T>
    extern inline Complex<T> operator*(T t, const Complex<T>& comp) { return Complex<T>(t) * comp; }
    template<typename T>
    extern inline Complex<T> operator/(T t, const Complex<T>& comp) { return Complex<T>(t) / comp; }
    template<typename T>
    extern inline bool operator<(T t, const Complex<T>& comp) { return Complex<T>(t) < comp; }
    template<typename T>
    extern inline bool operator==(T t, const Complex<T>& comp) { return Complex<T>(t) < comp; }
    template<typename T>
    extern std::ostream& operator<<(std::ostream& os, const Complex<T>& comp) { return os << comp.real << (comp.imaginary < 0 ? "" : "+") << comp.imaginary << (comp.imaginary == 0 ? "" : "i"); }
}
using scomplex = slib::Complex<float>;
using scomplexd = slib::Complex<double>;
#endif