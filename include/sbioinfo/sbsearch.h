#ifndef SBIO_SBSEARCH_H
#define SBIO_SBSEARCH_H

#include "smath/la.h"
#include "sutil/sthread.h"
#include "sbioinfo/sbalign.h"
#include "sbioinfo/sbioseq.h"

namespace slib {
    using namespace smath;
    
    namespace sbio {
        
        constexpr sint DEFAULT_MIN_SIZE = 20;
        constexpr sint DEFAULT_SEED = 12;
        constexpr sint DEFAULT_AASEED = 3;
        constexpr sint DEFAULT_MAX_GAP = 0;
        constexpr sint DEFAULT_MAX_MISS = 1;
        constexpr sint DEFAULT_THREAD = 8;
        constexpr sint DEFAULT_COMPRESS = 1;
        constexpr double DEFAULT_THRESHOLD = 1.0;
        
        struct SBIOINFO_DLL sbsearch_param {
            sushort ref_type, code_size;
            bool ds_search, multi_thread;
            sint min_match, seed_len, coded_seed_len, max_gap, max_miss, thread_count;
            double extend_threshold;
            
            salign_param aln_par;
            
            sbsearch_param(uint16_t t = MISC_SEQ);
            ~sbsearch_param();
            
            sbsearch_param &operator=(const sbsearch_param &par);
            
            void setType(int t);
            void setSeed(int s);
            void set(const sobj &obj);
            sobj toObj();
        };
        
        struct SBIOINFO_DLL spma {
            typedef Array<spma *, CMemory<spma*>> pma_vec;
            typedef Array<std::pair<sint, sint>, RMemory<std::pair<sint, sint>>> pair_vec;
            
            pma_vec child;
            pair_vec match;
            
            spma();
            spma(int n);
            spma(const spma &p);
            ~spma();
            
            spma &operator=(const spma &p);
            
            void add(int i1, int i2);
            void resize(int n);
            void init();
        };
        
        class SBIOINFO_DLL SBQuery {
            typedef Array<spma> pma_vec;
            
        private:
            sbsearch_param *_par;
            ubytearray2d _seqs;
            pma_vec _pmas;
            
            int _pma_size;
            size_t _total_length;
            
            SEQ_CONVERTER converter;
            std::function<short(subyte *, sbyte)> encoder;
            std::function<bool(subyte*, sbyte)> isAvailable;
            
        private:
            void complete();
            
        public:
            SBQuery();
            SBQuery(sbsearch_param *p);
            ~SBQuery();
            
            spma *root();
            
            ubytearray &query(int idx);
            const ubytearray &query(int idx) const;
            size_t count() const;
            void setSize(size_t s);
            
            void addQuery(const char *seq);
			void addQuery(ubytearray *seq, size_t off = 0, size_t len = -1);
            void addDSQuery(const char *seq);
            
            void setTotalLength(size_t len);
            /*
            void addQuery(const ubytearray &seq);
            void addDSQuery(const ubytearray &seq);
            */
            void makeTrie();
            void makeStrictTrie();
            
            void setParam(sbsearch_param *p);
            void reset();
            
        };
		class SBIOINFO_DLL SBExtend {
			sbsearch_param* _par;
			subyte* _ref, * _que;
			int _rlen, _qlen, _rl, _ql, _len, * _score;
			float _s;
			bool _ext;
			ubytearray _ref_seq;

		public:
			SAlignment align;

		private:
			void _extendExactHead(salign* al);
			void _extendExactTail(salign* al);
			void _extendHead(salign* al);
			void _extendTail(salign* al);

		public:
			SBExtend();
			SBExtend(sbsearch_param* p);
			~SBExtend();

			void extendHead(SBioSeq* ref, ubytearray* que, salign* al);
			void extendTail(SBioSeq* ref, ubytearray* que, salign* al);
			void extend(SBioSeq* ref, ubytearray* que, salign* al);
			bool joint(SBioSeq* ref, ubytearray* que, salign* a1, salign* a2);
			void assemble(SBioSeq* ref, ubytearray* que, Array<salign>* aligns);

			void setParam(sbsearch_param* p);
			void reset();
		};
        class SBIOINFO_DLL SBSearch {
        public:
            typedef Array<std::pair<sint, sint>, RMemory<std::pair<sint, sint>>> match_array;
            
        private:
            sbsearch_param *_par;
            SBQuery *_que;
			UArray<SBExtend> _extender;
			SBExtend _ext;
            SWork _threads;
            
            sint _qnum, _rnum;
            Array<match_array> _matched;
            
        public:
            Array<Array<salign>> aligns;
            
        private:
            void _resize(size_t rn, size_t qn);
            
        public:
            SBSearch();
            SBSearch(sbsearch_param *p);
            ~SBSearch();
            
            void search(SBioSeq *ref, SBQuery *que);
            void search(SBSeqList *ref, SBQuery *que);
			void searchEx(SBioSeq* ref, SBQuery* que);
			void searchEx(SBSeqList* ref, SBQuery* que);
            void makeAlign();
            
            void setParam(sbsearch_param *p);
            void reset();
        };
        
        
    }
}

#endif