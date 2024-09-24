#ifndef SLIB_SIMAGE_H
#define SLIB_SIMAGE_H

#include "sbasic/tuple.h"
#include "sobj/sdict.h"
#include "sio/sfile.h"
#include "smedia/smedutil.h"
#include "smedia/spixel.h"

namespace slib {
    enum class RESAMPLING : subyte {
        NN_PX = 0,
        BI_LINEAR = 1,
        BI_CUBIC = 2,
    };
    
    class SLIB_DLL SImage : public SObject {
        friend slib::smath::Matrix<subyte>;

    protected:
        subyte _type;
        slib::smath::Matrix<subyte> _bytes;
        SDictionary _attribute;
        String _url;
        bool _load;

    private:
        void _resampling(const size_t w, const size_t h, RESAMPLING m);

    public:
        SImage();
        SImage(const subyte type, const size_t w, const size_t h);
        SImage(const char* url, bool l);
        SImage(SImage&& img) noexcept;
        SImage(const SImage& img);
        ~SImage();
        SImage& operator=(const SImage& img);
        
        subyte type() const;
        bool loaded() const;
        const String& url() const;
        bool empty() const;
        size_t count() const; // Pixel count
        size_t size() const; // Total size(bytes)
        size_t linesize() const; // Bytes per line
        size_t width() const;
        size_t height() const;
        int channel() const;
        int depth() const;
        int bpp() const; // bytes per pixel
        Tuple<int, int, int> dimension() const;

        ImageRow operator[](const int idx);
        const ImageRow operator[](const int idx) const;
        PixelIterator begin();
        PixelCIterator begin() const;
        PixelIterator end();
        PixelCIterator end() const;

        subyte* data();
        const subyte* data() const;
        slib::smath::Matrix<subyte>& mat();
        const slib::smath::Matrix<subyte>& mat() const;

        void fill(const Color col);
        void convert(const subyte t);
        void resize(const size_t w, const size_t h, RESAMPLING m = RESAMPLING::BI_CUBIC);
        void swap(SImage& img);

        void load(const char* path);
        void link(const char* path);
        void save(const char* path, sobj attribute = snull) const;

        String getClass() const;
        String toString(const char* format = nullptr) const;
        SObject* clone() const;

    };
}

#endif
