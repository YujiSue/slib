#ifndef SLIB_SMEDIO_H
#define SLIB_SMEDIO_H

#include "sbasic/sbasic.h"
#include "sio/sfile.h"
#include "smedia/smedia.h"

namespace slib {
    namespace smedia {
        class SOBJ_DLL SImage;
        
        class SOBJ_DLL SImageIO {
        public:
			static void load(const sio::SFile& file, SImage *image);
			static void save(const sio::SFile& file, SImage *image);

            static void importTIFF(const char *path, SImage *img);
            static void importJPG(const char *path, SImage *img);
            static void importPNG(const char *path, SImage *img);
            
            static void exportTIFF(const char *path, int compress, SImage *img);
            static void exportJPG(const char *path, int qual, SImage *img);
            static void exportPNG(const char *path, SImage *img);
        };
        
        class SOBJ_DLL SMovieIO {};
    }
}


#endif
