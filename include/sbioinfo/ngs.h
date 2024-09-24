#ifndef SNGS_SNGS_H
#define SNGS_SNGS_H
#include "smath/stat.h"
#include "sio/sfile.h"
#include "sio/stream.h"
#include "sutil/sthread.h"
#include "sbioinfo/variant.h"
#include "sbioinfo/bam.h"


namespace slib {
    namespace sbio {
		namespace sngs {
			enum class SEQ_TYPE : subyte {
				SINGLE = 0x00,
				PAIRED = 0x01,
				LONG = 0x02,
			};

			class SLIB_DLL Summary {
			public:
				//
				String seqid;
				String sample;
				String platform;
				//
				subyte seqtype;
				//
				sint refnum;
				sveci reflen;
				//
				suinteger total;
				sveci count;
				double avelen;
				double avedp;
				svecd bases;
				double cover;
				//
				sint bin;
			public:
				Summary();
				~Summary();
				bool comparable(Summary& sum);
				void reset(int n = -1, sint *l = nullptr);
			};
			extern bool comparable(Summary& sum1, Summary& sum2);
		}
        
		class SLIB_DLL NGSData {
		private:
			SFile _file;
			uintegerarray _index;
			bool _loaded;

		public:
			sngs::Summary summary;
			Array<svecf> depth;
			Array<Array<SVar>> variants;

		public:
			NGSData(slib::sbio::SeqList* ref = nullptr, sngs::SEQ_TYPE type = sngs::SEQ_TYPE::SINGLE, int bin = 1);
			NGSData(const char* path);
			~NGSData();

			bool isLoaded();
			String path() const;
			void open(const char* path);
			void load(const char* path);
			void save(const char* path);
			float* readDp(int idx, size_t off = 0, size_t len = -1, float *dp = nullptr);
			
			void setSource(BamFile& bam);
			void setRefNum(int n);
			void setRefLength(int i, int l);
			void setBin(int bin);
			void addVariant(SVar& v);

			void print(IOStream& strm = DEFAULT_OSTREAM);
			void reset(int n = -1, sint* l = nullptr);
        };
    }
}
#endif
