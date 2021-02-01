#ifndef SLIB_SGRAPHIC_H
#define SLIB_SGRAPHIC_H

#include "sbasic.h"
#include "sobj/snumber.h"
#include "smath/geom.h"
#include "smedia/smedia.h"

namespace slib {
    using namespace smath;

    

        namespace smedia {

                class SLIB_DLL SFigure;

                extern inline v2f oriPos(sareaf& area, sgeom::ORIGIN ori) {
                        switch (ori) {
                        case sgeom::UPPER_LEFT:
                                return v2f(area.ori_x, area.ori_y);
                                break;
                        case sgeom::LOWER_LEFT:
                                return v2f(area.ori_x, area.ori_y + area.height);
                                break;
                        case sgeom::UPPER_RIGHT:
                                return v2f(area.ori_x + area.width, area.ori_y);
                                break;
                        case sgeom::LOWER_RIGHT:
                                return v2f(area.ori_x + area.width, area.ori_y + area.height);
                                break;
                        case sgeom::MASS_CENTER:
                                return v2f(area.ori_x + area.width / 2.0f, area.ori_y + area.height / 2.0f);
                                break;
                        default:
                                return v2f(0.f, 0.f);
                                break;
                        }
                }

                class SLIB_DLL STransform2D {

                public:
                        v2f scale, translate, skew;
                        float rotation;
                        subyte reflection;

                public:
                        STransform2D();
                        STransform2D(const STransform2D& trans);
                        ~STransform2D();

                        static void expand(v2f& s, v2fvec& vertex, v2f ori);
                        static void shift(v2f& t, v2fvec& vertex);
                        static void shear(v2f& s, v2fvec& vertex, v2f ori);
                        static void rotate(float rot, v2fvec& vertex, v2f ori);
                        static void reflect(subyte ref, v2fvec& vertex, v2f ori);
                        void clear();
                };

#define col3i v3ub
#define col3f v3f
#define col4i v4ub
#define col4f v4f

#define kui kvpair<String, suint>
                extern Map<String, suint> ColorMap;

                constexpr subyte RGB_SPACE = 0x10;
                constexpr subyte CMYK_SPACE = 0x20;
                //constexpr subyte HSV_SPACE = 0x40;

                constexpr sushort GRAY8 = 0x1101;
                constexpr sushort GRAY16 = 0x1102;
                constexpr sushort GRAYF = 0x1104;
                //constexpr sushort RGB8 = 0x1301;
                //constexpr sushort RGB16 = 0x1302;
                constexpr sushort RGB24 = 0x1303;
                constexpr sushort RGB32 = 0x1304;
                constexpr sushort RGBF = 0x130C;
                constexpr sushort RGBA = 0x1404;
                constexpr sushort RGBAF = 0x1410;
                constexpr sushort CMYK = 0x2404;
                constexpr sushort CMYKF = 0x2410;

                extern inline subyte colorSpace(sushort type) { return ((type & 0xF000) >> 8); }
                extern inline subyte colorChannel(sushort type) { return ((type & 0x0F00) >> 8); }
                extern inline subyte bytePerPixel(sushort type) { return (type & 0x00FF); }
                extern inline subyte colorDepth(sushort type) { return bytePerPixel(type) / colorChannel(type); }
                extern inline sushort colorType(subyte depth, subyte channel, subyte space) {
                        return (depth * channel) | ((channel | space) << 8);
                }
                extern inline sushort b2scolor(subyte i) { return (sushort)(((float)i / smath::MAX_UBYTE) * smath::MAX_USHORT); }
                extern inline subyte s2bcolor(sushort i) { return (subyte)(((float)i / smath::MAX_USHORT) * smath::MAX_UBYTE); }
                extern inline float b2fcolor(subyte i) { return (float)i / smath::MAX_UBYTE; }
                extern inline float s2fcolor(sushort i) { return (float)i / smath::MAX_USHORT; }
                extern inline subyte f2bcolor(float f) { return (subyte)(f * smath::MAX_UBYTE); }
                extern inline sushort f2scolor(float f) { return (sushort)(f * smath::MAX_USHORT); }
                extern inline suint f2icolor(float f) { return (suint)(f * smath::MAX_UBYTE); }
                extern inline suint f2icolor(float* f) {
                        return f2icolor(f[3]) | (f2icolor(f[2]) << 8) | (f2icolor(f[1]) << 16) | (f2icolor(f[0]) << 24);
                }

                class SLIB_DLL SColor : public SObject {
                public:
                        typedef enum {
                                DEFAULT_NUMERIC = 1,
                                DEFAULT_HEX = 2,
                                HTML_CODE = 3,
                                HTML_HEX = 4,
                                CSS_NUMERIC = 5,

                        } COLOR_TEXT_MODE;

                protected:
                        sushort _type;
                        ubytearray _data;

                public:
                        SColor(sushort t = GRAY8);
                        SColor(sushort t, void* bytes);
                        SColor(subyte col);
                        SColor(suint col);
                        SColor(const col3i& col);
                        SColor(const col4i& col);
                        SColor(const col3f& col);
                        SColor(const col4f& col);
                        SColor(int r, int g, int b, int a = 255);
                        SColor(const char* s);
                        SColor(const SColor& col);
                        SColor(const sobj& obj);
                        virtual ~SColor();

                        SColor& operator=(SColor&& col);
                        SColor& operator=(const SColor& col);

                        sushort type() const;
                        sbyte channel() const;
                        sbyte bpp() const;
                        const subyte* bytes() const;
                        const float* floats() const;

                        subyte gray8() const;
                        sushort gray16() const;
                        float grayf() const;
                        suint rgb() const;
                        suint rgba() const;
                        col3i toVec3i() const;
                        col4i toVec4i() const;
                        col3f toVec3f() const;
                        col4f toVec4f() const;
                        subyte red() const;
                        subyte green() const;
                        subyte blue() const;
                        subyte alpha() const;
                        float redf() const;
                        float greenf() const;
                        float bluef() const;
                        float alphaf() const;
                        /*
                        subyte cyan() const;
                        subyte magenta() const;
                        subyte yellow() const;
                        subyte keyplate() const;
                        float cyanf() const;
                        float magentaf() const;
                        float yellowf() const;
                        float keyplatef() const;
                        subyte hue() const;
                        subyte saturation() const;
                        subyte value() const;
                        */
                        bool isInt() const;
                        bool isFloat() const;
                        bool isClear() const;
                        bool isMono() const;
                        bool hasAlpha() const;
                        bool isRGB() const;
                        bool isCMYK() const;
                        bool isGradient() const;
                        void convert(sushort t);

                        virtual String getClass() const;
                        virtual String toString() const;
                        String toString(COLOR_TEXT_MODE mode) const;
                        virtual SObject* clone() const;

                        bool operator<(const SColor& col) const;
                        bool operator==(const SColor& col) const;
                };

                class SLIB_DLL SGradient : public SColor {
                protected:
                        sgeom::COORDINATE _coord;
                        float _angle;
                        floatarray _points;

                public:
                        SGradient();
                        SGradient(const SGradient& grad);
                        ~SGradient();

                        sgeom::COORDINATE coordinate() const;
                        size_t count() const;
                        float angle() const;
                        SColor operator[](size_t idx) const;
                        SColor colorAt(size_t idx) const;
                        floatarray& points();
                        const floatarray& points() const;

                        void setCoordinate(sgeom::COORDINATE coord);
                        void setAngle(float f);
                        void setPosition(size_t idx, float f);

                        void addColor(float f, const SColor& col);
                        void setColor(int idx, const SColor& col);
                        void removeColor(size_t idx);
                        void clear();

                        String getClass() const;
                        String toString() const;
                        SObject* clone() const;
                };

                namespace color {
                        const SColor CLEAR = (suint)0x00000000;
                        const SColor BLACK = (suint)0xFF000000;
                        const SColor WHITE = (suint)0xFFFFFFFF;
                        const SColor GRAY = (suint)0xFF808080;
                        const SColor LIGHTGRAY = (suint)0xFFD3D3D3;
                        const SColor DIMGRAY = (suint)0xFF696969;
                        const SColor RED = (suint)0xFF0000FF;
                        const SColor MAGENTA = (suint)0xFFFF00FF;
                        const SColor PINK = (suint)0xFFCBC0FF;
                        const SColor CRIMSON = (suint)0xFF143CDC;
                        const SColor DARKRED = (suint)0xFF00008B;
                        const SColor ORANGE = (suint)0xFF00A5FF;
                        const SColor YELLOW = (suint)0xFF00FFFF;
                        const SColor BROWN = (suint)0xFF2A2AA5;
                        const SColor LIME = (suint)0xFF00FF00;
                        const SColor GREEN = (suint)0xFF008000;
                        const SColor DARKGREEN = (suint)0xFF006400;
                        const SColor SPRING = (suint)0xFF7FFF00;
                        const SColor BLUE = (suint)0xFFFF0000;
                        const SColor TURQUOISE = (suint)0xFFD0E040;
                        const SColor CYAN = (suint)0xFFFFFF00;
                        const SColor DEEPSKY = (suint)0xFFFFBF00;
                        const SColor INDIGO = (suint)0xFF82004B;
                        const SColor NAVY = (suint)0xFF800000;
                        const SColor VIOLET = (suint)0xFFEE82EE;
                        const SColor PURPLE = (suint)0xFF800080;
                };
        }

}
#endif