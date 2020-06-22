#ifndef SNGS_SNGS_H
#define SNGS_SNGS_H

#include "sbioinfo/sbam.h"
#include "sbioinfo/sbioseq.h"

namespace slib {
    namespace sbio {
        
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

			depth_data();
			~depth_data();

			void init();
		};

		struct srvar_data {
			Array<vararray> variants;
			uintegerarray offset;
			//Array<varparray> index;
			/*
			Array<Array< delidx, insidx, invidx;
			intarray3d trsidx, trinvidx;
			*/
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
			//Array<SLock> _locks;
            
        public:
			summary_data summary;
			depth_data depth;
			srvar_data srvs;

			/*
            sint ref_num, depth_bin;
            intarray ref_length, depth_size, uncovered;
            suinteger total_length, total_reads;
            bool target_seq;
            Array<sregion> target;
            integerarray read_count;
            double average_length, average_depth, covered_region;
            doublearray read_length;
            
			floatarray2d depth;

            vararray2d variants;
			intarray2d delidx, insidx, invidx;
            intarray3d trsidx, trinvidx;
			*/
            
        public:
            SNGSData();
			SNGSData(sint bin, SBSeqList* ref);
            //SNGSData(sint bin, SBamFile *bam);
            SNGSData(const char *path);
			~SNGSData();
            
			void open(const char* path);
            void load(const char *path);
            void save(const char *path);
            
            void setNum(sint num);
            void setLength(int idx, sint len);
			void setBin(sint bin);

            //void setParam(sngs_param *p);
            //void lock(int r, int v = 0);
            //void unlock(int r, int v = 0);
            //double depthAt(const sbpos &pos);

            void makeVIndex(Array<varparray> &index, svariant_param *vp, SWork* threads = nullptr);
			void subtract(SNGSData& dat, svariant_param* vp, SWork* threads = nullptr);
            //void tidy(svariant_param *vp);
			void integrate(SNGSData& dat, svariant_param* vp, SWork* threads = nullptr);
            
			bool isLoaded();
			void reset(sint bin, SBSeqList* ref);
            void init();
        };
    }
}
#endif
