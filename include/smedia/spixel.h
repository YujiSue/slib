#ifndef SLIB_SPIXEL_H
#define SLIB_SPIXEL_H

#include "sobj/sobject.h"
#include "smedia/sgraphic.h"

namespace slib {
    class SLIB_DLL SImage;
    class SLIB_DLL Color;
    class SLIB_DLL PixelIterator;
    class SLIB_DLL PixelCIterator;

    class SLIB_DLL Pixel {
        friend PixelIterator;
        friend PixelCIterator;

    protected:
        subyte _color, * _data;

    public:
        Pixel();
        Pixel(subyte t, subyte* p);
        Pixel(const Pixel& px);
        ~Pixel();

        Pixel& operator=(const Pixel& px);
        Pixel& operator=(const subyte i);
        Pixel& operator=(const sushort i);
        Pixel& operator=(const int i);
        Pixel& operator=(const Color& col);
        Pixel& operator=(const SColor& col);
        Pixel& operator++();
        Pixel& operator--();

        int operator[](const int idx) const;
        void set(const int idx, const int val);
        subyte type() const;
        subyte* data() const;
        Color color() const;

        bool operator<(const Pixel& px) const;
        bool operator==(const Pixel& px) const;
    };

    class SLIB_DLL ImageRow {
    protected:
        SImage* _img;
        size_t _row;

    public:
        ImageRow();
        ImageRow(SImage *img, const int ridx);
        ImageRow(const SImage* img, const int ridx);
        ImageRow(const ImageRow& row);
        ~ImageRow();

        ImageRow& operator=(const ImageRow& row);
        ImageRow& operator++();
        ImageRow& operator--();

        Pixel operator[](const int idx) const;
        subyte* data(const int idx = 0) const;

        bool operator<(const ImageRow& row) const;
        bool operator==(const ImageRow& row) const;
    };

    class SLIB_DLL PixelIterator {
        friend Pixel;
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef Pixel value_type;
        typedef std::ptrdiff_t difference_type;
        typedef Pixel* pointer;
        typedef Pixel& reference;
    protected:
        SImage* _img;
        Pixel _px;

    public:
        PixelIterator();
        PixelIterator(SImage *img, ArrayIterator<subyte> it);
        PixelIterator(const PixelIterator& it);
        ~PixelIterator();
        PixelIterator& operator=(const PixelIterator& it);
        reference operator *();
        pointer operator ->();
        reference operator [](std::ptrdiff_t diff);
        PixelIterator& operator ++();
        PixelIterator operator ++(int);
        PixelIterator& operator --();
        PixelIterator operator --(int);
        PixelIterator& operator +=(std::ptrdiff_t diff);
        PixelIterator& operator -=(std::ptrdiff_t diff);
        PixelIterator operator +(std::ptrdiff_t diff);
        PixelIterator operator -(std::ptrdiff_t diff);
        int operator -(PixelIterator it);
        void swap(PixelIterator it1, PixelIterator it2);
        bool operator <(const PixelIterator& it) const;
        bool operator <=(const PixelIterator& it) const;
        bool operator >(const PixelIterator& it) const;
        bool operator >=(const PixelIterator& it) const;
        bool operator ==(const PixelIterator& it) const;
        bool operator !=(const PixelIterator& it) const;
    };

    class SLIB_DLL PixelCIterator {
        friend Pixel;
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef const Pixel value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const Pixel* pointer;
        typedef const Pixel& reference;
    protected:
        const SImage* _img;
        Pixel _px;

    public:
        PixelCIterator();
        PixelCIterator(const SImage* img, ArrayCIterator<subyte> it);
        PixelCIterator(const PixelCIterator& it);
        ~PixelCIterator();
        PixelCIterator& operator=(const PixelCIterator& it);
        reference operator *();
        pointer operator ->();
        reference operator [](std::ptrdiff_t diff);
        PixelCIterator& operator ++();
        PixelCIterator operator ++(int);
        PixelCIterator& operator --();
        PixelCIterator operator --(int);
        PixelCIterator& operator +=(std::ptrdiff_t diff);
        PixelCIterator& operator -=(std::ptrdiff_t diff);
        PixelCIterator operator +(std::ptrdiff_t diff);
        PixelCIterator operator -(std::ptrdiff_t diff);
        int operator -(PixelCIterator it);
        void swap(PixelCIterator it1, PixelCIterator it2);
        bool operator <(const PixelCIterator& it) const;
        bool operator <=(const PixelCIterator& it) const;
        bool operator >(const PixelCIterator& it) const;
        bool operator >=(const PixelCIterator& it) const;
        bool operator ==(const PixelCIterator& it) const;
        bool operator !=(const PixelCIterator& it) const;
    };
}
#endif
