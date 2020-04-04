#include "smedia/smedio.h"
#include "smedia/simage.h"

#ifdef _USE_CV
#include "opencv/cv.h"
#include "smedia/scv.h"
#else
extern "C" {
    #include "libtiff/tiff.h"
    #include "libtiff/tiffio.h"
    #include "libjpeg/jpeglib.h"
    #include "libpng/png.h"
}
#endif
using namespace slib;
using namespace slib::smedia;

void SImageIO::load(const sio::SFile& file, SImage *image) {
#ifdef _USE_CV
	cv::Mat img = cv::imread(file.path().cstr());
	cv2simg(img, image);
#else
	auto ext = file.extension();
	if (ext.beginWith("tif")) importTIFF(file.path(), image);
	else if (ext == "jpg" || ext == "jpeg") importJPG(file.path(), image);
	else if (ext == "png") importPNG(file.path(), image);
	else throw SException(ERR_INFO, SLIB_FORMAT_ERROR, ext, "SLIB IMAGE");
#endif
}
void SImageIO::save(const sio::SFile& file, SImage *image) {
#ifdef _USE_CV
	cv::Mat img = s2cvimg(image);
	cv::imwrite(file.path().cstr(), img);
#else
	auto ext = file.extension();
	if (ext.beginWith("tif")) exportTIFF(file.path(), 0, image);
	else if (ext == "jpg" || ext == "jpeg") exportJPG(file.path(), 1.0, image);
	else if (ext == "png") exportPNG(file.path(), image);
	else throw SException(ERR_INFO, SLIB_FORMAT_ERROR, file.path());
#endif
}
#ifndef _USE_CV
void SImageIO::importTIFF(const char *path, SImage *img) {
    TIFF *tiff = TIFFOpen(path, "r");
	if (!tiff) throw SMediaException(ERR_INFO, sio::FILE_OPEN_ERROR, path);
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &img->_height);
    TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &img->_width);
    sushort bit, channel;
    TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bit);
    TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &channel);
    auto depth = bit/8;
	img->_type = smedia::imageType(depth, channel, RGB_SPACE);
	img->resize(img->_width, img->_height);
    tmsize_t size = TIFFScanlineSize(tiff);
    auto rowptr = img->ptr(0, 0);
    SImageRow row = img->rowAt(0);
    sforin(r, 0, img->_height)  {
        TIFFReadScanline(tiff, rowptr, r); rowptr += size; ++row;
    }
    TIFFClose(tiff);
}

void SImageIO::importJPG(const char *path, SImage *img) {
    FILE *fp = NULL;
    fp = fopen(path, "rb");
    if (!fp) throw SMediaException(ERR_INFO, sio::FILE_OPEN_ERROR, path);
    jpeg_decompress_struct jpegd;
    struct jpeg_error_mgr jerr;
    JSAMPROW buffer = NULL;
    int row_stride;
    
    jpegd.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&jpegd);
	jpeg_stdio_src(&jpegd, fp);
	
	auto res = jpeg_read_header(&jpegd, TRUE);
	if (res != JPEG_HEADER_OK)
		throw SMediaException(ERR_INFO, SLIB_EXEC_ERROR, "jpeg_read_header", EXEC_TEXT(std::to_string(res)));
	jpeg_start_decompress(&jpegd);
    int depth;
    switch (jpegd.out_color_space) {
        case JCS_GRAYSCALE:
        {
            img->_type = GRAY8;
            depth = 1;
            break;
        }
        case JCS_RGB:
        {
            img->_type = RGB32;
            depth = 1;
            break;
        }
        default:
        {
			img->_type = GRAY8; 
			depth = 1;
            break;
        }
    }
    row_stride = sizeof(JSAMPLE) * jpegd.output_width * jpegd.output_components;
    if ((buffer = (JSAMPROW)calloc(row_stride, 1)) == NULL)
        throw SException(ERR_INFO, SLIB_NULL_ERROR, "buffer");
    img->_width = jpegd.output_width;
    img->_height = jpegd.output_height;
    img->resize(img->_width, img->_height);
    
    for (int h = 0; h < img->_height; ++h) {
        jpeg_read_scanlines(&jpegd, &buffer, 1);
        uint8_t *offset = (uint8_t *)buffer;
        for (int w = 0; w < img->_width; ++w) {
            uint8_t *val = img->ptr(w, h);
            val[0] = *(offset++);
            val[1] = *(offset++);
            val[2] = *(offset++);
            val[3] = 0xFF;
        }
    }
    jpeg_finish_decompress(&jpegd);
    jpeg_destroy_decompress(&jpegd);
    free(buffer);
    fclose(fp);
}
void SImageIO::importPNG(const char *path, SImage *img) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) throw SMediaException(ERR_INFO, sio::FILE_OPEN_ERROR, path);
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_byte magic[8];
    fread(magic, sizeof(png_byte), 8, fp);
	if (png_sig_cmp(magic, 0, 8))
		throw SMediaException(ERR_INFO, SLIB_FORMAT_ERROR, (char *)magic, "PNG");
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr) info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr || !info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        throw SMediaException(ERR_INFO, SLIB_EXEC_ERROR, "png_create_read_struct or png_create_info_struct", EXEC_TEXT("null"));
    }
    setjmp(png_jmpbuf(png_ptr));
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16, NULL);
    
    img->_width = png_get_image_width(png_ptr, info_ptr),
    img->_height = png_get_image_height(png_ptr, info_ptr);
    png_bytepp rows = png_get_rows(png_ptr, info_ptr);
    
    switch (png_get_color_type(png_ptr, info_ptr)) {
        case PNG_COLOR_TYPE_GRAY:
        {
            img->setType(GRAY8);
            img->resize(img->_width,img-> _height);
            for (int h = 0; h < img->_height; ++h) {
                auto row = rows[h];
                for (int w = 0; w < img->_width; ++w) img->set(w, h, *row++);
            }
            break;
        }
        case PNG_COLOR_TYPE_RGB:
        {
            img->setType(RGB32);
            img->resize(img->_width, img->_height);
            for (int h = 0; h < img->_height; ++h) {
                auto row = rows[h];
                for (int w = 0; w < img->_width; ++w) {
                    img->set(w, h, 0, *(row++));
                    img->set(w, h, 1, *(row++));
                    img->set(w, h, 2, *(row++));
                }
            }
            break;
        }
        case PNG_COLOR_TYPE_RGB_ALPHA:
        {
            img->setType(RGBA);
            img->resize(img->_width, img->_height);
            for (int h = 0; h < img->_height; ++h) {
                auto row = rows[h];
                for (int w = 0; w < img->_width; ++w) {
                    img->set(w, h, 0, *(row++));
                    img->set(w, h, 1, *(row++));
                    img->set(w, h, 2, *(row++));
                    img->set(w, h, 3, *(row++));
                }
            }
            break;
        }
        default:
            throw SMediaException(ERR_INFO, SLIB_FORMAT_ERROR, "png_get_color_type", "GRAY/RGB/RGBA");
            break;
    }
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}
void SImageIO::exportTIFF(const char *path, int compress, SImage *img) {
    TIFF* tiff = TIFFOpen(path, "w");
    TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, img->_width);
    TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, img->_height);
    TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, img->bpp()/img->channel()*8);
    TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, img->channel());
    TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    if (img->channel() == 1) TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    else TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tiff, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
    TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tiff, TIFFTAG_XRESOLUTION, 150.0);
    TIFFSetField(tiff, TIFFTAG_YRESOLUTION, 150.0);
    TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
    TIFFSetField(tiff, TIFFTAG_SOFTWARE, "LibTiffInSlib");
    TIFFWriteEncodedStrip(tiff, 0, img->ptr(0, 0), img->_width*img->_height*img->bpp());
    TIFFClose(tiff);
}
inline void delAlpha(char *p, uint8_t *ori, int size) {
    for (int i = 0; i < size; ++i) {
        memcpy(p, ori, 3);
        p+=3; ori+=4;
    }
}
void SImageIO::exportJPG(const char *path, int qual, SImage *img) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *fp = NULL;
    JSAMPROW row_pointer[1];
    int row_stride;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    if ((fp = fopen(path, "wb")) == NULL)
		throw SMediaException(ERR_INFO, sio::FILE_OPEN_ERROR, path);
    jpeg_stdio_dest(&cinfo, fp);
    cinfo.image_width = img->_width;
    cinfo.image_height = img->_height;
    switch (img->channel()) {
        case 1:
            cinfo.input_components = 1;
            cinfo.in_color_space = JCS_GRAYSCALE;
            break;
        default:
            cinfo.input_components = 3;
            cinfo.in_color_space = JCS_RGB;
            break;
    }
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, qual, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = img->_width*img->bpp();
    if (img->channel() == 4) {
        auto p = img->ptr(0, 0);
        row_pointer[0] = (char *)malloc(img->_width*3);
        delAlpha(row_pointer[0], p, img->_width);
        while (cinfo.next_scanline < cinfo.image_height) {
            delAlpha(row_pointer[0], p, img->_width);
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
            p += row_stride;
        }
    }
    else {
        row_pointer[0] = (char *)img->ptr(0, 0);
        while (cinfo.next_scanline < cinfo.image_height) {
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
            row_pointer[0] += row_stride;
        }
    }
    jpeg_finish_compress(&cinfo);
    fclose(fp);
    jpeg_destroy_compress(&cinfo);
}
void SImageIO::exportPNG(const char *path, SImage *img) {
    FILE *fp = fopen(path, "wb");
    if (fp == NULL)  throw SMediaException(ERR_INFO, sio::FILE_OPEN_ERROR, path);
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr) info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr || !info_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        throw SMediaException(ERR_INFO, SLIB_EXEC_ERROR, "png_create_write_struct or png_create_info_struct", EXEC_TEXT("null"));
    }
    setjmp(png_jmpbuf(png_ptr));
    png_init_io(png_ptr, fp);
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
    int color_type;
    switch (img->channel()) {
        case 1:
            color_type = PNG_COLOR_TYPE_GRAY;
            break;
        case 3:
            color_type = PNG_COLOR_TYPE_RGB;
            break;
        case 4:
            color_type = PNG_COLOR_TYPE_RGB_ALPHA;
            break;
        default:
            color_type = PNG_COLOR_TYPE_GRAY;
            break;
    }
    png_set_IHDR(png_ptr, info_ptr, img->_width, img->_height,
                 img->bpp()/img->channel()*8, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    auto datap = (uint8_t *)img->ptr(0, 0);
    auto rowsize = img->_width * img->bpp();
    uint8_t *row = (uint8_t *)malloc(img->_width * img->bpp());
    for (int h = 0; h < img->_height; ++h) {
        memcpy(row, datap, rowsize);
        png_write_row(png_ptr, row);
        datap += rowsize;
    }
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}
#endif