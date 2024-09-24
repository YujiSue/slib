#include "smedia/simage.h"
slib::Pixel::Pixel() : _color(scolor::GRAY8), _data(nullptr) {}
slib::Pixel::Pixel(subyte t, subyte* p) : _color(t), _data(p) {}
slib::Pixel::Pixel(const Pixel& px) : _color(px._color), _data(px._data) {}
slib::Pixel::~Pixel() {}
slib::Pixel& slib::Pixel::operator=(const Pixel& px) {
	_color = px._color; _data = px._data; return *this;
}
slib::Pixel& slib::Pixel::operator=(const subyte i) {
	_data[0] = i; 
	return *this; 
}
slib::Pixel& slib::Pixel::operator=(const sushort i) {
	memcpy(_data, reinterpret_cast<const subyte*>(&i), 2); 
	return *this;
}
slib::Pixel& slib::Pixel::operator=(const int i) {
	Memory<subyte>::copy(_data, reinterpret_cast<const subyte*>(&i), 4);
	return *this;
}
slib::Pixel& slib::Pixel::operator=(const Color& col) {
	if (_color == col.type()) memcpy(_data, col.data(), scolor::size(_color));
	else {
		Color c_ = scolor::convertTo(col, _color);
		memcpy(_data, c_.data(), scolor::size(_color));
	}
	return *this;
}
slib::Pixel& slib::Pixel::operator=(const SColor& col) {
	if (col.isGradient()) *this = ((const GradientColor&)col).colors()[0].second;
	else *this = (const Color&)col;
	return *this;
}
slib::Pixel& slib::Pixel::operator++() { _data += (int)scolor::size(_color); return *this; }
slib::Pixel& slib::Pixel::operator--() { _data -= (int)scolor::size(_color); return *this; }
int slib::Pixel::operator[](const int idx) const {
	auto channel = scolor::channel(_color);
	if (idx < 0 || channel <= idx) throw RangeException(outRangeErrorText("Channel index", idx, 0, channel - 1));
	auto depth = scolor::depth(_color);
	if (depth == 1) return (int)_data[idx];
	else if (depth == 2) return (int)*reinterpret_cast<const sushort*>(&_data[depth * idx]);
	else return *reinterpret_cast<const sint*>(&_data[depth * idx]);
}
void slib::Pixel::set(const int idx, const int val) {
	auto channel = scolor::channel(_color);
	if (idx < 0 || channel <= idx) throw RangeException(outRangeErrorText("Channel index", idx, 0, channel - 1));
	auto depth = scolor::depth(_color);
	memcpy(&_data[depth * idx], reinterpret_cast<const subyte*>(&val), channel);
}
slib::subyte slib::Pixel::type() const { return _color; }
slib::subyte* slib::Pixel::data() const { return _data; }
slib::Color slib::Pixel::color() const { return Color(_color, _data); }
bool slib::Pixel::operator<(const slib::Pixel& px) const { return _data < px._data; }
bool slib::Pixel::operator==(const slib::Pixel& px) const { return _data == px._data; }

slib::ImageRow::ImageRow() : _img(nullptr), _row(0) {}
slib::ImageRow::ImageRow(SImage* img, const int ridx) {
	_img = img; _row = ridx < 0 ? (int)_img->height() + ridx : ridx;
}
slib::ImageRow::ImageRow(const SImage* img, const int ridx) {
	_img = const_cast<SImage*>(img); _row = ridx < 0 ? (int)_img->height() + ridx : ridx;
}
slib::ImageRow::ImageRow(const ImageRow &row) : _img(row._img), _row(row._row) {}
slib::ImageRow::~ImageRow() {}
slib::ImageRow & slib::ImageRow::operator=(const ImageRow &row) {
	_img = row._img; _row = row._row; return *this;
}
slib::ImageRow& slib::ImageRow::operator++() { 
	if (!_img) throw NullException(nullErrorText("Image data source"));
	++_row;
	if (_img->height() <= _row) throw RangeException(outRangeErrorText("Row index", _row, 0, _img->height()));
	return *this; 
}
slib::ImageRow & slib::ImageRow::operator--() {
	if (!_img) throw NullException(nullErrorText("Image data source"));
	if (_row == 0) throw RangeException(outRangeErrorText("Row index", -1, 0, _img->height()));
	--_row;
	return *this;
}
slib::Pixel slib::ImageRow::operator[](const int idx) const {
	if (!_img) throw NullException(nullErrorText("Image data source"));
	return Pixel(_img->type(), data(idx));
}
slib::subyte* slib::ImageRow::data(const int idx) const {
	if (!_img) throw NullException(nullErrorText("Image data source"));
	auto p = _img->data() + _img->linesize() * _row;
	return p + _img->bpp() * (idx < 0 ? (int)_img->width() + idx : idx);
}
bool slib::ImageRow::operator<(const ImageRow &row) const {

	return _row < row._row; 
}
bool slib::ImageRow::operator==(const ImageRow& row) const { 

	return _row == row._row; 
}
slib::PixelIterator::PixelIterator() : _img(nullptr) {}
slib::PixelIterator::PixelIterator(SImage* img, ArrayIterator<subyte> it) : PixelIterator() { 
	_img = img; _px = Pixel(_img->type(), it.ptr()); 
}
slib::PixelIterator::PixelIterator(const PixelIterator &it) : _img(it._img), _px(it._px) {}
slib::PixelIterator::~PixelIterator() {}
slib::PixelIterator& slib::PixelIterator::operator=(const slib::PixelIterator& it) { 
	_img = it._img; _px = it._px; return *this; 
}
slib::Pixel & slib::PixelIterator::operator *() { return _px; }
slib::Pixel * slib::PixelIterator::operator ->() { return &_px; }
slib::Pixel & slib::PixelIterator::operator [](std::ptrdiff_t diff) { sforin(i, 0, diff) ++_px; return _px; }
slib::PixelIterator & slib::PixelIterator::operator ++() { ++_px; return *this; }
slib::PixelIterator slib::PixelIterator::operator ++(int) { return ++PixelIterator(*this); }
slib::PixelIterator & slib::PixelIterator::operator --() { --_px; return *this; }
slib::PixelIterator slib::PixelIterator::operator --(int) { return --PixelIterator(*this); }
slib::PixelIterator & slib::PixelIterator::operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++_px; return *this; }
slib::PixelIterator & slib::PixelIterator::operator -=(std::ptrdiff_t diff) { sforin(i, 0, diff) --_px; return *this; }
slib::PixelIterator slib::PixelIterator::operator +(std::ptrdiff_t diff) { return PixelIterator(*this)+=diff; }
slib::PixelIterator slib::PixelIterator::operator -(std::ptrdiff_t diff) { return PixelIterator(*this)-=diff; }
int slib::PixelIterator::operator -(slib::PixelIterator it) { return (_px._data - it._px._data) / _img->bpp(); }
void slib::PixelIterator::swap(slib::PixelIterator it1, PixelIterator it2) { Memory<Pixel>::swap(&it1._px, &it2._px); }
bool slib::PixelIterator::operator <(const slib::PixelIterator &it) const { return _px < it._px; }
bool slib::PixelIterator::operator <=(const slib::PixelIterator& it) const { return _px < it._px || _px == it._px; }
bool slib::PixelIterator::operator >(const slib::PixelIterator &it) const { return it._px < _px; }
bool slib::PixelIterator::operator >=(const slib::PixelIterator &it) const { return it._px < _px || _px == it._px; }
bool slib::PixelIterator::operator ==(const slib::PixelIterator &it) const { return _px == it._px; }
bool slib::PixelIterator::operator !=(const slib::PixelIterator &it) const { return !(_px == it._px); }
slib::PixelCIterator::PixelCIterator() : _img(nullptr) {}
slib::PixelCIterator::PixelCIterator(const SImage* img, ArrayCIterator<subyte> it) : PixelCIterator() { 
	_img = img; _px = Pixel(_img->type(), const_cast<subyte*>(it.ptr())); 
}
slib::PixelCIterator::PixelCIterator(const slib::PixelCIterator &it) : _img(it._img), _px(it._px) {}
slib::PixelCIterator::~PixelCIterator() {}
slib::PixelCIterator &slib::PixelCIterator::operator=(const slib::PixelCIterator &it) { 
	_img = it._img; _px = it._px; return *this;
	return *this; 
}
const slib::Pixel& slib::PixelCIterator::operator *() { return _px; }
const slib::Pixel* slib::PixelCIterator::operator ->() { return &_px; }
const slib::Pixel& slib::PixelCIterator::operator [](std::ptrdiff_t diff) { sforin(i, 0, diff) ++_px; return _px; }
slib::PixelCIterator &slib::PixelCIterator::operator ++() { ++_px; return *this; }
slib::PixelCIterator slib::PixelCIterator::operator ++(int) { return ++PixelCIterator(*this); }
slib::PixelCIterator & slib::PixelCIterator::operator --() { --_px; return *this; }
slib::PixelCIterator  slib::PixelCIterator::operator --(int) { return --PixelCIterator(*this); }
slib::PixelCIterator & slib::PixelCIterator::operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++_px; return *this; }
slib::PixelCIterator & slib::PixelCIterator::operator -=(std::ptrdiff_t diff) { sforin(i, 0, diff) --_px; return *this; }
slib::PixelCIterator  slib::PixelCIterator::operator +(std::ptrdiff_t diff) { return PixelCIterator(*this)+=diff; }
slib::PixelCIterator  slib::PixelCIterator::operator -(std::ptrdiff_t diff) { return PixelCIterator(*this)-=diff; }
int slib::PixelCIterator::operator -(PixelCIterator it) { return (_px._data - it._px._data) / _img->bpp(); }
void slib::PixelCIterator::swap(PixelCIterator it1, PixelCIterator it2) { Memory<Pixel>::swap(&it1._px, &it2._px); }
bool slib::PixelCIterator::operator <(const PixelCIterator &it) const { return _px < it._px; }
bool slib::PixelCIterator::operator <=(const PixelCIterator &it) const { return _px < it._px || _px == it._px; }
bool slib::PixelCIterator::operator >(const PixelCIterator &it) const { return it._px < _px; }
bool slib::PixelCIterator::operator >=(const PixelCIterator &it) const { return it._px < _px || it._px == _px; }
bool slib::PixelCIterator::operator ==(const PixelCIterator &it) const { return _px == it._px; }
bool slib::PixelCIterator::operator !=(const PixelCIterator &it) const { return !(_px == it._px); }
