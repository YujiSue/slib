#ifndef SLIB_SIMAGE_H
#define SLIB_SIMAGE_H

#include "sio/sfile.h"
#include "smedia/smedia.h"
#include "smedia/smedio.h"
#include "smedia/spixel.h"
#include "smath/stat.h"

namespace slib {
    using namespace smath;
    
    namespace smedia {
        class SLIB_DLL SImage : public SObject {
            friend SImageIO;
        public:            
            typedef enum {
                NN_PX = 0,
                BI_LINEAR = 1,
                BI_CUBIC = 2,
            } RESAMPLING;
            
        protected:
            size_t _width, _height;
			sushort _type;
            ubytearray _data;
            sio::SFile _file;
            SDictionary _attribute;
            
        private:
            void _changeType(sushort t);
            void _resampling(size_t w, size_t h, RESAMPLING m);
            
        public:
            SImage();
            SImage(sushort type, size_t w = 0, size_t h = 0, const void *dat = nullptr);
            SImage(const char *path, bool l);
            SImage(const sobj &ptr);
            SImage(const SImage &img);
            ~SImage();
            
            SImage &operator=(const SImage &img);
            
            //IO
            void load(const SDictionary &dict);
            void load(const char *path);
            void link(const char *path);
            void save(const SDictionary &dict);
            void save(const char *path);
            
            //Property
            sushort type() const;
            bool empty() const;
            size_t size() const;
            size_t width() const;
            size_t height() const;
            subyte channel() const;
			subyte bpp() const;
            const ubytearray &data() const;
            const sio::SFile &file() const;
            bool isLink() const;
            
            //Edit
            void setType(sushort t);
            void resize(size_t w, size_t h, RESAMPLING m = BI_CUBIC);
            void release();
            void discard();
            
            //Data access
            SImageRow operator[](int idx) const;
            SImageRow rowAt(int idx) const;
            subyte *ptr(sint x = 0, sint y = 0, sbyte c = 0);
            const subyte *ptr(sint x = 0, sint y = 0, sbyte c = 0) const;
            subyte &at(sint x, sint y = 0, sbyte c = 0);
            const subyte &at(sint x, sint y, sbyte c = 0) const;
            
            spxl_iter begin();
            spxl_citer begin() const;
            spxl_iter end();
            spxl_citer end() const;
            
            SImage subimage(sint x, sint y, sint w, sint h);
            SImage subimage(sarea area);
            
            void set(size_t x, size_t y, sbyte c, subyte val);
            void set(size_t x, size_t y, subyte *val);
            void set(size_t x, size_t y, suint val);
            
            void clear();
            void reset(subyte val);
            
            void copy(const subyte *ptr, size_t s);
            void copyTo(void *ptr) const;
            void moveTo(void *ptr);
            void swap(SImage &img);
            
            void toMatrix(smath::smat<subyte> &mat);
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
    }
}

#endif
