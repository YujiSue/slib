//#include "smedia/smovie.h"
/*
#include "libtiff/tiff.h"
#include "libtiff/tiffio.h"
*/

//TIFF *tiff;
/*
SMovie::SMovie() {
    mov_width = 0;
    mov_height = 0;
    frame_count = 0;
    current_frame = 0;
    load_frames = false;
    buffering = true;
    //threads.setSize(2);
}
SMovie::SMovie(const SMovie &mov) {
    
    
}
SMovie::~SMovie() {
    //if (tiff) TIFFClose(tiff);
}

void SMovie::load(const char *path, bool l) {
    mov_file = path;
    load_frames = l;
    auto ext = mov_file.extension();
    if (ext.beginWith("tif")) {
        //mov_type = MULTI_TIFF;
        tiff = TIFFOpen(path, "r");
        frame_count = 0;
        current_frame = 0;
        do { ++frame_count; } while (TIFFReadDirectory(tiff));
        TIFFSetDirectory(tiff, 0);
        
        TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &mov_height);
        TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &mov_width);
        
        uint16_t bit, channel;
        TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bit);
        TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &channel);
        
        if (load_frames) frames.resize(frame_count);
        else if(buffering) frames.resize(2);
        else frames.resize(1);
        for (int f = 0; f < frames.size(); ++f) {
            if(frames[f].isNull()) {}
                //frames[f] = simg( (channel, bit/8), mov_width, mov_height);
            tmsize_t size = TIFFScanlineSize(tiff);
            for (int r = 0; r < mov_height; ++r)
                TIFFReadScanline(tiff, frames[f]->ptr(r*size), r);
            TIFFReadDirectory(tiff);
        }
    }
}

void SMovie::save(const char *path, SDictionary &config) {
    
    
}
sframe SMovie::operator[](size_t idx) {
    if (frame_count <= idx)
        throw Exception();// (ERR_INFO, SLIB_RANGE_ERROR);
    if (load_frames) return frames[idx];
    else {
        //if (threads.isWorking()) threads.complete();
        if(current_frame+1 == idx) frames[0].swap(frames[1]);
        else if(mov_type == MULTI_TIFF) {
            TIFFSetDirectory(tiff, (uint16)idx);
            tmsize_t size = TIFFScanlineSize(tiff);
            for (int r = 0; r < mov_height; ++r)
                TIFFReadScanline(tiff, frames[0]->ptr(r*size), r);
        }
        //threads.addTask(loader, idx+1, frames[1]);
        current_frame = idx;
        return frames[0];
    }
}
sframe SMovie::frameAt(size_t idx) {
    if(frame_count <= idx)
        throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    if (load_frames) return frames[idx];
    else {
        //if (threads.isWorking()) threads.complete();
        if(current_frame+1 == idx) frames[0].swap(frames[1]);
        else if(mov_type == MULTI_TIFF) {
            TIFFSetDirectory(tiff, (uint16)idx);
            tmsize_t size = TIFFScanlineSize(tiff);
            for (int r = 0; r < mov_height; ++r)
                TIFFReadScanline(tiff, frames[0]->ptr(r*size), r);
        }
        //threads.addTask(loader, idx+1, frames[1]);
        current_frame = idx;
        return frames[0];
    }
}
bool SMovie::nextFrame() {
    
    
    return false;
}
size_t SMovie::frameNum() { return frame_count; }

SObject *SMovie::clone() const { return new SMovie(*this); }
String SMovie::getClass() const { return "movie"; }
String SMovie::toString() const { return ""; }
*/