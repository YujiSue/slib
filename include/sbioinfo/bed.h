#ifndef SBIO_SBED_H
#define SBIO_SBED_H
#include "sio/sfile.h"
#include "sbioinfo/seq.h"
namespace slib {
    namespace sbio {
        /**
        * @class BEDData
        * \~english @brief Data stricture class for handling BED format data.
        * \~japanese @brief BED形式の情報を保持するクラス
        */
        class SLIB_DLL BEDData : public RefPos {
        public:

            String name;
            float score;
            sattribute attribute;
        public:
            BEDData();
            BEDData(const BEDData& bd);
            ~BEDData();
            BEDData& operator=(const BEDData& bd);
            void clear();
        };
        /**
        * @class BEDFile
        * \~english @brief File stream class for loading BED format file.
        * \~japanese @brief BED形式のファイルから情報を取得するストリームクラス
        */
        class SLIB_DLL BEDFile : public Array<BEDData> {
        private:
            SFile _file;
            String _ln;
            BEDData _buffer;
            SeqList* _ref;

        public:
            BEDFile();
            BEDFile(const char* path);
            BEDFile(const char *path, SeqList& ref);
            ~BEDFile();
            void operator>>(Array<Region<sint>>& reg);
            
            void open(const char* path);
            void setRef(SeqList& ref);
            void load(const char* path, SeqList& ref);
            const String path() const;
            BEDData* next(bool stack = false);
            operator bool() const;
        };
    }
}

#endif
