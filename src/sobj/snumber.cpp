#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sdata.h"
#include "sobj/sarray.h"

using namespace slib;

SNumber SNumber::toNumber(const char *s) { return String(s).number(); }
SNumber::SNumber() : _type(SNumber::INTEGER) { _value._i = 0; }
SNumber::SNumber(char c) : _type(SNumber::INTEGER) { _value._i = (sbyte)c; }
SNumber::SNumber(bool b) : _type(SNumber::BOOL) { _value._b = b; }
SNumber::SNumber(sbyte i) : _type(SNumber::INTEGER) { _value._i = i; }
SNumber::SNumber(subyte ui) : _type(SNumber::UINTEGER) { _value._ui = ui; }
SNumber::SNumber(sshort i) : _type(SNumber::INTEGER) { _value._i = i; }
SNumber::SNumber(sushort ui) : _type(SNumber::UINTEGER) { _value._ui = ui; }
SNumber::SNumber(int i) : _type(SNumber::INTEGER) { _value._i = i; }
SNumber::SNumber(unsigned int ui) : _type(SNumber::UINTEGER) { _value._ui = ui; }
SNumber::SNumber(size_t ui) : _type(SNumber::UINTEGER) { _value._ui = ui; }
#if defined(WIN64_OS)
SNumber::SNumber(long i) : _type(SNumber::INTEGER) { _value._i = i; }
SNumber::SNumber(unsigned long ui) : _type(SNumber::UINTEGER) { _value._ui = ui; }
#endif
SNumber::SNumber(long long i) : _type(SNumber::INTEGER) { _value._i = i; }
#if defined(MAC_OS)
SNumber::SNumber(unsigned long long ui) : _type(SNumber::UINTEGER) { _value._ui = ui; }
#endif
SNumber::SNumber(float f) : _type(SNumber::REAL) { _value._r = f; }
SNumber::SNumber(double d) : _type(SNumber::REAL) { _value._r = d; }
SNumber::SNumber(sint n, sint d) : _type(SNumber::FRAC) { _value._f[0] = n; _value._f[1] = d; }
SNumber::SNumber(const sfrac &frac) : _type(SNumber::FRAC) {
    _value._f[0] = frac.numerator; _value._f[1] = frac.denominator;
}
SNumber::SNumber(float r, float i) : _type(SNumber::COMPLEX) { _value._c[0] = r; _value._c[1] = i; }
SNumber::SNumber(const scomp &comp) : _type(SNumber::COMPLEX) {
    _value._c[0] = comp.real; _value._c[1] = comp.imaginary;
}
SNumber::SNumber(const sobj &obj) {
    if(obj.isNull()) *this = 0;
    else if (obj.isNum()) *this = obj.number();
    else *this = SNumber::toNumber(obj);
}
SNumber::SNumber(const SNumber &num) : _type(num._type) {
    memcpy(&_value, &num._value, sizeof(_num));
}
SNumber::~SNumber() {}

SNumber & SNumber::operator=(const SNumber &num) {
    _type = num._type;
    memcpy(&_value, &num._value, sizeof(_num));
    return *this;
}
SNumber &SNumber::operator+=(const SNumber &num) {
    switch (_type) {
        case SNumber::INTEGER:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._i += num._value._i;
                    break;
                case SNumber::UINTEGER:
                    _value._i += num._value._ui;
                    break;
                case SNumber::REAL:
                {
                    _value._r = (double)_value._i + num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
                    _value._i += num._value._b?1:0;
                    break;
                case SNumber::FRAC:
                {
                    _value._f[0] = num._value._f[1]*_value._i + num._value._f[0];
                    _value._f[1] = num._value._f[1];
                    _type = SNumber::FRAC;
                    break;
                }
                case SNumber::COMPLEX:
                {
                    _value._c[0] = (float)_value._i + num._value._c[0];
                    _value._c[1] = num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::UINTEGER:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                {
                    if (num._value._i < 0) {
                        if (_value._ui < -num._value._i) {
                            suinteger i = _value._ui;
                            _value._i = num._value._i + i;
                            _type = SNumber::INTEGER;
                        }
                        else _value._ui -= (-num._value._i);
                    }
                    else _value._ui += num._value._i;
                    break;
                }
                case SNumber::UINTEGER:
                    _value._ui += num._value._ui;
                    break;
                case SNumber::REAL:
                {
                    _value._r = (double)_value._ui + num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
                    _value._ui += num._value._b?1:0;
                    break;
                case SNumber::FRAC:
                {
                    _value._f[0] = num._value._f[1]*_value._ui + num._value._f[0];
                    _value._f[1] = num._value._f[1];
                    _type = SNumber::FRAC;
                    break;
                }
                case SNumber::COMPLEX:
                {
                    _value._c[0] = (float)_value._ui + num._value._c[0];
                    _value._c[1] = num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::REAL:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._r += (double)num._value._i;
                    break;
                case SNumber::UINTEGER:
                    _value._r += (double)num._value._ui;
                    break;
                case SNumber::REAL:
                    _value._r += num._value._r;
                    break;
                case SNumber::BOOL:
                    _value._r += num._value._b?1.0:0.0;
                    break;
                case SNumber::FRAC:
                    _value._r += (double)num._value._f[0]/ num._value._f[1];
                    break;
                case SNumber::COMPLEX:
                {
                    _value._c[0] = _value._r + num._value._c[0];
                    _value._c[1] = num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::BOOL:
            _value._b = _value._b== num._value._b?false:true;
            break;
        case SNumber::FRAC:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._f[0] += _value._f[1] * num._value._i;
                    break;
                case SNumber::UINTEGER:
                    _value._f[0] += _value._f[1] * num._value._ui;
                    break;
                case SNumber::REAL:
                {
                    _value._r = (double)_value._f[0]/_value._f[1] + num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
                    _value._f[0] += _value._f[1] * (num._value._b?1:0);
                    break;
                case SNumber::FRAC:
                {
                    sfrac tmp = fraction() + num.fraction();
                    _value._f[0] = tmp.numerator;
                    _value._f[1] = tmp.denominator;
                    break;
                }
                case SNumber::COMPLEX:
                {
                    _value._c[0] = (float)_value._f[0]/_value._f[1] + num._value._c[0];
                    _value._c[1] = num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::COMPLEX:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._c[0] += (float)num._value._i;
                    break;
                case SNumber::UINTEGER:
                    _value._c[0] += (float)num._value._ui;
                    break;
                case SNumber::REAL:
                    _value._c[0] += (float)num._value._r;
                    break;
                case SNumber::BOOL:
                    _value._c[0] += num._value._b?1.0f:0.0f;
                    break;
                case SNumber::FRAC:
                    _value._c[0] += (float)num._value._f[0]/num._value._f[1];
                    break;
                case SNumber::COMPLEX:
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
        case SNumber::INTEGER:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._i -= num._value._i;
                    break;
                case SNumber::UINTEGER:
                    _value._i -= num._value._ui;
                    break;
                case SNumber::REAL:
                {
                    _value._r = (double)_value._i - num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
                    _value._i -= num._value._b?1:0;
                    break;
                case SNumber::FRAC:
                {
                    _value._f[0] = num._value._f[1]*_value._i - num._value._f[0];
                    _value._f[1] = num._value._f[1];
                    _type = SNumber::FRAC;
                    break;
                }
                case SNumber::COMPLEX:
                {
                    _value._c[0] = (float)_value._i - num._value._c[0];
                    _value._c[1] = -num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::UINTEGER:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                {
                    if (num._value._i < 0) _value._ui += (-num._value._i);
                    else if (_value._ui < num._value._i) {
                        _value._i = num._value._i - _value._ui;
						_value._i *= -1;
                        _type = SNumber::INTEGER;
                    }
                    else _value._ui -= num._value._i;
                    break;
                }
                case SNumber::UINTEGER:
                    if (_value._ui < num._value._ui) {
                        sinteger i = num._value._ui - _value._ui;
						_value._i *= -1;
                        _type = SNumber::INTEGER;
                    }
                    else _value._ui -= num._value._ui;
                    break;
                case SNumber::REAL:
                {
                    _value._r = (double)_value._ui - num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
                    _value._ui -= num._value._b?1:0;
                    break;
                case SNumber::FRAC:
                {
                    _value._f[0] = num._value._f[1]*_value._ui - num._value._f[0];
                    _value._f[1] = num._value._f[1];
                    _type = SNumber::FRAC;
                    break;
                }
                case SNumber::COMPLEX:
                {
                    _value._c[0] = (float)_value._ui - num._value._c[0];
                    _value._c[1] = -num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::REAL:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._r -= (double)num._value._i;
                    break;
                case SNumber::UINTEGER:
                    _value._r -= (double)num._value._ui;
                    break;
                case SNumber::REAL:
                    _value._r -= num._value._r;
                    break;
                case SNumber::BOOL:
                    _value._r -= num._value._b?1.0:0.0;
                    break;
                case SNumber::FRAC:
                    _value._r -= (double)num._value._f[0]/ num._value._f[1];
                    break;
                case SNumber::COMPLEX:
                {
                    _value._c[0] = _value._r - num._value._c[0];
                    _value._c[1] = -num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::BOOL:
            _value._b = _value._b==num._value._b?false:true;
            break;
        case SNumber::FRAC:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._f[0] -= num.integer() * _value._f[1];
                    break;
                case SNumber::UINTEGER:
                    _value._f[0] -= num.uinteger() * _value._f[1];
                    break;
                case SNumber::REAL:
                {
                    _value._r = (double)_value._f[0]/_value._f[1] - num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
                    _value._f[0] -= _value._f[1] * (num._value._b?1:0);
                    break;
                case SNumber::FRAC:
                {
                    sfrac tmp = fraction() - num.fraction();
                    _value._f[0] = tmp.numerator;
                    _value._f[1] = tmp.denominator;
                    break;
                }
                case SNumber::COMPLEX:
                {
                    _value._c[0] = (float)_value._f[0]/_value._f[1] - num._value._c[0];
                    _value._c[1] = -num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::COMPLEX:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._c[0] -= (float)num._value._i;
                    break;
                case SNumber::UINTEGER:
                    _value._c[0] -= (float)num._value._ui;
                    break;
                case SNumber::REAL:
                    _value._c[0] -= (float)num._value._r;
                    break;
                case SNumber::BOOL:
                    _value._c[0] -= num._value._b?1.0f:0.0f;
                    break;
                case SNumber::FRAC:
                    _value._c[0] -= (float)num._value._f[0]/ num._value._f[1];
                    break;
                case SNumber::COMPLEX:
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
        case SNumber::INTEGER:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._i *= num._value._i;
                    break;
                case SNumber::UINTEGER:
                    _value._i *= num._value._ui;
                    break;
                case SNumber::REAL:
                {
                    _value._r = (double)_value._i * num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
                    _value._i *= num._value._b?1:0;
                    break;
                case SNumber::FRAC:
                {
                    _value._f[0] = num._value._f[0]*_value._i;
                    _value._f[1] = num._value._f[1];
					smath::reduction(_value._f[0], _value._f[1]);
                    _type = SNumber::FRAC;
                    break;
                }
                case SNumber::COMPLEX:
                {
                    float tmp = _value._i;
                    _value._c[0] = tmp * num._value._c[0];
                    _value._c[1] = tmp * num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::UINTEGER:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._ui *= num._value._i;
                    break;
                case SNumber::UINTEGER:
                    _value._ui *= num._value._ui;
                    break;
                case SNumber::REAL:
                {
                    _value._r = (double)_value._ui * num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
                    _value._ui *= num._value._b?1:0;
                    break;
                case SNumber::FRAC:
                {
                    _value._f[0] = num._value._f[0]*_value._ui;
                    _value._f[1] = num._value._f[1];
					smath::reduction(_value._f[0], _value._f[1]);
                    _type = SNumber::FRAC;
                    break;
                }
                case SNumber::COMPLEX:
                {
                    float tmp = _value._ui;
                    _value._c[0] = tmp * num._value._c[0];
                    _value._c[1] = tmp * num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::REAL:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._r *= (double)num._value._i;
                    break;
                case SNumber::UINTEGER:
                    _value._r *= (double)num._value._ui;
                    break;
                case SNumber::REAL:
                    _value._r *= num._value._r;
                    break;
                case SNumber::BOOL:
                    _value._r *= num._value._b?1.0:0.0;
                    break;
                case SNumber::FRAC:
                    _value._r *= (double)num._value._f[0]/ num._value._f[1];
                    break;
                case SNumber::COMPLEX:
                {
                    double tmp = _value._r;
                    _value._c[0] = tmp * num._value._c[0];
                    _value._c[1] = tmp * num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::BOOL:
            _value._b = _value._b&& num._value._b?true:false;
            break;
        case SNumber::FRAC:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._f[0] *= num._value._i;
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                case SNumber::UINTEGER:
                    _value._f[0] *= num._value._ui;
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                case SNumber::REAL:
                {
                    _value._r = (double)_value._f[0]/_value._f[1] * num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
                    _value._f[0] *= num._value._b?1:0;
                    break;
                case SNumber::FRAC:
                {
                    _value._f[0] *= num._value._f[0];
                    _value._f[1] *= num._value._f[1];
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                }
                case SNumber::COMPLEX:
                {
                    float tmp = floatValue();
                    _value._c[0] = tmp * num._value._c[0];
                    _value._c[1] = tmp * num._value._c[1];
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::COMPLEX:
        {
            switch (num._type) {
                case SNumber::INTEGER:
				{
					_value._c[0] *= (float)num._value._i;
					_value._c[1] *= (float)num._value._i;
					break;
				}
                case SNumber::UINTEGER:
				{
					_value._c[0] *= (float)num._value._ui;
					_value._c[1] *= (float)num._value._ui;
					break;
				}
                case SNumber::REAL:
				{
					_value._c[0] *= (float)num._value._r;
					_value._c[1] *= (float)num._value._r;
					break;
				}
                case SNumber::BOOL:
				{
					_value._c[0] *= num._value._b ? 1.0f : 0.0f;
					_value._c[1] *= num._value._b ? 1.0f : 0.0f;
					break;
				}
                case SNumber::FRAC:
				{
					float tmp = (float)num._value._f[0] / num._value._f[1];
					_value._c[0] *= tmp;
					_value._c[1] *= tmp;
					break;
				}
                case SNumber::COMPLEX:
                {
                    scomp tmp = complex() * num.complex();
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
        case SNumber::INTEGER:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                {
                    if (num._value._i == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
					_value._r = (double)_value._i / num._value._i;
					_type = SNumber::REAL;
                    break;
                }
                case SNumber::UINTEGER:
                {
                    if (num._value._ui == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
					_value._r = (double)_value._i / num._value._ui;
					_type = SNumber::REAL;
                    break;
                }
                case SNumber::REAL:
                {
                    if (num._value._r == 0.0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
                    _value._r = (double)_value._i / num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
                    throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
                case SNumber::FRAC:
                {
                    if (num._value._f[0] == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
                    _value._f[0] = num._value._f[1]*_value._i;
                    _value._f[1] = num._value._f[0];
                    _type = SNumber::FRAC;
                    break;
                }
                case SNumber::COMPLEX:
                {
					scomp tmp = (float)_value._i/ num.complex();
                    _value._c[0] = tmp.real;
                    _value._c[1] = tmp.imaginary;
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::UINTEGER:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                {
                    if (num._value._i == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
					_value._r = (double)_value._ui / num._value._i;
					_type = SNumber::REAL;
					break;
                }
                case SNumber::UINTEGER:
                {
                    if (num._value._ui == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
					_value._r = (double)_value._ui / num._value._ui;
					_type = SNumber::REAL;
					break;
                }
                case SNumber::REAL:
                {
					if (num._value._r == 0.0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
					_value._r = (double)_value._ui / num._value._r;
					_type = SNumber::REAL;
					break;
                }
                case SNumber::BOOL:
					throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
                case SNumber::FRAC:
                {
					if (num._value._f[0] == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
					_value._f[0] = num._value._f[1] * _value._ui;
					_value._f[1] = num._value._f[0];
					_type = SNumber::FRAC;
					break;
                }
                case SNumber::COMPLEX:
                {
					scomp tmp = (float)_value._ui / num.complex();
					_value._c[0] = tmp.real;
					_value._c[1] = tmp.imaginary;
					_type = SNumber::COMPLEX;
					break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::REAL:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                {
                    if (num._value._i == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
					_value._r /= num._value._i;
					break;
                }
                case SNumber::UINTEGER:
                {
                    if (num._value._ui == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
					_value._r /= num._value._ui;
					break;
                }
                case SNumber::REAL:
                {
					if (num._value._r == 0.0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
					_value._r /= num._value._r;
					break;
                }
                case SNumber::BOOL:
					throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
                case SNumber::FRAC:
                {
					if (num._value._f[0] == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
					_value._r /= ((float)num._value._f[0]/ num._value._f[1]);
                    break;
                }
                case SNumber::COMPLEX:
                {
					scomp tmp = (float)_value._r / num.complex();
					_value._c[0] = tmp.real;
					_value._c[1] = tmp.imaginary;
					_type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::BOOL:
			throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
        case SNumber::FRAC:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                {
					if (num._value._i == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
                    _value._f[1] *= num._value._i;
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                }
                case SNumber::UINTEGER:
                {
					if (num._value._ui == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
                    _value._f[1] *= num._value._ui;
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                }
                case SNumber::REAL:
                {
					if (num._value._r == 0.0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
                    _value._r = ((double)_value._f[0]/_value._f[1]) / num._value._r;
                    _type = SNumber::REAL;
                    break;
                }
                case SNumber::BOOL:
					throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
                case SNumber::FRAC:
                {
					if (num._value._f[0] == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
                    _value._f[0] *= num._value._f[1];
                    _value._f[1] *= num._value._f[0];
					smath::reduction(_value._f[0], _value._f[1]);
                    break;
                }
                case SNumber::COMPLEX:
                {
                    scomp tmp = ((float)_value._f[0]/_value._f[1]) / num.complex();
                    _value._c[0] = tmp.real;
                    _value._c[1] = tmp.imaginary;
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SNumber::COMPLEX:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                {
					if (num._value._i == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
                    _value._c[0] /= (float)num._value._i;
                    _value._c[1] /= (float)num._value._i;
                    break;
                }
                case SNumber::UINTEGER:
                {
					if (num._value._ui == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
                    _value._c[0] /= (float)num._value._ui;
                    _value._c[1] /= (float)num._value._ui;
                    break;
                }
                case SNumber::REAL:
                {
					if (num._value._r == 0.0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
                    _value._c[0] /= (float)num._value._r;
                    _value._c[1] /= (float)num._value._r;
                    break;
                }
                case SNumber::BOOL:
					throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
                case SNumber::FRAC:
                {
					if (num._value._f[0] == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
                    float tmp = num.floatValue();
                    _value._c[0] /= tmp;
                    _value._c[1] /= tmp;
                    break;
                }
                case SNumber::COMPLEX:
                {
                    scomp tmp = complex() / num.complex();
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
        case SNumber::INTEGER:
        {
            switch (num._type) {
                case SNumber::INTEGER:
                    _value._i -= q * num._value._i;
                    break;
                case SNumber::UINTEGER:
					_value._i -= q * (sint)num._value._ui;
                    break;
                case SNumber::REAL:
                {
					_value._r = (double)_value._i - num._value._r * q;
                    _type = SNumber::REAL;
                    break;
                }
				case SNumber::BOOL:
					throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
                case SNumber::FRAC:
                {
					sfrac tmp = (sint)_value._i - num.fraction() * (sint)q;
					_value._f[0] = tmp.numerator;
					_value._f[1] = tmp.denominator;
					_type = SNumber::FRAC;
                    break;
                }
                case SNumber::COMPLEX:
                {
                    scomp tmp = (float)_value._i - num.complex() * (float)q;
                    _value._c[0] = tmp.real;
                    _value._c[1] = tmp.imaginary;
                    _type = SNumber::COMPLEX;
                    break;
                }
                default:
					break;
            }
            break;
        }
        case SNumber::UINTEGER:
        {
            switch (num._type) {
			case SNumber::INTEGER:
				_value._ui -= q * num._value._i;
				break;
			case SNumber::UINTEGER:
				_value._ui -= q * (sint)num._value._ui;
				break;
			case SNumber::REAL:
			{
				_value._r = (double)_value._ui - num._value._r * q;
				_type = SNumber::REAL;
				break;
			}
			case SNumber::BOOL:
				throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
			case SNumber::FRAC:
			{
				sfrac tmp = (sint)_value._ui - num.fraction() * (sint)q;
				_value._f[0] = tmp.numerator;
				_value._f[1] = tmp.denominator;
				_type = SNumber::FRAC;
				break;
			}
			case SNumber::COMPLEX:
			{
				scomp tmp = (float)_value._ui - num.complex() * (float)q;
				_value._c[0] = tmp.real;
				_value._c[1] = tmp.imaginary;
				_type = SNumber::COMPLEX;
				break;
			}
			default:
				break;
            }
            break;
        }
        case SNumber::REAL:
        {
            switch (num._type) {
			case SNumber::INTEGER:
				_value._r -= q * num._value._i;
				break;
			case SNumber::UINTEGER:
				_value._r -= q * (sint)num._value._ui;
				break;
			case SNumber::REAL:
				_value._r -= num._value._r * q;
				break;
			case SNumber::BOOL:
				throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
			case SNumber::FRAC:
				_value._r -= num.doubleValue() * q;
				break;
			case SNumber::COMPLEX:
			{
				scomp tmp = (float)_value._r - num.complex() * (float)q;
				_value._c[0] = tmp.real;
				_value._c[1] = tmp.imaginary;
				_type = SNumber::COMPLEX;
				break;
			}
			default:
				break;
            }
            break;
        }
		case SNumber::BOOL:
			throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
		case SNumber::FRAC:
        {
            switch (num._type) {
			case SNumber::INTEGER:
			{
				sfrac tmp = fraction();
				tmp -= q * num._value._i;
				_value._f[0] = tmp.numerator;
				_value._f[1] = tmp.denominator;
				break;
			}
			case SNumber::UINTEGER:
			{
				sfrac tmp = fraction();
				tmp -= q * (sint)num._value._ui;
				_value._f[0] = tmp.numerator;
				_value._f[1] = tmp.denominator;
				break;
			}
			case SNumber::REAL:
			{
				_value._r = doubleValue() - num._value._r * q;
				_type = SNumber::REAL;
				break;
			}
			case SNumber::BOOL:
				throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
			case SNumber::FRAC:
			{
				sfrac tmp = fraction() - num.fraction() * (sint)q;
				_value._f[0] = tmp.numerator;
				_value._f[1] = tmp.denominator;
				break;
			}
			case SNumber::COMPLEX:
			{
				scomp tmp = floatValue() - num.complex() * (float)q;
				_value._c[0] = tmp.real;
				_value._c[1] = tmp.imaginary;
				_type = SNumber::COMPLEX;
				break;
			}
			default:
				break;
            }
            break;
        }
        case SNumber::COMPLEX:
        {
            switch (num._type) {
			case SNumber::INTEGER:
				_value._c[0] -= (float)q * num._value._i;
				break;
			case SNumber::UINTEGER:
				_value._c[0] -= (float)q * num._value._ui;
				break;
			case SNumber::REAL:
				_value._c[0] -= num._value._r * q;
				break;
			case SNumber::BOOL:
				throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
			case SNumber::FRAC:
				_value._c[0] -= num.floatValue() * q;
				break;
			case SNumber::COMPLEX:
			{
				scomp tmp = num.complex() * (float)q;
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
        case SNumber::INTEGER:
            ++_value._i;
            break;
        case SNumber::UINTEGER:
            ++_value._ui;
            break;
        case SNumber::REAL:
            _value._r += 1.0;
            break;
        case SNumber::BOOL:
            _value._b = _value._b?false:true;
            break;
        case SNumber::FRAC:
            _value._f[0] += _value._f[1];
            break;
        case SNumber::COMPLEX:
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
        case SNumber::INTEGER:
            --_value._i;
            break;
        case SNumber::UINTEGER:
            --_value._ui;
            break;
        case SNumber::REAL:
            _value._r-=1.0;
            break;
        case SNumber::BOOL:
            _value._b = _value._b?false:true;
            break;
        case SNumber::FRAC:
            _value._f[0] -= _value._f[1];
            break;
        case SNumber::COMPLEX:
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
        case SNumber::INTEGER:
            return SNumber(_value._i * -1);
        case SNumber::UINTEGER:
            throw SMathException(ERR_INFO, UNSIGNED_MINUS_ERR, "_value");
        case SNumber::REAL:
            return SNumber(_value._r * -1.0);
        case SNumber::BOOL:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "_value");
        case SNumber::FRAC:
            return SNumber(_value._f[0]*-1, _value._f[1]*-1);
        case SNumber::COMPLEX:
            return SNumber(_value._c[0]*-1.0f, _value._c[1]*-1.0f);
        default:
            return *this;
    }
}
SNumber SNumber::abs() const { if (*this < 0) return -(*this); else return SNumber(*this); }
SNumber SNumber::operator+(const sbyte& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const subyte& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const sshort& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const sushort& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const int& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const unsigned int& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const size_t& num) const { return SNumber(*this) += num; }
#if defined(WIN64_OS)
SNumber SNumber::operator+(const long& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const unsigned long& num) const { return SNumber(*this) += num; }
#endif
SNumber SNumber::operator+(const long long& num) const { return SNumber(*this) += num; }
#if defined(MAC_OS)
SNumber SNumber::operator+(const unsigned long long& num) const { return SNumber(*this) += num; }
#endif
SNumber SNumber::operator+(const float& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const double& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const sfrac& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const scomp& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator+(const SNumber& num) const { return SNumber(*this) += num; }
SNumber SNumber::operator-(const sbyte& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const subyte& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const sshort& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const sushort& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const int& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const unsigned int& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const size_t& num) const { return SNumber(*this) -= num; }
#if defined(WIN64_OS)
SNumber SNumber::operator-(const long& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const unsigned long& num) const { return SNumber(*this) -= num; }
#endif
SNumber SNumber::operator-(const long long& num) const { return SNumber(*this) -= num; }
#if defined(MAC_OS)
SNumber SNumber::operator-(const unsigned long long& num) const { return SNumber(*this) -= num; }
#endif
SNumber SNumber::operator-(const float& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const double& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const sfrac& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const scomp& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator-(const SNumber& num) const { return SNumber(*this) -= num; }
SNumber SNumber::operator*(const sbyte& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const subyte& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const sshort& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const sushort& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const int& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const unsigned int& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const size_t& num) const { return SNumber(*this) *= num; }
#if defined(WIN64_OS)
SNumber SNumber::operator*(const long& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const unsigned long& num) const { return SNumber(*this) *= num; }
#endif
SNumber SNumber::operator*(const long long& num) const { return SNumber(*this) *= num; }
#if defined(MAC_OS)
SNumber SNumber::operator*(const unsigned long long& num) const { return SNumber(*this) *= num; }
#endif
SNumber SNumber::operator*(const float& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const double& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const sfrac& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const scomp& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator*(const SNumber& num) const { return SNumber(*this) *= num; }
SNumber SNumber::operator/(const sbyte& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const subyte& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const sshort& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const sushort& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const int& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const unsigned int& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const size_t& num) const { return SNumber(*this) /= num; }
#if defined(WIN64_OS)
SNumber SNumber::operator/(const long& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const unsigned long& num) const { return SNumber(*this) /= num; }
#endif
SNumber SNumber::operator/(const long long& num) const { return SNumber(*this) /= num; }
#if defined(MAC_OS)
SNumber SNumber::operator/(const unsigned long long& num) const { return SNumber(*this) /= num; }
#endif
SNumber SNumber::operator/(const float& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const double& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const sfrac& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const scomp& num) const { return SNumber(*this) /= num; }
SNumber SNumber::operator/(const SNumber& num) const { return SNumber(*this) /= num; }

template<typename T1, typename T2>
inline sinteger _quot(const T1& t1, const T2& t2) {
	if (t2 == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
	if (t1 < 0) {
		if (t2 < 0) return (sinteger)((-t1) / (-t2));
		else return -(sinteger)((-t1) / t2);
	}
	else {
		if (t2 < 0) return -(sinteger)(t1 / (-t2));
		else return (sinteger)(t1 / t2);
	}
}
sinteger SNumber::quot(const SNumber& num) const {
	sinteger q;
	switch (_type) {
	case SNumber::INTEGER:
	{
		switch (num._type) {
		case SNumber::INTEGER:
		{
			if (num._value._i == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._i, num._value._i);
			break;
		}
		case SNumber::UINTEGER:
		{
			if (num._value._ui == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._i, (sinteger)num._value._ui);
			break;
		}
		case SNumber::REAL:
		{
			if (num._value._r == 0.0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._i, num._value._r);
			break;
		}
		case SNumber::BOOL:
			throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
		case SNumber::FRAC:
		{
			if (num._value._f[0] == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(num._value._f[1] * _value._i, num._value._f[0]);
			break;
		}
		case SNumber::COMPLEX:
		{
			if (num.complex() == 0.0f) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._i, num._value._c[0]);
			break;
		}
		default:
			break;
		}
		break;
	}
	case SNumber::UINTEGER:
	{
		switch (num._type) {
		case SNumber::INTEGER:
		{
			if (num._value._i == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot((sinteger)_value._ui, num._value._i);
			break;
		}
		case SNumber::UINTEGER:
		{
			if (num._value._ui == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot((sinteger)_value._ui, (sinteger)num._value._ui);
			break;
		}
		case SNumber::REAL:
		{
			if (num._value._r == 0.0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot((sinteger)_value._ui, num._value._r);
			break;
		}
		case SNumber::BOOL:
			throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
		case SNumber::FRAC:
		{
			if (num._value._f[0] == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(num._value._f[1] * (sinteger)_value._ui, num._value._f[0]);
			break;
		}
		case SNumber::COMPLEX:
		{
			if (num.complex() == 0.0f) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot((sinteger)_value._ui, num._value._c[0]);
			break;
		}
		default:
			break;
		}
		break;
	}
	case SNumber::REAL:
	{
		switch (num._type) {
		case SNumber::INTEGER:
		{
			if (num._value._i == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._r, num._value._i);
			break;
		}
		case SNumber::UINTEGER:
		{
			if (num._value._ui == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._r, (sinteger)num._value._ui);
			break;
		}
		case SNumber::REAL:
		{
			if (num._value._r == 0.0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._r, num._value._r);
			break;
		}
		case SNumber::BOOL:
			throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
		case SNumber::FRAC:
		{
			if (num._value._f[0] == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(num._value._f[1] * _value._r, num._value._f[0]);
			break;
		}
		case SNumber::COMPLEX:
		{
			if (num.complex() == 0.0f) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._r, num._value._c[0]);
			break;
		}
		default:
			break;
		}
		break;
	}
	case SNumber::BOOL:
		throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
	case SNumber::FRAC:
	{
		switch (num._type) {
		case SNumber::INTEGER:
		{
			if (num._value._i == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._f[0], _value._f[1] * num._value._i);
			break;
		}
		case SNumber::UINTEGER:
		{
			if (num._value._ui == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._f[0], _value._f[1] * (sinteger)num._value._ui);
			break;
		}
		case SNumber::REAL:
		{
			if (num._value._r == 0.0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._f[0], _value._f[1] * num._value._r);
			break;
		}
		case SNumber::BOOL:
			throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
		case SNumber::FRAC:
		{
			if (num._value._f[0] == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._f[0] * num._value._f[1], _value._f[1] * num._value._f[0]);
			break;
		}
		case SNumber::COMPLEX:
		{
			if (num.complex() == 0.0f) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(floatValue(), num._value._c[0]);
			break;
		}
		default:
			break;
		}
		break;
	}
	case SNumber::COMPLEX:
	{
		switch (num._type) {
		case SNumber::INTEGER:
		{
			if (num._value._i == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._c[0], num._value._i);
			break;
		}
		case SNumber::UINTEGER:
		{
			if (num._value._ui == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._c[0], (sinteger)num._value._ui);
			break;
		}
		case SNumber::REAL:
		{
			if (num._value._r == 0.0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._c[0], num._value._r);
			break;
		}
		case SNumber::BOOL:
			throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "bool", "division");
		case SNumber::FRAC:
		{
			if (num._value._f[0] == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._c[0]* num._value._f[1], num._value._f[0]);
			break;
		}
		case SNumber::COMPLEX:
		{
			if (num.complex() == 0.0f) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "num");
			q = _quot(_value._c[0], num._value._c[0]);
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
SNumber SNumber::operator%(const sbyte& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const subyte& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const sshort& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const sushort& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const int& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const unsigned int& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const size_t& num) const { return SNumber(*this) %= num; }
#if defined(WIN64_OS)
SNumber SNumber::operator%(const long& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const unsigned long& num) const { return SNumber(*this) %= num; }
#endif
SNumber SNumber::operator%(const long long& num) const { return SNumber(*this) %= num; }
#if defined(MAC_OS)
SNumber SNumber::operator%(const unsigned long long& num) const { return SNumber(*this) %= num; }
#endif
SNumber SNumber::operator%(const float& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const double& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const sfrac& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const scomp& num) const { return SNumber(*this) %= num; }
SNumber SNumber::operator%(const SNumber& num) const { return SNumber(*this) %= num; }
String SNumber::operator+(const char *s) { return toString()+s; }

subyte SNumber::type() const { return _type; }
void *SNumber::ptr() const { return const_cast<void *>((const void *)&_value); }
void SNumber::setType(subyte type) {
    if(_type == type) return;
    switch(_type) {
        case SNumber::INTEGER:
        {
            switch (type) {
                case SNumber::UINTEGER:
                    _value._ui = (suinteger)_value._i;
                    break;
                case SNumber::REAL:
                    _value._r = (sreal)_value._i;
                    break;
                case SNumber::BOOL:
                    _value._b = _value._i != 0;
                    break;
                case SNumber::FRAC:
                {
                    _value._f[0] = _value._i;
                    _value._f[1] = 1;
                    break;
                }
                case SNumber::COMPLEX:
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
        case SNumber::UINTEGER:
        {
            switch (type) {
                case SNumber::INTEGER:
                    _value._i = (sinteger)_value._ui;
                    break;
                case SNumber::REAL:
                    _value._r = (sreal)_value._ui;
                    break;
                case SNumber::BOOL:
                    _value._b = _value._ui != 0;
                    break;
                case SNumber::FRAC:
                {
                    _value._f[0] = _value._ui;
                    _value._f[1] = 1;
                    break;
                }
                case SNumber::COMPLEX:
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
        case SNumber::REAL:
        {
            switch (type) {
                case SNumber::INTEGER:
                    _value._i = (sinteger)_value._r;
                    break;
                case SNumber::UINTEGER:
                    _value._ui = (sreal)_value._r;
                    break;
                case SNumber::BOOL:
                    _value._b = _value._r != 0.0;
                    break;
                case SNumber::FRAC:
                {
                    sfrac tmp = _value._r;
                    _value._f[0] = tmp.numerator;
                    _value._f[1] = tmp.denominator;
                    break;
                }
                case SNumber::COMPLEX:
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
        case SNumber::BOOL:
        {
            switch (type) {
                case SNumber::INTEGER:
                    _value._i = _value._b?1:0;
                    break;
                case SNumber::UINTEGER:
                    _value._ui = _value._b?1:0;
                    break;
                case SNumber::REAL:
                    _value._r = _value._b?1.0:0.0;
                    break;
                case SNumber::FRAC:
                {
                    _value._f[0] = _value._b?1:0;
                    _value._f[1] = 1;
                    break;
                }
                case SNumber::COMPLEX:
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
        case SNumber::FRAC:
        {
            switch (type) {
                case SNumber::INTEGER:
                    _value._i = _value._f[0]/_value._f[1];
                    break;
                case SNumber::UINTEGER:
                    _value._ui = _value._f[0]/_value._f[1];
                    break;
                case SNumber::REAL:
                    _value._r = (double)_value._f[0]/_value._f[1];
                    break;
                case SNumber::BOOL:
                    _value._b = _value._f[0] == 0;
                    break;
                case SNumber::COMPLEX:
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
        case SNumber::COMPLEX:
        {
            switch (type) {
                case SNumber::INTEGER:
                    _value._i = (sinteger)_value._c[0];
                    break;
                case SNumber::UINTEGER:
                    _value._ui = (suinteger)_value._c[0];
                    break;
                case SNumber::REAL:
                    _value._r = _value._c[0];
                    break;
                case SNumber::BOOL:
                    _value._b = _value._c[0]==0.0 && _value._c[1]==0.0;
                    break;
                case SNumber::FRAC:
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

String SNumber::precised(size_t size, smath::ROUND round) const {
    if(_type == SNumber::INTEGER || _type == SNumber::UINTEGER)
        return toString()+"."+SString('0')*size;
    else if(_type == SNumber::REAL) {
        auto tmp = real();
        if (tmp == INFINITY) return "inf";
        else if (tmp == -INFINITY) return "-inf";
        else if (tmp == NAN) return "NaN";
        tmp *= smath::power(10, size);
        SString str;
        switch (round) {
            case FLOOR:
                str = SString((int)tmp);
                break;
            case CEIL:
            {
                if (tmp < 0) str = SString((int)tmp-1);
                else str = SString((int)tmp+1);
                break;
            }
            default:
            {
                auto remain = std::abs(tmp-(int)tmp);
                if (remain < 0.5) str = SString((int)tmp);
                else {
                    if (tmp < 0) str = SString((int)tmp-1);
                    else str = SString((int)tmp+1);
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
    else if(_type == SNumber::FRAC) return fraction().precised(size, round);
    else if (_type == SNumber::COMPLEX) return complex().precised(size, round);
    else return toString();
}

String SNumber::toBinary() const { return ""; }
String SNumber::toOct() const { return ""; }
String SNumber::toHex() const { return ""; }
String SNumber::toWideString() const {
    auto str = toString();
    String str_;
    sforeach(str) { str_ += Char::wideChar(E_); }
    return str;
}
String SNumber::getClass() const {
    if (_type == SNumber::INTEGER || _type == SNumber::UINTEGER) return "integer";
    else if (_type == SNumber::REAL) return "real";
    else if (_type == SNumber::BOOL) return "bool";
    else if (_type == SNumber::FRAC) return "fraction";
    else if (_type == SNumber::COMPLEX) return "complex";
    return "number";
}
String SNumber::toString() const {
    if (_type == SNumber::INTEGER) return std::to_string(integer());
    else if (_type == SNumber::UINTEGER) return std::to_string(uinteger());
    else if (_type == SNumber::REAL) return std::to_string(doubleValue());
    else if (_type == SNumber::BOOL) return boolean()?"true":"false";
    else if (_type == SNumber::FRAC) return fraction().toString();
    else if (_type == SNumber::COMPLEX) return complex().toString();
    return "";
}
SObject *SNumber::clone() const { return new SNumber(*this); }

bool SNumber::boolean() const {
    switch (_type) {
        case SNumber::INTEGER:
            return _value._i != 0;
        case SNumber::UINTEGER:
            return _value._ui != 0;
        case SNumber::REAL:
            return _value._r != 0.0;
        case SNumber::BOOL:
            return _value._b;
        case SNumber::FRAC:
            return _value._f[0] != 0;
        case SNumber::COMPLEX:
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
        case SNumber::INTEGER:
            return _value._i;
        case SNumber::UINTEGER:
            return _value._ui;
        case SNumber::REAL:
            return _value._r;
        case SNumber::BOOL:
            return _value._b;
        case SNumber::FRAC:
            return _value._f[0]/_value._f[1];
        case SNumber::COMPLEX:
            throw SException(ERR_INFO);
        default:
            return 0;
    }
}
suinteger SNumber::uinteger() const {
    switch (_type) {
        case SNumber::INTEGER:
            return _value._i;
        case SNumber::UINTEGER:
            return _value._ui;
        case SNumber::REAL:
            return _value._r;
        case SNumber::BOOL:
            return _value._b;
        case SNumber::FRAC:
            return _value._f[0]/_value._f[1];
        case SNumber::COMPLEX:
            throw SException(ERR_INFO);
        default:
            return 0;
    }
}
float SNumber::floatValue() const { return (float)doubleValue(); }
double SNumber::doubleValue() const {
    switch (_type) {
        case SNumber::INTEGER:
            return _value._i;
        case SNumber::UINTEGER:
            return _value._ui;
        case SNumber::REAL:
            return _value._r;
        case SNumber::BOOL:
            return _value._b;
        case SNumber::FRAC:
            return (double)_value._f[0]/_value._f[1];
        case SNumber::COMPLEX:
            throw SException(ERR_INFO);
        default:
            return 0;
    }
}
sreal SNumber::real() const { return doubleValue(); }
sfrac SNumber::fraction() const {
    switch (_type) {
        case SNumber::INTEGER:
            return sfrac((sint)_value._i);
        case SNumber::UINTEGER:
            return sfrac((sint)_value._ui);
        case SNumber::REAL:
            return sfrac(_value._r);
        case SNumber::BOOL:
            return sfrac((sint)_value._b);
        case SNumber::FRAC:
            return sfrac(_value._f[0], _value._f[1]);
        case SNumber::COMPLEX:
            throw SException(ERR_INFO);
        default:
            return sfrac();
    }
}
scomp SNumber::complex() const {
    switch (_type) {
        case SNumber::INTEGER:
            return scomp((float)_value._i);
        case SNumber::UINTEGER:
            return scomp((float)_value._ui);
        case SNumber::REAL:
            return scomp(_value._r);
        case SNumber::BOOL:
            return scomp((float)_value._b);
        case SNumber::FRAC:
            return scomp((float)_value._f[0]/_value._f[1]);
        case SNumber::COMPLEX:
            return scomp(_value._c[0], _value._c[1]);
        default:
            return scomp();
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
#if defined(WIN64_OS)
SNumber::operator long() const { return longValue(); }
SNumber::operator unsigned long() const { return ulongValue(); }
#endif
SNumber::operator long long() const { return llongValue(); }
#if defined(MAC_OS)
SNumber::operator unsigned long long() const { return ullongValue(); }
#endif
SNumber::operator float() const { return floatValue(); }
SNumber::operator double() const { return doubleValue(); }
SNumber::operator sfrac() const { return fraction(); }
SNumber::operator scomp() const { return complex(); }

bool SNumber::operator<(const SNumber &sn) const {
    switch (_type) {
        case SNumber::INTEGER:
        {
            auto &tmp = _value._i;
            switch (_type) {
                case SNumber::INTEGER:
                    return tmp < sn.integer();
                case SNumber::UINTEGER:
                    return tmp < sn.uinteger();
                case SNumber::REAL:
                    return tmp < sn.real();
                case SNumber::BOOL:
                    return tmp < sn.integer();
                case SNumber::FRAC:
                    return (sint)tmp < sn.fraction();
                case SNumber::COMPLEX:
                    return (float)tmp < sn.complex();
                default:
                    break;
            }
            break;
        }
        case SNumber::UINTEGER:
        {
            auto &tmp = _value._ui;
            switch (_type) {
                case SNumber::INTEGER:
                    return tmp < sn.integer();
                case SNumber::UINTEGER:
                    return tmp < sn.uinteger();
                case SNumber::REAL:
                    return tmp < sn.real();
                case SNumber::BOOL:
                    return tmp < sn.integer();
                case SNumber::FRAC:
                    return (sint)tmp < sn.fraction();
                case SNumber::COMPLEX:
                    return (float)tmp < sn.complex();
                default:
                    break;
            }
            break;
        }
        case SNumber::REAL:
        {
            auto &tmp = _value._r;
            switch (_type) {
                case SNumber::INTEGER:
                    return tmp < sn.integer();
                case SNumber::UINTEGER:
                    return tmp < sn.uinteger();
                case SNumber::REAL:
                    return tmp < sn.real();
                case SNumber::BOOL:
                    return tmp < sn.integer();
                case SNumber::FRAC:
                    return tmp < (double)sn.fraction();
                case SNumber::COMPLEX:
                    return (float)tmp < sn.complex();
                default:
                    break;
            }
            break;
        }
        case SNumber::BOOL:
        {
            auto &tmp = _value._b;
            switch (_type) {
                case SNumber::INTEGER:
                    return tmp < sn.integer();
                case SNumber::UINTEGER:
                    return tmp < sn.uinteger();
                case SNumber::REAL:
                    return tmp < sn.real();
                case SNumber::BOOL:
                    return tmp < sn.integer();
                case SNumber::FRAC:
                    return (sint)tmp < sn.fraction();
                case SNumber::COMPLEX:
                    return (float)tmp < sn.complex();
                default:
                    break;
            }
            break;
        }
        case SNumber::FRAC:
        {
            auto tmp = fraction();
            switch (_type) {
                case SNumber::INTEGER:
                    return tmp < sn.intValue();
                case SNumber::UINTEGER:
                    return tmp < sn.intValue();
                case SNumber::REAL:
                    return (double)tmp < sn.real();
                case SNumber::BOOL:
                    return tmp < sn.intValue();
                case SNumber::FRAC:
                    return tmp < sn.fraction();
                case SNumber::COMPLEX:
                    return (float)tmp < sn.complex();
                default:
                    break;
            }
            break;
        }
        case SNumber::COMPLEX:
            return complex() < sn.complex();
        default:
            break;
    }
    return false;
}
bool SNumber::operator>(const SNumber &sn) const { return sn < *this; }
bool SNumber::operator<=(const SNumber &sn) const { return *this < sn || *this == sn; }
bool SNumber::operator>=(const SNumber &sn) const { return sn < *this || *this == sn; }
bool SNumber::operator==(const sobj &obj) const {
    if (obj.isNum()) return (*this) == obj.number();
    return false;
}
bool SNumber::operator==(const SNumber &sn) const {
    if (_type != sn._type) {
        switch (_type) {
            case SNumber::INTEGER:
            {
                auto &tmp = _value._i;
                switch (_type) {
                    case SNumber::UINTEGER:
                        return tmp == sn.uinteger();
                    case SNumber::REAL:
                        return tmp == sn.real();
                    case SNumber::BOOL:
                        return tmp == sn.integer();
                    case SNumber::FRAC:
                        return (sint)tmp == sn.fraction();
                    case SNumber::COMPLEX:
                        return scomp((float)tmp, 0.f) == sn.complex();
                    default:
                        break;
                }
                break;
            }
            case SNumber::UINTEGER:
            {
                auto &tmp = _value._ui;
                switch (_type) {
                    case SNumber::INTEGER:
                        return tmp == sn.integer();
                    case SNumber::REAL:
                        return tmp == sn.real();
                    case SNumber::BOOL:
                        return tmp == sn.integer();
                    case SNumber::FRAC:
                        return (sint)tmp == sn.fraction();
                    case SNumber::COMPLEX:
                        return scomp((float)tmp, 0.f) == sn.complex();
                    default:
                        break;
                }
                break;
            }
            case SNumber::REAL:
            {
                auto &tmp = _value._r;
                switch (_type) {
                    case SNumber::INTEGER:
                        return tmp == sn.integer();
                    case SNumber::UINTEGER:
                        return tmp == sn.uinteger();
                    case SNumber::BOOL:
                        return tmp == sn.integer();
                    case SNumber::FRAC:
                        return tmp == sn.real();
                    case SNumber::COMPLEX:
                        return scomp(tmp, 0.f) == sn.complex();
                    default:
                        break;
                }
                break;
            }
            case SNumber::BOOL:
                return _value._b == sn.boolean();
            case SNumber::FRAC:
            {
                auto tmp = fraction();
                switch (_type) {
                    case SNumber::INTEGER:
                        return tmp == sn.fraction();
                    case SNumber::UINTEGER:
                        return tmp == sn.fraction();
                    case SNumber::REAL:
                        return (double)tmp == sn.real();
                    case SNumber::BOOL:
                        return tmp == sn.intValue();
                    case SNumber::COMPLEX:
                        return scomp((float)tmp, 0.f) == sn.complex();
                    default:
                        break;
                }
                break;
            }
            case SNumber::COMPLEX:
            {
                auto tmp = complex();
                switch (_type) {
                    case SNumber::INTEGER:
                        return tmp == sn.complex();
                    case SNumber::UINTEGER:
                        return tmp == sn.complex();
                    case SNumber::REAL:
                        return tmp == sn.complex();
                    case SNumber::BOOL:
                        return tmp == sn.complex();
                    case SNumber::FRAC:
                        return tmp == sn.complex();
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }
    return !memcmp(&_value, &sn._value, sizeof(_num));
}
bool SNumber::operator!=(const SNumber &sn) const { return !(*this == sn); }
String slib::operator+(const char *s, const SNumber &num) { return String(s) + num.toString(); }