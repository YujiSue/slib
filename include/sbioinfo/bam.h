#ifndef SBIO_BAM_H
#define SBIO_BAM_H

#include "sutil/sthread.h"
#include "sio/sfile.h"
#include "sbioinfo/biutil.h"
#include "sbioinfo/align.h"
#include "sbioinfo/seq.h"

namespace slib {
    namespace sbio {
        //Error
#define BAM_FORM_ERR "Not BAM magic."
#define BAI_FORM_ERR "Not BAI magic."
#define BSM_FORM_ERR "Not BSM magic."

#define BGZF_FORM_ERR "Not BGZF magic."
#define BGZF_BLOCKSIZE_ERR "BGZF block size error."

#define GZIP_STREAM_ERR_MSG "zlib stream error."
#define GZIP_DATA_ERR_MSG "zlib data error."
#define GZIP_BUFFER_ERR_MSG "zlib buffer error."
#define GZIP_INFLATE_ERR_MSG "zlib inflate error."

#define READ_FORMAT_ERR_MSG "Format is not supported."
#define READ_SIZE_ERR_MSG "Read data size is too short."

        extern SLIB_DLL String bamFormatErrorText(char*);
        extern SLIB_DLL String baiFormatErrorText(char*);
        extern SLIB_DLL String bgzfFormatErrorText(char*);
        extern SLIB_DLL String bsmFormatErrorText(char*);

        //MAGIC
        constexpr char BGZF_MAGIC[17] = { 
            (char)0x1F, (char)0x8B, (char)0x08, (char)0x04, 
            (char)0x00, (char)0x00, (char)0x00, (char)0x00, 
            (char)0x00, (char)0xFF, (char)0x06, (char)0x00,
            (char)0x42, (char)0x43, (char)0x02, (char)0x00 };
        constexpr char BAM_MAGIC[5] = { (char)0x42, (char)0x41, (char)0x4D, (char)0x01 };
        constexpr char BAI_MAGIC[5] = { (char)0x42, (char)0x41, (char)0x49, (char)0x01 };
        constexpr char BSM_MAGIC[5] = { (char)0x42, (char)0x53, (char)0x4D, (char)0x02 };

        //Constant
        constexpr int DEFAULT_THREAD_COUNT = 8;
        constexpr int DEFAULT_DEPTH_BIN = 4;
        
        constexpr int BGZF_MAX_BLOCK_SIZE = 0x10000;
        constexpr int MAX_BIN = (((1<<18)-1)/7);
        constexpr int MAX_READ_NAME_LENGTH = 0xFF;
        constexpr int BAM_INDEX_BIN = 1<<14;
        
        class SLIB_DLL BamFile;
        namespace sbam {
            //Constant
            constexpr sushort MULTI_SEGMENT_READ = 0x0001;
            constexpr sushort PROPER_ALIGNED_READ = 0x0002;
            constexpr sushort UNMAPPED_READ = 0x0004;
            constexpr sushort NEXT_UNMAPPED_READ = 0x0008;
            constexpr sushort COMPLEMENT_READ = 0x0010;
            constexpr sushort NEXT_REVERSE_READ = 0x0020;
            constexpr sushort FIRST_SEGMENT = 0x0040;
            constexpr sushort LAST_SEGMENT = 0x0080;
            constexpr sushort SECONDARY_ALIGN_READ = 0x0100;
            constexpr sushort QUALITY_FILTERED = 0x0200;
            constexpr sushort PCR_DUPLICATE = 0x0400;
            constexpr sushort SUPPLEMENTAL = 0x0800;

            // Util
            extern SLIB_DLL size_t countBin(const srange& range);
            extern SLIB_DLL sushort getBin(srange range);
            extern SLIB_DLL ushortarray getBins(srange range);
            extern SLIB_DLL ushortarray getBins(const sregion& region);

            /**
            * @class sbam::VOffset
            * \~english @brief Virtual offset
            */
            class SLIB_DLL VOffset {
            public:
                sinteger file_offset;
                sushort block_offset;
            public:
                VOffset();
                VOffset(sinteger fo, sushort bo);
                VOffset(const suinteger offset);
                VOffset(const VOffset& v);
                ~VOffset();
                VOffset& operator = (const VOffset& v);
                operator suinteger() const;
                bool operator < (const VOffset& v) const;
                bool operator == (const VOffset& v) const;
            };
            /**
            * @class sbam::Header
            * \~english @brief BAM header
            */
            class SLIB_DLL Header {
            public:
                sint ref_num;
                intarray ref_length;
                stringarray ref_name;
                String text;
            public:
                Header();
                ~Header();
                void set(int n);
                String toString() const;
                void init();
            };
            /**
            * @class sbam::Index
            * \~english @brief BAM index
            */
            class SLIB_DLL Index {
                friend BamFile;
            public:
                sint ref_num;
                Array<Array<Array<Range<sbam::VOffset>>>> chunks;
                Array<Array<sbam::VOffset>> loffsets;
                Array<Map<suint, suint>> bin_map;

            private:
                void setNum(int n);

            public:
                Index();
                Index(const char* path);
                ~Index();
                void load(const char* path);
                //void save(const char *path);
                void init();
            };
            /**
            * @class sbam::Block
            * \~english @brief BAM data block
            */
            class SLIB_DLL Block {
            public:
                VOffset offset;
                sushort ori_length;
                sint block_length;
                ubytearray ori_data, bam_data;
                subyte* current;
            public:
                Block();
                ~Block();
                void init();
                size_t read(void* dest, const size_t sz);
                void setOffset(const sushort boff);
            };

            /**
            * @class sbam::ReadBytes
            * \~english @brief BAM read byte array
            */
            class SLIB_DLL ReadBytes : public ubytearray {
            private:
                int _namelen, _cigsize, _seqsize;
            public:
                ReadBytes();
                ReadBytes(const ReadBytes& rb);
                ~ReadBytes();
                ReadBytes& operator=(const ReadBytes& rb);
                ReadBytes& operator=(subyte* data);
                void interpret();
                RefPos pos() const;
                RefPos next() const;
                String name() const;
                const subyte* seq() const;
                void expand(ubytearray& seq) const;
                void decode(String& seq) const;
                sint length() const;
                const subyte* qual() const;
                void toQuality(intarray& seq) const;
                int cigsize() const;
                const suint* cigars() const;
                void toCigarArray(CigarArray& array) const;
                sint tmplen() const;
                sushort bin() const;
                sushort flag() const;
                subyte mapq() const;
                int auxsize() const;
                const subyte* auxiliary() const;
                void clear();
                String toString(int qinflate = 33);
            };

            /**
            * @class sbam::ReadInfo
            * \~english @brief BAM read info
            */
            class SLIB_DLL ReadInfo : public AlignPair {
                friend BamFile;
            public:
                sint tmplen;
                RefPos next;
                String name, qual;
                subyte mapq;
                sushort bin, flag;
                ubytearray seq, auxiliary;
            public:
                ReadInfo();
                ReadInfo(const ReadInfo& ri);
                ~ReadInfo();
                void clear();
                String raw() const;
                String toString() const;
                bool operator<(const ReadInfo& ri) const;
                bool operator==(const ReadInfo& ri) const;
            };
        }
        using vchunk = Range<sbam::VOffset>;
        using vchunks = Region<sbam::VOffset>;
        using voffsets = Array<sbam::VOffset>;
        /**
        * @class BamFile
        * \~english @brief BAM file class
        */
		class SLIB_DLL BamFile {
            friend sbam::Block;
            friend sbam::ReadInfo;
		private:
			SFile _file;
            Array<sbam::Block> _blocks;
            CycleIterator<sbam::Block> _current;
            sbam::ReadBytes _data;
            sbam::ReadInfo _read;
            SWork _worker;
		public:
			sbam::Header info;
			sbam::Index index;
		public:
			void readHeader();
            void readBlock(sbam::Block* b);
            void readBlocks();
            void readBytes(void* dest, const size_t sz);
		public:
            BamFile(const size_t buffer_size = 2);
            BamFile(const BamFile &bam);
			~BamFile();
			void init();
			void open(const char* path);
			void close();
            //void sort();
            //void makeIndex();
            void loadIndex(const char* path);
			bool hasIndex() const;
			suinteger filesize() const;
			//suinteger offset();
			//void setOffset(suinteger off);
			String path() const;
			bool eof();
            sbam::VOffset voffset();
			void setVOffset(const sbam::VOffset& off);
            sbam::ReadBytes* read();
            sbam::ReadInfo* next();
            //operator bool() const;
		};
    }
}

#endif