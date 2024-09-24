#include "sobj/sobjptr.h"
#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "smedia/sgraphic.h"

slib::COLOR_SPACE slib::scolor::space(slib::subyte type) { return (COLOR_SPACE)((type >> 4) & 0x07); }
bool slib::scolor::isFloat(slib::subyte type) { return type & 0x80; }
bool slib::scolor::hasAlpha(slib::subyte type) { return type & 0x08; }
slib::subyte slib::scolor::channel(slib::subyte type) { 
	auto cs = space(type); 
	return (cs == COLOR_SPACE::GRAY ? 1 : (cs == COLOR_SPACE::CMYK ? 4 : 3)) + (hasAlpha(type) ? 1 : 0); 
}
slib::subyte slib::scolor::depth(slib::subyte type) { return type & 0x07; }
slib::subyte slib::scolor::size(slib::subyte type) { return depth(type) * channel(type); }

const slib::Map<slib::String, slib::sint> slib::COLOR_MAP = {
	slib::Pair<slib::String, slib::sint>("black", slib::scolor::BLACK),
	slib::Pair<slib::String, slib::sint>("white", slib::scolor::WHITE),
	slib::Pair<slib::String, slib::sint>("gray", slib::scolor::GRAY),
	slib::Pair<slib::String, slib::sint>("lightgray", slib::scolor::LIGHTGRAY),
	slib::Pair<slib::String, slib::sint>("dimgray", slib::scolor::DIMGRAY),
	slib::Pair<slib::String, slib::sint>("red", slib::scolor::RED),
	slib::Pair<slib::String, slib::sint>("magenta", slib::scolor::MAGENTA),
	slib::Pair<slib::String, slib::sint>("pink", slib::scolor::PINK),
	slib::Pair<slib::String, slib::sint>("crimson", slib::scolor::CRIMSON),
	slib::Pair<slib::String, slib::sint>("darkred", slib::scolor::DARKRED),
	slib::Pair<slib::String, slib::sint>("orange", slib::scolor::ORANGE),
	slib::Pair<slib::String, slib::sint>("yellow", slib::scolor::YELLOW),
	slib::Pair<slib::String, slib::sint>("brown", slib::scolor::BROWN),
	slib::Pair<slib::String, slib::sint>("lime", slib::scolor::LIME),
	slib::Pair<slib::String, slib::sint>("green", slib::scolor::GREEN),
	slib::Pair<slib::String, slib::sint>("darkgreen", slib::scolor::DARKGREEN),
	slib::Pair<slib::String, slib::sint>("spring", slib::scolor::SPRING),
	slib::Pair<slib::String, slib::sint>("blue", slib::scolor::BLUE),
	slib::Pair<slib::String, slib::sint>("turquoise", slib::scolor::TURQUOISE),
	slib::Pair<slib::String, slib::sint>("cyan", slib::scolor::CYAN),
	slib::Pair<slib::String, slib::sint>("deepsky", slib::scolor::DEEPSKY),
	slib::Pair<slib::String, slib::sint>("indigo", slib::scolor::INDIGO),
	slib::Pair<slib::String, slib::sint>("navy", slib::scolor::NAVY),
	slib::Pair<slib::String, slib::sint>("violet", slib::scolor::VIOLET),
	slib::Pair<slib::String, slib::sint>("purple", slib::scolor::PURPLE)
};

slib::Color slib::scolor::convertTo(const Color& c, const subyte t) {
	subyte vals[4];



	return slib::Color(t, vals);
}
slib::Color slib::scolor::mix(const Color& c1, const Color& c2, const smath::Vector2D<float>& ratio) {
	auto t = c1.type();
	auto v1 = c1.toVec4f(), v2 = c2.toVec4f();
	return scolor::convertTo(slib::Color((v1 * ratio[0] + v2 * ratio[1]) / (ratio[0] + ratio[1])), t);
}
slib::Color slib::scolor::mix(const Array<Color>& cols, const smath::Vector<float>& ratio) {
	if (cols.empty()) throw NullException(nullErrorText("color list"));
	auto t = cols[0].type();
	slib::smath::Vector4D<float> vec;
	smath::Vector<float> _ratio(ratio);
	if (ratio.size() < cols.size()) {
		auto sum = sstat::sum(ratio);
		float f = sum < 1.f ? (1.f - sum) / (cols.size() - ratio.size()) : 0.f;
		_ratio.resize(cols.size(), f);
	}
	sfor2(cols, _ratio) vec += ($_1.toVec4f() * $_2);
	return scolor::convertTo(Color(vec / sstat::sum(ratio)), t);
}

slib::Color::Color() {
	_type = scolor::GRAY8;
	memset(_bytes, 0, 4);
}
slib::Color::Color(const int col) : Color() {
	_type = scolor::RGBA;
	_bytes[3] = col & 0xFF; _bytes[2] = (col >> 8) & 0xFF; _bytes[1] = (col >> 16) & 0xFF; _bytes[0] = (col >> 24) & 0xFF;
}
slib::Color::Color(std::initializer_list<int> col) : Color() {
	if (col.size() == 1) {
		auto c = *col.begin();
		if (c < 0 || snum::MAX16B < c) throw RangeException(outRangeErrorText("color value", c, 0, snum::MAX16B));
		if (c < 256) {
			_type = scolor::GRAY8;
			_bytes[0] = c;
		}
		else {
			_type = scolor::GRAY16;
			memcpy(_bytes, reinterpret_cast<subyte*>(&c), 2);
		}
	}
	else if (col.size() == 3) {
		_type = scolor::RGB;
		auto it = col.begin();
		sforin(i, 0, 3) {
			_bytes[i] = $_; $NEXT;
		} 
	}
	else if (col.size() == 4) {
		_type = scolor::RGBA;
		auto it = col.begin();
		sforin(i, 0, 4) {
			_bytes[i] = $_; $NEXT;
		}
	}
}
slib::Color::Color(const subyte type, const subyte* bytes) : Color() {
	_type = type;
	if (bytes) memcpy(_bytes, bytes, scolor::size(type));
}
slib::Color::Color(const svec3b& col) : Color() {
	_type = scolor::RGB;
	sforin(i, 0, 3) _bytes[i] = col[i];
}
slib::Color::Color(const svec4b& col) : Color() {
	_type = scolor::RGBA;
	sforin(i, 0, 4) _bytes[i] = col[i];
}
slib::Color::Color(const svec3f& col) : Color() {
	_type = scolor::RGB;
	sforin(i, 0, 3) _bytes[i] = (subyte)(col[i] * 255.f);
}
slib::Color::Color(const svec4f& col) : Color() {
	_type = scolor::RGBA;
	sforin(i, 0, 4) _bytes[i] = (subyte)(col[i] * 255.f);
}
slib::Color::Color(const char* s) : Color() {
	if (!s || s[0] == '\0') return;
	String code = sstr::trim(s);
	if (COLOR_MAP.hasKey(code)) *this = COLOR_MAP[code];
	else {
		if (code[0] == '#') {
			code.replace("#", "0x");
			*this = code.intValue();
		}
		else if (code.beginWith("0x")) {
			*this = code.intValue();
		}
		else if (code.beginWith("rgb(") && code.endWith(")")) {
			code.clip(4, code.length() - 5);
			auto list = code.split(",");
			*this = svec3b(list[0].ubyteValue(), list[1].ubyteValue(), list[2].ubyteValue());
		}
		else if (code.beginWith("rgba(") && code.endWith(")")) {
			code.clip(5, code.length() - 6);
			auto list = code.split(",");
			*this = svec4b(list[0].ubyteValue(), list[1].ubyteValue(), list[2].ubyteValue(), list[3].ubyteValue());
		}
		throw FormatException(formatErrorText("Color name/code", s, "either " + slib::toString(slib::COLOR_MAP.keyset()) + ", 0xXXXXX <hex value>, or rgb(X,X,X) <css format>."));
	}
}
slib::Color::Color(const slib::String& str) : Color(str.cstr()) {}
slib::Color::Color(const slib::SObjPtr& obj) : Color(0) {
	if (obj.isNum()) (*this) = obj.intValue();
	else if (obj.isStr()) (*this) = (const char*)obj;
}
slib::Color::Color(const slib::SElement& elem) : Color(0) {
	(*this) = elem.object();
}
slib::Color::Color(const Color& col) {
	_type = col._type;
	memcpy(_bytes, col._bytes, 4);
}
slib::Color::~Color() {}
slib::Color& slib::Color::operator=(const Color& col) {
	_type = col._type;
	memcpy(_bytes, col._bytes, 4);
	return *this;
}
bool slib::Color::isMono() const { return _type & 0x10; }
bool slib::Color::isRGB() const { return _type & 0x20; }
bool slib::Color::isClear() const { return hasAlpha() && _bytes[3] == 0; }
bool slib::Color::hasAlpha() const { return _type & 0x80; }
slib::subyte slib::Color::type() const { return _type; }
int slib::Color::channel() const { return scolor::channel(_type); }
int slib::Color::size() const { return scolor::size(_type); }
int slib::Color::depth() const { return scolor::depth(_type); }
slib::subyte* slib::Color::data() { return _bytes; }
const slib::subyte* slib::Color::data() const { return _bytes; }
void slib::Color::setType(const subyte t) {
	if (_type == t) return;
	auto dp = scolor::depth(t);
	auto a = scolor::hasAlpha(t);
	auto f = scolor::isFloat(t);
	//
	if (t & 0x10) {
		switch (dp) {
		case 2:
		{
			sushort c = gray16();
			memcpy(_bytes, &c, 2);
			break;
		}
		default:
			if (f) {
				float v = (float)gray8();
				v /= (float)snum::MAX8B;
				memcpy(_bytes, &v, 4);
			}
			else _bytes[0] = gray8();
			break;
		}
	}
	else if (t & 0x20) {
		if (isMono()) {
			subyte v = gray8();
			if (f) {
				float v_ = (float)v / (float)snum::MAX8B,
					* bytes_ = reinterpret_cast<float*>(_bytes);
				sforin(i, 0, 3) bytes_[i] = v_;
			}
			else {
				sforin(i, 0, 3) _bytes[i] = v;
				if (a) _bytes[3] = 0xFF;
			}
		}
		else if (isRGB()) {



		}
	}
	_type = t;
}
int slib::Color::gray8() const { 
	if (isMono()) {
		auto dp = depth();
		switch (dp) {
		case 2:
		{
			auto b = (suinteger)(reinterpret_cast<const sushort*>(_bytes)[0]) * snum::MAX8B / snum::MAX16B;
			return (int)(b & 0xFF);
		}
		case 4:
		{
			auto b = (suinteger)(reinterpret_cast<const suint*>(_bytes)[0]) * snum::MAX8B / snum::MAX32B;
			return (int)(b & 0xFF);
		}
		default:
			return (int)_bytes[0];
		}
	}
	else if (isRGB()) {
		auto sum = (int)_bytes[0] + (int)_bytes[1] + (int)_bytes[2];
		if (hasAlpha()) sum = (int)((float)sum * (float)_bytes[3] / 255.0f);
		return sum / 3;
	}
	else return (int)_bytes[3];
}
int slib::Color::gray16() const { 
	if (isMono()) {
		auto dp = depth();
		switch (dp) {
		case 2:
		{
			return (int)reinterpret_cast<const sushort*>(_bytes)[0];
		}
		case 4:
		{
			auto b = (suinteger)(reinterpret_cast<const suint*>(_bytes)[0]) * snum::MAX16B / snum::MAX32B;
			return (int)(b & 0xFFFF);
		}
		default:
			return (int)_bytes[0];
		}
	}
	else return (int)gray8() * snum::MAX16B / snum::MAX8B;
}
int slib::Color::intValue() const { return *reinterpret_cast<const int*>(_bytes); }
slib::suint slib::Color::integer() const { return *reinterpret_cast<const suint*>(_bytes); }
slib::svec3b slib::Color::toVec3i() const { return svec3b(_bytes[0], _bytes[1], _bytes[2]); }
slib::svec4b slib::Color::toVec4i() const { return svec4b(_bytes[0], _bytes[1], _bytes[2], _bytes[3]); }
slib::svec3f slib::Color::toVec3f() const { return svec3f((float)_bytes[0], (float)_bytes[1], (float)_bytes[2]) / 255.f; }
slib::svec4f slib::Color::toVec4f() const { return svec4f((float)_bytes[0], (float)_bytes[1], (float)_bytes[2], (float)_bytes[3]) / 255.f; }
int slib::Color::operator[](const int ch) const { 
	if (isMono()) {
		if (ch < 0 || 1 <= ch) throw RangeException(outRangeErrorText("Channel", ch, 0, 0));
		auto dp = depth();
		switch (dp) {
		case 2:
		{
			return (int )*reinterpret_cast<const sushort *>(_bytes);
		}
		case 4:
		{
			return (int)*reinterpret_cast<const sint*>(_bytes);
		}
		default:
			return (int)_bytes[0];
		}
	}
	else {
		if (ch < 0 || 4 <= ch) throw RangeException(outRangeErrorText("Channel", ch, 0, 4));
		return (int)_bytes[ch];
	}
}
void slib::Color::set(const int ch, const int val) {
	if (isMono()) {
		if (ch < 0 || 1 <= ch) throw RangeException(outRangeErrorText("Channel", ch, 0, 0));
		auto dp = depth();
		switch (dp) {
		case 2:
		{
			sushort s = val & 0xFFFF;
			memcpy(_bytes, &s, 2);
			break;
		}
		case 4:
		{
			memcpy(_bytes, &val, 4);
			break;
		}
		default:
			_bytes[0] = (subyte)(val & 0xFF);
			break;
		}
	}
	else {
		if (ch < 0 || 4 <= ch) throw RangeException(outRangeErrorText("Channel", ch, 0, 4));
		_bytes[ch] = (subyte)(val & 0xFF);
	}
}
slib::String slib::Color::toString(const char* format) const {
	slib::String str;
	if (format) {
		String f(format);
		if (f == "html") {
			str << "#" << numToString(_bytes[0], "hex") << numToString(_bytes[1], "hex") << numToString(_bytes[2], "hex");
		}

	}
	else {
		str = COLOR_MAP.rlookup(intValue());
		if (str.empty()) 
			str << "0x" << numToString(_bytes[0], "hex") << numToString(_bytes[1], "hex") << numToString(_bytes[2], "hex") << numToString(_bytes[3], "hex");
	}
	return str;
}
slib::GradientColor::GradientColor() : _type(scolor::LINEAR_GRAD) {
	_colors.add(GC_(0.f, scolor::BLACK));
	_colors.add(GC_(1.f, scolor::WHITE));
}
slib::GradientColor::GradientColor(const Color& c1, const Color& c2) : GradientColor() {
	_colors.add(GC_(0.f, c1));
	_colors.add(GC_(1.f, c2));
}
slib::GradientColor::GradientColor(const GradientColor& col) {
	_type = col._type;
	_colors = col._colors;
}
slib::GradientColor::~GradientColor() {}
slib::GradientColor& slib::GradientColor::operator=(const GradientColor& col) {
	_type = col._type;
	_colors = col._colors;
	return *this;
}
size_t slib::GradientColor::count() const { return _colors.size(); }
slib::subyte slib::GradientColor::type() const { return _type; }
bool slib::GradientColor::hasAlpha() const { return false; }
slib::Array<slib::Pair<float, slib::Color>>& slib::GradientColor::colors() { return _colors; }
const slib::Array<slib::Pair<float, slib::Color>>& slib::GradientColor::colors() const { return _colors; }
slib::String slib::GradientColor::toString(const char* format) const {

	return "";
}
slib::SColor::color::color() { uni = Color(); }
slib::SColor::color::~color() {}
slib::SColor::SColor() { _color.uni = scolor::WHITE; }
slib::SColor::SColor(const int col) : SColor(Color(col)) {}
slib::SColor::SColor(const char* s) : SColor(Color(s)) {}
slib::SColor::SColor(const Color& col) { _color.uni = col; }
slib::SColor::SColor(const GradientColor& gcol) { _color.grad = gcol; }
slib::SColor::SColor(const SColor& col) {
	if (col.isGradient()) _color.grad = col._color.grad;
	else _color.uni = col._color.uni;
}
slib::SColor::~SColor() {}
slib::SColor& slib::SColor::operator=(const SColor& col) {
	if (col.isGradient()) _color.grad = col._color.grad;
	else _color.uni = col._color.uni;
	return *this;
}
slib::subyte slib::SColor::type() const { return (isGradient() ? _color.grad.type() : _color.uni.type()); }
bool slib::SColor::isGradient() const { return (reinterpret_cast<const subyte*>(&_color)[0] & 0x0F) == 0; }
bool slib::SColor::hasAlpha() const { return (isGradient() ? _color.grad.hasAlpha() : _color.uni.hasAlpha()); }
slib::Color& slib::SColor::color() { return _color.uni; }
const slib::Color& slib::SColor::color() const { return _color.uni; }
slib::GradientColor& slib::SColor::grad() { return _color.grad; }
const slib::GradientColor& slib::SColor::grad() const { return _color.grad; }

int slib::SColor::operator[](const int ch) const {

	return 0;
}
void slib::SColor::set(const int ch, const int val) {


}

slib::SColor::operator slib::Color& () {
	if (isGradient()) return _color.grad._colors[0].second;
	return _color.uni; 
}
slib::SColor::operator const slib::Color& () const {
	if (isGradient()) return _color.grad._colors[0].second;
	return _color.uni;
}
slib::SColor::operator slib::GradientColor& () {
	if (isGradient()) return _color.grad;
	throw CastException(castErrorText("Color", "GradientColor"));
}
slib::SColor::operator const slib::GradientColor& () const {
	if (isGradient()) return _color.grad;
	throw CastException(castErrorText("Color", "GradientColor"));
}
slib::String slib::SColor::toString(const char* format) const {
	return isGradient() ? _color.grad.toString(format) : _color.uni.toString(format);
}
/*
slib::SColor::SColor() : _mode(COLOR_MODE::UNITARY), _color() {}
slib::SColor::SColor(const int col) : _mode(COLOR_MODE::UNITARY), _color() { _color.mono = col; }

slib::SColor::SColor(const svec3b& col) : slib::SColor(slib::scolor::RGB) {
	_data[0] = col[0] & 0xFF; _data[1] = col[1] & 0xFF; _data[2] = col[2] & 0xFF; 
}
slib::SColor::SColor(const svec4b& col) : slib::SColor(slib::scolor::RGBA) {
	_data[0] = col[0] & 0xFF; _data[1] = col[1] & 0xFF; _data[2] = col[2] & 0xFF; _data[3] = col[3] & 0xFF; 
}
slib::SColor::SColor(const svec3f& col) : slib::SColor(slib::scolor::RGBF) {
	Memory<subyte>::copy(&_data[0], reinterpret_cast<const subyte*>(&col[0]), sizeof(float));
	Memory<subyte>::copy(&_data[4], reinterpret_cast<const subyte*>(&col[1]), sizeof(float));
	Memory<subyte>::copy(&_data[8], reinterpret_cast<const subyte*>(&col[2]), sizeof(float));
}
slib::SColor::SColor(const svec4f& col) : slib::SColor(slib::scolor::RGBAF) {
	Memory<subyte>::copy(&_data[0], reinterpret_cast<const subyte*>(&col[0]), sizeof(float));
	Memory<subyte>::copy(&_data[4], reinterpret_cast<const subyte*>(&col[1]), sizeof(float));
	Memory<subyte>::copy(&_data[8], reinterpret_cast<const subyte*>(&col[2]), sizeof(float));
	Memory<subyte>::copy(&_data[12], reinterpret_cast<const subyte*>(&col[3]), sizeof(float));
}
slib::SColor::SColor(int r, int g, int b, int a) : SColor(slib::scolor::RGBA) {
	_data[0] = r & 0xFF; _data[1] = g & 0xFF; _data[2] = b & 0xFF; _data[3] = a & 0xFF;
}
slib::SColor::SColor(const char* s) : slib::SColor() {
	if (!s || s[0] == '\0') return;
	String code(s);
	if (COLOR_MAP.hasKey(code)) *this = COLOR_MAP[code];
	else {
		if (code[0] == '#') {
			code.replace("#", "0x");
			*this = code.intValue();
		}
		else if (code.beginWith("0x")) {
			*this = code.intValue();
		}
		else if (code.beginWith("rgb(") && code.endWith(")")) {
			code.clip(4, code.length() - 5);
			auto list = code.split(",");
			*this = svec3b(list[0].ubyteValue(), list[1].ubyteValue(), list[2].ubyteValue());
		}
		else if (code.beginWith("rgba(") && code.endWith(")")) {
			code.clip(5, code.length() - 6);
			auto list = code.split(",");
			*this = svec4b(list[0].ubyteValue(), list[1].ubyteValue(), list[2].ubyteValue(), list[3].ubyteValue());
		}
	}
	throw FormatException("");
}
slib::SColor::SColor(const slib::SColor& col) : slib::SColor() {
	_type = col._type; _data = col._data;
}
slib::SColor::~SColor() {}
slib::SColor& slib::SColor::operator=(slib::SColor&& col) noexcept {
	_type = col._type; _data.swap(col._data); return *this;
}
slib::SColor& slib::SColor::operator=(const slib::SColor& col) {
	_type = col._type; _data = col._data; return *this;
}

slib::subyte slib::SColor::channel() const { return scolor::channel(_type); }
slib::subyte slib::SColor::size() const { return scolor::size(_type); }

slib::subyte slib::SColor::alpha() const {
	if (!scolor::hasAlpha(_type)) return 0;
	if (isFloat()) return (slib::subyte)(255.f * (*reinterpret_cast<const float*>(&_data[-4])));
	else return _data[-1];
}
bool slib::SColor::isFloat() const { return scolor::isFloat(_type); }
bool slib::SColor::isClear() const { return hasAlpha() && alpha() == 0; }
bool slib::SColor::isMono() const { return channel() == 1; }
bool slib::SColor::hasAlpha() const { return scolor::hasAlpha(_type); }
bool slib::SColor::isGradient() const { return false; }
void slib::SColor::convert(subyte t) {
	if (_type == t) return;
	if (isRGB()) {
		switch (t)
		{
		case scolor::GRAY8:
			_data.resize(1);
			_data[0] = gray8();
			break;
		case scolor::GRAY16:
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
			CMemory<subyte>::copy(&_data[0], reinterpret_cast<subyte*>(&cf3[0]), 4);
			CMemory<subyte>::copy(&_data[4], reinterpret_cast<subyte*>(&cf3[1]), 4);
			CMemory<subyte>::copy(&_data[8], reinterpret_cast<subyte*>(&cf3[2]), 4);
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
			CMemory<subyte>::copy(&_data[0], reinterpret_cast<subyte*>(&cf4[0]), 4);
			CMemory<subyte>::copy(&_data[4], reinterpret_cast<subyte*>(&cf4[1]), 4);
			CMemory<subyte>::copy(&_data[8], reinterpret_cast<subyte*>(&cf4[2]), 4);
			CMemory<subyte>::copy(&_data[12], reinterpret_cast<subyte*>(&cf4[2]), 4);
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
		else return f2bcolor(sstat::average(tmp, 3));
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return _data[0];
			else return s2bcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
		}
		else return sstat::average(_data.ptr(), 3);
	}
}
sushort SColor::gray16() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) return f2scolor(tmp[0]);
		else return f2scolor(sstat::average(tmp, 3));
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return b2scolor(_data[0]);
			else return *reinterpret_cast<const sushort*>(_data.ptr());
		}
		else return b2scolor(sstat::average(_data.ptr(), 3));
	}
}
float SColor::grayf() const {
	if (!isRGB()) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
	if (isFloat()) {
		float* tmp = reinterpret_cast<float*>(_data.ptr());
		if (channel() == 1) return tmp[0];
		else return sstat::average(tmp, 3);
	}
	else {
		if (channel() == 1) {
			if (bpp() == 1) return b2fcolor(_data[0]);
			else return s2fcolor(*reinterpret_cast<const sushort*>(_data.ptr()));
		}
		else return b2fcolor(sstat::average(_data.ptr(), 3));
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
	if (hasAlpha()) col[3] = alpha();
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
	if(hasAlpha()) col[3] = alphaf();
	return col;
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

String SColor::getClass() const { return "color"; }
String SColor::toString() const {
	suint col = rgba();
	sforeach(ColorMap) { if (E_.value == col) return E_.key; }
	String str;
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
	return str;
}
String SColor::toString(COLOR_TEXT_MODE mode) const {
	String str;
	switch (mode)
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
		try { str = ColorMap.rlookup(rgba()); }
		catch (SException ex) { 
			str.clear(); 
			str << "#" << SNumber::toHex(red()) << SNumber::toHex(green()) << SNumber::toHex(blue());
		}
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
			str << "rgba(" << tmp[0] << "," << tmp[1] << "," << tmp[2] << "," << tmp[3] << ")";
		}
		else {
			auto tmp = toVec3i();
			str << "rgb(" << tmp[0] << "," << tmp[1] << "," << tmp[2] << ")";
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
	addColor(0.0f, color::WHITE); addColor(1.0f, color::BLACK);
}
SGradient::SGradient(const SGradient &grad) : _coord(grad._coord), _angle(grad._angle) {
    SColor::_type = grad._type; grad._data.copyTo(SColor::_data);  grad._points.copyTo(_points);
}
SGradient::~SGradient() {}
sgeom::COORDINATE SGradient::coordinate() const { return _coord; }
size_t SGradient::count() const { return _points.size(); }
float SGradient::angle() const { return _angle; }
SColor SGradient::operator[](size_t idx) const { return colorAt(idx); }
SColor SGradient::colorAt(size_t idx) const {
	return SColor(type(), SColor::_data.ptr(idx * bpp()));
}
floatarray &SGradient::points() { return _points; }
const floatarray &SGradient::points() const { return _points; }
void SGradient::setCoordinate(sgeom::COORDINATE coord) { _coord = coord; }
void SGradient::setAngle(float f) { _angle = f; }
void SGradient::setPosition(size_t idx, float f) { _points[(int)idx] = f; }
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
				if (E_ < f && f < E_NEXT) {
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
				if (E_ < f && f < E_NEXT) {
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
String SGradient::getClass() const { return _coord == sgeom::XYZ ? "lgrad" : "rgrad"; }
String SGradient::toString() const {
	String str;


	return str;
}
SObject* SGradient::clone() const { return new SGradient(*this); }
*/