#ifndef SLIB_SPIXEL_H
#define SLIB_SPIXEL_H

#include "sobj/sobject.h"
#include "smedia/smedia.h"

namespace slib {
    namespace smedia {
        class SOBJ_DLL SColor;
        class SOBJ_DLL SPixel;
        
        class SOBJ_DLL SImageRow {
        protected:
            subyte _bpp, *_data;
			sushort _type;
            size_t _width;
            
        public:
            SImageRow();
            SImageRow(sushort t, size_t w, subyte *p);
            SImageRow(const SImageRow &row);
            ~SImageRow();
            
            SImageRow &operator=(const SImageRow &row);
            SImageRow &operator++();
            SImageRow &operator--();
            
            SPixel operator[](size_t idx) const;
            
            subyte *ptr() const;
            
            bool operator<(const SImageRow &row) const;
            bool operator==(const SImageRow &row) const;
        };
        
		using spxl = scobj<SPixel, PIXEL_OBJ>;
        class SOBJ_DLL SPixel : public SObject {
        protected:
            subyte _bpp, *_data;
			sushort _type;

        public:
            SPixel();
            SPixel(sushort t, subyte *p);
            SPixel(const SPixel &px);
            ~SPixel();
            
            SPixel &operator=(const SPixel &px);
            SPixel &operator=(subyte i);
            SPixel &operator=(sushort i);
            SPixel &operator=(suint i);
            SPixel &operator=(const SColor &col);
            SPixel &operator++();
            SPixel &operator--();
            
            subyte &operator[](size_t idx);
            const subyte &operator[](size_t idx) const;
            subyte *ptr() const;
            subyte channel() const;
            subyte depth() const;
            subyte bpp() const;
            SColor color() const;
            
            void swap(SPixel px);
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
            
            bool operator<(const SPixel &px) const;
            bool operator==(const SPixel &px) const;
        };
        
        #define spxl_iter SPixelIterator
        #define spxl_citer SPixelCIterator

        class SOBJ_DLL SPixelIterator {
        public:
            typedef std::random_access_iterator_tag iterator_category;
            typedef SPixel value_type;
            typedef std::ptrdiff_t difference_type;
            typedef SPixel* pointer;
            typedef SPixel& reference;
            
            SPixel px;
            
        public:
            SPixelIterator(subyte t, subyte* p);
            SPixelIterator(const SPixelIterator &it);
            ~SPixelIterator();
            SPixelIterator& operator=(const SPixelIterator &it);
            reference operator *();
            pointer operator ->();
            reference operator [](std::ptrdiff_t diff);
            SPixelIterator &operator ++();
            SPixelIterator operator ++(int);
            SPixelIterator &operator --();
            SPixelIterator operator --(int);
            SPixelIterator &operator +=(std::ptrdiff_t diff);
            SPixelIterator &operator -=(std::ptrdiff_t diff);
            SPixelIterator operator +(std::ptrdiff_t diff);
            SPixelIterator operator -(std::ptrdiff_t diff);
            int operator -(SPixelIterator it);
            void swap(SPixelIterator it1, SPixelIterator it2);
            bool operator <(const SPixelIterator &it) const;
            bool operator <=(const SPixelIterator &it) const;
            bool operator >(const SPixelIterator &it) const;
            bool operator >=(const SPixelIterator &it) const;
            bool operator ==(const SPixelIterator &it) const;
            bool operator !=(const SPixelIterator &it) const;
        };
        
        class SOBJ_DLL SPixelCIterator {
        public:
            typedef std::random_access_iterator_tag iterator_category;
            typedef const SPixel value_type;
            typedef std::ptrdiff_t difference_type;
            typedef const SPixel* pointer;
            typedef const SPixel& reference;
            
            SPixel px;
            
        public:
            SPixelCIterator(subyte t, const subyte *p);
            SPixelCIterator(const SPixelCIterator &it);
            ~SPixelCIterator();
            SPixelCIterator& operator=(const SPixelCIterator &it);
            reference operator *();
            pointer operator ->();
            reference operator [](std::ptrdiff_t diff);
            SPixelCIterator &operator ++();
            SPixelCIterator operator ++(int);
            SPixelCIterator &operator --();
            SPixelCIterator operator --(int);
            SPixelCIterator &operator +=(std::ptrdiff_t diff);
            SPixelCIterator &operator -=(std::ptrdiff_t diff);
            SPixelCIterator operator +(std::ptrdiff_t diff);
            SPixelCIterator operator -(std::ptrdiff_t diff);
            int operator -(SPixelCIterator it);
            void swap(SPixelCIterator it1, SPixelCIterator it2);
            bool operator <(const SPixelCIterator &it) const;
            bool operator <=(const SPixelCIterator &it) const;
            bool operator >(const SPixelCIterator &it) const;
            bool operator >=(const SPixelCIterator &it) const;
            bool operator ==(const SPixelCIterator &it) const;
            bool operator !=(const SPixelCIterator &it) const;
        };
    }
}

#endif
