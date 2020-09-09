#ifndef SLIB_BASIC_H
#define SLIB_BASIC_H

#include "sbasic/ptr.h"
#include "sbasic/range.h"
#include "sbasic/region.h"
#include "sbasic/area.h"
#include "sbasic/zone.h"
#include "sbasic/array.h"
#include "sbasic/biarray.h"
#include "sbasic/fixedarray.h"
//#include "sbasic/cyclearray.h"
#include "sbasic/list.h"
#include "sbasic/locale.h"
#include "sbasic/map.h"
#include "sbasic/node.h"
#include "sbasic/char.h"
#include "sbasic/sutf8.h"
#include "sbasic/string.h"
#include "sbasic/time.h"
//#include "sbasic/tree.h"
#include "sbasic/style.h"
#include "smath.h"

namespace slib {
        template<typename T>
        extern std::ostream& operator<<(std::ostream& os, const Range<T>& range) { return os << "(" << range.begin << ", " << range.end << ")"; }
        template<typename T>
        extern std::ostream& operator<<(std::ostream& os, const Area<T>& area) { return os << "(" << area.ori_x << ", " << area.ori_y << ", " << area.width << ", " << area.height << ")"; }
        template<typename T>
        extern std::ostream& operator<<(std::ostream& os, const Zone<T>& zone) { return os << "(" << zone.ori_x << ", " << zone.ori_y << ", " << zone.ori_z << ", " << zone.width << ", " << zone.height << ", " << zone.depth << ")"; }
        template<typename T>
        extern std::ostream& operator<<(std::ostream& os, const smath::Fraction<T>& frac) { return os << frac.numerator << "/" << frac.denominator; }        template<typename T>
        extern std::ostream& operator<<(std::ostream& os, const smath::Complex<T>& comp) { return os << comp.real << (comp.imaginary<0?"":"+") << comp.imaginary << (comp.imaginary==0?"":"i"); }
        extern inline std::ostream& operator<<(std::ostream& os, const Char& c) {
#if defined(WIN32_OS) || defined(WIN64_OS)
                return os << c.toString().localize().cstr();
#else
                return os << c.toString().cstr();
#endif
        }
        extern inline std::ostream& operator<<(std::ostream& os, const String& str) {
#if defined(WIN32_OS) || defined(WIN64_OS)
                return os << str.localize().cstr();
#else
                return os << str.cstr();
#endif
        }
        extern inline std::istream& operator>>(std::istream& is, String& str) {
                is.seekg(0, std::ios::end);
                size_t size = is.tellg();
                is.clear();
                is.seekg(0);
#if defined(WIN32_OS) || defined(WIN64_OS)
                char* buf = new char[size + 1];
                is.read(buf, size);
                buf[size] = 0;
                str = String::toUTF8(buf);
#else
                str.resize(size);
                return is.read(str.ptr(), size);
#endif
                return is;
        }
        template<typename T, class M>
        extern inline std::ostream& operator<<(std::ostream& os, const Array<T, M>& array) { return os << toString(array); }
        template<typename T, size_t S, class M>
        extern inline std::ostream& operator<<(std::ostream& os, const FixedArray<T, S, M>& array) { return os << toString(array); }
        template<typename T, class M>
        extern inline std::ostream& operator<<(std::ostream& os, const BiArray<T, M>& array) { return os << toString(array); }
        template<typename T>
        extern inline std::ostream& operator<<(std::ostream& os, const List<T>& list) { return os << toString(list); }
        template<class Key, class Val>
        extern inline std::ostream& operator<<(std::ostream& os, const Map<Key, Val>& map) { return os << toString(map); }
        template<typename T>
        extern inline std::ostream& operator<<(std::ostream& os, const Region<T>& reg) { return os << toString(reg); }
        extern inline std::istream& operator>>(std::istream& is, ubytearray& array) {
                is.seekg(0, std::ios::end);
                size_t size = is.tellg();
                is.clear();
                is.seekg(0);
                array.resize(size);
                is.read((char *)array.ptr(), size);
                return is;
        }
}


#endif