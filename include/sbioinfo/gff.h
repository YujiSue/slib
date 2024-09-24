#ifndef SBIO_SGFF_H
#define SBIO_SGFF_H
#include "sio/sfile.h"
#include "sbioinfo/biutil.h"
#include "sbioinfo/annotation.h"
namespace slib {
    namespace sbio {
        class SLIB_DLL GffData : public RefPos {
        public:
            String seqid, source, type, strand;
            sbyte phase;
            float score;
            sattribute attribute;
        public:
            GffData();
            GffData(const GffData& g);
            ~GffData();
            GffData& operator=(const GffData& gd);
            void init();
        };
        
        class SLIB_DLL GffFile : public SFile {
        private:
            //SFile _file;
            String _ln;
            stringarray _buffer;
        protected:
            GffData _data;
        public:
            sattribute attribute;
        private:
            void readHeader();
        public:
            GffFile();
            GffFile(const char *path);
            ~GffFile();
            void open(const char* path, SeqList* reference = nullptr);
            //void load(const char* path, SeqList* reference = nullptr);
            GffData *next();
        };
    }
}

#endif
