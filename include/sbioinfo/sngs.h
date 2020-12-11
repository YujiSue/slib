#ifndef SNGS_SNGS_H
#define SNGS_SNGS_H

#include "sbioinfo/sbam.h"
#include "sbioinfo/sbioseq.h"

namespace slib {
    namespace sbio {
        
		struct SBIOINFO_DLL sngs_param {
			bool splitread, paired, pcrdup, parallele;
			sint bin;
			SBSeqList* ref;
			Array<sregion> target;

			sngs_param();
			sngs_param(bool sr, bool p, bool dp, sint b);
			~sngs_param();
			sngs_param& operator=(const sngs_param& par);
			void setRef(SBSeqList* r);
			void loadTarget(const char* s);
			void decodeTarget(sobj obj);
			sobj encodeTarget();
			void set(const sobj& obj);
			sobj toObj();
		};

		struct summary_data {
			sint refnum, bin;
			suinteger reads;
			double length, depth, cover;
			sveci reflen;
			svecs count;
			svecd bases;
			Array<sregion> target;

			summary_data();
			~summary_data();
			suinteger total();
			bool comparable(summary_data& dat);
			void init();
		};
		struct depth_data {
			floatarray2d count;
			uintegerarray offset;
			float current;

			depth_data();
			~depth_data();
			suinteger total();
			void init();
		};

		struct srvar_data {
			Array<vararray> variants;
			uintegerarray offset;
			svar_data current;

			srvar_data();
			~srvar_data();
			void init();
		};
        
        class SBIOINFO_DLL SNGSData {
        public:
            typedef Array<vararray> vararray2d;
            
        private:
			sio::SFile _file;
			bool _loaded;
            SWork _threads;
            std::mutex *_mtxs;
            
        public:
			summary_data summary;
			depth_data depth;
			srvar_data srvs;
            
        public:
            SNGSData();
			SNGSData(sngs_param *p);
			SNGSData(sint bin, SBSeqList* ref);
            SNGSData(const char *path);
			~SNGSData();
            
			void open(const char* path);
            void load(const char *path);
            void save(const char *path);
			void readVariant(sint r = 0, sint v = 0);
			void nextVar();
			void readDepth(sint r = 0, sint p = 0);
			void nextDp();
			
            void setNum(size_t num);
            void setLength(int idx, size_t len);
			void setBin(sint bin);
			void setParam(sngs_param *p);
            
            //void makeVIndex(Array<varparray> &index, svariant_param *vp, SWork* threads = nullptr);
			void subtract(SNGSData& dat, svariant_param* vp, SWork* threads = nullptr);
            //void tidy(svariant_param *vp);
			void integrate(SNGSData& dat, svariant_param* vp, SWork* threads = nullptr);
            
			//String brief() const;
			bool isLoaded();
			void reset(sint bin, SBSeqList* ref);
            void init();
        };
    }
}
#endif
