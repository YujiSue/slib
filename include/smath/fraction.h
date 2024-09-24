#ifndef SMATH_FRACTION_H
#define SMATH_FRACTION_H
#include "smath/calc.h"
#include "sbasic/array.h"
#include "sbasic/string.h"
namespace slib {
	template<typename T>
	class Fraction;
	template<typename T>
	inline void fracform(T& n, T& d) {
		if (d == 0) throw DivZeroException("");
		if (d < 0) { n *= -1; d *= -1; }
		if (n < 0) { n *= -1; smath::reduction(n, d); n *= -1; }
		else smath::reduction(n, d);
	}
	template<typename T>
	class Fraction {
	public:
		T numerator, denominator;
	public:
		Fraction() : numerator(0), denominator(1) {}
		Fraction(const T n, const T d = 1) {
			if (!d) throw DivZeroException("Denominator of fraction is zero.");
			numerator = n; denominator = d;
			fracform(numerator, denominator);
		}
		Fraction(double d, int iter = 5, double e = 0.001) {
			bool neg = d < 0;
			auto e_ = 1.0 - e;
			if (neg) d *= -1;
			T i0 = (T)d, i1 = 0;
			d -= (double)i0;
			if (d < e || e_ < d) {
				numerator = (neg ? -1 : 1) * (i0 + (e_ < d ? 1 : 0));
				denominator = 1;
			}
			else {
				double d_ = d, diff;
				auto count = 1;
				while(count <= iter) {
					d_ *= 10.0;
					i1 = (T)d_;
					diff = d_ - (double)i1;
					if (diff < e || e_ < diff) {
						denominator = smath::power(10, count);
						numerator = (neg ? -1 : 1) * (i0 * denominator + i1 + (e_ < diff ? 1 : 0));
						fracform(numerator, denominator);
						return;
					}
					else {
						diff -= d;
						if (diff < e || e_ < diff) {
							denominator = smath::power(10, count) - 1;
							numerator = (neg ? -1 : 1) * (i0 * denominator + i1 + (e_ < diff ? 1 : 0));
							fracform(numerator, denominator);
							return;
						}
					}
					++count;
				}
				denominator = smath::power(10, iter);
				numerator = (neg ? -1 : 1) * (i0 * denominator + (T)d_);
				fracform(numerator, denominator);
			}
		}
		Fraction(const char* s) {
			auto tmp = sstr::trim(s);
			if (tmp.match("/")) {
				numerator = (T)tmp.substring(0, tmp.find("/")).integer();
				denominator = (T)tmp.substring(tmp.find("/") + 1).integer();
			}
			else if (tmp.match("\\frac")) {
				auto vals = tmp.split("{");
				numerator = (T)(vals[1].substring(0, -2).integer());
				denominator = (T)(vals[2].substring(0, -2).integer());
			}
			else {
				numerator = (T)tmp.integer();
				denominator = 1;
			}
			fracform(numerator, denominator);
		}
		Fraction(const Fraction& frac) : numerator(frac.numerator), denominator(frac.denominator) {}
		~Fraction() {}
		Fraction<T>& operator=(const Fraction<T>& f) { numerator = f.numerator; denominator = f.denominator; return *this; }
		Fraction<T>& operator+=(const Fraction<T>& f) {
			if (denominator != f.denominator) {
				auto d = smath::lcm(denominator, f.denominator);
				numerator = numerator * d / denominator + f.numerator * d / f.denominator;
				denominator = d;
			}
			else { numerator += f.numerator; smath::reduction(numerator, denominator); }
			return *this;
		}
		Fraction<T>& operator-=(const Fraction<T>& f) {
			auto d = smath::lcm(denominator, f.denominator);
			auto n = numerator * d / denominator - f.numerator * d / f.denominator;
			*this = Fraction(n, d);
			return *this;
		}
		Fraction<T>& operator*=(const Fraction<T>& f) {
			numerator *= f.numerator;
			denominator *= f.denominator;
			fracform(numerator, denominator);
			return *this;
		}
		Fraction<T>& operator/=(const Fraction<T>& f) {
			if (!f.numerator) throw DivZeroException("Numerator of divider fraction is zero.");
			return (*this) *= f.reciprocal();
		}
		Fraction operator-() const { return Fraction<T>(-numerator, denominator); }
		Fraction operator+(const Fraction& f) const { Fraction<T> frac(*this); return frac += f; }
		Fraction operator-(const Fraction& f) const { Fraction<T> frac(*this); return frac -= f; }
		Fraction operator*(const Fraction& f) const { Fraction<T> frac(*this); return frac *= f; }
		Fraction operator/(const Fraction& f) const { Fraction<T> frac(*this); return frac /= f; }
		Fraction reciprocal() const { return Fraction<T>(denominator, numerator); }
		bool isInteger() const { return denominator == 1 || numerator == 0; }
		int quotient() const {
			if (numerator < 0) return -((-numerator) / denominator);
			else return numerator / denominator;
		}
		int remainder() const {
			return numerator - (quotient() * denominator);
		}
		int intValue() const { return (int)numerator / denominator; }
		float floatValue() const { return (float)numerator / denominator; }
		double doubleValue() const { return (double)numerator / denominator; }
		String toString(const char* format = nullptr) const {
			if (!denominator) return "NaN";
			else if (denominator == 1) return String(numerator);
			else {
				String f(format), numer(numerator), denom(denominator);
				if (f.endWith("tex")) return "\\frac{" + numer + "}{" + denom + "}";
				else if (f == "aa") {
					int sz = (int)(numer.size() < denom.size() ? denom.size() : numer.size()) + 2;
					return sstr::bfill(numer, ' ', sz) + NL + (S("-") * sz) + NL + sstr::bfill(denom, ' ', sz);
				}
				else return numer + "/" + denom;
			}
		}
		bool operator < (const Fraction& f) const {
			auto d = smath::lcm(denominator, f.denominator);
			return numerator * d / denominator < f.numerator* d / f.denominator;
		}
		bool operator == (const Fraction& f) const {
			return f.numerator == numerator && f.denominator == denominator;
		}
	};
	template<typename T>
	extern Fraction<T> operator+(T t, const Fraction<T>& f) { return Fraction<T>(t) + f; }
	template<typename T>
	extern Fraction<T> operator-(T t, const Fraction<T>& f) { return Fraction<T>(t) - f; }
	template<typename T>
	extern Fraction<T> operator*(T t, const Fraction<T>& f) { return Fraction<T>(t) * f; }
	template<typename T>
	extern Fraction<T> operator/(T t, const Fraction<T>& f) { return Fraction<T>(t) / f; }

	template<typename T>
	extern bool operator<(T t, const Fraction<T>& f) { return Fraction<T>(t) < f; }
	template<typename T>
	extern bool operator==(T t, const Fraction<T>& f) { return Fraction<T>(t) == f; }

	using sfrac = Fraction<sint>;
	using sfracl = Fraction<sinteger>;




/*
template<typename T>
String Fraction<T>::rounded(size_t size, smath::ROUND rnd) const {
	if (!denominator) return "NaN";
	bool neg = false;
	auto tmp = (double)numerator * smath::power(10, (int)size) / denominator;
	String str;
	switch (rnd) {
	case smath::FLOOR:
		str = std::to_string((int)tmp);
		break;
	case smath::CEIL:
	{
		if (tmp < 0) str = std::to_string(((int)tmp) - 1);
		else str = std::to_string(((int)tmp) + 1);
		break;
	}
	default:
	{
		auto remain = abs(tmp - (int)tmp);
		if (remain < 0.5) str = std::to_string((int)tmp);
		else {
			if (tmp < 0) str = std::to_string(((int)tmp) - 1);
			else str = std::to_string(((int)tmp) + 1);
		}
		break;
	}
	}
	if (tmp < 0) {
		if (size < str.length() - 1) str.insert(str.length() - size, ".");
		else if (size == str.length() - 1) str.insert(1, "0.");
		else {
			str.clip(1);
			str.fill(size, '0', true);
			str.insert(0, "-0.");
		}
	}
	else {
		if (size < str.length()) str.insert(str.length() - size, ".");
		else if (size == str.length()) str.insert(0, "0.");
		else {
			str.fill(size, '0', true);
			str.insert(0, "0.");
		}
	}
	return str;
}
*/


}
template<typename T>
extern std::ostream& operator<<(std::ostream& os, const slib::Fraction<T>& frac) { return os << frac.toString(); }

#endif