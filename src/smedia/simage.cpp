#include "smedia/simage.h"
#include "sio/sio.h"
#include "sutil/scode.h"
#include "sutil/snet.h"
slib::SImage::SImage() : SObject() { _type = scolor::GRAY8; _load = false; }

slib::SImage::SImage(const subyte type, const size_t w, const size_t h) : SImage() {
    _type = type;
    _bytes.resize(h, w * scolor::size(type));
}
slib::SImage::SImage(const char* url, bool l) : SImage() { if (l) load(url); else link(url); }
slib::SImage::SImage(SImage&& img) noexcept { swap(img); }
slib::SImage::SImage(const SImage& img) : SImage() {
    _type = img._type;
    _bytes = img._bytes;
    _attribute = img._attribute;
    _url = img._url;
    _load = img._load;
}
slib::SImage::~SImage() {}
slib::SImage& slib::SImage::operator=(const SImage& img) {
    _type = img._type;
    _bytes = img._bytes;
    _attribute = img._attribute;
    _url = img._url;
    _load = img._load;
    return *this;
}
slib::subyte slib::SImage::type() const { return _type; }
bool slib::SImage::loaded() const { return _load; }
const slib::String& slib::SImage::url() const { return _url; }
bool slib::SImage::empty() const { return _bytes.empty(); }
size_t slib::SImage::count() const { return _bytes.size() / scolor::size(_type); }
size_t slib::SImage::size() const { return _bytes.size(); }
size_t slib::SImage::linesize() const { return _bytes.col; }
size_t slib::SImage::width() const { return _bytes.col / scolor::size(_type); }
size_t slib::SImage::height() const { return _bytes.row; }
int slib::SImage::channel() const { return scolor::channel(_type); }
int slib::SImage::depth() const { return scolor::depth(_type); }
int slib::SImage::bpp() const { return scolor::size(_type); }
slib::Tuple<int, int, int> slib::SImage::dimension() const {
    return slib::Tuple<int, int, int>(_bytes.col, _bytes.row, scolor::size(_type));
}
slib::ImageRow slib::SImage::operator[](const int idx) { return ImageRow(this, idx); }
const slib::ImageRow slib::SImage::operator[](const int idx) const { return ImageRow(this, idx); }

slib::PixelIterator slib::SImage::begin() { return PixelIterator(this, _bytes.begin()); }
slib::PixelCIterator slib::SImage::begin() const { return PixelCIterator(this, _bytes.begin()); }
slib::PixelIterator slib::SImage::end() { return PixelIterator(this, _bytes.end()); }
slib::PixelCIterator slib::SImage::end() const { return PixelCIterator(this, _bytes.end()); }

slib::subyte* slib::SImage::data() { return _bytes.data(); }
const slib::subyte* slib::SImage::data() const { return _bytes.data(); }
slib::smath::Matrix<slib::subyte>& slib::SImage::mat() { return _bytes; }
const slib::smath::Matrix<slib::subyte>& slib::SImage::mat() const { return _bytes; }

void slib::SImage::fill(const Color col) { sfor(*this) $_ = col; }
void slib::SImage::convert(const subyte t) {
    if (t == _type) return;
    if (empty()) _type = t;
    else {
        SImage tmp(t, width(), height());
        sfor2(*this, tmp) $_2 = scolor::convertTo($_1.color(), t);
    }
}
inline void nn_interpolator(slib::PixelIterator it, float x, float y, const slib::SImage& src) {
    int x_ = (int)x, y_ = (int)y;
    $_ = src[(y - y_ < 0.5f ? y_ : y_ + 1)][(x - x_ < 0.5f ? x_ : x_ + 1)];
}
inline void bl_interpolator(slib::PixelIterator it, float x, float y, const slib::SImage& src) {
    int x_ = (int)x, y_ = (int)y;
    slib::smath::Vector2D<float> r1(x - x_, 1.f + x_ - x), r2(y - y_, 1.f + y_ - y);
    slib::Pixel px[4] = { src[y_][x_], src[y_ + 1][x_], src[y_][x_ + 1], src[y_ + 1][x_ + 1] };
    auto col1 = slib::scolor::mix(px[0].color(), px[2].color(), r1), col2 = slib::scolor::mix(px[1].color(), px[3].color(), r1);
    $_ = slib::scolor::mix(col1, col2, r2);
}
inline float bc_weight(float f) {
    if (f <= 1.f) return 1.f - 2.f * f * f + f * f * f;
    else if (f <= 2.f) return 4.f - 8.f * f + 5.f * f * f - f * f * f;
    return 0.f;
}
inline void bc_interpolator(slib::PixelIterator it, float x, float y, const slib::SImage& src) {
    int x_ = (int)x, y_ = (int)y;

    slib::smath::Vector<float> weight;
    slib::Array<slib::Color> neighbors;

    sforin(h, 0, 4) {
        sforin(w, 0, 4) {
            if (h < 0 || w < 0 || (int)src.height() <= h || (int)src.width() <= w) {
                neighbors.add(slib::Color(src.type(), nullptr));
                weight.add(0.f);
            }
            else {



            }

        }
    }



    $_ = slib::scolor::mix(neighbors, weight);



    /*
    float wx[4], wy[4];
    sforin(i, 0, 4) {
        if (x_ == 0 && i == 0) wx[i] = 0.0;
        else if (src.width() <= x_ + 1 && i == 2) wx[i] = 0.0;
        else if (src.width() <= x_ + 2 && i == 3) wx[i] = 0.0;
        else wx[i] = bc_weight(abs(x - (x_ - 1 + i)));
        if (y_ == 0 && i == 0) wy[i] = 0.0;
        else if (src.height() <= y_ + 1 && i == 2) wy[i] = 0.0;
        else if (src.height() <= y_ + 2 && i == 3) wy[i] = 0.0;
        else wy[i] = bc_weight(abs(y - (y_ - 1 + i)));
    }
    float total = 0.0;
    sforin(j, 0, 4) {
        sforin(k, 0, 4) total += wx[j] * wy[k];
    }




    sforin(b, 0, increment) {
        float val = 0.0;
        sforin(h, y_ - 1, y_ + 3) {
            sforin(w, x_ - 1, x_ + 3) {
                if (h < 0 || w < 0 || src.height() <= h || src.width() <= w) continue;
                val += src->at(w, h, b) * wx[w + 1 - x_] * wy[h + 1 - y_];
            }
        }
        dest[b] = (slib::subyte)(val / total);
    }
    */
}
void slib::SImage::_resampling(const size_t w, const size_t h, RESAMPLING m) {
    if (_bytes.col == w && _bytes.row == h) return;
    slib::SImage buffer(_type, w, h);
    smath::Vector2D<float> scale((float)_bytes.col / w, (float)_bytes.row / h);
    auto it = buffer.begin();
    sforin(r, 0_u, h) {
        sforin(c, 0_u, w) {
            switch (m) {
            case RESAMPLING::NN_PX:
                nn_interpolator($, scale[0] * c, scale[1] * r, *this);
                break;
            case RESAMPLING::BI_LINEAR:
                bl_interpolator($, scale[0] * c, scale[1] * r, *this);
                break;
            case RESAMPLING::BI_CUBIC:
                bc_interpolator($, scale[0] * c, scale[1] * r, *this);
                break;
            default:
                break;
            }
            $NEXT;
        }
    }
    swap(buffer);
}
void slib::SImage::resize(const size_t w, const size_t h, RESAMPLING m) {
    if (empty()) _bytes.resize(h, w * scolor::size(_type));
    else _resampling(w, h, m);
}
void slib::SImage::swap(SImage& img) {
    Memory<subyte>::swap(&_type, &img._type);
    Memory<bool>::swap(&_load, &img._load);
    _bytes.swap(img._bytes);
    _attribute.swap(img._attribute);
    _url.swap(img._url);
}
void slib::SImage::load(const char* path) {
    auto ext = sfs::extension(path);
    _url = path;
    if (ext.match(REG("/jp[e]*g/i"))) {
        sio::importJPG(path, *this);
        _load = true;
    }
    else if (ext.match(REG("/png/i"))) {
        sio::importPNG(path, *this);
        _load = true;
    }
    else if (ext.match(REG("/tif+/i"))) {
        sio::importTIFF(path, *this);
        _load = true;
    }
    else {
        _load = false;
        throw FormatException(formatErrorText("Image format", ext, "JPEG/PNG/TIFF"));
    }
}
void slib::SImage::link(const char* path) {
    _url = path;
    _load = false;
}
void slib::SImage::save(const char* path, sobj attribute) const {
    auto ext = sfs::extension(path);
    if (ext.match(REG("/jp[e]*g/i"))) {
        sio::exportJPG(path, *this, (attribute.hasKey("qual") ? attribute["qual"].intValue() : 7));
    }
    else if (ext.match(REG("/png/i"))) {
        sio::exportPNG(path, *this, (attribute.hasKey("level") ? attribute["level"].intValue() : 9));
    }
    else if (ext.match(REG("/tif+/i"))) {
        sio::exportTIFF(path, *this, (attribute.hasKey("compression") ? attribute["compression"].intValue() : 0));
    }
    else {
        throw FormatException(formatErrorText("Image format", ext, "JPEG/PNG/TIFF"));
    }
}

slib::String slib::SImage::getClass() const { return "image"; }
slib::String slib::SImage::toString(const char* format) const {
    String f(format ? format : "base64");
    if (_load) {
        if (f == "base64") {
            String str;
            sutil::encodeBase64(_bytes.array(), str);
            return str;
        }
        else throw FormatException(formatErrorText("Image to text format", f, "base64"));
    }
    else  return _url;
}
slib::SObject* slib::SImage::clone() const { return new SImage(*this); }




/*
slib::subyte& slib::SImage::at(int x, int y, int c) { return *data(x, y, c); }

slib::SImage slib::SImage::subimage(int x, int y, int w, int h) { return SImage(); }
slib::SImage slib::SImage::subimage(const sarea& area) { return SImage(); }

void slib::SImage::set(int x, int y, int c, subyte val) { at(x, y, c) = val; }
void slib::SImage::set(int x, int y, subyte* vals) {}
void slib::SImage::set(int x, int y, suint val) {}
*/
/*
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

SImage SImage::subimage(sint x, sint y, sint w, sint h) { return subimage(sarea(x, y, w, h)); }
SImage SImage::subimage(sarea area) {
    SImage img(type(), area.width, area.height);
    auto row = rowAt(area.ori_y), row_ = img[0];
    sforin(r, 0, area.height) {
		CMemory<subyte>::copy(row_.ptr(), row[area.ori_x].ptr(), area.width * bytePerPixel(_type));
        ++row; ++row_;
    }
    return img;
}
void SImage::set(size_t x, size_t y, sbyte c, subyte val) { at((sint)x, (sint)y, c) = val; }
void SImage::set(size_t x, size_t y, subyte *val) {
    CMemory<subyte>::copy(ptr((sint)x, (sint)y), val, 1);
}
void SImage::set(size_t x, size_t y, suint val) {
    CMemory<subyte>::copy(ptr((sint)x, (sint)y), (subyte *)&val, 4);
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
*/