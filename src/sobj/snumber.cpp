#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sdata.h"
#include "sobj/sarray.h"
using namespace slib;

String SNumber::toBinary(subyte b, bool le) {
	String str(8, 0);
	if (le) srforin(i, 0, 8) { str[i] = slib::sstr::HEX[(b & 0x01)]; b >>= 1; }
	else sforin(i, 0, 8) { str[i] = slib::sstr::HEX[(b & 0x01)]; b >>= 1; }
	return str;
}
String SNumber::toOct(subyte b) { return String({ slib::sstr::HEX[(b >> 6) & 0x07], slib::sstr::HEX[((b >> 3) & 0x07)] , slib::sstr::HEX[(b & 0x07)] }); }
String SNumber::toHex(subyte b) { return String({ slib::sstr::HEX[(b >> 4) & 0x0F], slib::sstr::HEX[(b & 0x0F)] }); }
//SNumber SNumber::toNumber(const char *s) { return String(s).number(); }
SNumber::SNumber() : _type(stype::INTEGER) { _value._i = 0; }
SNumber::SNumber(char c) : _type(stype::INTEGER) { _value._i = (sbyte)c; }
SNumber::SNumber(bool b) : _type(stype::BOOL) { _value._b = b; }
SNumber::SNumber(sbyte i) : _type(stype::INTEGER) { _value._i = i; }
SNumber::SNumber(subyte ui) : _type(stype::UINTEGER) { _value._ui = ui; }
SNumber::SNumber(sshort i) : _type(stype::INTEGER) { _value._i = i; }
SNumber::SNumber(sushort ui) : _type(stype::UINTEGER) { _value._ui = ui; }
SNumber::SNumber(int i) : _type(stype::INTEGER) { _value._i = i; }
SNumber::SNumber(unsigned int ui) : _type(stype::UINTEGER) { _value._ui = ui; }
SNumber::SNumber(size_t ui) : _type(stype::UINTEGER) { _value._ui = ui; }
#ifdef WIN_OS
SNumber::SNumber(long i) : _type(stype::INTEGER) { _value._i = i; }
#ifndef MAC_OS
SNumber::SNumber(unsigned long ui) : _type(stype::UINTEGER) { _value._ui = ui; }
#endif
#endif
SNumber::SNumber(long long i) : _type(stype::INTEGER) { _value._i = i; }
#ifdef MAC_OS
SNumber::SNumber(unsigned long long ui) : _type(stype::UINTEGER) { _value._ui = ui; }
#endif
#ifdef LINUX_OS
SNumber::SNumber(sinteger i) : _type(stype::INTEGER) { _value._i = i; }
#endif
SNumber::SNumber(float f) : _type(stype::REAL) { _value._r = f; }
SNumber::SNumber(double d) : _type(stype::REAL) { _value._r = d; }
SNumber::SNumber(sint n, sint d) : _type(stype::FRACTION) { _value._f[0] = n; _value._f[1] = d; }
SNumber::SNumber(const sfrac &frac) : _type(stype::FRACTION) {
    _value._f[0] = frac.numerator; _value._f[1] = frac.denominator;
}
SNumber::SNumber(float r, float i) : _type(stype::COMPLEX) { _value._c[0] = r; _value._c[1] = i; }
SNumber::SNumber(const Complex<float>& comp) : _type(stype::COMPLEX) {
    _value._c[0] = comp.real; _value._c[1] = comp.imaginary;
}
SNumber::SNumber(const char* s) {
    String str(sstr::toLower(sstr::trim(s)));
#if defined(__GNUC__) && (__GNUC__ < 5)
    if (str.match("null")) {
        _type = stype::INTEGER;
        _value._i = 0;
    }
    else if (str.match("nan")) {
        _type = stype::REAL;
        _value._r = snum::NaN;
    }
    else if (str.match("nan")) {
        _type = stype::REAL;
        _value._r = snum::NaN;
    }
    else if (str.match("inf")) {
        _type = stype::REAL;
        if (str[0] == '-') _value._r = snum::nInf;
        else _value._r = snum::Inf;
    }
    else if (str.match("inf")) {
        _type = stype::REAL;
        if (str[0] == '-') _value._r = snum::nInf;
        else _value._r = snum::Inf;
    }
    else if (str == "true" || str == "ok" || str == "yes") {
        _type = stype::BOOL;
        _value._b = true;
    }
    else if (str == "false" || str == "ng" || str == "no") {
        _type = stype::BOOL;
        _value._b = false;
    }
    else if (str.beginWith("0x") || str.beginWith("0o") || str.beginWith("0b")) {
        _type = stype::INTEGER;
        _value._i = atoll(str.cstr());
    }
    else if (str.beginWith("0x") || str.beginWith("0o") || str.beginWith("0b")) {
        _type = stype::INTEGER;
        _value._i = atoll(str.cstr());
    }
    else if (str.match("/")) {
        _type = stype::FRACTION;
        auto frac = sfrac(str);
        _value._f[0] = frac.numerator;
        _value._f[1] = frac.denominator;
    }
    else if (str.endWith("i")) {
        _type = stype::COMPLEX;
        auto cmp = Complex<float>(str);
        _value._c[0] = cmp.real;
        _value._c[1] = cmp.imaginary;
    }
    else if (str.match(".")) {
        _type = stype::REAL;
        _value._r = atof(s);
    }
    else {
        _type = stype::INTEGER;
        _value._i = atoll(s);
    }
#else
    //Integer
    if (str.equal(REG("/^[+-]*\\d+/")) ||
        str.equal(REG("/^0x[0-9a-fA-F]+/")) ||
        str.equal(REG("/^0o[0-7]+/")) ||
        str.equal(REG("/^0b[01]+/"))) {
        _type = stype::INTEGER;
        _value._i = atoll(s);
    }
    else if (str.equal(REG("/^null/i"))) {
        _type = stype::INTEGER;
        _value._i = 0;
    }
    //Real number
    else if (str.equal(REG("/^nan/i"))) {
        _type = stype::REAL;
        _value._r = snum::NaN;
    }
    else if (str.equal(REG("/^[+-]*inf/i"))) {
        _type = stype::REAL;
        if (str[0] == '-') _value._r = snum::nInf;
        else _value._r = snum::Inf;
    }
    else if (str.equal(REG("/^[+-]*\\d+\\.\\d+/i")) ||
        str.equal(REG("/^[+-]*\\d+\\.*\\d*[eE][+-]*\\d+/"))) {
        _type = stype::REAL;
        _value._r = atof(s);
    }
    //Boolean
    else if (str.equal(REG("/^TRUE|YES|OK/i"))) {
        _type = stype::BOOL;
        _value._b = true;
    }
    else if (str.equal(REG("/^FALSE|NO|NG/i"))) {
        _type = stype::BOOL;
        _value._b = false;
    }
    //Fraction
    else if (str.equal(REG("/^[+-]*\\d+\\/\\d+/"))) {
        _type = stype::FRACTION;
        auto frac = sfrac(str);
        _value._f[0] = frac.numerator;
        _value._f[1] = frac.denominator;
    }
    //Complex
    else if (str.equal(REG("/^[+-]*[0-9.]*[+-]*[0-9.]+i/"))) {
        _type = stype::COMPLEX;
        auto cmp = Complex<float>(str);
        _value._c[0] = cmp.real;
        _value._c[1] = cmp.imaginary;
    }
    else throw FormatException(formatErrorText("Numeric string", str, "Integer, real, fraction, complex or boolean format."));
#endif
}
/*
SNumber::SNumber(const sobj &obj) {
    if(obj.isNull()) *this = 0;
    else if (obj.isNum()) *this = obj.number();
    else *this = SNumber::toNumber(obj);
}
*/
SNumber::SNumber(const SNumber &num) : _type(num._type) {
    memcpy(&_value, &num._value, sizeof(numvalue));
}
SNumber::~SNumber() {}

SNumber & SNumber::operator=(const SNumber &num) {
    _type = num._type;
    memcpy(&_value, &num._value, sizeof(numvalue));
    return *this;
}
SNumber &SNumber::operator+=(const SNumber &num) {
    switch (_type) {
        case stype::INTEGER:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._i += num._value._i;
                    break;
                case stype::UINTEGER:
                    _value._i += num._value._ui;
                    break;
                case stype::REAL:
                {
                    _value._r = (double)_value._i + num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
                    _value._i += num._value._b?1:0;
                    break;
                case stype::FRACTION:
                {
                    _value._f[0] = num._value._f[1]*(sint)_value._i + num._value._f[0];
                    _value._f[1] = num._value._f[1];
                    _type = stype::FRACTION;
                    break;
                }
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._i + num._value._c[0];
                    _value._c[1] = num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::UINTEGER:
        {
            switch (num._type) {
                case stype::INTEGER:
                {
                    if (num._value._i < 0) {
                        if ((sinteger)_value._ui < -num._value._i) {
                            suinteger i = _value._ui;
                            _value._i = num._value._i + i;
                            _type = stype::INTEGER;
                        }
                        else _value._ui -= (-num._value._i);
                    }
                    else _value._ui += num._value._i;
                    break;
                }
                case stype::UINTEGER:
                    _value._ui += num._value._ui;
                    break;
                case stype::REAL:
                {
                    _value._r = (double)_value._ui + num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
                    _value._ui += num._value._b?1:0;
                    break;
                case stype::FRACTION:
                {
                    _value._f[0] = num._value._f[1]*(sint)_value._ui + num._value._f[0];
                    _value._f[1] = num._value._f[1];
                    _type = stype::FRACTION;
                    break;
                }
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._ui + num._value._c[0];
                    _value._c[1] = num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::REAL:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._r += (double)num._value._i;
                    break;
                case stype::UINTEGER:
                    _value._r += (double)num._value._ui;
                    break;
                case stype::REAL:
                    _value._r += num._value._r;
                    break;
                case stype::BOOL:
                    _value._r += num._value._b?1.0:0.0;
                    break;
                case stype::FRACTION:
                    _value._r += (double)num._value._f[0]/ num._value._f[1];
                    break;
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._r + num._value._c[0];
                    _value._c[1] = num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::BOOL:
            _value._b = _value._b== num._value._b?false:true;
            break;
        case stype::FRACTION:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._f[0] += _value._f[1] * (sint)num._value._i;
                    break;
                case stype::UINTEGER:
                    _value._f[0] += _value._f[1] * (sint)num._value._ui;
                    break;
                case stype::REAL:
                {
                    _value._r = (double)_value._f[0]/_value._f[1] + num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
                    _value._f[0] += _value._f[1] * (num._value._b?1:0);
                    break;
                case stype::FRACTION:
                {
                    sfrac tmp = fraction() + num.fraction();
                    _value._f[0] = tmp.numerator;
                    _value._f[1] = tmp.denominator;
                    break;
                }
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._f[0]/_value._f[1] + num._value._c[0];
                    _value._c[1] = num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::COMPLEX:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._c[0] += (float)num._value._i;
                    break;
                case stype::UINTEGER:
                    _value._c[0] += (float)num._value._ui;
                    break;
                case stype::REAL:
                    _value._c[0] += (float)num._value._r;
                    break;
                case stype::BOOL:
                    _value._c[0] += num._value._b?1.0f:0.0f;
                    break;
                case stype::FRACTION:
                    _value._c[0] += (float)num._value._f[0]/num._value._f[1];
                    break;
                case stype::COMPLEX:
                {
                    _value._c[0] += num._value._c[0];
                    _value._c[1] += num._value._c[1];
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    return *this;
}
SNumber &SNumber::operator-=(const SNumber & num) {
    switch (_type) {
        case stype::INTEGER:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._i -= num._value._i;
                    break;
                case stype::UINTEGER:
                    _value._i -= num._value._ui;
                    break;
                case stype::REAL:
                {
                    _value._r = (double)_value._i - num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
                    _value._i -= num._value._b?1:0;
                    break;
                case stype::FRACTION:
                {
                    _value._f[0] = num._value._f[1]* (sint)_value._i - num._value._f[0];
                    _value._f[1] = num._value._f[1];
                    _type = stype::FRACTION;
                    break;
                }
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._i - num._value._c[0];
                    _value._c[1] = -num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::UINTEGER:
        {
            switch (num._type) {
                case stype::INTEGER:
                {
                    if (num._value._i < 0) _value._ui += (-num._value._i);
                    else if ((sint)_value._ui < num._value._i) {
                        _value._i = num._value._i - _value._ui;
						_value._i *= -1;
                        _type = stype::INTEGER;
                    }
                    else _value._ui -= num._value._i;
                    break;
                }
                case stype::UINTEGER:
                    if (_value._ui < num._value._ui) {
                        _value._i = num._value._ui - _value._ui;
						_value._i *= -1;
                        _type = stype::INTEGER;
                    }
                    else _value._ui -= num._value._ui;
                    break;
                case stype::REAL:
                {
                    _value._r = (double)_value._ui - num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
                    _value._ui -= num._value._b?1:0;
                    break;
                case stype::FRACTION:
                {
                    _value._f[0] = num._value._f[1]* (sint)_value._ui - num._value._f[0];
                    _value._f[1] = num._value._f[1];
                    _type = stype::FRACTION;
                    break;
                }
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._ui - num._value._c[0];
                    _value._c[1] = -num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::REAL:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._r -= (double)num._value._i;
                    break;
                case stype::UINTEGER:
                    _value._r -= (double)num._value._ui;
                    break;
                case stype::REAL:
                    _value._r -= num._value._r;
                    break;
                case stype::BOOL:
                    _value._r -= num._value._b?1.0:0.0;
                    break;
                case stype::FRACTION:
                    _value._r -= (double)num._value._f[0]/ num._value._f[1];
                    break;
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._r - num._value._c[0];
                    _value._c[1] = -num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::BOOL:
            _value._b = _value._b==num._value._b?false:true;
            break;
        case stype::FRACTION:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._f[0] -= (sint)num.integer() * _value._f[1];
                    break;
                case stype::UINTEGER:
                    _value._f[0] -= (sint)num.uinteger() * _value._f[1];
                    break;
                case stype::REAL:
                {
                    _value._r = (double)_value._f[0]/_value._f[1] - num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
                    _value._f[0] -= _value._f[1] * (num._value._b?1:0);
                    break;
                case stype::FRACTION:
                {
                    sfrac tmp = fraction() - num.fraction();
                    _value._f[0] = tmp.numerator;
                    _value._f[1] = tmp.denominator;
                    break;
                }
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._f[0]/_value._f[1] - num._value._c[0];
                    _value._c[1] = -num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::COMPLEX:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._c[0] -= (float)num._value._i;
                    break;
                case stype::UINTEGER:
                    _value._c[0] -= (float)num._value._ui;
                    break;
                case stype::REAL:
                    _value._c[0] -= (float)num._value._r;
                    break;
                case stype::BOOL:
                    _value._c[0] -= num._value._b?1.0f:0.0f;
                    break;
                case stype::FRACTION:
                    _value._c[0] -= (float)num._value._f[0]/ num._value._f[1];
                    break;
                case stype::COMPLEX:
                {
                    _value._c[0] -= num._value._c[0];
                    _value._c[1] -= num._value._c[1];
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    return *this;
}
SNumber &SNumber::operator*=(const SNumber & num) {
    switch (_type) {
        case stype::INTEGER:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._i *= num._value._i;
                    break;
                case stype::UINTEGER:
                    _value._i *= num._value._ui;
                    break;
                case stype::REAL:
                {
                    _value._r = (double)_value._i * num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
                    _value._i *= num._value._b?1:0;
                    break;
                case stype::FRACTION:
                {
                    _value._f[0] = num._value._f[0]* (sint)_value._i;
                    _value._f[1] = num._value._f[1];
					smath::reduction(_value._f[0], _value._f[1]);
                    _type = stype::FRACTION;
                    break;
                }
                case stype::COMPLEX:
                {
                    float tmp = (float)_value._i;
                    _value._c[0] = tmp * num._value._c[0];
                    _value._c[1] = tmp * num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::UINTEGER:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._ui *= num._value._i;
                    break;
                case stype::UINTEGER:
                    _value._ui *= num._value._ui;
                    break;
                case stype::REAL:
                {
                    _value._r = (double)_value._ui * num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
                    _value._ui *= num._value._b?1:0;
                    break;
                case stype::FRACTION:
                {
                    _value._f[0] = num._value._f[0]* (sint)_value._ui;
                    _value._f[1] = num._value._f[1];
					smath::reduction(_value._f[0], _value._f[1]);
                    _type = stype::FRACTION;
                    break;
                }
                case stype::COMPLEX:
                {
                    float tmp = (float)_value._ui;
                    _value._c[0] = tmp * num._value._c[0];
                    _value._c[1] = tmp * num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::REAL:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._r *= (double)num._value._i;
                    break;
                case stype::UINTEGER:
                    _value._r *= (double)num._value._ui;
                    break;
                case stype::REAL:
                    _value._r *= num._value._r;
                    break;
                case stype::BOOL:
                    _value._r *= num._value._b?1.0:0.0;
                    break;
                case stype::FRACTION:
                    _value._r *= (double)num._value._f[0]/ num._value._f[1];
                    break;
                case stype::COMPLEX:
                {
                    double tmp = _value._r;
                    _value._c[0] = (float)(tmp * num._value._c[0]);
                    _value._c[1] = (float)(tmp * num._value._c[1]);
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::BOOL:
            _value._b = _value._b&& num._value._b?true:false;
            break;
        case stype::FRACTION:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._f[0] *= (sint)num._value._i;
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                case stype::UINTEGER:
                    _value._f[0] *= (sint)num._value._ui;
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                case stype::REAL:
                {
                    _value._r = (double)_value._f[0]/_value._f[1] * num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
                    _value._f[0] *= num._value._b?1:0;
                    break;
                case stype::FRACTION:
                {
                    _value._f[0] *= num._value._f[0];
                    _value._f[1] *= num._value._f[1];
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                }
                case stype::COMPLEX:
                {
                    float tmp = floatValue();
                    _value._c[0] = tmp * num._value._c[0];
                    _value._c[1] = tmp * num._value._c[1];
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::COMPLEX:
        {
            switch (num._type) {
                case stype::INTEGER:
				{
					_value._c[0] *= (float)num._value._i;
					_value._c[1] *= (float)num._value._i;
					break;
				}
                case stype::UINTEGER:
				{
					_value._c[0] *= (float)num._value._ui;
					_value._c[1] *= (float)num._value._ui;
					break;
				}
                case stype::REAL:
				{
					_value._c[0] *= (float)num._value._r;
					_value._c[1] *= (float)num._value._r;
					break;
				}
                case stype::BOOL:
				{
					_value._c[0] *= num._value._b ? 1.0f : 0.0f;
					_value._c[1] *= num._value._b ? 1.0f : 0.0f;
					break;
				}
                case stype::FRACTION:
				{
					float tmp = (float)num._value._f[0] / num._value._f[1];
					_value._c[0] *= tmp;
					_value._c[1] *= tmp;
					break;
				}
                case stype::COMPLEX:
                {
                    Complex<float> tmp = complex() * num.complex();
                    _value._c[0] = tmp.real;
                    _value._c[1] = tmp.imaginary;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    return *this;
}
SNumber & SNumber::operator/=(const SNumber & num) {
    switch (_type) {
        case stype::INTEGER:
        {
            switch (num._type) {
                case stype::INTEGER:
                {
                    if (num._value._i == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._r = (double)_value._i / num._value._i;
					_type = stype::REAL;
                    break;
                }
                case stype::UINTEGER:
                {
                    if (num._value._ui == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._r = (double)_value._i / num._value._ui;
					_type = stype::REAL;
                    break;
                }
                case stype::REAL:
                {
                    if (num._value._r == 0.0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
                    _value._r = (double)_value._i / num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
                    throw UndefinedException("Division by boolean variable is not defined.");
                case stype::FRACTION:
                {
                    if (num._value._f[0] == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._f[0] = num._value._f[1] * (sint)_value._i;
					_value._f[1] = num._value._f[0];
					_type = stype::FRACTION;
					break;
                }
                case stype::COMPLEX:
                {
                    Complex<float> tmp = (float)_value._i/ num.complex();
                    _value._c[0] = tmp.real;
                    _value._c[1] = tmp.imaginary;
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::UINTEGER:
        {
            switch (num._type) {
                case stype::INTEGER:
                {
                    if (num._value._i == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._r = (double)_value._ui / num._value._i;
					_type = stype::REAL;
					break;
                }
                case stype::UINTEGER:
                {
                    if (num._value._ui == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._r = (double)_value._ui / num._value._ui;
					_type = stype::REAL;
					break;
                }
                case stype::REAL:
                {
					if (num._value._r == 0.0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._r = (double)_value._ui / num._value._r;
					_type = stype::REAL;
					break;
                }
                case stype::BOOL:
					throw UndefinedException("Division by boolean variable is not defined.");
                case stype::FRACTION:
                {
					if (num._value._f[0] == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._f[0] = num._value._f[1] * (sint)_value._ui;
					_value._f[1] = num._value._f[0];
					_type = stype::FRACTION;
					break;
                }
                case stype::COMPLEX:
                {
                    Complex<float> tmp = (float)_value._ui / num.complex();
					_value._c[0] = tmp.real;
					_value._c[1] = tmp.imaginary;
					_type = stype::COMPLEX;
					break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::REAL:
        {
            switch (num._type) {
                case stype::INTEGER:
                {
                    if (num._value._i == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._r /= num._value._i;
					break;
                }
                case stype::UINTEGER:
                {
                    if (num._value._ui == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._r /= num._value._ui;
					break;
                }
                case stype::REAL:
                {
					if (num._value._r == 0.0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._r /= num._value._r;
					break;
                }
                case stype::BOOL:
					throw UndefinedException("Division by boolean variable is not defined.");
                case stype::FRACTION:
                {
					if (num._value._f[0] == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
					_value._r /= ((float)num._value._f[0]/ num._value._f[1]);
                    break;
                }
                case stype::COMPLEX:
                {
                    Complex<float> tmp = (float)_value._r / num.complex();
					_value._c[0] = tmp.real;
					_value._c[1] = tmp.imaginary;
					_type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::BOOL:
			throw UndefinedException("Division of boolean variable is not defined.");
        case stype::FRACTION:
        {
            switch (num._type) {
                case stype::INTEGER:
                {
					if (num._value._i == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
                    _value._f[1] *= (sint)num._value._i;
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                }
                case stype::UINTEGER:
                {
					if (num._value._ui == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
                    _value._f[1] *= (sint)num._value._ui;
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                }
                case stype::REAL:
                {
					if (num._value._r == 0.0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
                    _value._r = ((double)_value._f[0]/_value._f[1]) / num._value._r;
                    _type = stype::REAL;
                    break;
                }
                case stype::BOOL:
					throw UndefinedException("Division by boolean variable is not defined.");
                case stype::FRACTION:
                {
					if (num._value._f[0] == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
                    _value._f[0] *= num._value._f[1];
                    _value._f[1] *= num._value._f[0];
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                }
                case stype::COMPLEX:
                {
                    Complex<float> tmp = ((float)_value._f[0]/_value._f[1]) / num.complex();
                    _value._c[0] = tmp.real;
                    _value._c[1] = tmp.imaginary;
                    _type = stype::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::COMPLEX:
        {
            switch (num._type) {
                case stype::INTEGER:
                {
					if (num._value._i == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
                    _value._c[0] /= (float)num._value._i;
                    _value._c[1] /= (float)num._value._i;
                    break;
                }
                case stype::UINTEGER:
                {
					if (num._value._ui == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
                    _value._c[0] /= (float)num._value._ui;
                    _value._c[1] /= (float)num._value._ui;
                    break;
                }
                case stype::REAL:
                {
					if (num._value._r == 0.0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
                    _value._c[0] /= (float)num._value._r;
                    _value._c[1] /= (float)num._value._r;
                    break;
                }
                case stype::BOOL:
					throw UndefinedException("Division by boolean variable is not defined.");
                case stype::FRACTION:
                {
					if (num._value._f[0] == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::operator /='.");
                    float tmp = num.floatValue();
                    _value._c[0] /= tmp;
                    _value._c[1] /= tmp;
                    break;
                }
                case stype::COMPLEX:
                {
                    Complex<float> tmp = complex() / num.complex();
                    _value._c[0] = tmp.real;
                    _value._c[1] = tmp.imaginary;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    return *this;
}
SNumber &SNumber::operator%=(const SNumber & num) {
	auto q = quot(num);
    switch (_type) {
        case stype::INTEGER:
        {
            switch (num._type) {
                case stype::INTEGER:
                    _value._i -= q * num._value._i;
                    break;
                case stype::UINTEGER:
					_value._i -= q * (sint)num._value._ui;
                    break;
                case stype::REAL:
                {
					_value._r = (double)_value._i - num._value._r * q;
                    _type = stype::REAL;
                    break;
                }
				case stype::BOOL:
					throw UndefinedException("Division by boolean variable is not defined.");
                case stype::FRACTION:
                {
					sfrac tmp = (sint)_value._i - num.fraction() * (sint)q;
					_value._f[0] = tmp.numerator;
					_value._f[1] = tmp.denominator;
					_type = stype::FRACTION;
                    break;
                }
                case stype::COMPLEX:
                {
                    Complex<float> tmp = (float)_value._i - num.complex() * (float)q;
                    _value._c[0] = tmp.real;
                    _value._c[1] = tmp.imaginary;
                    _type = stype::COMPLEX;
                    break;
                }
                default:
					break;
            }
            break;
        }
        case stype::UINTEGER:
        {
            switch (num._type) {
			case stype::INTEGER:
				_value._ui -= q * num._value._i;
				break;
			case stype::UINTEGER:
				_value._ui -= q * (sint)num._value._ui;
				break;
			case stype::REAL:
			{
				_value._r = (double)_value._ui - num._value._r * q;
				_type = stype::REAL;
				break;
			}
			case stype::BOOL:
				throw UndefinedException("Division by boolean variable is not defined.");
			case stype::FRACTION:
			{
				sfrac tmp = (sint)_value._ui - num.fraction() * (sint)q;
				_value._f[0] = tmp.numerator;
				_value._f[1] = tmp.denominator;
				_type = stype::FRACTION;
				break;
			}
			case stype::COMPLEX:
			{
                Complex<float> tmp = (float)_value._ui - num.complex() * (float)q;
				_value._c[0] = tmp.real;
				_value._c[1] = tmp.imaginary;
				_type = stype::COMPLEX;
				break;
			}
			default:
				break;
            }
            break;
        }
        case stype::REAL:
        {
            switch (num._type) {
			case stype::INTEGER:
				_value._r -= q * num._value._i;
				break;
			case stype::UINTEGER:
				_value._r -= q * (sint)num._value._ui;
				break;
			case stype::REAL:
				_value._r -= num._value._r * q;
				break;
			case stype::BOOL:
				throw UndefinedException("Division by boolean variable is not defined.");
			case stype::FRACTION:
				_value._r -= num.doubleValue() * q;
				break;
			case stype::COMPLEX:
			{
                Complex<float> tmp = (float)_value._r - num.complex() * (float)q;
				_value._c[0] = tmp.real;
				_value._c[1] = tmp.imaginary;
				_type = stype::COMPLEX;
				break;
			}
			default:
				break;
            }
            break;
        }
		case stype::BOOL:
			throw UndefinedException("Division by boolean variable is not defined.");
		case stype::FRACTION:
        {
            switch (num._type) {
			case stype::INTEGER:
			{
				sfrac tmp = fraction();
				tmp -= sfrac((sint)(q * num._value._i), 1);
				_value._f[0] = tmp.numerator;
				_value._f[1] = tmp.denominator;
				break;
			}
			case stype::UINTEGER:
			{
				sfrac tmp = fraction();
				tmp -= sfrac((sint)(q * num._value._ui), 1);
				_value._f[0] = tmp.numerator;
				_value._f[1] = tmp.denominator;
				break;
			}
			case stype::REAL:
			{
				_value._r = doubleValue() - num._value._r * q;
				_type = stype::REAL;
				break;
			}
			case stype::BOOL:
				throw UndefinedException("Division by boolean variable is not defined.");
			case stype::FRACTION:
			{
				sfrac tmp = fraction() - num.fraction() * (sint)q;
				_value._f[0] = tmp.numerator;
				_value._f[1] = tmp.denominator;
				break;
			}
			case stype::COMPLEX:
			{
                Complex<float> tmp = floatValue() - num.complex() * (float)q;
				_value._c[0] = tmp.real;
				_value._c[1] = tmp.imaginary;
				_type = stype::COMPLEX;
				break;
			}
			default:
				break;
            }
            break;
        }
        case stype::COMPLEX:
        {
            switch (num._type) {
			case stype::INTEGER:
				_value._c[0] -= (float)q * num._value._i;
				break;
			case stype::UINTEGER:
				_value._c[0] -= (float)q * num._value._ui;
				break;
			case stype::REAL:
				_value._c[0] -= (float)num._value._r * q;
				break;
			case stype::BOOL:
				throw UndefinedException("Division by boolean variable is not defined.");
			case stype::FRACTION:
				_value._c[0] -= num.floatValue() * q;
				break;
			case stype::COMPLEX:
			{
                Complex<float> tmp = num.complex() * (float)q;
				_value._c[0] -= tmp.real;
				_value._c[1] -= tmp.imaginary;
				break;
			}
			default:
				break;
            }
            break;
        }
        default:
            break;
    }
    return *this;
}
SNumber &SNumber::operator++() {
    switch (_type) {
        case stype::INTEGER:
            ++_value._i;
            break;
        case stype::UINTEGER:
            ++_value._ui;
            break;
        case stype::REAL:
            _value._r += 1.0;
            break;
        case stype::BOOL:
            _value._b = _value._b?false:true;
            break;
        case stype::FRACTION:
            _value._f[0] += _value._f[1];
            break;
        case stype::COMPLEX:
            _value._c[0] += 1.0f;
            break;
        default:
            break;
    }
    return *this;
}
SNumber SNumber::operator++(int) const { SNumber num(*this); ++num; return num; }
SNumber &SNumber::operator--() {
    switch (_type) {
        case stype::INTEGER:
            --_value._i;
            break;
        case stype::UINTEGER:
            --_value._ui;
            break;
        case stype::REAL:
            _value._r-=1.0;
            break;
        case stype::BOOL:
            _value._b = _value._b?false:true;
            break;
        case stype::FRACTION:
            _value._f[0] -= _value._f[1];
            break;
        case stype::COMPLEX:
            _value._c[0] -= 1.0f;
            break;
        default:
            break;
    }
    return *this;
}
SNumber SNumber::operator--(int) const { SNumber num(*this); --num; return num; }
SNumber SNumber::operator-() const {
    switch (_type) {
        case stype::INTEGER:
            return SNumber(_value._i * -1);
        case stype::UINTEGER:
            throw Exception();
        case stype::REAL:
            return SNumber(_value._r * -1.0);
        case stype::BOOL:
            throw Exception();
        case stype::FRACTION:
            return SNumber(_value._f[0]*-1, _value._f[1]*-1);
        case stype::COMPLEX:
            return SNumber(_value._c[0]*-1.0f, _value._c[1]*-1.0f);
        default:
            return *this;
    }
}
SNumber SNumber::absolute() const {
	if (_type == stype::COMPLEX) return complex().absolute();
	else if (*this < 0) return -(*this); 
	else return *this; 
}
/*
SNumber SNumber::operator+(const sbyte& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const subyte& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const sshort& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const sushort& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const int& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const unsigned int& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const size_t& num) const { return SNumber(*this) += num; }
#ifdef WIN64_OS
SNumber SNumber::operator+(const long& num) const { return SNumber(*this) += num; }
#ifndef MAC_OS
SNumber SNumber::operator+(const unsigned long& num) const { return SNumber(*this) += num; }
#endif
#endif
SNumber SNumber::operator+(const long long& num) const { return SNumber(*this) += num; }
#ifdef MAC_OS
SNumber SNumber::operator+(const unsigned long long& num) const { return SNumber(*this) += num; }
#endif
#ifdef LINUX_OS
SNumber SNumber::operator+(const sinteger& num) const { return SNumber(*this) += num; }
#endif
SNumber SNumber::operator+(const float& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const double& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const sfrac& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const scomp& num) const { return SNumber(*this) += num; }
*/
SNumber SNumber::operator+(const SNumber& num) const { return SNumber(*this) += num; }
/*
SNumber SNumber::operator-(const sbyte& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const subyte& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const sshort& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const sushort& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const int& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const unsigned int& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const size_t& num) const { return SNumber(*this) -= num; }
#ifdef WIN64_OS
SNumber SNumber::operator-(const long& num) const { return SNumber(*this) -= num; }
#ifndef MAC_OS
SNumber SNumber::operator-(const unsigned long& num) const { return SNumber(*this) -= num; }
#endif
#endif
SNumber SNumber::operator-(const long long& num) const { return SNumber(*this) -= num; }
#ifdef MAC_OS
SNumber SNumber::operator-(const unsigned long long& num) const { return SNumber(*this) -= num; }
#endif
#ifdef LINUX_OS
SNumber SNumber::operator-(const sinteger& num) const { return SNumber(*this) -= num; }
#endif
SNumber SNumber::operator-(const float& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const double& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const sfrac& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const scomp& num) const { return SNumber(*this) -= num; }
*/
SNumber SNumber::operator-(const SNumber& num) const { return SNumber(*this) -= num; }
/*
SNumber SNumber::operator*(const sbyte& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const subyte& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const sshort& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const sushort& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const int& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const unsigned int& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const size_t& num) const { return SNumber(*this) *= num; }
#ifdef WIN64_OS
SNumber SNumber::operator*(const long& num) const { return SNumber(*this) *= num; }
#ifndef MAC_OS
SNumber SNumber::operator*(const unsigned long& num) const { return SNumber(*this) *= num; }
#endif
#endif
SNumber SNumber::operator*(const long long& num) const { return SNumber(*this) *= num; }
#ifdef MAC_OS
SNumber SNumber::operator*(const unsigned long long& num) const { return SNumber(*this) *= num; }
#endif
#ifdef LINUX_OS
SNumber SNumber::operator*(const sinteger& num) const { return SNumber(*this) *= num; }
#endif
SNumber SNumber::operator*(const float& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const double& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const sfrac& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const scomp& num) const { return SNumber(*this) *= num; }
*/
SNumber SNumber::operator*(const SNumber& num) const { return SNumber(*this) *= num; }
/*
SNumber SNumber::operator/(const sbyte& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const subyte& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const sshort& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const sushort& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const int& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const unsigned int& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const size_t& num) const { return SNumber(*this) /= num; }
#ifdef WIN64_OS
SNumber SNumber::operator/(const long& num) const { return SNumber(*this) /= num; }
#ifndef MAC_OS
SNumber SNumber::operator/(const unsigned long& num) const { return SNumber(*this) /= num; }
#endif
#endif
SNumber SNumber::operator/(const long long& num) const { return SNumber(*this) /= num; }
#ifdef MAC_OS
SNumber SNumber::operator/(const unsigned long long& num) const { return SNumber(*this) /= num; }
#endif
#ifdef LINUX_OS
SNumber SNumber::operator/(const sinteger& num) const { return SNumber(*this) /= num; }
#endif
SNumber SNumber::operator/(const float& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const double& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const sfrac& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const scomp& num) const { return SNumber(*this) /= num; }
*/
SNumber SNumber::operator/(const SNumber& num) const { return SNumber(*this) /= num; }
SNumber SNumber::reciprocal() const {
	switch (_type) {
	case stype::INTEGER:
		return sfrac(1, (sint)_value._i);
	case stype::UINTEGER:
		return sfrac(1, (sint)_value._ui);
	case stype::REAL:
		return 1.0 / _value._r;
	case stype::BOOL:
		return !_value._b;
	case stype::FRACTION:
		return sfrac(_value._f[1], _value._f[0]);
	case stype::COMPLEX:
		return Complex<float>(_value._c[0], -_value._c[1]) / (float)absolute();
	default:
		return 1.0;
	}
}
template<typename T1, typename T2>
inline sinteger _quot(const T1& t1, const T2& t2) {
	if (t1 < 0) {
		if (t2 < 0) return (-(sinteger)t1) / (-(sinteger)t2);
		else return -(-(sinteger)t1) / (sinteger)t2;
	}
	else {
		if (t2 < 0) return -((sinteger)t1 / (-(sinteger)t2));
		else return (sinteger)t1 / (sinteger)t2;
	}
}
sinteger SNumber::quot(const SNumber& num) const {
	sinteger q;
	switch (_type) {
	case stype::INTEGER:
	{
		switch (num._type) {
		case stype::INTEGER:
		{
			if (num._value._i == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._i, num._value._i);
			break;
		}
		case stype::UINTEGER:
		{
			if (num._value._ui == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._i, (sinteger)num._value._ui);
			break;
		}
		case stype::REAL:
		{
			if (num._value._r == 0.0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot((double)_value._i, num._value._r);
			break;
		}
		case stype::BOOL:
		{
			if (!num._value._b) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _value._i;
			break;
		}
		case stype::FRACTION:
		{
			if (num._value._f[0] == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(num._value._f[1] * _value._i, num._value._f[0]);
			break;
		}
		case stype::COMPLEX:
		{
			if (num.complex() == 0.0f) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot((double)_value._i * num._value._c[0], num.complex().absolute());
			break;
		}
		default:
			return 0;
		}
		break;
	}
	case stype::UINTEGER:
	{
		switch (num._type) {
		case stype::INTEGER:
		{
			if (num._value._i == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._ui, num._value._i);
			break;
		}
		case stype::UINTEGER:
		{
			if (num._value._ui == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._ui, (sinteger)num._value._ui);
			break;
		}
		case stype::REAL:
		{
			if (num._value._r == 0.0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._ui, num._value._r);
			break;
		}
		case stype::BOOL:
		{
			if (!num._value._b) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = (sinteger)_value._ui;
			break;
		}
		case stype::FRACTION:
		{
			if (num._value._f[0] == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot((sinteger)num._value._f[1] * _value._ui, num._value._f[0]);
			break;
		}
		case stype::COMPLEX:
		{
			if (num.complex() == 0.0f) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot((double)_value._ui * num._value._c[0], num.complex().absolute());
			break;
		}
		default:
			return 0;
		}
		break;
	}
	case stype::REAL:
	{
		switch (num._type) {
		case stype::INTEGER:
		{
			if (num._value._i == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._r, num._value._i);
			break;
		}
		case stype::UINTEGER:
		{
			if (num._value._ui == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._r, num._value._ui);
			break;
		}
		case stype::REAL:
		{
			if (num._value._r == 0.0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._r, num._value._r);
			break;
		}
		case stype::BOOL:
		{
			if (!num._value._b) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._r, 1);
			break;
		}
		case stype::FRACTION:
		{
			if (num._value._f[0] == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot((double)num._value._f[1] * _value._r, num._value._f[0]);
			break;
		}
		case stype::COMPLEX:
		{
			if (num.complex() == 0.0f) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._r * num._value._c[0], num.complex().absolute());
			break;
		}
		default:
			break;
		}
		break;
	}
	case stype::BOOL:
	{
		if (!_value._b) return 0;
		else return num.reciprocal().integer();
	}
	case stype::FRACTION:
	{
		switch (num._type) {
		case stype::INTEGER:
		{
			if (num._value._i == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._f[0], _value._f[1] * num._value._i);
			break;
		}
		case stype::UINTEGER:
		{
			if (num._value._ui == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._f[0], (sinteger)_value._f[1] * num._value._ui);
			break;
		}
		case stype::REAL:
		{
			if (num._value._r == 0.0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._f[0], _value._f[1] * num._value._r);
			break;
		}
		case stype::BOOL:
		{
			if (!num._value._b) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._f[0], _value._f[1]);
			break;
		}
		case stype::FRACTION:
		{
			if (num._value._f[0] == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._f[0] * num._value._f[1], _value._f[1] * num._value._f[0]);
			break;
		}
		case stype::COMPLEX:
		{
			if (num.complex() == 0.0f) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._f[0] * num._value._c[0], (double)_value._f[1] * num.complex().absolute());
			break;
		}
		default:
			break;
		}
		break;
	}
	case stype::COMPLEX:
	{
		switch (num._type) {
		case stype::INTEGER:
		{
			if (num._value._i == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._c[0], num._value._i);
			break;
		}
		case stype::UINTEGER:
		{
			if (num._value._ui == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._c[0], num._value._ui);
			break;
		}
		case stype::REAL:
		{
			if (num._value._r == 0.0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._c[0], num._value._r);
			break;
		}
		case stype::BOOL:
		{
			if (!num._value._b) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._c[0], 1);
			break;
		}
		case stype::FRACTION:
		{
			if (num._value._f[0] == 0) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._c[0]* num._value._f[1], num._value._f[0]);
			break;
		}
		case stype::COMPLEX:
		{
			if (num.complex() == 0.0f) throw DivZeroException("Zero was passed as a divider at 'SNumber::quot' function.");
			q = _quot(_value._c[0] * num._value._c[0] + _value._c[1] * num._value._c[1], num.complex().absolute());
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
	return q;
}
/*
SNumber SNumber::operator%(const sbyte& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const subyte& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const sshort& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const sushort& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const int& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const unsigned int& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const size_t& num) const { return SNumber(*this) %= num; }
#ifdef WIN64_OS
SNumber SNumber::operator%(const long& num) const { return SNumber(*this) %= num; }
#ifndef MAC_OS
SNumber SNumber::operator%(const unsigned long& num) const { return SNumber(*this) %= num; }
#endif
#endif
SNumber SNumber::operator%(const long long& num) const { return SNumber(*this) %= num; }
#ifdef MAC_OS
SNumber SNumber::operator%(const unsigned long long& num) const { return SNumber(*this) %= num; }
#endif
#ifdef LINUX_OS
SNumber SNumber::operator%(const sinteger& num) const { return SNumber(*this) %= num; }
#endif
SNumber SNumber::operator%(const float& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const double& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const sfrac& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const scomp& num) const { return SNumber(*this) %= num; }
*/
SNumber SNumber::operator%(const SNumber& num) const { return SNumber(*this) %= num; }
//String SNumber::operator+(const char *s) { return toString()+s; }

subyte SNumber::type() const { return _type; }
void *SNumber::ptr() const { return const_cast<void *>((const void *)&_value); }
void SNumber::setType(subyte type) {
    if(_type == type) return;
    switch(_type) {
        case stype::INTEGER:
        {
            switch (type) {
                case stype::UINTEGER:
                    _value._ui = (suinteger)_value._i;
                    break;
                case stype::REAL:
                    _value._r = (sreal)_value._i;
                    break;
                case stype::BOOL:
                    _value._b = _value._i != 0;
                    break;
                case stype::FRACTION:
                {
                    _value._f[0] = (sint)_value._i;
                    _value._f[1] = 1;
                    break;
                }
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._i;
                    _value._c[1] = 0.0f;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::UINTEGER:
        {
            switch (type) {
                case stype::INTEGER:
                    _value._i = (sinteger)_value._ui;
                    break;
                case stype::REAL:
                    _value._r = (sreal)_value._ui;
                    break;
                case stype::BOOL:
                    _value._b = _value._ui != 0;
                    break;
                case stype::FRACTION:
                {
                    _value._f[0] = (sint)_value._ui;
                    _value._f[1] = 1;
                    break;
                }
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._ui;
                    _value._c[1] = 0.0f;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::REAL:
        {
            switch (type) {
                case stype::INTEGER:
                    _value._i = (sinteger)_value._r;
                    break;
                case stype::UINTEGER:
                    _value._ui = (suinteger)_value._r;
                    break;
                case stype::BOOL:
                    _value._b = _value._r != 0.0;
                    break;
                case stype::FRACTION:
                {
                    sfrac tmp = _value._r;
                    _value._f[0] = tmp.numerator;
                    _value._f[1] = tmp.denominator;
                    break;
                }
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._r;
                    _value._c[1] = 0.0f;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::BOOL:
        {
            switch (type) {
                case stype::INTEGER:
                    _value._i = _value._b?1:0;
                    break;
                case stype::UINTEGER:
                    _value._ui = _value._b?1:0;
                    break;
                case stype::REAL:
                    _value._r = _value._b?1.0:0.0;
                    break;
                case stype::FRACTION:
                {
                    _value._f[0] = _value._b?1:0;
                    _value._f[1] = 1;
                    break;
                }
                case stype::COMPLEX:
                {
                    _value._c[0] = _value._b?1.0f:0.0f;
                    _value._c[1] = 0.0f;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::FRACTION:
        {
            switch (type) {
                case stype::INTEGER:
                    _value._i = _value._f[0]/_value._f[1];
                    break;
                case stype::UINTEGER:
                    _value._ui = _value._f[0]/_value._f[1];
                    break;
                case stype::REAL:
                    _value._r = (double)_value._f[0]/_value._f[1];
                    break;
                case stype::BOOL:
                    _value._b = (_value._f[0] != 0);
                    break;
                case stype::COMPLEX:
                {
                    _value._c[0] = (float)_value._f[0]/_value._f[1];
                    _value._c[1] = 0.0f;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case stype::COMPLEX:
        {
            switch (type) {
                case stype::INTEGER:
                    _value._i = (sinteger)_value._c[0];
                    break;
                case stype::UINTEGER:
                    _value._ui = (suinteger)_value._c[0];
                    break;
                case stype::REAL:
                    _value._r = _value._c[0];
                    break;
                case stype::BOOL:
                    _value._b = !(_value._c[0]==0.0 && _value._c[1]==0.0);
                    break;
                case stype::FRACTION:
                {
                    sfrac tmp(_value._c[0]);
                    _value._f[0] = tmp.numerator;
                    _value._f[1] = tmp.denominator;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    _type = type;
}
bool SNumber::isNaN() const { return real() == snum::NaN; }
bool SNumber::isInf() const { return real() == snum::Inf; }
bool SNumber::isNInf() const { return real() == snum::nInf; }
/*
String SNumber::precised(size_t size, smath::ROUND round) const {
	if (_type == stype::INTEGER || _type == stype::UINTEGER)
		return toString() + "." + String('0') * size;
    else if(_type == stype::REAL) {
        auto tmp = real();
        if (tmp == sinf) return "inf";
        else if (tmp == sninf) return "-inf";
        else if (tmp == snan) return "NaN";
        tmp *= smath::power(10, (int)size);
        String str;
        switch (round) {
            case FLOOR:
                str = (int)tmp;
                break;
            case CEIL:
            {
				if (tmp < 0) str = (int)tmp - 1;
				else str = (int)tmp + 1;
                break;
            }
            default:
            {
                auto remain = std::abs(tmp-(int)tmp);
                if (remain < 0.5) str = (int)tmp;
                else {
					if (tmp < 0) str = (int)tmp - 1;
					else str = (int)tmp + 1;
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
    else if(_type == stype::FRACTION) return fraction().rounded(size, round);
    else if (_type == stype::COMPLEX) return complex().rounded(size, round);
    else return toString();
}
*/
String SNumber::getClass() const {
    if (_type == stype::INTEGER || _type == stype::UINTEGER) return "integer";
    else if (_type == stype::REAL) return "real";
    else if (_type == stype::BOOL) return "bool";
    else if (_type == stype::FRACTION) return "fraction";
    else if (_type == stype::COMPLEX) return "complex";
    return "number";
}
String SNumber::toString(const char *format) const {
    if (_type == stype::INTEGER) return numToString(integer(), format);//std::to_string(integer());
    else if (_type == stype::UINTEGER) return numToString(uinteger(), format);//std::to_string(uinteger());
    else if (_type == stype::REAL) return numToString(doubleValue(), format);//std::to_string(doubleValue());
    else if (_type == stype::BOOL) return boolean()?"true":"false";
    else if (_type == stype::FRACTION) return fraction().toString(format);
    else if (_type == stype::COMPLEX) return complex().toString(format);
    return "";
}
SObject *SNumber::clone() const { return new SNumber(*this); }
bool SNumber::boolean() const {
    switch (_type) {
        case stype::INTEGER:
            return _value._i != 0;
        case stype::UINTEGER:
            return _value._ui != 0;
        case stype::REAL:
            return _value._r != 0.0;
        case stype::BOOL:
            return _value._b;
        case stype::FRACTION:
            return _value._f[0] != 0;
        case stype::COMPLEX:
            return _value._c[0] != 0.0f && _value._c[1] != 0.0f;
        default:
            return false;
    }
}
sbyte SNumber::byteValue() const { return (sbyte)integer(); }
subyte SNumber::ubyteValue() const { return (subyte)uinteger(); }
sshort SNumber::shortValue() const { return (sshort)integer(); }
sushort SNumber::ushortValue() const { return (sushort)uinteger(); }
int SNumber::intValue() const { return (int)integer(); }
unsigned int SNumber::uintValue() const { return (unsigned int)uinteger(); }
size_t SNumber::sizeValue() const { return (size_t)uinteger(); }
long SNumber::longValue() const { return (long)integer(); }
unsigned long SNumber::ulongValue() const { return (unsigned long)uinteger(); }
long long SNumber::llongValue() const { return (long long)integer(); }
unsigned long long SNumber::ullongValue() const { return (unsigned long long)uinteger(); }
sinteger SNumber::integer() const {
    switch (_type) {
        case stype::INTEGER:
            return _value._i;
        case stype::UINTEGER:
            return (sinteger)_value._ui;
        case stype::REAL:
            return (sinteger)_value._r;
        case stype::BOOL:
            return (sinteger)_value._b;
        case stype::FRACTION:
            return _value._f[0]/_value._f[1];
        case stype::COMPLEX:
            throw Exception();
        default:
            return 0;
    }
}
suinteger SNumber::uinteger() const {
    switch (_type) {
        case stype::INTEGER:
            return _value._i;
        case stype::UINTEGER:
            return _value._ui;
        case stype::REAL:
            return (suinteger)_value._r;
        case stype::BOOL:
            return _value._b;
        case stype::FRACTION:
            return _value._f[0]/_value._f[1];
        case stype::COMPLEX:
            throw Exception();
        default:
            return 0;
    }
}
float SNumber::floatValue() const { return (float)doubleValue(); }
double SNumber::doubleValue() const {
    switch (_type) {
        case stype::INTEGER:
            return (double)_value._i;
        case stype::UINTEGER:
            return (double)_value._ui;
        case stype::REAL:
            return _value._r;
        case stype::BOOL:
            return _value._b;
        case stype::FRACTION:
            return (double)_value._f[0]/_value._f[1];
        case stype::COMPLEX:
            throw Exception();
        default:
            return 0;
    }
}
sreal SNumber::real() const { return doubleValue(); }
sfrac SNumber::fraction() const {
    switch (_type) {
        case stype::INTEGER:
            return sfrac((sint)_value._i);
        case stype::UINTEGER:
            return sfrac((sint)_value._ui);
        case stype::REAL:
            return sfrac(_value._r);
        case stype::BOOL:
            return sfrac((sint)_value._b);
        case stype::FRACTION:
            return sfrac(_value._f[0], _value._f[1]);
        case stype::COMPLEX:
            throw Exception();
        default:
            return sfrac();
    }
}
Complex<float> SNumber::complex() const {
    switch (_type) {
        case stype::INTEGER:
            return Complex<float>((float)_value._i);
        case stype::UINTEGER:
            return Complex<float>((float)_value._ui);
        case stype::REAL:
            return Complex<float>((float)_value._r);
        case stype::BOOL:
            return Complex<float>((float)_value._b);
        case stype::FRACTION:
            return Complex<float>((float)_value._f[0]/_value._f[1]);
        case stype::COMPLEX:
            return Complex<float>(_value._c[0], _value._c[1]);
        default:
            return Complex<float>();
    }
}
SNumber::operator bool() const { return boolean(); }
SNumber::operator sbyte() const { return byteValue(); }
SNumber::operator subyte() const { return ubyteValue(); }
SNumber::operator sshort() const { return shortValue(); }
SNumber::operator sushort() const { return ushortValue(); }
SNumber::operator int() const { return intValue(); }
SNumber::operator unsigned int() const { return uintValue(); }
SNumber::operator size_t() const { return sizeValue(); }
#ifdef WIN64_OS
SNumber::operator long() const { return longValue(); }
#ifndef MAC_OS
SNumber::operator unsigned long() const { return ulongValue(); }
#endif
#endif
SNumber::operator long long() const { return llongValue(); }
#ifdef MAC_OS
SNumber::operator unsigned long long() const { return ullongValue(); }
#endif
SNumber::operator float() const { return floatValue(); }
SNumber::operator double() const { return doubleValue(); }
SNumber::operator sfrac() const { return fraction(); }
SNumber::operator Complex<float>() const { return complex(); }
bool SNumber::operator<(const SNumber &sn) const {
    switch (_type) {
        case stype::INTEGER:
        {
            auto &tmp = _value._i;
            switch (sn._type) {
                case stype::INTEGER:
                    return _value._i < sn.integer();
                case stype::UINTEGER:
                    return _value._i < (sinteger)sn.uinteger();
                case stype::REAL:
                    return _value._i < sn.real();
                case stype::BOOL:
                    return _value._i < sn.integer();
                case stype::FRACTION:
                    return (sint)tmp < sn.fraction();
                case stype::COMPLEX:
                    return (float)tmp < sn.complex();
                default:
                    break;
            }
            break;
        }
        case stype::UINTEGER:
        {
            auto &tmp = _value._ui;
            switch (_type) {
                case stype::INTEGER:
                    return (sinteger)tmp < sn.integer();
                case stype::UINTEGER:
                    return tmp < sn.uinteger();
                case stype::REAL:
                    return tmp < sn.real();
                case stype::BOOL:
                    return (sinteger)tmp < sn.integer();
                case stype::FRACTION:
                    return (sint)tmp < sn.fraction();
                case stype::COMPLEX:
                    return (float)tmp < sn.complex();
                default:
                    break;
            }
            break;
        }
        case stype::REAL:
        {
            auto &tmp = _value._r;
            switch (_type) {
                case stype::INTEGER:
                    return tmp < sn.integer();
                case stype::UINTEGER:
                    return tmp < sn.uinteger();
                case stype::REAL:
                    return tmp < sn.real();
                case stype::BOOL:
                    return tmp < sn.integer();
                case stype::FRACTION:
                    return tmp < sn.fraction().doubleValue();
                case stype::COMPLEX:
                    return (float)tmp < sn.complex();
                default:
                    break;
            }
            break;
        }
        case stype::BOOL:
        {
            auto &tmp = _value._b;
            switch (_type) {
                case stype::INTEGER:
                    return (int)tmp < sn.integer();
                case stype::UINTEGER:
                    return (int)tmp < sn.uinteger();
                case stype::REAL:
                    return (int)tmp < sn.real();
                case stype::BOOL:
                    return (int)tmp < sn.integer();
                case stype::FRACTION:
                    return (sint)tmp < sn.fraction();
                case stype::COMPLEX:
                    return (float)tmp < sn.complex();
                default:
                    break;
            }
            break;
        }
        case stype::FRACTION:
        {
            auto tmp = fraction();
            switch (_type) {
                case stype::INTEGER:
                    return tmp < sn.intValue();
                case stype::UINTEGER:
                    return tmp < sn.intValue();
                case stype::REAL:
                    return tmp.doubleValue() < sn.real();
                case stype::BOOL:
                    return tmp < sn.intValue();
                case stype::FRACTION:
                    return tmp < sn.fraction();
                case stype::COMPLEX:
                    return tmp.floatValue() < sn.complex();
                default:
                    break;
            }
            break;
        }
        case stype::COMPLEX:
            return complex() < sn.complex();
        default:
            break;
    }
    return false;
}
SNumber::operator const char* () const {
    return toString().cstr();
}
bool SNumber::operator>(const SNumber &sn) const { return sn < *this; }
bool SNumber::operator<=(const SNumber &sn) const { return *this < sn || *this == sn; }
bool SNumber::operator>=(const SNumber &sn) const { return sn < *this || *this == sn; }
/*
bool SNumber::operator==(const sobj &obj) const {
    if (obj.isNum()) return (*this) == obj.number();
    return false;
}
*/
bool SNumber::operator==(const SNumber &sn) const {
    if (_type != sn._type) {
        switch (_type) {
            case stype::INTEGER:
				return _value._i == sn.integer();
            case stype::UINTEGER:
				return _value._ui == sn.uinteger();
            case stype::REAL:
				return _value._r == sn.real();
            case stype::BOOL:
                return _value._b == sn.boolean();
            case stype::FRACTION:
            {
                auto frac = sn.fraction();
                return _value._f[0] == frac.numerator && _value._f[1] == frac.denominator;
            }	
            case stype::COMPLEX:
				return _value._c[0] == sn.complex().real && _value._c[1] == sn.complex().imaginary;
            default:
				return false;
        }
    }
    return !memcmp(&_value, &sn._value, sizeof(_value));
}
bool SNumber::operator!=(const SNumber &sn) const { return !(*this == sn); }
//String slib::operator+(const char *s, const SNumber &num) { return String(s) + num.toString(); }
slib::String slib::toString(const slib::SNumber& num, const char* format) { return num.toString(format); }
std::ostream& operator<<(std::ostream& os, const slib::SNumber& num) { return os << num.toString(); }