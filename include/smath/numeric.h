#ifndef SMATH_NUMERIC_H
#define SMATH_NUMERIC_H
#include "smath/mbasic.h"
#include "smath/calc.h"
#include "sbasic/string.h"
namespace slib {
	
	/**
	* @cond
	*/
	template<typename T>
	extern String toHex(const T& val, bool upper = true, ENDIAN en = ENDIAN::LITTLE) {
		int sz = sizeof(val);
		auto hs = upper ? slib::sstr::HEX : slib::sstr::hex;
		auto ptr = reinterpret_cast<const slib::subyte*>(&val);
		slib::String str(2 * sz, '0');
		if (en == slib::ENDIAN::LITTLE) {
			auto cptr = &str[0];
			sforin(i, 0, sz) {
				(*cptr) = hs[((*ptr) >> 4) & 0x0F]; ++cptr;
				(*cptr) = hs[(*ptr) & 0x0F]; ++ptr; ++cptr;
			}
		}
		else {
			auto cptr = &str[-1];
			sforin(i, 0, sz) {
				(*cptr) = hs[(*ptr) & 0x0F]; --cptr;
				(*cptr) = hs[((*ptr) >> 4) & 0x0F]; --ptr; --cptr;
			}
		}
		return str;
	}
	template<typename T>
	extern String toOct(const T& val, ENDIAN en = ENDIAN::LITTLE) {
		int sz = sizeof(val);
		auto ptr = reinterpret_cast<const slib::subyte*>(&val);
		slib::String str(3 * sz, '0');
		if (en == slib::ENDIAN::LITTLE) {
			auto cptr = &str[0];
			sforin(i, 0, sz) {
				(*cptr) = slib::sstr::arabian[((*ptr) >> 6) & 0x07]; ++cptr;
				(*cptr) = slib::sstr::arabian[((*ptr) >> 3) & 0x07]; ++cptr;
				(*cptr) = slib::sstr::arabian[(*ptr) & 0x07]; ++ptr; ++cptr;
			}
		}
		else {
			auto cptr = &str[-1];
			sforin(i, 0, sz) {
				(*cptr) = slib::sstr::arabian[(*ptr) & 0x07]; --cptr;
				(*cptr) = slib::sstr::arabian[((*ptr) >> 3) & 0x07]; --cptr;
				(*cptr) = slib::sstr::arabian[((*ptr) >> 6) & 0x07]; --ptr; --cptr;
			}
		}
		return str;
	}
	template<typename T>
	extern String toBinary(const T& val, ENDIAN en = ENDIAN::LITTLE) {
		int sz = sizeof(val);
		auto ptr = reinterpret_cast<const slib::subyte*>(&val);
		slib::String str(8 * sz, '0');
		if (en == slib::ENDIAN::LITTLE) {
			auto cptr = &str[0];
			sforin(i, 0, sz) {
				sforin(j, 0, 8) { (*cptr) = (((*ptr) >> j) & 0x01) ? '1' : '0'; ++cptr; } ++ptr;
			}
		}
		else {
			auto cptr = &str[-1];
			sforin(i, 0, sz) {
				sforin(j, 0, 8) { (*cptr) = (((*ptr) >> j) & 0x01) ? '1' : '0'; --cptr; } --ptr;
			}
		}
		return str;
	}
	template<typename T>
	extern T round(const T& val, int digit, smath::ROUND rnd = smath::ROUND::DEFAULT) {
		auto border = smath::power<int>(10, digit);
		if (std::abs(val) < border) {
			if (rnd == smath::ROUND::TO_ZERO) return 0;
			else if (val < 0) {
				if (rnd == smath::ROUND::FLOOR || rnd == smath::ROUND::TO_NINF) return -border;
				else if (rnd == smath::ROUND::CEIL || rnd == smath::ROUND::TO_INF) return 0;
				else return (val <= -(border / 2) ? -border : 0);
			}
			else {
				if (rnd == smath::ROUND::FLOOR || rnd == smath::ROUND::TO_NINF) return 0;
				else if (rnd == smath::ROUND::CEIL || rnd == smath::ROUND::TO_INF) return border;
				else return (val < (border / 2) ? 0: border);
			}
		}
		else {
			if (val < 0) {
				auto val_ = -val;
				if (rnd == smath::ROUND::FLOOR || rnd == smath::ROUND::TO_NINF) return -(val_ - (val_ % border) + border);
				else if (rnd == smath::ROUND::CEIL || rnd == smath::ROUND::TO_INF || rnd == smath::ROUND::TO_ZERO) return -(val_ - (val_ % border));
				else {
					if ((val_ % border) < (border / 2)) return -(val_ - (val_ % border));
					else return -(val_ - (val_ % border) + border);
				}
			}
			else {
				if (rnd == smath::ROUND::FLOOR || rnd == smath::ROUND::TO_NINF || rnd == smath::ROUND::TO_ZERO) return val - (val % border);
				else if (rnd == smath::ROUND::CEIL || rnd == smath::ROUND::TO_INF) return val - (val % border) + border;
				else {
					if ((val % border) < (border / 2)) return val - (val % border);
					else return val - (val % border) + border;
				}
			}
		}
	}
	template<typename T>
	extern String precisedString(const T& val, int digit, smath::ROUND rnd = smath::ROUND::DEFAULT) {
		auto str = String(val);
		auto pos = str.find(".");
		if (pos == NOT_FOUND) str << "." << (String("0") * digit);
		else {
			auto substr = str.substring(pos + 1);
			if (substr.size() < digit) str << (String("0") * (digit - substr.size()));
			else if (digit < substr.size()) {
				str.resize(str.size() - (substr.size() - digit));
				if (rnd == smath::ROUND::DEFAULT && 4 < sutf8::toNum(substr[digit])) {
					str[-1] = sutf8::toChar(sutf8::toNum(str[-1]) + 1);
				}
				if (val < 0 && 
					(rnd == smath::ROUND::TO_NINF || rnd == smath::ROUND::FLOOR)) {
					str[-1] = sutf8::toChar(sutf8::toNum(str[-1]) + 1);
				}
				else if (0 < val && 
					(rnd == smath::ROUND::TO_INF || rnd == smath::ROUND::CEIL)) {
					str[-1] = sutf8::toChar(sutf8::toNum(str[-1]) + 1);
				}
			}
		}
		return str;
	}
	template<typename T>
	extern String numToString(const T& val, const char *f = nullptr, ENDIAN en = ENDIAN::LITTLE) {
		if (!f || f[0] == '\0') return std::to_string(val);
		slib::String form(f);
		if (form.beginWith("bin")) return toBinary(val, en);
		else if (form.beginWith("oct")) return toOct(val, en);
		else if (form.beginWith("hex")) return toHex(val, true, en);
		Regex reg("/^%([#0+-]*)(\\d*)(\\.*)(\\d*)([hl]*)([defoux]{1})$/");
		if (reg.equal(form)) {
			int sz = 256, count = 0;
			String str(sz, '\0');
			while (true) {
				count = sprintf(&str[0], form, val);
				if (count < sz) {
					str.resize(count);
					break;
				}
				else {
					sz <<= 1;
					str.resize(sz);
				}
			}
			return str;
		}
		else throw FormatException(formatErrorText("format", f, "^%([#0+-]*)(\\d*)(\\.*)(\\d*)([hl]*)([defoux]{1})$"));
	}
	extern SLIB_DLL String toString(const bool val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	extern SLIB_DLL String toString(const sbyte val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	extern SLIB_DLL String toString(const subyte val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	extern SLIB_DLL String toString(const sshort val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	extern SLIB_DLL String toString(const sushort val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	extern SLIB_DLL String toString(const int val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	extern SLIB_DLL String toString(const unsigned int val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	extern SLIB_DLL String toString(const size_t val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	//extern String toString(const sint val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	//extern String toString(const suint val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	extern SLIB_DLL String toString(const sinteger val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	//extern SLIB_DLL String toString(const suinteger val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	extern SLIB_DLL String toString(const float val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	extern SLIB_DLL String toString(const double val, const char* f = nullptr, ENDIAN en = ENDIAN::LITTLE);
	
	/**
	* @endcond
	*/
}
#endif
