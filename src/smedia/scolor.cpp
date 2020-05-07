#include "smedia/sgraphic.h"

using namespace slib;
using namespace slib::smedia;

const SColor SColor::CLEAR = (suint)0x00000000;
const SColor SColor::BLACK = (suint)0xFF000000;
const SColor SColor::WHITE = (suint)0xFFFFFFFF;
const SColor SColor::GRAY = (suint)0xFF808080;
const SColor SColor::LIGHTGRAY = (suint)0xFFD3D3D3;
const SColor SColor::DIMGRAY = (suint)0xFF696969;
const SColor SColor::RED = (suint)0xFF0000FF;
const SColor SColor::MAGENTA = (suint)0xFFFF00FF;
const SColor SColor::PINK = (suint)0xFFCBC0FF;
const SColor SColor::CRIMSON = (suint)0xFF143CDC;
const SColor SColor::DARKRED = (suint)0xFF00008B;
const SColor SColor::ORANGE = (suint)0xFF00A5FF;
const SColor SColor::YELLOW = (suint)0xFF00FFFF;
const SColor SColor::BROWN = (suint)0xFF2A2AA5;
const SColor SColor::LIME = (suint)0xFF00FF00;
const SColor SColor::GREEN = (suint)0xFF008000;
const SColor SColor::DARKGREEN = (suint)0xFF006400;
const SColor SColor::SPRING = (suint)0xFF7FFF00;
const SColor SColor::BLUE = (suint)0xFFFF0000;
const SColor SColor::TURQUOISE = (suint)0xFFD0E040;
const SColor SColor::CYAN = (suint)0xFFFFFF00;
const SColor SColor::DEEPSKY = (suint)0xFFFFBF00;
const SColor SColor::INDIGO = (suint)0xFF82004B;
const SColor SColor::NAVY = (suint)0xFF800000;
const SColor SColor::VIOLET = (suint)0xFFEE82EE;
const SColor SColor::PURPLE = (suint)0xFF800080;
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

SColor::SColor(sushort t) : _type(t), _mode(DEFAULT_NUMERIC), SObject() { _data.resize(bytePerPixel(_type), 0); }
SColor::SColor(sushort t, void* bytes) : SColor(t) {
	_data.copy(static_cast<subyte*>(bytes), _data.size());
}
SColor::SColor(subyte col) : SColor(GRAY8, &col) {}
SColor::SColor(suint col) : SColor(RGBA, &col) {}
SColor::SColor(const col3i& col) : SColor(RGB24) { _data[0] = col.x & 0xFF; _data[1] = col.y & 0xFF; _data[2] = col.z & 0xFF; }
SColor::SColor(const col4i& col) : SColor(RGBA) { _data[0] = col.x & 0xFF; _data[1] = col.y & 0xFF; _data[2] = col.z & 0xFF; _data[3] = col.w & 0xFF; }
SColor::SColor(const col3f& col) : SColor(RGBF) {
	CMemory<subyte>::copy(&_data[0], reinterpret_cast<const subyte*>(&col.x), sizeof(float));
	CMemory<subyte>::copy(&_data[4], reinterpret_cast<const subyte*>(&col.y), sizeof(float));
	CMemory<subyte>::copy(&_data[8], reinterpret_cast<const subyte*>(&col.z), sizeof(float));
}
SColor::SColor(const col4f& col) : SColor(RGBAF) {
	CMemory<subyte>::copy(&_data[0], reinterpret_cast<const subyte*>(&col.x), sizeof(float));
	CMemory<subyte>::copy(&_data[4], reinterpret_cast<const subyte*>(&col.y), sizeof(float));
	CMemory<subyte>::copy(&_data[8], reinterpret_cast<const subyte*>(&col.z), sizeof(float));
	CMemory<subyte>::copy(&_data[12], reinterpret_cast<const subyte*>(&col.w), sizeof(float));
}
SColor::SColor(int r, int g, int b, int a) : SColor(RGBA) {
	_data[0] = r & 0xFF; _data[1] = g & 0xFF; _data[2] = b & 0xFF; _data[3] = a & 0xFF;
}
SColor::SColor(const char *s) : SColor() {
	if (!s || s[0] == '\0') {
		_mode = DEFAULT_NUMERIC;
		return;
	}
	if (SColorMap.hasKey(s)) {
		*this = SColorMap[s];
		_mode = HTML_CODE;
	}
    else {
        String code(s);
		if (code[0] == '#') {
			_mode = HTML_HEX;
			code.replace("#", "0x");
			*this = code.number().uintValue();
		}
		else if (code.beginWith("0x")) {
			_mode = DEFAULT_HEX;
			*this = code.number().uintValue();
		}
		else if (code.beginWith("rgb(") && code.endWith(")")) {
			_mode = CSS_NUMERIC;
			code.clip(4, code.length() - 5);
			auto list = code.split(",");
			*this = col3i(list[0].ubyteValue(), list[1].ubyteValue(), list[2].ubyteValue());
		}
		else if (code.beginWith("rgba(") && code.endWith(")")) {
			_mode = CSS_NUMERIC;
			code.clip(5, code.length() - 6);
			auto list = code.split(",");
			*this = col4i(list[0].ubyteValue(), list[1].ubyteValue(), list[2].ubyteValue(), list[3].ubyteValue());
		}
		else if (code.beginWith("(") && code.endWith(")")) {
			_mode = DEFAULT_NUMERIC;
			code.transform(DELETE_QUOTE);
			auto list = code.split(",");
			if (list.size() == 3) *this = col3i(list[0].ubyteValue(), list[1].ubyteValue(), list[2].ubyteValue());
			else if (list.size() == 4) *this = col4i(list[0].ubyteValue(), list[1].ubyteValue(), list[2].ubyteValue(), list[3].ubyteValue());
		}
		else throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
    }
}
SColor::SColor(const SColor& col) : SColor() {
    _type = col._type; _data.copy(col._data.ptr(), col._data.size());
}
SColor::SColor(const sobj& obj) {
	if (obj.isNull()) *this = SColor();
	else if (obj.isStr()) *this = (const char*)obj;
	else if (obj.isNum()) *this = obj.uintValue();
	else if (obj.isColor()) *this = obj.color();
}
SColor::~SColor() {}
SColor& SColor::operator=(SColor&& col) {
	_type = col._type; col._data.moveTo(_data); return *this;
}
SColor &SColor::operator=(const SColor &col) {
    _type = col._type; _data.copy(col._data.ptr(), col._data.size()); return *this;
}
sushort SColor::type() const { return _type; }
sbyte SColor::channel() const { return colorChannel(_type); }
sbyte SColor::bpp() const { return bytePerPixel(_type); }
const subyte* SColor::bytes() const { return _data.ptr(); }
const float* SColor::floats() const { return reinterpret_cast<const float *>(_data.ptr()); }
subyte SColor::gray8() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) return f2bcolor(tmp[0]);
		else return f2bcolor(smath::sstat::average(tmp, 3));
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return _data[0];
			else return s2bcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
		}
		else return smath::sstat::average(_data.ptr(), 3);
	}
}
sushort SColor::gray16() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) return f2scolor(tmp[0]);
		else return f2scolor(smath::sstat::average(tmp, 3));
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return b2scolor(_data[0]);
			else return *reinterpret_cast<const sushort*>(_data.ptr());
		}
		else return b2scolor(smath::sstat::average(_data.ptr(), 3));
	}
}
float SColor::grayf() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) return tmp[0];
		else return smath::sstat::average(tmp, 3);
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return b2fcolor(_data[0]);
			else return s2fcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
		}
		else return b2fcolor(smath::sstat::average(_data.ptr(), 3));
	}
}
suint SColor::rgb() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	suint col = 0xFF000000;
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) {
			auto i = f2icolor(tmp[0]);
			col |= i | (i << 8) | (i << 16);
		}
		else col |= f2icolor(tmp[2]) | (f2icolor(tmp[1]) << 8) | (f2icolor(tmp[0]) << 16);
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) col |= _data[0] | (_data[0] << 8) | (_data[0] << 16);
			else {
				int i = s2bcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
				col |= i | (i << 8) | (i << 16);
			}
		}
		else CMemory<subyte>::copy(reinterpret_cast<subyte *>(&col), _data.ptr(), 3);
	}
	return col;
}
suint SColor::rgba() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	suint col = hasAlpha()?(alpha()<<24):0;
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) {
			auto i = f2icolor(tmp[0]);
			col |= i | (i << 8) | (i << 16);
		}
		else col |= f2icolor(tmp[2]) | (f2icolor(tmp[1]) << 8) | (f2icolor(tmp[0]) << 16);
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) col |= _data[0] | (_data[0] << 8) | (_data[0] << 16);
			else {
				int i = s2bcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
				col |= i | (i << 8) | (i << 16);
			}
		}
		else CMemory<subyte>::copy(reinterpret_cast<subyte*>(&col), _data.ptr(), 3);
	}
	return col;
}
col3i SColor::toVec3i() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	col3i col;
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) col = col3i(f2bcolor(tmp[0]));
		else col = col3i(f2bcolor(tmp[0]), f2bcolor(tmp[1]), f2bcolor(tmp[2]));
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) col = col3i(_data[0]);
			else col = col3i(s2bcolor(*reinterpret_cast<const sushort*>(_data.ptr())));
		}
		else col = col3i(_data[0], _data[1], _data[2]);
	}
	return col;
}
col4i SColor::toVec4i() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	col4i col;
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) col = col4i(f2bcolor(tmp[0]));
		else col = col4i(f2bcolor(tmp[0]), f2bcolor(tmp[1]), f2bcolor(tmp[2]), 255);
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) col = col4i(_data[0]);
			else col = col4i(s2bcolor(*reinterpret_cast<const sushort*>(_data.ptr())));
		}
		else col = col4i(_data[0], _data[1], _data[2], 255);
	}
	if (hasAlpha()) col.w = alpha();
	return col;
}
col3f SColor::toVec3f() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	col3f col;
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) col = col3f(tmp[0]);
		else col = col3f(tmp[0], tmp[1], tmp[2]);
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) col = col3f(b2fcolor(_data[0]));
			else col = col3f(s2fcolor(*reinterpret_cast<const sushort*>(_data.ptr())));
		}
		else col = col3f(b2fcolor(_data[0]), b2fcolor(_data[1]), b2fcolor(_data[2]));
	}
	return col;
}
col4f SColor::toVec4f() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	col4f col;
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) col = col4f(tmp[0]);
		else col = col4f(tmp[0], tmp[1], tmp[2], 0.0f);
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) col = col4f(b2fcolor(_data[0]));
			else col = col4f(s2fcolor(*reinterpret_cast<const sushort*>(_data.ptr())));
		}
		else col = col4f(b2fcolor(_data[0]), b2fcolor(_data[1]), b2fcolor(_data[2]), 1.0f);
	}
	if(hasAlpha()) col.w = alphaf();
	return col;
}
subyte SColor::red() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		if (channel() == 1) return f2bcolor(*reinterpret_cast<const float*>(_data.ptr()));
		else return f2bcolor(reinterpret_cast<const float*>(_data.ptr())[0]);
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return _data[0];
			else return s2bcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
		}
		else return _data[0];
	}
}
subyte SColor::green() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		if (channel() == 1) return f2bcolor(*reinterpret_cast<const float*>(_data.ptr()));
		else return f2bcolor(reinterpret_cast<const float*>(_data.ptr())[1]);
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return _data[0];
			else return s2bcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
		}
		else return _data[1];
	}
}
subyte SColor::blue() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		if (channel() == 1) return f2bcolor(*reinterpret_cast<const float*>(_data.ptr()));
		else return f2bcolor(reinterpret_cast<const float*>(_data.ptr())[2]);
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return _data[0];
			else return s2bcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
		}
		else return _data[2];
	}
}
subyte SColor::alpha() const {
	if (!isRGB() || !hasAlpha()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) return f2bcolor(reinterpret_cast<const float*>(_data.ptr())[3]);
	else return _data[3];
}
float SColor::redf() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		if (channel() == 1) return *reinterpret_cast<const float*>(_data.ptr());
		else return reinterpret_cast<const float*>(_data.ptr())[0];
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return b2fcolor(_data[0]);
			else return s2fcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
		}
		else return b2fcolor(_data[0]);
	}
}
float SColor::greenf() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		if (channel() == 1) return *reinterpret_cast<const float*>(_data.ptr());
		else return reinterpret_cast<const float*>(_data.ptr())[1];
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return b2fcolor(_data[0]);
			else return s2fcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
		}
		else return b2fcolor(_data[1]);
	}
}
float SColor::bluef() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		if (channel() == 1) return *reinterpret_cast<const float*>(_data.ptr());
		else return reinterpret_cast<const float*>(_data.ptr())[2];
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return b2fcolor(_data[0]);
			else return s2fcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
		}
		else return b2fcolor(_data[2]);
	}
}
float SColor::alphaf() const {
	if (!isRGB() || !hasAlpha()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) return reinterpret_cast<const float*>(_data.ptr())[3];
	else return b2fcolor(_data[3]);
}
bool SColor::isInt() const { return colorDepth(_type) < 4; }
bool SColor::isFloat() const { return colorDepth(_type) == 4; }
bool SColor::isClear() const { return hasAlpha()&& alpha()==0.0f; }
bool SColor::isMono() const { return channel() == 1; }
bool SColor::hasAlpha() const { return isRGB() && (channel() == 4); }
bool SColor::isRGB() const { return colorSpace(_type) == RGB_SPACE; }
bool SColor::isCMYK() const { return colorSpace(_type) == CMYK_SPACE; }
void SColor::convert(sushort t) {
	if (_type == t) return;
	if (isRGB()) {
		switch (t)
		{
		case GRAY8:
			_data.resize(1);
			_data[0] = gray8();
			break;
		case GRAY16:
		{
			_data.resize(2);
			auto g16 = gray16();
			CMemory<subyte>::copy(_data.ptr(), reinterpret_cast<subyte*>(&g16), 2);
			break;
		}
		case GRAYF:
		{
			_data.resize(4);
			auto gf = grayf();
			CMemory<subyte>::copy(_data.ptr(), reinterpret_cast<subyte*>(&gf), 4);
			break;
		}
		case RGB24:
		{
			_data.resize(3);
			auto ui3 = rgb();
			CMemory<subyte>::copy(_data.ptr(), reinterpret_cast<subyte*>(&ui3), 3);
			break;
		}
		case RGB32:
		{
			_data.resize(4);
			auto ui3 = rgb();
			CMemory<subyte>::copy(_data.ptr(), reinterpret_cast<subyte*>(&ui3), 3);
			break;
		}
		case RGBF:
		{
			_data.resize(12);
			auto cf3 = toVec3f();
			CMemory<subyte>::copy(&_data[0], reinterpret_cast<subyte*>(&cf3.x), 4);
			CMemory<subyte>::copy(&_data[4], reinterpret_cast<subyte*>(&cf3.y), 4);
			CMemory<subyte>::copy(&_data[8], reinterpret_cast<subyte*>(&cf3.z), 4);
			break;
		}
		case RGBA:
		{
			_data.resize(4);
			auto ui4 = rgb();
			CMemory<subyte>::copy(_data.ptr(), reinterpret_cast<subyte*>(&ui4), 4);
			break;
		}
		case RGBAF:
		{
			_data.resize(16);
			auto cf4 = toVec4f();
			CMemory<subyte>::copy(&_data[0], reinterpret_cast<subyte*>(&cf4.x), 4);
			CMemory<subyte>::copy(&_data[4], reinterpret_cast<subyte*>(&cf4.y), 4);
			CMemory<subyte>::copy(&_data[8], reinterpret_cast<subyte*>(&cf4.z), 4);
			CMemory<subyte>::copy(&_data[12], reinterpret_cast<subyte*>(&cf4.z), 4);
			break;
		}
		case CMYK:
			
			break;
		case CMYKF:
			
			break;
		default:
			break;
		}
	}
	_type = t;
}
void SColor::setMode(COLOR_TEXT_MODE m) { _mode = m; }
String SColor::getClass() const { return "color"; }
String SColor::toString() const {
	String str;
	switch (_mode)
	{
	case DEFAULT_NUMERIC:
	{
		if (isInt()) {
			str << "(" << red() << "," << green() << "," << blue();
			if (hasAlpha()) str << "," << alpha();
			str << ")";
		}
		else {
			str << "(" << redf() << "," << greenf() << "," << bluef();
			if (hasAlpha()) str << "," << alphaf();
			str << ")";
		}
		break;
	}
	case DEFAULT_HEX:
	{
		str << "0x" << SNumber::toHex(red()) << SNumber::toHex(green()) << SNumber::toHex(blue());
		if (hasAlpha()) str << SNumber::toHex(alpha());
		break;
	}
	case HTML_CODE:
	{
		try { str = SColorMap.rlookup(rgba()); }
		catch (SException ex) { str.clear(); }
	}
	case HTML_HEX:
	{
		if (!str.empty()) break;
		str << "#" << SNumber::toHex(red()) << SNumber::toHex(green()) << SNumber::toHex(blue());
		break;
	}
	case CSS_NUMERIC:
	{
		if (hasAlpha()) {
			auto tmp = toVec4i();
			str << "rgba(" << tmp.x << "," << tmp.y << "," << tmp.z << "," << tmp.w << ")";
		}
		else {
			auto tmp = toVec3i();
			str << "rgb(" << tmp.x << "," << tmp.y << "," << tmp.z << ")";
		}
		break;
	}
	default:
		break;
	}
    return str;
}
SObject* SColor::clone() const { return new SColor(*this); }
bool SColor::operator<(const SColor &col) const {
    return _data < col._data;
}
bool SColor::operator==(const SColor &col) const {
    return _type == col._type && _data == col._data;
}

SGradient::SGradient() : _coord(sgeom::XYZ), _angle(0.0f), SColor() {
	addColor(0.0f, SColor::WHITE); addColor(1.0f, SColor::BLACK);
}
SGradient::SGradient(const SGradient &grad) : _coord(grad._coord), _angle(grad._angle) {
    SColor::_type = grad._type; grad._data.copyTo(SColor::_data);  grad._points.copyTo(_points);
}
SGradient::~SGradient() {}
smath::sgeom::COORDINATE SGradient::coordinate() const { return _coord; }
size_t SGradient::count() const { return _points.size(); }
float SGradient::angle() const { return _angle; }
SColor SGradient::operator[](size_t idx) const { return colorAt(idx); }
SColor SGradient::colorAt(size_t idx) const {
	return SColor(type(), SColor::_data.ptr(idx * bpp()));
}
floatarray &SGradient::points() { return _points; }
const floatarray &SGradient::points() const { return _points; }
void SGradient::setCoordinate(smath::sgeom::COORDINATE coord) { _coord = coord; }
void SGradient::setAngle(float f) { _angle = f; }
void SGradient::setPosition(size_t idx, float f) { _points[idx] = f; }
void SGradient::addColor(float f, const SColor &col) {

	if (_type != col.type()) {
		SColor tmp = col; tmp.convert(_type);
		if (_points.last() < f) {
			_points.add(f);
			_data.resize(_data.size() + bpp());
			CMemory<subyte>::copy(_data.ptr(_data.size() - bpp()), tmp.bytes(), bpp());
		}
		else {
			size_t idx = 0;
			auto it = _points.begin(), end = _points.end();
			while (it + 1 < end) {
				if (E_ < f && f < E_NXT) {
					idx = it - _points.begin() + 1; break;
				}
			}
			_points.insert(idx, f);
			_data.insert(idx, const_cast<subyte*>(tmp.bytes()), (size_t)bpp());
		};

	}
	else {
		if (_points.last() < f) {
			_points.add(f);
			_data.resize(_data.size() + bpp());
			CMemory<subyte>::copy(_data.ptr(_data.size()-bpp()), col.bytes(), bpp());
		}
		else {
			size_t idx = 0;
			auto it = _points.begin(), end = _points.end();
			while (it + 1 < end) {
				if (E_ < f && f < E_NXT) {
					idx = it - _points.begin() + 1; break;
				}
			}
			_points.insert(idx, f);
			_data.insert(idx, const_cast<subyte *>(col.bytes()), (size_t)bpp());
		}
	}
}
void SGradient::setColor(int idx, const SColor &col) {
	if (_type != col.type()) {
		SColor tmp = col; tmp.convert(_type);
		CMemory<subyte>::copy(_data.ptr(bpp() * idx), tmp.bytes(), bpp());
	}
	else CMemory<subyte>::copy(_data.ptr(bpp() * idx), col.bytes(), bpp());
}
void SGradient::removeColor(size_t idx) {
    _data.remove(bpp()*idx, bpp());
    _points.removeAt(idx);
}
void SGradient::clear() {
    _angle = 0.0f; _points.clear(); SColor::_data.clear();
}
String SGradient::getClass() const { return _coord == smath::sgeom::XYZ ? "lgrad" : "rgrad"; }
String SGradient::toString() const {
	String str;


	return str;
}
SObject* SGradient::clone() const { return new SGradient(*this); }