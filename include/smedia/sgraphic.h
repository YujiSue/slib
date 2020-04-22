#ifndef SLIB_SGRAPHIC_H
#define SLIB_SGRAPHIC_H

#include "sbasic/sbasic.h"
#include "sobj/snumber.h"
#include "smath/geom.h"
#include "smedia/smedia.h"

namespace slib {
    using namespace smath;
    
    namespace sshape {
        constexpr int MULTIPLE = 0x0100;
        constexpr int ROUND = 0x0200;
        constexpr int CIRCULAR = 0x0400;
        constexpr int REGULAR = 0x0800;
        constexpr int CONCAVE = 0x1000;
        
        constexpr int TEXTURE = 0x4000;
        constexpr int GROUP = 0x8000;
        
        constexpr int POINT = 0x0001;
        constexpr int MULTI_POINT = MULTIPLE|POINT;
        constexpr int LINE = 0x0002;
        constexpr int CURVE = ROUND|LINE;
        constexpr int PATH = MULTIPLE|LINE;
        
        constexpr int RECTANGLE = 0x0004;
        constexpr int SQUARE = REGULAR|RECTANGLE;
        //constexpr int ROUND_RECTANGLE = ROUND|RECTANGLE;
        constexpr int PLANE = 0x0004;
        constexpr int POLYGON = 0x0008;
        constexpr int STAR = CONCAVE|POLYGON;
        
        constexpr int ELLIPSE = CIRCULAR|RECTANGLE;
        constexpr int CIRCLE = REGULAR|CIRCULAR|RECTANGLE;
        constexpr int ARC = CIRCULAR|LINE;
        
        constexpr int PICTURE = TEXTURE|RECTANGLE;
        constexpr int TEXT = 0x0010;
    }
    
    namespace smedia {
        
        #define strans2d sptr<STransform2D>
        
        extern inline v2f oriPos(Area<float> area, sgeom::ORIGIN ori) {
            switch (ori) {
                case sgeom::UP_LEFT:
                    return v2f(area.ori_x, area.ori_y);
                    break;
                case sgeom::DOWN_LEFT:
                    return v2f(area.ori_x, area.ori_y+area.height);
                    break;
                case sgeom::UP_RIGHT:
                    return v2f(area.ori_x+area.width, area.ori_y);
                    break;
                case sgeom::DOWN_RIGHT:
                    return v2f(area.ori_x+area.width, area.ori_y+area.height);
                    break;
                case sgeom::MASS_CENTER:
                    return v2f(area.ori_x+area.width/2.0f, area.ori_y+area.height/2.0f);
                    break;
                default:
                    return v2f(0, 0);
                    break;
            }
        }
        
        class SOBJ_DLL STransform2D {
        public:
            static void scaling(v2f v, v2f ori, v2fvec &points, Area<float> &boundary);
            static void shift(v2f v, v2fvec &points, Area<float> &boundary);
            static void shear(v2f v, v2f ori, v2fvec &points, Area<float> &boundary);
            static void rotate(float f, v2f ori, v2fvec &points, Area<float> &boundary);
            static void reflect(subyte axis, v2f ori, v2fvec &points, Area<float> &boundary);
            static void trans(mat3f affine, v2f ori, v2fvec &points, Area<float> &boundary);
            
        public:
            v2f scale, translate, skew;
            float rotation;
            subyte reflection;
            
        public:
            STransform2D();
            STransform2D(const STransform2D &trans);
            ~STransform2D();
            
            mat3f transMatrix() const;
            void clear();
        };
        
#define col3i svec3d<subyte>
#define col3f svec3d<float>
#define col4i svec4d<subyte>
#define col4f svec4d<float>
        
		#define kui kvpair<String, suint>
        extern Map<String, suint> SColorMap;
        
        constexpr subyte RGB_SPACE = 0x01;
        constexpr subyte CMYK_SPACE = 0x02;
		constexpr subyte HSV_SPACE = 0x03;
        
        constexpr sushort GRAY8 = 0x0111;
        constexpr sushort GRAY16 = 0x0112;
        constexpr sushort GRAY32 = 0x0114;
        constexpr sushort RGB8 = 0x0131;
        constexpr sushort RGB16 = 0x0132;
        constexpr sushort RGB24 = 0x0133;
        constexpr sushort RGB32 = 0x0134;
        constexpr sushort RGBA = 0x0144;
        constexpr sushort CMYK = 0x0244;
        constexpr sushort CMYKA = 0x0255;

		extern inline subyte colorSpace(sushort type) { return (type >> 8); }
		extern inline subyte colorChannel(sushort type) { return ((type&0xF0) >> 4); }
		extern inline subyte bytePerPixel(sushort type) { return (type & 0x0F); }
		extern inline subyte colorDepth(sushort type) { return bytePerPixel(type)/colorChannel(type); }
		extern inline sushort imageType(subyte depth, subyte channel, subyte space) {
			return (depth * channel) | (channel << 4) | (space << 8);
		}
        
#define scolor sptr<SColor>
        
        class SOBJ_DLL SColor {
        public:
            static const SColor CLEAR;
            static const SColor BLACK;
            static const SColor WHITE;
            static const SColor GRAY;
            static const SColor LIGHTGRAY;
            static const SColor DIMGRAY;
            static const SColor RED;
            static const SColor MAGENTA;
            static const SColor PINK;
            static const SColor CRIMSON;
            static const SColor DARKRED;
            static const SColor ORANGE;
            static const SColor YELLOW;
            static const SColor BROWN;
            static const SColor LIME;
            static const SColor GREEN;
            static const SColor DARKGREEN;
            static const SColor SPRING;
            static const SColor BLUE;
            static const SColor TURQUOISE;
            static const SColor CYAN;
            static const SColor DEEPSKY;
            static const SColor INDIGO;
            static const SColor NAVY;
            static const SColor VIOLET;
            static const SColor PURPLE;
            
        protected:
            sushort _type;
            floatarray _data;
            
        public:
            SColor(sushort t = GRAY8);
            SColor(subyte col);
            SColor(suint col);
            SColor(int r, int g, int b);
            SColor(const char *s);
            SColor(const SColor &col);
            virtual ~SColor();
            
            SColor &operator=(suint col);
            SColor &operator=(const SColor &col);
            
            sushort type() const;
            sbyte channel() const;
			sbyte bpp() const;
            subyte gray8() const;
            sushort gray16() const;
            col3i rgb() const;
            col4i rgba() const;
            suint intColor() const;
            subyte byteColor(sbyte ch) const;
            float floatColor(sbyte ch) const;
            float *ptr() const;
            
            bool isClear() const;
            bool isMono() const;
            bool hasAlpha() const;
            
            bool isRGB() const;
            bool isCMYK() const;
            
            String toHex(const char *prefix = "0x") const;
            String toString() const;
            
            void convert(sushort t);
            
            bool operator<(const SColor &col) const;
            bool operator==(const SColor &col) const;
        };
        
        class SOBJ_DLL SGradient : public SColor {
        protected:
            smath::sgeom::COORDINATE _mode;
            float _angle;
            floatarray _point;
            
        public:
            SGradient();
            SGradient(const SColor &col);
            SGradient(const SGradient &grad);
            ~SGradient();
            
            smath::sgeom::COORDINATE mode() const;
            size_t count() const;
            float angle() const;
            SColor operator[](size_t idx) const;
            SColor colorAt(size_t idx) const;
            floatarray &points();
            const floatarray &points() const;
            
            void setMode(smath::sgeom::COORDINATE m);
            void setAngle(float f);
            void setPosition(size_t idx, float f);
            
            void addColor(float f, const SColor &col);
            void setColor(int idx, const SColor &col);
            void removeColor(size_t idx);
            void clear();
            
            
        };
    }
    
}
#endif
