#ifndef SLIB_SMEDUTIL_H
#define SLIB_SMEDUTIL_H

#include "sio/stream.h"

namespace slib {
    class SLIB_DLL SImage;

    namespace sio {
        extern SLIB_DLL void importTIFF(const char* path, SImage& image);
        extern SLIB_DLL void importJPG(const char* path, SImage& image);
        extern SLIB_DLL void importPNG(const char* path, SImage& image);

        extern SLIB_DLL void exportTIFF(const char* path, const SImage& image, int compress = 0);
        extern SLIB_DLL void exportJPG(const char* path, const SImage& image, int qual = 7);
        extern SLIB_DLL void exportPNG(const char* path, const SImage& image, int level = 9);
    }

    namespace smedia {
        class SLIB_DLL SImage;
        
        class SLIB_DLL SImageIO {
        public:
			static void load(const SFile& file, SImage *image);
			static void save(const SFile& file, SImage *image);

            static void importTIFF(const char *path, SImage *img);
            static void importJPG(const char *path, SImage *img);
            static void importPNG(const char *path, SImage *img);
            
            static void exportTIFF(const char *path, int compress, SImage *img);
            static void exportJPG(const char *path, int qual, SImage *img);
            static void exportPNG(const char *path, SImage *img);
        };
        
        class SLIB_DLL SMovieIO {};
    }
}


#endif
