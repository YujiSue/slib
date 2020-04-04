#ifndef SMATH_FRAC_H
#define SMATH_FRAC_H

#include "smath/calc.h"
#include "smath/la.h"
#include "sbasic/string.h"

namespace slib {
  	namespace smath {
		#define sfrac Fraction<sint>
        template<typename T>
        struct Fraction {
            T numerator, denominator;
            
            Fraction();
            Fraction(T n, T d = 1, bool r = true);
			Fraction(SVector2D<T> vec, bool r = true);
            Fraction(double d);
            Fraction(const char *s);
            Fraction(const Fraction &frac);
            ~Fraction();
            
            Fraction &operator=(const Fraction &f);
            Fraction &operator+=(const Fraction &f);
            Fraction &operator+=(T t);
            Fraction &operator-=(const Fraction &f);
            Fraction &operator-=(T t);
            Fraction &operator*=(const Fraction &f);
            Fraction &operator*=(T t);
            Fraction &operator/=(const Fraction &f);
            Fraction &operator/=(T t);
            
            Fraction operator+(const Fraction &f) const;
            Fraction operator+(T t) const;
            Fraction operator-(const Fraction &f) const;
            Fraction operator-(T t) const;
            Fraction operator*(const Fraction &f) const;
            Fraction operator*(T t) const;
            Fraction operator/(const Fraction &f) const;
            Fraction operator/(T t) const;
            Fraction reciprocal() const;
            
            bool isInteger() const;
            int quotient() const;
            int remainder() const;
            
            operator int() const;
            operator float() const;
            operator double() const;
            
            String precised(size_t size, smath::ROUND round = smath::DEFAULT) const;
            String toString() const;
            
            bool operator < (T t) const;
            bool operator < (const Fraction &f) const;
            bool operator == (T t) const;
            bool operator == (const Fraction &f) const;
            bool operator != (T t) const;
            bool operator != (const Fraction &f) const;
        };
        template<typename T>
        extern Fraction<T> operator+(T t, const Fraction<T> &f) { return Fraction<T>(t) + f; }
        template<typename T>
        extern Fraction<T> operator*(T t, const Fraction<T> &f) { return Fraction<T>(t) - f; }
        template<typename T>
        extern Fraction<T> operator-(T t, const Fraction<T> &f) { return Fraction<T>(t) * f; }
        template<typename T>
        extern Fraction<T> operator/(T t, const Fraction<T> &f) { return Fraction<T>(t) / f; }
        template<typename T>
        extern bool operator<(T t, const Fraction<T> &f) { return sfrac(t) < f; }
        template<typename T>
        extern bool operator==(T t, const Fraction<T> &f) { return sfrac(t) == f; }

        /*============================================================*/
        
        template<typename T>
        inline void sfracForm(Fraction<T> &f) {
            if(f.denominator < 0) {
                f.numerator *= -1; f.denominator *= -1;
            }
            if (f.numerator < 0) {
                f.numerator *= -1;
                smath::reduction(f.numerator, f.denominator);
                f.numerator *= -1;
            }
            else smath::reduction(f.numerator, f.denominator);
        }
        template<typename T>
        Fraction<T>::Fraction() : numerator(1), denominator(1) {}
        template<typename T>
        Fraction<T>::Fraction(T n, T d, bool r) {
            if (!d) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR, "denominator");
            numerator = n; denominator = d;
            if(numerator*denominator <= 0 && denominator < 0) {
                numerator *= -1; denominator *= -1;
            }
            if(r) smath::reduction(numerator, denominator);
        }
		template<typename T>
		Fraction<T>::Fraction(SVector2D<T> vec, bool r) {
			if (!vec.y) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR, "denominator");
			numerator = vec.x; denominator = vec.y;
			if (numerator * denominator <= 0 && denominator < 0) {
				numerator *= -1; denominator *= -1;
			}
			if (r) smath::reduction(numerator, denominator);
		}
        template<typename T>
        Fraction<T>::Fraction(double d) {
            String s(d);
            size_t pos = s.find(".");
            if (pos == std::string::npos) { numerator = (int)d; denominator = 1; }
            else {
                int i = 1, deg = 10;
                while (i < 10) {
                    double dif = d*deg-d;
                    auto tmp = String(dif);
                    if ((pos = tmp.find("000")) != NOT_FOUND) {
                        tmp.clip(0, pos);
                        if ((pos = tmp.find(".")) != NOT_FOUND) {
                            --deg;
                            for (int p = pos+1; p < tmp.length(); ++p) deg*=10;
                            tmp.replace(".", "");
                            numerator = tmp.intValue(); denominator = deg;
                        }
                        sfracForm(*this);
                        return;
                    }
                    deg *= 10;
                    ++i;
                }
                numerator = (int)(d*deg-d); denominator = deg-1;
                sfracForm(*this);
            }
        }
        template<typename T>
        Fraction<T>::Fraction(const char *s) {
            auto tmp = String::trim(s);
            if (tmp.contain("/")) {
                numerator = (T)tmp.substring(0, tmp.find("/")).integer();
                denominator = (T)tmp.substring(tmp.find("/")+1).integer();
            }
            else {
                numerator = (T)tmp.integer();
                denominator = 1;
            }
            sfracForm(*this);
        }
        template<typename T>
        Fraction<T>::Fraction(const Fraction<T> &frac) : numerator(frac.numerator), denominator(frac.denominator) {}
        template<typename T>
        Fraction<T>::~Fraction() {}
        
        template<typename T>
        Fraction<T> &Fraction<T>::operator=(const Fraction<T> &f) { numerator = f.numerator; denominator = f.denominator; return *this; }
        template<typename T>
        Fraction<T> &Fraction<T>::operator+=(const Fraction<T> &f) {
            int d = smath::lcm(denominator, f.denominator);
            int n = numerator*d/denominator + f.numerator*d/f.denominator;
            *this = Fraction(n, d);
            return *this;
        }
        template<typename T>
        Fraction<T> &Fraction<T>::operator+=(T t) { return (*this)+=sfrac(t,1); }
        template<typename T>
        Fraction<T> &Fraction<T>::operator-=(const Fraction<T> &f) { 
			int d = smath::lcm(denominator, f.denominator);
			int n = numerator * d / denominator - f.numerator * d / f.denominator;
			*this = Fraction(n, d);
			return *this;
		}
        template<typename T>
        Fraction<T> &Fraction<T>::operator-=(T t) { return (*this)-=sfrac(t,1); }
        template<typename T>
        Fraction<T> &Fraction<T>::operator*=(const Fraction<T> &f) {
            numerator *= f.numerator;
            denominator *= f.denominator;
            sfracForm(*this);
            return *this;
        }
        template<typename T>
        Fraction<T> &Fraction<T>::operator*=(T t) { return (*this)*=sfrac(t,1); }
        template<typename T>
        Fraction<T> &Fraction<T>::operator/=(const Fraction &f) {
            if (!f.numerator) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR, "f.numerator");
            return (*this) *= f.reciprocal();
        }
        template<typename T>
        Fraction<T> &Fraction<T>::operator/=(T t) {
            if (!t) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR, "t");
            return (*this)/=sfrac(t,1);
        }
        template<typename T>
        Fraction<T> Fraction<T>::operator+(const Fraction<T> &f) const { Fraction<T> frac = *this; return frac+=f; }
        template<typename T>
        Fraction<T> Fraction<T>::operator+(T t) const { Fraction<T> frac = *this; return frac+=t; }
        template<typename T>
        Fraction<T> Fraction<T>::operator-(const Fraction<T> &f) const { Fraction<T> frac(*this); return frac-=f; }
        template<typename T>
        Fraction<T> Fraction<T>::operator-(T t) const { Fraction frac(*this); return frac-=t; }
        template<typename T>
        Fraction<T> Fraction<T>::operator*(const Fraction<T> &f) const { Fraction<T> frac = *this; return frac*=f; }
        template<typename T>
        Fraction<T> Fraction<T>::operator*(T t) const { Fraction<T> frac = *this; return frac*=t; }
        template<typename T>
        Fraction<T> Fraction<T>::operator/(const Fraction<T> &f) const { Fraction<T> frac = *this; return frac/=f; }
        template<typename T>
        Fraction<T> Fraction<T>::operator/(T t) const { Fraction<T> frac = *this; return frac/=t; }
        template<typename T>
        Fraction<T> Fraction<T>::reciprocal() const { return Fraction<T>(denominator, numerator); }
        template<typename T>
        bool Fraction<T>::isInteger() const { return denominator==1 || numerator == 0; }
        template<typename T>
        int Fraction<T>::quotient() const {
            if (numerator < 0) return -((-numerator)/denominator);
            else return numerator/denominator;
        }
        template<typename T>
        int Fraction<T>::remainder() const {
            return numerator-(quotient()*denominator);
        }
        template<typename T>
        Fraction<T>::operator int() const { return numerator/denominator; }
        template<typename T>
        Fraction<T>::operator float() const { return (float)numerator/denominator; }
        template<typename T>
        Fraction<T>::operator double() const { return (double)numerator/denominator; }
        template<typename T>
        String Fraction<T>::precised(size_t size, smath::ROUND round) const {
            if (!denominator) return "NaN";
            bool neg = false;
            auto tmp = (double)numerator*smath::power(10, size)/denominator;
            String str;
            switch (round) {
                case smath::FLOOR:
                    str = std::to_string((int)tmp);
                    break;
                case smath::CEIL:
                {
                    if (tmp < 0) str = std::to_string(((int)tmp)-1);
                    else str = std::to_string(((int)tmp)+1);
                    break;
                }
                default:
                {
                    auto remain = abs(tmp-(int)tmp);
                    if (remain < 0.5) str = std::to_string((int)tmp);
                    else {
                        if (tmp < 0) str = std::to_string(((int)tmp)-1);
                        else str = std::to_string(((int)tmp)+1);
                    }
                    break;
                }
            }
            if (tmp < 0) {
                if (size < str.length()-1) str.insert(str.length()-size, ".");
                else if (size == str.length()-1) str.insert(1, "0.");
                else {
                    str.clip(1);
                    str.fill(size, '0', true);
                    str.insert(0, "-0.");
                }
            }
            else {
                if (size < str.length()) str.insert(str.length()-size, ".");
                else if (size == str.length()) str.insert(0, "0.");
                else {
                    str.fill(size, '0', true);
                    str.insert(0, "0.");
                }
            }
            return str;
        }
        template<typename T>
        String Fraction<T>::toString() const {
            if (!denominator) return "NaN";
            return String(numerator)+"/"+String(denominator);
        }
        template<typename T>
        bool Fraction<T>::operator < (T t) const { return *this < sfrac(t); }
        template<typename T>
        bool Fraction<T>::operator < (const Fraction<T> &f) const {
            int d = smath::lcm(denominator, f.denominator);
            return numerator*d/denominator < f.numerator*d/f.denominator;
        }
        template<typename T>
        bool Fraction<T>::operator == (T t) const{ return *this == sfrac(t); }
        template<typename T>
        bool Fraction<T>::operator == (const Fraction &f) const {
            return f.numerator == numerator && f.denominator == denominator;
        }
        template<typename T>
        bool Fraction<T>::operator != (T t) const { return !(*this == t); }
        template<typename T>
        bool Fraction<T>::operator != (const Fraction<T> &f) const { return !(*this == f); }
        
        
    }
}
#endif
