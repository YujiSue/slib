#ifndef SLIB_SMOVIE_H
#define SLIB_SMOVIE_H

//#include "sutil/sthread.h"
#include "sio/sfile.h"
#include "smedia/simage.h"

namespace slib {
    using namespace sio;
    namespace smedia {
        
        namespace scodec {
            
        }
        
        #define sframe simg
        #define smov scobj<SMovie, MOVIE_OBJ>
        
        class SOBJ_DLL SMovie : public SObject {
        private:
            //TIFF *tiff;
            
        protected:
            int mov_type;
            SFile mov_file;
            uint32_t mov_width, mov_height, frame_count, current_frame;
            sarray frames;
            integerarray frame_offset, key_frames;
            bool load_frames, buffering;
            
            std::function<void(size_t ifx, SImage *)> loader;
            //SWork threads;
            
        private:
            void swap();
            
        public:
            SMovie();
            SMovie(const SMovie &mov);
            ~SMovie();
            
            void load(const char *path, bool l);
            void save(const char *path, SDictionary &config);
            
            sframe operator[](size_t idx);
            sframe frameAt(size_t idx);
            bool nextFrame();
            size_t frameNum();
            
            void addFrame(sframe f);
            
            SObject *clone() const;
            String getClass() const;
            String toString() const;
        };
        
    }
}



#endif
