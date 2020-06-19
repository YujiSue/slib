#ifndef SBIO_SBAM_H
#define SBIO_SBAM_H

////////////////////////////////////////////////////////////
#include "sobj.h"
#include "sutil/sutil.h"
#include "sbioinfo/sbalign.h"
#include "sbioinfo/svariant.h"

namespace slib {
    using namespace smath;
    using namespace sio;
    
    namespace sbio {
        //Error
#define NOT_BAM_ERR_MSG "Not BAM magic."
#define NOT_BAI_ERR_MSG "Not BAI magic."
#define NOT_BGZF_ERR_MSG "Not BGZF magic."
#define NOT_BSM_ERR_MSG "Not BGZF magic."
        
#define GZIP_STREAM_ERR_MSG "zlib stream error."
#define GZIP_DATA_ERR_MSG "zlib data error."
#define GZIP_BUFFER_ERR_MSG "zlib buffer error."
#define GZIP_INFLATE_ERR_MSG "zlib inflate error."

#define READ_FORMAT_ERR_MSG "Format is not supported."
#define READ_SIZE_ERR_MSG "Read data length is too short."
        
        //MAGIC
        constexpr char GZ_MAGIC[17] = "\037\213\010\4\0\0\0\0\0\377\6\0\102\103\2\0";
        constexpr char BAM_MAGIC[5] = "\102\101\115\001";
        constexpr char BAI_MAGIC[5] = "\102\101\111\001";
        constexpr char BSM_MAGIC[5] = "\102\123\115\002";
        
        //Constant
        constexpr int DEFAULT_THREAD_COUNT = 8;
        constexpr int DEFAULT_DEPTH_BIN = 4;
        
        constexpr int BGZF_MAX_BLOCK_SIZE = 0x10000;
        constexpr int MAX_BIN = (((1<<18)-1)/7);
        constexpr int MAX_READ_NAME_LENGTH = 0xFF;
        constexpr int BAM_INDEX_BIN = 1<<14;
        
        class SBamFile;
        
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
            struct SBIOINFO_DLL voffset {
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
            struct SBIOINFO_DLL header {
                sint ref_num;
                intarray ref_length;
                stringarray ref_name;
                String text;
                
                header();
                ~header();
                
                void set(int n);
                void init();
            };
            
            /*
             BAM read info
             */
            struct SBIOINFO_DLL readinfo {
                sint length, seq_length;
                sbpos pos;
                SCigarArray cigars;
                sushort bin, flag;
                subyte mapq;
                sint next_refid, next_pos, template_length;
                ubytearray sequence, qual;
                String name, auxiliary;
                
                readinfo();
                readinfo(const readinfo &ri);
                ~readinfo();
                
                bool headclip(int len);
                bool tailclip(int len);
				void set(ubytearray &data);
                void init();
                srange range();
                String toString();
                
                bool operator<(const readinfo &ri) const;
                bool operator==(const readinfo &ri) const;
            };
            typedef Range<sbam::voffset> voff_chunk;
            
            /*
             BAM index
             */
            typedef Array<Array<sbam::voffset, RMemory<sbam::voffset>>> voff_vec;
            typedef Array<Array<Region<sbam::voffset>>> voff_chunk_vec;
            
            struct SBIOINFO_DLL bai {
                friend SBamFile;
            private:
                char _magic[4];
                sint _lin_num, _bin_num, _chunk_num;
                suint _bin;
                suinteger _beg, _end;

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
            struct SBIOINFO_DLL bgzf_dat {
            private:
                char _magic[16];
                
            public:
                int result;
                sbam::voffset offset;
                sushort ori_length;
                sint block_length;
                subyte *ori_data, *bam_data, *current;
                
            public:
                bgzf_dat();
                ~bgzf_dat();
                
                void init();
                void load(SBamFile *bam);
                void setOffset(sushort boff);
                size_t left();
                void read(void *dest, size_t size, size_t off = 0);
            };
        }
        class SBIOINFO_DLL SBamFile : public sio::SFile {
        public:
            
        private:
            SWork _threads;
            sbam::bgzf_dat *_data, *_buffer;
            
        public:
            //sbam::readinfo read;
			ubytearray read;
            sbam::header info;
            sbam::bai index;
            
        private:
            void _swapdat();
            bool _readData(void *dest, size_t size, size_t off = 0);
            void _readHeader();
            void _checkError();
            
        public:
            SBamFile();
            SBamFile(const char *path, bool load = true);
            ~SBamFile();
            
            void init();
            
            void load(const char *path);
            void loadIndex(const char *path);
            bool hasIndex() const;
            
            sbam::voffset voff() const;
            void setVOff(const sbam::voffset &v);
            //void sort();
            //void makeIndex();
            //bool next(sbam::readinfo *ri = nullptr);
			bool next(ubytearray* dat = nullptr);
            //void reads(sbam::read_vec &list, int idx, const srange &rng);
            //void reads(sbam::read_vec &list, int idx, const sregion &reg);
        };
    }
}

#endif