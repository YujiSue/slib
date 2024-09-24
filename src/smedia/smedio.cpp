#include "smedia/smedutil.h"
#include "smedia/simage.h"
#ifdef USE_CV
#include "opencv/cv.h"
#include "smedia/scv.h"
#else
extern "C" {
    #include "zlib/zlib.h"
    #include "libtiff/tiff.h"
    #include "libtiff/tiffio.h"
    #include "libjpeg/jpeglib.h"
    #include "libpng/png.h"
}
#endif

void slib::sio::importTIFF(const char* path, slib::SImage& image) {
    TIFF* tiff = TIFFOpen(path, "r");
    
    //if (!tiff) throw FileOpenException(sio::d (ERR_INFO, sio::FILE_OPEN_ERROR, path);
    int width, heihgt;
    TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &heihgt);
    TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
    sushort bit, channel;
    TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bit);
    TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &channel);
    subyte imtype = bit / 8;
    if (channel == 1) imtype |= 0x10;
    else if (3 <= channel) imtype |= 0x20;
    if (channel == 4) imtype |= 0x80;
    image.convert(imtype);
    image.resize(width, heihgt);
    tmsize_t size = TIFFScanlineSize(tiff);
    auto rowptr = image.data();
    ImageRow row = image[0];
    sforin(r, 0_u, image.height()) {
        TIFFReadScanline(tiff, rowptr, r); rowptr += size; ++row;
    }
    TIFFClose(tiff);
}
void slib::sio::importJPG(const char* path, slib::SImage& image) {
    FILE* fp = NULL;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW buffer = NULL;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    fp = fopen(path, "rb");
    if (!fp) throw FileOpenException(fileopenErrorText(path));
    jpeg_stdio_src(&cinfo, fp);
    auto res = jpeg_read_header(&cinfo, TRUE);
    //if (res != JPEG_HEADER_OK) throw SMediaException(ERR_INFO, SLIB_EXEC_ERROR, "jpeg_read_header", EXEC_TEXT(std::to_string(res)));
    jpeg_start_decompress(&cinfo);
    buffer = (JSAMPROW)calloc(sizeof(JSAMPLE) * cinfo.output_width * cinfo.output_components, 1);
    if (buffer == NULL) throw NullException(nullErrorText("Buffer"));
    switch (cinfo.output_components) {
    case 1:
        image.convert(scolor::GRAY8);
        break;
    case 3:
        image.convert(scolor::RGB);
        break;
    case 4:
        image.convert(scolor::RGBA);
        break;
    default:
        break;
    }
    image.resize(cinfo.output_width, cinfo.output_height);
    auto offset = image.data();
    sforin(h, 0_u, cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, &buffer, 1);
        auto px = (subyte*)buffer;
        sforin(w, 0_u, cinfo.output_width) {
            sforin(c, 0, cinfo.num_components) {
                *offset = *px;
                ++offset; ++px;
            }
        }
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(buffer);
    fclose(fp);
}
void slib::sio::importPNG(const char* path, slib::SImage& image) {
    FILE* fp = fopen(path, "rb");
    //if (fp == NULL) throw SMediaException(ERR_INFO, sio::FILE_OPEN_ERROR, path);
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_byte magic[8];
    fread(magic, sizeof(png_byte), 8, fp);
    //if (png_sig_cmp(magic, 0, 8)) throw SMediaException(ERR_INFO, SLIB_FORMAT_ERROR, (char*)magic, "PNG");
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr) info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr || !info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        //throw SMediaException(ERR_INFO, SLIB_EXEC_ERROR, "png_create_read_struct or png_create_info_struct", EXEC_TEXT("null"));
    }
    setjmp(png_jmpbuf(png_ptr));
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16, NULL);

    auto width = png_get_image_width(png_ptr, info_ptr),
        height = png_get_image_height(png_ptr, info_ptr);
    png_bytepp rows = png_get_rows(png_ptr, info_ptr);
    auto offset = image.data();
    switch (png_get_color_type(png_ptr, info_ptr)) {
    case PNG_COLOR_TYPE_GRAY:
    {
        image.convert(scolor::GRAY8);
        image.resize(width, height);
        sforin(h, 0_u, height) {
            auto row = rows[h];
            memcpy(offset, row, width);
            offset += width; row += width;
        }
        break;
    }
    case PNG_COLOR_TYPE_RGB:
    {
        image.convert(scolor::RGB);
        image.resize(width, height);
        sforin(h, 0_u, height) {
            auto row = rows[h];
            memcpy(offset, row, width * 3);
            offset += width * 3; row += width * 3;
        }
        break;
    }
    case PNG_COLOR_TYPE_RGB_ALPHA:
    {
        image.convert(scolor::RGBA);
        image.resize(width, height);
        sforin(h, 0_u, height) {
            auto row = rows[h];
            memcpy(offset, row, width * 4);
            offset += width * 4; row += width * 4;
        }
        break;
    }
    default:
        break;
    }
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}
void slib::sio::exportTIFF(const char* path, const slib::SImage& image, int compress) {
    TIFF* tiff = TIFFOpen(path, "w");
    TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, image.width());
    TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, image.height());
    TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, image.bpp() / image.channel() * 8);
    TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, image.channel());
    TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    if (image.channel() == 1) TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    else TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tiff, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
    TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tiff, TIFFTAG_XRESOLUTION, 150.0);
    TIFFSetField(tiff, TIFFTAG_YRESOLUTION, 150.0);
    TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
    TIFFSetField(tiff, TIFFTAG_SOFTWARE, "LibTiffInSlib");
    TIFFWriteEncodedStrip(tiff, 0, (void*)image.data(), image.mat().size());
    TIFFClose(tiff);
}
void slib::sio::exportJPG(const char* path, const SImage& image, int qual) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* fp = NULL;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    if ((fp = fopen(path, "wb")) == NULL) throw FileWriteException(filewriteErrorText(path));
    jpeg_stdio_dest(&cinfo, fp);
    cinfo.image_width = image.width();
    cinfo.image_height = image.height();
    switch (image.channel()) {
    case 1:
        cinfo.input_components = 1;
        cinfo.in_color_space = JCS_GRAYSCALE;
        break;
    case 3:
        cinfo.input_components = 3;
        cinfo.in_color_space = JCS_RGB;
        break;
    case 4:
        //cinfo.input_components = 3;
        //cinfo.in_color_space = JCS_RGB;
        break;
    }
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, qual, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    JSAMPROW row = (char*)image.data();
    auto rows = image.linesize();
    while (cinfo.next_scanline < cinfo.image_height) {
        jpeg_write_scanlines(&cinfo, &row, 1);
        row += rows;
    }
    jpeg_finish_compress(&cinfo);
    fclose(fp);
    jpeg_destroy_compress(&cinfo);
}
void slib::sio::exportPNG(const char* path, const SImage& image, int level) {
    FILE* fp = fopen(path, "wb");
    //if (fp == NULL)  throw SMediaException(ERR_INFO, sio::FILE_OPEN_ERROR, path);
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr) info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr || !info_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr); 
        //throw SMediaException(ERR_INFO, SLIB_EXEC_ERROR, "png_create_write_struct or png_create_info_struct", EXEC_TEXT("null"));
    }
    setjmp(png_jmpbuf(png_ptr));
    png_init_io(png_ptr, fp);
    png_set_compression_level(png_ptr, level);
    int color_type;
    switch (image.channel()) {
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
    png_set_IHDR(png_ptr, info_ptr, image.width(), image.height(),
        image.depth() * 8, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    auto datap = image.data();
    auto rowsize = image.linesize();
    uint8_t* row = (uint8_t*)malloc(rowsize);
    sforin(h, 0_u, image.height()) {
        memcpy(row, datap, rowsize);
        png_write_row(png_ptr, row);
        datap += rowsize;
    }
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(row);
    fclose(fp);
}


/*
void SImageIO::load(const sio::SFile& file, SImage *image) {
#ifdef USE_CV
	cv::Mat img = cv::imread(file.path().cstr());
	cv2simg(img, image);
#else
	auto ext = file.extension(), path = file.path();
#ifdef WIN_OS
	path = path.localize();
#endif
	if (ext.beginWith("tif")) importTIFF(path.cstr(), image);
	else if (ext == "jpg" || ext == "jpeg") importJPG(path.cstr(), image);
	else if (ext == "png") importPNG(path.cstr(), image);
	else throw SException(ERR_INFO, SLIB_FORMAT_ERROR, ext, "SLIB IMAGE");
#endif
}
void SImageIO::save(const sio::SFile& file, SImage *image) {
#ifdef USE_CV
	cv::Mat img = s2cvimg(image);
	cv::imwrite(file.path().cstr(), img);
#else
	auto ext = file.extension(), path = file.path();
#ifdef WIN_OS
	path = path.localize();
#endif
	if (ext.beginWith("tif")) exportTIFF(path.cstr(), 0, image);
	else if (ext == "jpg" || ext == "jpeg") exportJPG(path.cstr(), 1.0, image);
	else if (ext == "png") exportPNG(path.cstr(), image);
	else throw SException(ERR_INFO, SLIB_FORMAT_ERROR, file.path());
#endif
}
#ifndef USE_CV

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
	JSAMPROW row = (char *)img->ptr();
	auto rows = img->_width * img->bpp();
	while (cinfo.next_scanline < cinfo.image_height) {
		jpeg_write_scanlines(&cinfo, &row, 1);
		row += rows;
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
*/