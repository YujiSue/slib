#ifndef SLIB_SGRAPHIC_H
#define SLIB_SGRAPHIC_H
#include "sbasic/ptr.h"
#include "sbasic/string.h"
#include "sbasic/map.h"
#include "smath/vector.h"
#include "smath/geom.h"

namespace slib {
    class SLIB_DLL SObjPtr;
    class SLIB_DLL SElement;
    class SLIB_DLL Color;
    class SLIB_DLL SColor;

    enum class COLOR_SPACE : subyte {
        GRAY = 1,
        RGB = 2,
        CMYK = 3,
    };
    namespace scolor {
        extern SLIB_DLL COLOR_SPACE space(subyte type);
        extern SLIB_DLL bool isFloat(subyte type);
        extern SLIB_DLL bool hasAlpha(subyte type);
        extern SLIB_DLL subyte channel(subyte type);
        extern SLIB_DLL subyte depth(subyte type);
        extern SLIB_DLL subyte size(subyte type);

        //ALPHA | FLOAT | COLOR | DEPTH
        //  1   |   1   |   2   |   4
        constexpr subyte GRAY8 = 0x11; // 0x10 | 0x01;
        constexpr subyte GRAY16 = 0x12; // 0x10 | 0x02;
        constexpr subyte GRAY32 = 0x14; // 0x10 | 0x04;
        constexpr subyte RGB = 0x21; // 0x20 | 0x01;
        constexpr subyte RGBA = 0xA1; // 0x80 | 0x20 | 0x01;
        constexpr subyte CMYK = 0x31; //0x30 | 0x01;
        constexpr subyte GRAYF = 0x54;// 0x40 | 0x10 | 0x04;
        constexpr subyte RGBF = 0x64; // 0x40 | 0x20 | 0x04;
        constexpr subyte RGBAF = 0xE4; // 0x80 | 0x40 | 0x20 | 0x04;

        // Gradient Color
        constexpr subyte LINEAR_GRAD = 0x10;
        constexpr subyte RADIAL_GRAD = 0x20;

        // Color code
        constexpr int BLACK = 0x000000FF;
        constexpr int WHITE = 0xFFFFFFFF;
        constexpr int GRAY = 0x808080FF;
        constexpr int LIGHTGRAY = 0xD3D3D3FF;
        constexpr int DIMGRAY = 0x696969FF;
        constexpr int RED = 0xFF0000FF;
        constexpr int MAGENTA = 0xFF00FFFF;
        constexpr int PINK = 0xFFC0CBFF;
        constexpr int CRIMSON = 0xDC3C14FF;
        constexpr int DARKRED = 0x8B0000FF;
        constexpr int ORANGE = 0xFFA500FF;
        constexpr int YELLOW = 0xFFFF00FF;
        constexpr int BROWN = 0xA52A2AFF;
        constexpr int LIME = 0x00FF00FF;
        constexpr int GREEN = 0x008000FF;
        constexpr int DARKGREEN = 0x006400FF;
        constexpr int SPRING = 0x00FF7FFF;
        constexpr int BLUE = 0x0000FFFF;
        constexpr int TURQUOISE = 0x40E0D0FF;
        constexpr int CYAN = 0x00FFFFFF;
        constexpr int DEEPSKY = 0x00BFFFFF;
        constexpr int INDIGO = 0x4B0082FF;
        constexpr int NAVY = 0x000080FF;
        constexpr int VIOLET = 0xEE82EEFF;
        constexpr int PURPLE = 0x800080FF;
        constexpr int CLEAR = 0x00000000;
        extern SLIB_DLL Color convertTo(const Color& c, const subyte t);
        extern SLIB_DLL Color mix(const Color& c1, const Color& c2, const smath::Vector2D<float>& ratio = smath::Vector2D<float>(0.5f, 0.5f));
        extern SLIB_DLL Color mix(const Array<Color>& cols, const smath::Vector<float>& ratio = smath::Vector<float>());
    }
    class SLIB_DLL Color {
        friend SColor;
    private:
        subyte _type;
        subyte _bytes[4];

    public:
        Color();
        Color(const int col);
        Color(std::initializer_list<int> col);
        Color(const subyte type, const subyte *bytes);
        Color(const svec3b& col);
        Color(const svec4b& col);
        Color(const svec3f& col);
        Color(const svec4f& col);
        Color(const char* s);
        Color(const String& str);
        Color(const SObjPtr& obj);
        Color(const SElement& elem);
        Color(const Color& col);
        ~Color();
        Color& operator=(const Color& col);

        bool isMono() const;
        bool isRGB() const;
        //bool isFloat() const;
        bool isClear() const;
        bool hasAlpha() const;
        subyte type() const;
        int channel() const;
        int size() const;
        int depth() const;
        subyte* data();
        const subyte* data() const;
        void setType(const subyte t);
        int gray8() const;
        int gray16() const;
        int intValue() const;
        suint integer() const;
        svec3b toVec3i() const;
        svec4b toVec4i() const;
        svec3f toVec3f() const;
        svec4f toVec4f() const;
        int operator[](const int ch) const;
        void set(const int ch, const int val);
        String toString(const char* format = nullptr) const;
    };
#define GC_(X,Y) slib::Pair<float, Color>(X,Color(Y))
    class SLIB_DLL GradientColor {
        friend SColor;
    private:
        subyte _type;
        Array<Pair<float, Color>> _colors;
    public:
        GradientColor();
        GradientColor(const Color &c1, const Color &c2);
        GradientColor(const GradientColor& col);
        ~GradientColor();
        GradientColor& operator=(const GradientColor& col);

        size_t count() const;
        subyte type() const;
        bool hasAlpha() const;
        Array<Pair<float, Color>>& colors();
        const Array<Pair<float, Color>>& colors() const;
        String toString(const char* format = nullptr) const;
    };


    class SLIB_DLL SColor {
    protected:
        //subyte _mode;
        union color {
            Color uni;
            GradientColor grad;
            color();
            ~color();
        };
        void _copyColor(color& src, color& dest);
        color _color;

    public:
        SColor();
        SColor(const int col);
        SColor(const char* s);
        SColor(const Color& col);
        SColor(const GradientColor& gcol);
        SColor(const SColor& col);
        ~SColor();
        SColor& operator=(const SColor& col);

        subyte type() const;
        bool isGradient() const;
        bool hasAlpha() const;
        Color& color();
        const Color& color() const;
        GradientColor& grad();
        const GradientColor& grad() const;



        int operator[](const int ch) const;
        void set(const int ch, const int val);

        operator Color& ();
        operator const Color& () const;
        operator GradientColor& ();
        operator const GradientColor& () const;

        String toString(const char* format = nullptr) const;

    };

    extern SLIB_DLL const Map<String, sint> COLOR_MAP;
    
    class SLIB_DLL Stroke {
    public:
        sushort type;
        float width;
        SColor color;
        int interval[2];

    public:
        Stroke(const sushort t = sstyle::SOLID_LINE, float w = 1.0, const SColor& col = scolor::BLACK);
        Stroke(const Stroke& stroke);
        ~Stroke();
        Stroke& operator=(const Stroke& stroke);
    };


    /*
    class SLIB_DLL SColor {
    protected:
        subyte _type;
        ubytearray _data;

    public:
        SColor();
        SColor(const sint col);
        SColor(const subyte t, const sint c);
        SColor(const svec3b& col);
        SColor(const svec4b& col);
        SColor(const svec3f& col);
        SColor(const svec4f& col);
        SColor(int r, int g, int b, int a = 0xFF);
        SColor(const char* s);
        SColor(const SColor& col);
        virtual ~SColor();
        SColor& operator=(SColor&& col) noexcept;
        SColor& operator=(const SColor& col);


        subyte& byteAt(const int i);
        const subyte& byteAt(const int i) const;
        float& floatAt(const int i);
        const float& floatAt(const int i) const;
        subyte& operator[](const int i);
        const subyte& operator[](const int i) const;


        //sushort type() const;

        
        subyte channel() const;
        subyte size() const;
        
        sbyte bpp() const;
        const subyte* bytes() const;
        const float* floats() const;

        subyte gray8() const;
        sushort gray16() const;
        //sushort gray32() const;
        sint rgb() const;
        suint rgba() const;
        svec3b toVec3i() const;
        svec4b toVec4i() const;
        svec3f toVec3f() const;
        svec4f toVec4f() const;
        //subyte red() const;
        //subyte green() const;
        //subyte blue() const;
        subyte alpha() const;
        //subyte cyan() const;
        //subyte magenta() const;
        //subyte yellow() const;
        //subyte keyplate() const;
        bool isFloat() const;
        bool isClear() const;
        bool isMono() const;
        bool hasAlpha() const;
        bool isRGB() const;
        bool isCMYK() const;
        virtual bool isGradient() const;
        void convert(subyte t);

        String toString(const char* format = "name") const;

        bool operator<(const SColor& col) const;
        bool operator==(const SColor& col) const;
    };
    class SLIB_DLL SGradient : public SColor {
    protected:
        smath::AXIS _axis;
        float _angle;
        floatarray _points;

    public:
        SGradient();
        SGradient(const SGradient& grad);
        ~SGradient();

        //sgeom::COORDINATE coordinate() const;
        size_t count() const;
        float angle() const;
        SColor operator[](const size_t idx) const;
        SColor colorAt(const size_t idx) const;
        floatarray& points();
        const floatarray& points() const;

        //void setCoordinate(sgeom::COORDINATE coord);
        void setAngle(float f);
        void setPosition(size_t idx, float f);

        void addSColor(float f, const SColor& col);
        void setSColor(int idx, const SColor& col);
        void removeSColor(size_t idx);
        void clear();

        String toString() const;
    };
    */

    enum class ORIGIN_TYPE : subyte {
        ZERO = 0x00,
        TOP_LEFT = 0x11,
        BOTTOM_LEFT = 0x21,
        TOP_RIGHT = 0x12,
        BOTTOM_RIGHT = 0x22,
        CENTER = 0x33,
    };
    class SLIB_DLL Transform2D {
    public:
        smath::Vector2D<float> origin, scale, shift, shear;
        float rotation;
        bool reflect[2];

    public:
        Transform2D();
        ~Transform2D();
        void setOrigin(const Area<float>& area, ORIGIN_TYPE type);
        void transform(smath::Vector<smath::Vector2D<float>>& vertex);
        void reset();
    };

}
#endif