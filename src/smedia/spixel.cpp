#include "smedia/spixel.h"
#include "smedia/sgraphic.h"

using namespace slib;
using namespace slib::smedia;

SImageRow::SImageRow() : _bpp(1), _type(0x11), _width(0), _data(nullptr) {}
SImageRow::SImageRow(sushort t, size_t w, subyte *p) : _bpp(bytePerPixel(t)), _type(t), _width(w), _data(p) {}
SImageRow::SImageRow(const SImageRow &row) : _bpp(row._bpp), _type(row._type), _width(row._width), _data(row._data) {}
SImageRow::~SImageRow() {}
SImageRow &SImageRow::operator=(const SImageRow &row) {
    _bpp = row._bpp; _type = row._type; _width = row._width; _data = row._data; return *this;
}
SImageRow &SImageRow::operator++() { _data += _bpp*_width; return *this; }
SImageRow &SImageRow::operator--() { _data -= _bpp*_width; return *this; }
SPixel SImageRow::operator[](size_t idx) const { return SPixel(_type, &_data[idx*_bpp]); }
subyte *SImageRow::ptr() const { return _data; }
bool SImageRow::operator<(const SImageRow &row) const { return _data < row._data; }
bool SImageRow::operator==(const SImageRow &row) const { return _bpp == row._bpp && _data == row._data; }

SPixel::SPixel() : _type(RGBA), _bpp(4), _data(nullptr) {}
SPixel::SPixel(sushort t, subyte *p) : _type(t), _bpp(bytePerPixel(t)), _data(p) {}
SPixel::SPixel(const SPixel &px) : _type(px._type), _bpp(px._bpp), _data(px._data) {}
SPixel::~SPixel() {}

SPixel &SPixel::operator=(const SPixel &px) {
	_bpp = px._bpp; _type = px._type; _data = px._data; return *this;
}
SPixel &SPixel::operator=(subyte i) { _data[0] = i; return *this; }
SPixel &SPixel::operator=(sushort i) {
    CMemory<subyte>::copy(_data, (subyte *)&i, 2); return *this;
}
SPixel &SPixel::operator=(suint i) {
    CMemory<subyte>::copy(_data, (subyte *)&i, 4); return *this;
}
SPixel& SPixel::operator=(const SColor& col) {
	switch (_type)
	{
	case GRAY8:
		*_data = col.gray8();
		break;
	case GRAY16:
	{
		auto c = col.gray16();
		memcpy(_data, &c, 2);
		break;
	}
	case RGB24:
	{
		auto rgb = col.toVec3i();
		_data[0] = rgb[0];
		_data[1] = rgb[1];
		_data[2] = rgb[2];
		_data[3] = 255;
		break;
	}
	case RGBA:
	{
		auto rgba = col.toVec4i();
		_data[0] = rgba[0];
		_data[1] = rgba[1];
		_data[2] = rgba[2];
		_data[3] = rgba[3];
		break;
	}
	default:
		*_data = col.gray8();
		break;
	}
    return *this;
}
SPixel &SPixel::operator++() { _data += _bpp; return *this; }
SPixel &SPixel::operator--() { _data -= _bpp; return *this; }
subyte& SPixel::operator[](size_t idx) { return _data[idx * colorDepth(_type)]; }
const subyte& SPixel::operator[](size_t idx) const { return _data[idx * colorDepth(_type)]; }
subyte *SPixel::ptr() const { return _data; }
subyte SPixel::channel() const { return colorChannel(_type); }
subyte SPixel::depth() const { return colorDepth(_type); }
subyte SPixel::bpp() const { return _bpp; }

SColor SPixel::color() const {
	switch (_type)
	{
	case GRAY8:
		return SColor(*_data);
	case GRAY16:
		return SColor(GRAY16, _data);
	case RGB24:
		return SColor(RGB24, _data);
	case RGBA:
		return SColor(RGBA, _data);
	default:
		return SColor(*_data);
	}
}
void SPixel::swap(SPixel px) {
    auto p = ptr(); _data = px.ptr(); px._data = p;
}
String SPixel::getClass() const { return "pixel"; }
String SPixel::toString() const { return color().toString(); }
SObject *SPixel::clone() const { return new SPixel(*this); }
bool SPixel::operator<(const SPixel &px) const { return _data < px._data; }
bool SPixel::operator==(const SPixel &px) const { return _data == px._data; }

SPixelIterator::SPixelIterator(subyte t, subyte *p) : px(t, p) {}
SPixelIterator::SPixelIterator(const SPixelIterator &it) : px(it.px) {}
SPixelIterator::~SPixelIterator() {}
SPixelIterator& SPixelIterator::operator=(const SPixelIterator &it) {
    px = it.px; return *this;
}
SPixel &SPixelIterator::operator *() { return px; }
SPixel *SPixelIterator::operator ->() { return &px; }
SPixel &SPixelIterator::operator [](std::ptrdiff_t diff) { sforin(i, 0, diff) ++px; return px; }
SPixelIterator &SPixelIterator::operator ++() { ++px; return *this; }
SPixelIterator SPixelIterator::operator ++(int) { return ++SPixelIterator(*this); }
SPixelIterator &SPixelIterator::operator --() { --px; return *this; }
SPixelIterator SPixelIterator::operator --(int) { return --SPixelIterator(*this); }
SPixelIterator &SPixelIterator::operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++px; return *this; }
SPixelIterator &SPixelIterator::operator -=(std::ptrdiff_t diff) { sforin(i, 0, diff) --px; return *this; }
SPixelIterator SPixelIterator::operator +(std::ptrdiff_t diff) { return SPixelIterator(*this)+=diff; }
SPixelIterator SPixelIterator::operator -(std::ptrdiff_t diff) { return SPixelIterator(*this)-=diff; }
int SPixelIterator::operator -(SPixelIterator it) { return (px.ptr()-it.px.ptr())/px.bpp(); }
void SPixelIterator::swap(SPixelIterator it1, SPixelIterator it2) { it1.px.swap(it2.px); }
bool SPixelIterator::operator <(const SPixelIterator &it) const { return px.ptr() < it.px.ptr(); }
bool SPixelIterator::operator <=(const SPixelIterator &it) const { return px.ptr() <= it.px.ptr(); }
bool SPixelIterator::operator >(const SPixelIterator &it) const { return px.ptr() > it.px.ptr(); }
bool SPixelIterator::operator >=(const SPixelIterator &it) const { return px.ptr() >= it.px.ptr(); }
bool SPixelIterator::operator ==(const SPixelIterator &it) const { return px.ptr() == it.px.ptr(); }
bool SPixelIterator::operator !=(const SPixelIterator &it) const { return px.ptr() != it.px.ptr(); }

SPixelCIterator::SPixelCIterator(subyte t, const subyte *p) : px(t, const_cast<subyte *>(p)) {}
SPixelCIterator::SPixelCIterator(const SPixelCIterator &it) : px(it.px) {}
SPixelCIterator::~SPixelCIterator() {}
SPixelCIterator &SPixelCIterator::operator=(const SPixelCIterator &it) {
    px = it.px; return *this;
}
const SPixel &SPixelCIterator::operator *() { return px; }
const SPixel *SPixelCIterator::operator ->() { return &px; }
const SPixel &SPixelCIterator::operator [](std::ptrdiff_t diff) { sforin(i, 0, diff) ++px; return px; }
SPixelCIterator &SPixelCIterator::operator ++() { ++px; return *this; }
SPixelCIterator SPixelCIterator::operator ++(int) { return ++SPixelCIterator(*this); }
SPixelCIterator &SPixelCIterator::operator --() { --px; return *this; }
SPixelCIterator SPixelCIterator::operator --(int) { return --SPixelCIterator(*this); }
SPixelCIterator &SPixelCIterator::operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++px; return *this; }
SPixelCIterator &SPixelCIterator::operator -=(std::ptrdiff_t diff) { sforin(i, 0, diff) --px; return *this; }
SPixelCIterator SPixelCIterator::operator +(std::ptrdiff_t diff) { return SPixelCIterator(*this)+=diff; }
SPixelCIterator SPixelCIterator::operator -(std::ptrdiff_t diff) { return SPixelCIterator(*this)-=diff; }
int SPixelCIterator::operator -(SPixelCIterator it) { return (px.ptr()-it.px.ptr())/px.bpp(); }
void SPixelCIterator::swap(SPixelCIterator it1, SPixelCIterator it2) { it1.px.swap(it2.px); }
bool SPixelCIterator::operator <(const SPixelCIterator &it) const { return px.ptr() < it.px.ptr(); }
bool SPixelCIterator::operator <=(const SPixelCIterator &it) const { return px.ptr() <= it.px.ptr(); }
bool SPixelCIterator::operator >(const SPixelCIterator &it) const { return px.ptr() > it.px.ptr(); }
bool SPixelCIterator::operator >=(const SPixelCIterator &it) const { return px.ptr() >= it.px.ptr(); }
bool SPixelCIterator::operator ==(const SPixelCIterator &it) const { return px.ptr() == it.px.ptr(); }
bool SPixelCIterator::operator !=(const SPixelCIterator &it) const { return px.ptr() != it.px.ptr(); }