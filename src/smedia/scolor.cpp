#include "smedia/sgraphic.h"

using namespace slib;
using namespace slib::smedia;

inline float b2fcolor(subyte i) { return (float)i/smath::MAX_UBYTE; }
inline float s2fcolor(sushort i) { return (float)i/smath::MAX_USHORT; }
inline void i2fcolor(float *f, suint i) {
    f[3] = b2fcolor(i&0xFF);
    f[2] = b2fcolor((i>>8)&0xFF);
    f[1] = b2fcolor((i>>16)&0xFF);
    f[0] = b2fcolor((i>>24)&0xFF);
}
inline subyte f2bcolor(float f) { return f*smath::MAX_UBYTE; }
inline sushort f2scolor(float f) { return f*smath::MAX_USHORT; }
inline suint f2icolor(float *f) {
    suint i = f2bcolor(f[0]); i<<=8;
    i += f2bcolor(f[1]); i<<=8;
    i += f2bcolor(f[2]); i<<=8;
    i += f2bcolor(f[3]);
    return i;
}
inline void rgb2cmyk(float *rgb, float *cmyk) {}

const SColor SColor::CLEAR = (suint)0x00000000;
const SColor SColor::BLACK = (suint)0x000000FF;
const SColor SColor::WHITE = (suint)0xFFFFFFFF;
const SColor SColor::GRAY = (suint)0x808080FF;
const SColor SColor::LIGHTGRAY = (suint)0xD3D3D3FF;
const SColor SColor::DIMGRAY = (suint)0x696969FF;
const SColor SColor::RED = (suint)0xFF0000FF;
const SColor SColor::MAGENTA = (suint)0xFF00FFFF;
const SColor SColor::PINK = (suint)0xFFC0CBFF;
const SColor SColor::CRIMSON = (suint)0xDC143CFF;
const SColor SColor::DARKRED = (suint)0x8B0000FF;
const SColor SColor::ORANGE = (suint)0xFFA500FF;
const SColor SColor::YELLOW = (suint)0xFFFF00FF;
const SColor SColor::BROWN = (suint)0xA52A2AFF;
const SColor SColor::LIME = (suint)0x00FF00FF;
const SColor SColor::GREEN = (suint)0x008000FF;
const SColor SColor::DARKGREEN = (suint)0x006400FF;
const SColor SColor::SPRING = (suint)0x00FF7FFF;
const SColor SColor::BLUE = (suint)0x0000FFFF;
const SColor SColor::TURQUOISE = (suint)0x40E0D0FF;
const SColor SColor::CYAN = (suint)0x00FFFFFF;
const SColor SColor::DEEPSKY = (suint)0x00BFFFFF;
const SColor SColor::INDIGO = (suint)0x4B0082FF;
const SColor SColor::NAVY = (suint)0x000080FF;
const SColor SColor::VIOLET = (suint)0xEE82EEFF;
const SColor SColor::PURPLE = (suint)0x800080FF;

Map<String, suint> slib::smedia::SColorMap =
{
    kui("clear", 0x00000000), kui("black", 0x000000FF), kui("white", 0xFFFFFFFF),
    kui("gray", 0x808080FF), kui("lightgray", 0xd3d3d3FF), kui("dimgray", 0x696969FF),
    kui("red", 0xFF0000FF), kui("magenta", 0xFF00FFFF), kui("pink", 0xFFC0CBFF),
    kui("crimson", 0xDC143CFF), kui("darkred", 0x88B000FF),
    kui("orange", 0xFFA500FF), kui("yellow", 0xFFFF00FF), kui("brown", 0xA52A2AFF),
    kui("lime", 0x00FF00FF), kui("green", 0x008000FF), kui("darkgreen", 0x006400FF),
    kui("spring", 0x00FF7FFF), kui("blue", 0x0000FFFF), kui("turquoise", 0x40E0D0FF),
    kui("cyan", 0x00FFFFFF), kui("deepsky", 0x00BFFFFF), kui("indigo", 0x4B0082FF),
    kui("navy", 0x000080FF), kui("violet", 0xEE82EEFF), kui("purple", 0x800080FF)
};

SColor::SColor(sushort t) : _type(t) { _data.resize(channel(), 0.0f); }
SColor::SColor(subyte col) : _type(GRAY8) { _data.resize(1); _data[0] = b2fcolor(col); }
SColor::SColor(suint col) : _type(RGBA) {
    _data.resize(4); i2fcolor(_data.ptr(), col);
    
}
SColor::SColor(int r, int g, int b) : _type(RGBA) {
    _data.resize(4);
    _data[0] = (float)r/255.0f;
    _data[1] = (float)g/255.0f;
    _data[2] = (float)b/255.0f;
    _data[3] = 1.0f;
}
SColor::SColor(const char *s) : SColor() {
    if(s && s[0] == '\0') return;
    if (SColorMap.hasKey(s)) *this = SColorMap[s];
    else {
        String code(s);
        if (code[0] == '#') code.replace("#", "0x");
        if (!code.beginWith("0x")) code = "0x"+code;
        *this = code.number().uintValue();
    }
}
SColor::SColor(const SColor &col) : SColor() {
    _type = col._type; _data = col._data;
}
SColor::~SColor() {}

SColor &SColor::operator=(suint col) {
    _type = RGBA; _data.resize(4); i2fcolor(_data.ptr(), col); return *this;
}
SColor &SColor::operator=(const SColor &col) {
    _type = col._type; _data = col._data; return *this;
}
sushort SColor::type() const { return _type; }
int8_t SColor::channel() const { return (_type&0x00F0)>>4; }
int8_t SColor::bpp() const { return _type&0x000F; }
subyte SColor::gray8() const {
    if (channel() == 1) return f2bcolor(_data[0]);
    else return f2bcolor((_data[0]+_data[1]+_data[2])/3.0f);
}
sushort SColor::gray16() const {
    if (channel() == 1) return f2scolor(_data[0]);
    else return f2scolor((_data[0]+_data[1]+_data[2])/3.0f);
}
col3i SColor::rgb() const {
    col3i col;
    if (channel() == 1) { auto u8 = gray8(); col.reset(u8); }
    else {
        col.x = f2bcolor(_data[0]); col.y = f2bcolor(_data[1]); col.z = f2bcolor(_data[2]);
    }
    return col;
}
col4i SColor::rgba() const {
    col4i col;
    if (channel() == 1) { auto u8 = gray8(); col.reset(u8); col[3] = 1.0f; }
    else {
        col.x = f2bcolor(_data[0]); col.y = f2bcolor(_data[1]);
        col.z = f2bcolor(_data[2]); col.w = channel()==4?f2bcolor(_data[3]):255;
    }
    return col;
}
suint SColor::intColor() const {
    if (channel() == 1) {
        suint b = gray8();
        suint i = (255<<24)|(b<<16)|(b<<8)|b;
        return i;
    }
    else return f2icolor(_data.ptr());
}
subyte SColor::byteColor(int8_t ch) const {
    return f2bcolor(_data[ch]);
}
float SColor::floatColor(int8_t ch) const{
    return _data[ch];
}
float *SColor::ptr() const { return _data.ptr(); }
void SColor::convert(sushort t) {
    if(_type == t) return;
    /*
    floatarray tmp((t&0x00F0)>>4);
    
    if (c == 1) _data[0] = gray8();
    else {
        if (_channel == 1) {
            if (d == 1) _data[0] = gray8();
            else { auto tmp = gray16(); memcpy(&_data[0], &tmp, 2); }
        }
        else _data[3] = 0;
    }

    _type = t; tmp.swap(_data);
     */
}

bool SColor::isClear() const { return hasAlpha()&&_data[3]==0.0f; }
bool SColor::isMono() const { return channel()==1; }
bool SColor::hasAlpha() const { return (isRGB() && channel() == 4) || (isCMYK() && channel() == 5); }

bool SColor::isRGB() const { return _type&RGB_SPACE; }
bool SColor::isCMYK() const { return _type&CMYK_SPACE; }

String SColor::toHex(const char *prefix) const {
    String str(prefix);
    if (channel() == 1) {
        auto tmp = gray8();
        sforin(i, 0, 3) str<<String::HEX_STR[(tmp>>4)&0x0F]<<String::HEX_STR[tmp&0x0F];
    }
    else {
        sforin(i, 0, 3) {
            str<<String::HEX_STR[(byteColor(i)>>4)&0x0F]<<String::HEX_STR[byteColor(i)&0x0F];
        }
    }
    return str;
}
String SColor::toString() const {
    auto icol = intColor();
    auto str = SColorMap.rlookup(icol);
    if (str.size()) return str;
    str = "(";
    sforin(i, 0, channel()) str<<(int)byteColor(i)<<",";
    str.last() = ')';
    return str;
}

bool SColor::operator<(const SColor &col) const {
    return intColor() < col.intColor();
}
bool SColor::operator==(const SColor &col) const {
    return _type == col._type && intColor() == col.intColor();
}

SGradient::SGradient() : SColor(SColor::WHITE), _mode(sgeom::XYZ), _angle(0.0f) {
    _point.add(0.0f); addColor(1.0f, SColor::BLACK);
}
SGradient::SGradient(const SColor &col) : SColor(SColor::WHITE), _mode(sgeom::XYZ), _angle(0.0f) {
    _point.add(0.0f); addColor(1.0f, col);
}
SGradient::SGradient(const SGradient &grad) : _mode(grad._mode), _angle(grad._angle) {
    SColor::_type = grad._type; SColor::_data = grad._data; _point = grad._point;
}
SGradient::~SGradient() {}

smath::sgeom::COORDINATE SGradient::mode() const { return _mode; }
size_t SGradient::count() const { return _point.size(); }
float SGradient::angle() const { return _angle; }
SColor SGradient::operator[](size_t idx) const {
    SColor col(SColor::type());
    CMemory<float>::copy(col.ptr(), SColor::_data.ptr(idx*channel()), channel()*sizeof(float));
    return col;
}
SColor SGradient::colorAt(size_t idx) const {
    SColor col(SColor::type());
    CMemory<float>::copy(col.ptr(), SColor::_data.ptr(idx*channel()), channel()*sizeof(float));
    return col;
}
floatarray &SGradient::points() { return _point; }
const floatarray &SGradient::points() const { return _point; }

void SGradient::setMode(smath::sgeom::COORDINATE m) { _mode = m; }
void SGradient::setAngle(float f) { _angle = f; }
void SGradient::setPosition(size_t idx, float f) { _point[idx] = f; }
void SGradient::addColor(float f, const SColor &col) {
    size_t idx = 0;
    if (f < _point.first()) idx = 0;
    else if (_point.last() < f) idx= _point.size();
    else {
        auto it = _point.begin(), end = _point.end();
        while (it+1 < end) {
            if (*it < f && f < *(it+1)) {
                idx = it-_point.begin()+1; break;
            }
        }
    }
    if (col.type() != _type) {
        SColor c(col); c.convert(_type);
        _data.insert(channel()*idx, c.ptr(), channel());
    }
    else _data.insert(channel()*idx, col.ptr(), channel());
}
void SGradient::setColor(int idx, const SColor &col) {
    if (col.type() != _type) {
        SColor c(col); c.convert(_type);
        CMemory<float>::copy(_data.ptr(channel()*idx), c.ptr(), channel());
    }
    else CMemory<float>::copy(_data.ptr(channel()*idx), col.ptr(), channel());
}
void SGradient::removeColor(size_t idx) {
    _data.remove(channel()*idx, channel());
    _point.removeAt(idx);
}
void SGradient::clear() {
    _angle = 0.0f; _point.clear(); SColor::_data.clear();
}