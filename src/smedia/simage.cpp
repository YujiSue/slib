#include "smedia/simage.h"
#include "sio/sio.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::smedia;

inline void gray8_16(const subyte *src, subyte *to) {
    sushort val = (int)(*src)*MAX_USHORT/MAX_UBYTE;
    memcpy(to, &val, 2);
}
inline void gray8_32(const subyte *src, subyte *to) {
    suint val = (suint)(*src)*MAX_UINT/MAX_UBYTE;
    memcpy(to, &val, 4);
}
inline void gray16_8(const subyte *src, subyte *to) {
    subyte val = (int)(*src)*MAX_UBYTE/MAX_USHORT;
    memcpy(to, &val, 1);
}
inline void gray32_8(const subyte *src, subyte *to) {
    suint val = (suint)(*src)*MAX_UBYTE/MAX_UINT;
    memcpy(to, &val, 1);
}

void SImage::_changeType(sushort t) {
    auto cur = type();
    if (t == cur) return;
	if (_data.empty()) {
		_type = t;
		return;
	}
	else {
		subyte ori_ch = channel(), novel_ch = (t << 4) & 0x0F;
		subyte ori_bpp = bpp(), novel_bpp = t & 0x0F;
		if (ori_bpp == novel_bpp) {
			if (ori_ch < novel_ch) {

			}
			else {

			}
		}
		else {
			ubytearray tmp(_width * _height * novel_bpp);
			if (novel_bpp < ori_bpp) {}
			else {}
			tmp.swap(_data);
		}
		_type = t;
	}
}

inline void nn_interpolator(subyte* dest, float x, float y, const SImage* src, subyte bpp) {
	int x_ = (int)x, y_ = (int)y;
	memcpy(dest, src->ptr((x - x_ < 0.5f ? x_ : x_ + 1), (y - y_ < 0.5f ? y_ : y_ + 1)), bpp);
}
inline void bl_interpolator(subyte *dest, float x, float y, const SImage *src, subyte bpp) {
    int x_ = (int)x, y_ = (int)y;
    float t = x-x_, s = y-y_;
    for (int b = 0; b < bpp; ++b) {
        float px1 = t*src->at(x_, y_, b)+(1-t)*src->at(x_+1, y_, b),
        px2 = t*src->at(x_, y_+1, b)+(1-t)*src->at(x_+1, y_+1, b);
        dest[b] = (subyte)(s*px1+(1-s)*px2);
    }
}
inline float bc_weight(float f) {
	if (f <= 1.f) return 1.f - 2.f * f * f + f * f * f;
	else if (f <= 2.f) return 4.f - 8.f * f + 5.f * f * f - f * f * f;
	return 0.f;
}
inline void bc_interpolator(subyte *dest, float x, float y, const SImage *src, subyte bpp) {
    int x_ = (int)x, y_ = (int) y;
    float wx[4], wy[4];
    sforin(i, 0, 4) {
        if (x_ == 0 && i == 0) wx[i] = 0.0;
        else if (src->width() <= x_+1 && i == 2) wx[i] = 0.0;
        else if (src->width() <= x_+2 && i == 3) wx[i] = 0.0;
        else wx[i] = bc_weight(abs(x-(x_-1+i)));
        if (y_ == 0 && i == 0) wy[i] = 0.0;
        else if (src->height() <= y_+1 && i == 2) wy[i] = 0.0;
        else if (src->height() <= y_+2 && i == 3) wy[i] = 0.0;
        else wy[i] = bc_weight(abs(y-(y_-1+i)));
    }
    float total = 0.0;
    sforin(j, 0, 4) {
        sforin(k, 0, 4) total += wx[j]*wy[k];
    }
    sforin(b, 0, bpp) {
        float val = 0.0;
        sforin(h, y_-1, y_+3) {
            sforin(w, x_-1, x_+3) {
                if (h < 0 || w < 0 || src->height() <= h || src->width() <= w) continue;
                val += src->at(w, h, b)*wx[w+1-x_]*wy[h+1-y_];
            }
        }
        dest[b] = (subyte)(val/total);
    }
}

void SImage::_resampling(size_t w, size_t h, RESAMPLING m) {
    if (_width == w && _height == h) return;
	auto bpp = bytePerPixel(_type);
	ubytearray tmp(w * h * bpp);
    float x_ratio = (float)_width/w, y_ratio = (float)_height/h;
    for (int row = 0; row < h; ++row) {
        for (int col = 0; col < w; ++col) {
            switch (m) {
                case NN_PX:
                    nn_interpolator(tmp.ptr((row*w+col)* bpp),
                                    x_ratio*col, y_ratio*row, this, bpp);
                    break;
                case BI_LINEAR:
                    bl_interpolator(tmp.ptr((row*w+col)* bpp),
                                    x_ratio*col, y_ratio*row, this, bpp);
                    break;
                case BI_CUBIC:
                    bc_interpolator(tmp.ptr((row*w+col)* bpp),
                                    x_ratio*col, y_ratio*row, this, bpp);
                    break;
                default:
                    break;
            }
        }
    }
    _width = w;
    _height = h;
    _data.swap(tmp);
}

SImage::SImage() : SObject() {
	_width = 0; _height = 0; _type = smedia::RGBA;
}
SImage::SImage(sushort type, size_t w, size_t h, const void *dat) : SImage() {
	_width = w; _height = h; _type = type;
    if (w && h) {
        resize(_width, _height);
        if (dat) memcpy(_data.ptr(), dat, _data.size());
    }
}
SImage::SImage(const char *path, bool l) {
    l?link(path):load(path);
}
SImage::SImage(const sobj &obj) {
    if (obj.isImg()) *this = obj.image();
    else throw SException(ERR_INFO, SLIB_CAST_ERROR, "obj", CAST_TEXT(obj->getClass(), "image"));
}
SImage::SImage(const SImage &img) : SImage(img.type(), img._width, img._height, img.ptr(0, 0)) {}
SImage::~SImage() {}

SImage &SImage::operator=(const SImage &img) {
    _width = img._width; _height = img._height;
	_type = img._type; _data = img._data;
	_file = img._file; _attribute = img._attribute;
    return *this;
}
void SImage::load(const char *path) {
    clear(); _file = path;
	SImageIO::load(_file, this);
}
void SImage::link(const char *path) {
    clear(); _file = path;
}
void SImage::save(const char *path) {
	_file = path;
	SImageIO::save(_file, this);
}
sushort SImage::type() const { return _type; }
bool SImage::empty() const { return _data.empty(); }
size_t SImage::size() const { return _data.size(); }
size_t SImage::width() const { return _width; }
size_t SImage::height() const { return _height; }
subyte SImage::channel() const { return colorChannel(_type); }
subyte SImage::bpp() const { return bytePerPixel(_type); }
const ubytearray &SImage::data() const { return _data; }
const sio::SFile &SImage::file() const { return _file; }
bool SImage::isLink() const { return _file.exist() && empty(); }

void SImage::setType(sushort t) { _changeType(t); }
void SImage::resize(size_t w, size_t h, RESAMPLING m) {
    if(!empty()) _resampling(w, h, m);
    else {
        _data.resize(w*h* bytePerPixel(_type));
        _width = w; _height = h;
    }
}
void SImage::release() { _data.release(); clear(); }
void SImage::discard() { _data.discard(); clear(); }

SImageRow SImage::operator[](int idx) const { return SImageRow(_type, _width, _data.ptr(_width * bytePerPixel(_type) * idx)); }
SImageRow SImage::rowAt(int idx) const { return SImageRow(_type, _width, _data.ptr(_width * bytePerPixel(_type) * idx)); }
subyte* SImage::ptr(int32_t x, int32_t y, int8_t c) { return _data.ptr((y * _width + x) * bytePerPixel(_type) + c); }
const subyte* SImage::ptr(int32_t x, int32_t y, int8_t c) const { return _data.ptr((y * _width + x) * bytePerPixel(_type) + c); }
subyte &SImage::at(int32_t x, int32_t y, int8_t c) { return *ptr(x, y, c); }
const subyte &SImage::at(int32_t x, int32_t y, int8_t c) const { return *ptr(x, y, c); }

SPixelIterator SImage::begin() { return SPixelIterator(type(), _data.ptr()); }
SPixelCIterator SImage::begin() const { return SPixelCIterator(type(), _data.ptr()); }
SPixelIterator SImage::end() { return SPixelIterator(type(), _data.ptr() + _data.size()); }
SPixelCIterator SImage::end() const { return SPixelCIterator(type(), _data.ptr() + _data.size()); }

SImage SImage::subimage(int32_t x, int32_t y, int32_t w, int32_t h) {
    return subimage(sarea(x, y, w, h));
}
SImage SImage::subimage(sarea area) {
    SImage img(type(), area.width, area.height);
    auto row = rowAt(area.ori_y), row_ = img[0];
    sforin(r, 0, area.height) {
		CMemory<subyte>::copy(row_.ptr(), row[area.ori_x].ptr(), area.width * bytePerPixel(_type));
        ++row; ++row_;
    }
    return img;
}
void SImage::set(size_t x, size_t y, int8_t c, subyte val) { at(x, y, c) = val; }
void SImage::set(size_t x, size_t y, subyte *val) {
    CMemory<subyte>::copy(ptr(x, y), val, 1);
}
void SImage::set(size_t x, size_t y, suint val) {
    CMemory<subyte>::copy(ptr(x, y), (subyte *)&val, 4);
}
void SImage::clear() {
    _width = 0; _height = 0; _type = 0;
    _data.clear(); _file.clear(); _attribute.clear();
}
void SImage::reset(subyte val) { _data.reset(val); }

void SImage::copy(const subyte *ptr, size_t s) {
    CMemory<subyte>::copy(_data.ptr(), ptr, s);
}
void SImage::copyTo(void *ptr) const {
    CMemory<subyte>::copy((subyte *)ptr, _data.ptr(), _data.size());
}
void SImage::moveTo(void *ptr) {
    ptr = _data.ptr(); discard();
}
void SImage::swap(SImage &img) {
    auto tmpt = _type; _type = img._type; img._type = tmpt;
	_data.swap(img._data);
    auto tmpf = _file; _file = img._file; img._file = tmpf;
    _attribute.swap(img._attribute);
}
void SImage::toMatrix(smatub &mat) {
	mat.resize(_height, _width * bytePerPixel(_type));
    memcpy(mat.ptr(), _data.ptr(), _data.size());
}
String SImage::getClass() const { return "image"; }
String SImage::toString() const {
    if (isLink()) return "img:/"+_file.path();
    else return "";
}
SObject *SImage::clone() const { return new SImage(*this); }