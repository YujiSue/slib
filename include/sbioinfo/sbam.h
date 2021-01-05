#ifndef SBIO_SBAM_H
#define SBIO_SBAM_H

////////////////////////////////////////////////////////////
#include "sobj.h"
#include "sutil/sutil.h"
#include "sbioinfo/sbalign.h"
#include "sbioinfo/sngs.h"

namespace slib {
    using namespace smath;
    using namespace sio;
    
    namespace sbio {
        //Error
#define NOT_BAM_ERR_MSG "Not BAM magic."
#define NOT_BAI_ERR_MSG "Not BAI magic."
#define NOT_BGZF_ERR_MSG "Not BGZF magic."
        
#define GZIP_STREAM_ERR_MSG "zlib stream error."
#define GZIP_DATA_ERR_MSG "zlib data error."
#define GZIP_BUFFER_ERR_MSG "zlib buffer error."
#define GZIP_INFLATE_ERR_MSG "zlib inflate error."

#define READ_FORMAT_ERR_MSG "Format is not supported."
#define READ_SIZE_ERR_MSG "Read data length is too short."
        
        
        //Constant
        constexpr int DEFAULT_THREAD_COUNT = 8;
        constexpr int DEFAULT_DEPTH_BIN = 4;
        
        constexpr int BGZF_MAX_BLOCK_SIZE = 0x10000;
        constexpr int MAX_BIN = (((1<<18)-1)/7);
        constexpr int MAX_READ_NAME_LENGTH = 0xFF;
        constexpr int BAM_INDEX_BIN = 1<<14;
        
        class SBam;
        
        namespace sbam {
            //Constant
            constexpr sushort MULTI_SEGMET_READ = 0x0001;
            constexpr sushort PROPER_ALIGNED_READ = 0x0002;
            constexpr sushort UNMAPPED_READ = 0x0004;
            constexpr sushort NEXT_UNMAPPED_READ = 0x0008;
            constexpr sushort COMPLEMET_READ = 0x0010;
            constexpr sushort NEXT_REVERSE_READ = 0x0010;
            constexpr sushort FIRST_SEGMENT = 0x0040;
            constexpr sushort LAST_SEGMENT = 0x0080;
            constexpr sushort SECONDARY_ALIGN_READ = 0x0100;
            constexpr sushort QUALITY_FILTERED = 0x0200;
            constexpr sushort PCR_DUPLICATE = 0x0400;
            
            /*
             Virtual offset
             */
			class SBIOINFO_DLL voffset {
			public:
                sinteger file_offset;
                sushort block_offset;

                voffset();
                voffset(sinteger fo, sushort bo);
                voffset(suinteger offset);
                voffset(const voffset &v);
                ~voffset();                
                voffset & operator = (const voffset &v);
                suinteger intOffset();
                bool operator < (const voffset &v) const;
                bool operator == (const voffset &v) const;
            };
            /*
             BAM header
             */
			class SBIOINFO_DLL header {
			public:
                sint ref_num;
                intarray ref_length;
                stringarray ref_name;
                String text;

				header();
                ~header();
                void set(int n);
				String toString() const;
                void init();
            };
            /*
             BAM read info
             */
            class SBIOINFO_DLL readinfo {
			public:
				sint seqlen, tmplen;
                sbpos pos, next;
                SCigarArray cigars;
                sushort bin, flag;
                subyte mapq;                
			    ubytearray sequence, qual, auxiliary;
				String name;

                readinfo();
				readinfo(ubytearray &dat);
                readinfo(const readinfo &ri);
                ~readinfo();
				
				static sint getRef(ubytearray& data);
				static sint getPos(ubytearray& data);
				static sint getLen(ubytearray& data);
				static sushort getBin(ubytearray& data);
				static sushort getFlag(ubytearray& data);
				static subyte getMapQ(ubytearray& data);
				void init();
				void interpret(ubytearray& data, bool aux = false);
				srange range();
				String decode();
                String toString();
                bool operator<(const readinfo &ri) const;
                bool operator==(const readinfo &ri) const;
            };
			typedef Array<sbam::readinfo> read_array;
            /*
             BAM index
             */
			typedef Range<sbam::voffset> voff_chunk;
			typedef Array<Array<sbam::voffset, RMemory<sbam::voffset>>> voff_vec;
            typedef Array<Array<Region<sbam::voffset>>> voff_chunk_vec;
            
			class SBIOINFO_DLL bai {
                friend SBam;
            public:
                sint ref_num;
                voff_chunk_vec chunks;
                voff_vec loffset;
				Array<Map<suint, suint>> bin_map;
                
            private:
                void setNum(int n);
                
            public:
                bai();
                bai(const char *path);
                ~bai();
                void load(const char *path);
                //void save(const char *path);
                void init();
            };
            /*
             BAM data
             */
			class SBIOINFO_DLL bgzf_dat {
            private:
                char _magic[16];
                
            public:
                int result;
                sbam::voffset offset;
                sushort ori_length;
                sint block_length;
				ubytearray ori_data, bam_data;
                subyte /**ori_data, *bam_data,*/ *current;
                
            public:
                bgzf_dat();
                ~bgzf_dat();
                
                void init();
                void load(SBam *bam);
                void setOffset(sushort boff);
                size_t left();
                void read(void *dest, size_t size, size_t off = 0);
            };

			

        }

		class SBIOINFO_DLL SBamFile;
		class SBIOINFO_DLL SBamData {
		private:
			char _magic[16];
			SBamFile* _bam;
			SWork _thread;
		public:
			sint idx, count;
			sbam::voffset offset, last;
			ushortarray ori_length;
			intarray block_length;
			ubytearray buffer[2], readinfo, * current;
			Array<ubytearray> raws;
			subyte* curptr;
			bool async;
		public:
			SBamData();
			SBamData(sngs_param* p);
			~SBamData();
			void init();
			void resize(sint n);
			void setBam(SBamFile* f);
			void load();
			void update();
			void setOffset(sushort o);
			void copy(void* p, size_t s);
			ubytearray* next();
			void swap();
		};
		class SBIOINFO_DLL SBamFile {
			friend SBamData;
		private:
			sio::SFile _file;
			SBamData _data, *_current;
			SLock _lock;
		public:
			sbam::header info;
			sbam::bai index;
		private:
			void _readHeader();
			void _checkError();
		public:
			SBamFile();
			~SBamFile();
			void init();
			void open(const char* path);
			void close();
			void loadIndex(const char* path);
			bool hasIndex() const;
			void lock();
			void unlock();
			suinteger filesize() const;
			suinteger offset();
			void setOffset(suinteger off);
			String path() const;
			bool eof();
			sbam::voffset voff() const;
			void setVOff(const sbam::voffset& off);
			void readBytes(void *p, size_t s);
			//void sort();
			//void makeIndex();
			SBamData *getData();
			void setData(SBamData* dat);
			ubytearray* next(ubytearray* dat = nullptr);
			void getReads(sbam::read_array& array, const sbpos& pos);
			void getReads(sbam::read_array& array, sint idx, const sregion& region);
		};


        class SBIOINFO_DLL SBam {
			friend sbam::bgzf_dat;

        private:
			sio::SFile _file;
            SWork _threads;
            sbam::bgzf_dat *_data, *_buffer;
            
        public:
            ubytearray read;
            sbam::header info;
            sbam::bai index;
            
        private:
            void _swapdat();
            bool _readData(void *dest, size_t size, size_t off = 0);
            void _readHeader();
            void _checkError();
            
        public:
			SBam();
			SBam(const char *path, bool load = true);
            ~SBam();
            
            void init();
            void open(const char* path);
            void load(const char *path);
			void close();
            void loadIndex(const char *path);
            bool hasIndex() const;
            
			suinteger size() const;
			suinteger offset();
			String path() const;

            sbam::voffset voff() const;
            void setVOff(const sbam::voffset &v);
            //void sort();
            //void makeIndex();
			ubytearray *next(ubytearray *dat = nullptr);
			void getReads(sbam::read_array& array, const sbpos &pos);
			void getReads(sbam::read_array& array, sint idx, const sregion& region);
        };
    }
}

#endif