#ifndef SBIO_SEQSEARCH_H
#define SBIO_SEQSEARCH_H

#include "sbasic/search.h"
#include "sbasic/rarray.h"
#include "sio/stream.h"
#include "sutil/sthread.h"
#include "sbioinfo/align.h"
#include "sbioinfo/seq.h"

namespace slib {
    
    namespace sbio {
        /**
        * @class SeqSearchParam
        * 
        */
        class SLIB_DLL SeqSearchParam {
        public:
            /**
            * \~english @brief reference sequence type
            */
            sushort reftype;
            //
            int seed; // default : 12 for DNA/RNA, 3 for AA
            int seed2;
            int min_match; // default : seed
            int max_gap;  // default : 0
            int max_miss; // default : 0
            float min_score; // default : 0
            float ext_threshold; // default 1.0
            bool complement; // default : true for DNA, false for others
            bool strict; // default : false
            //
            int max_match_count; // default : 1000
            //
            int nthread; // defualt 1
            SWork threads;
            //
            AlignmentParam apar;

        public:
            SeqSearchParam();
            SeqSearchParam(const sushort t);
            ~SeqSearchParam();
            SeqSearchParam& operator=(const SeqSearchParam& p);

            int depth() const;
            void setRefType(const sushort t);
            void setSeed(int s);
            void setAsync(int n);
            void set(const sobj& obj);
            sobj toObj();
        };

        template<size_t S>
        class SeqTrie : public Trie<Pair<int, int>, S> {
        protected:
            SeqSearchParam* _par;
        public:
            RecycleArray<ubytearray> queries;
        public:
            SeqTrie() : _par(nullptr) {}
            virtual ~SeqTrie() {}
            size_t qcount() const { return queries.size(); }
            void resize(const size_t s) { queries.resize(s); }
            void reserve(const size_t s) { queries.reserve(s); }
            virtual void addQuery(const char* seq) = 0;
            virtual void addQuery(const ubytearray& seq) = 0;
            virtual void makeTrie() = 0;
            void setParam(SeqSearchParam* p) { _par = p; }
            void reset() {
                slib::Trie<Pair<int, int>, S>::clear();
                queries.clear();
            }
        };

        // ACGT(U)N only
        //class SLIB_DLL NASeqTrie : SeqTrie<6> {};
        
        // =ACMGRSVTWYHKDBN
        class SLIB_DLL DNASeqTrie : public SeqTrie<17> {
        private:
            void addQuery1(int q, const ubytearray& seq);

        public:
            DNASeqTrie();
            DNASeqTrie(SeqSearchParam* p);
            ~DNASeqTrie();
            void addQuery(const char* seq);
            void addQuery(const ubytearray& seq);
            void makeTrie();
        };
        
        // 8bit encoded seq {ACGT}*4 or {=ACMGRSVTWYHKDBN}*2
        class SLIB_DLL DNASeqTrie2 : public SeqTrie<257> {
        private:
            void addQuery2(int q, const ubytearray& seq);
            void addQuery4(int q, const ubytearray& seq);

        public:
            DNASeqTrie2();
            DNASeqTrie2(SeqSearchParam* p);
            ~DNASeqTrie2();
            void addQuery(const char* seq);
            void addQuery(const ubytearray& seq);
            void makeTrie();
        };

        //class SLIB_DLL AASeqTrie {};

        class SLIB_DLL AlignExtend {
        protected:
            SeqSearchParam* _par;
            int _rlimit, _qlimit, _rlen, _qlen, _maxlen;
            float _score;
            ubytearray _buffer;
            SEQ_CONVERTER _encoder;

        public:
            Alignment aligner;

        public:
            AlignExtend();
            AlignExtend(SeqSearchParam* p);
            ~AlignExtend();
        private:
            void _update(Sequence* ref, ubytearray* que, salign* al, slib::DIRECTION dir);
            void _extend(Sequence* ref, ubytearray* que, salign* al, slib::DIRECTION dir);
            void _deflimit(Sequence* ref, ubytearray* que, salign* al, slib::DIRECTION dir);
        public:
            void extendHead(Sequence* ref, ubytearray* que, salign* al);
            void extendHeadTo(Sequence* ref, ubytearray* que, salign* al, salign* lim);
            void extendTail(Sequence* ref, ubytearray* que, salign* al);
            void extendTailTo(Sequence* ref, ubytearray* que, salign* al, salign* lim);
            void link(Sequence* ref, ubytearray* que, salign* al1, salign* al2);
            void extend(Sequence* ref, ubytearray* que, salign* al);
            void extendExact(Sequence* ref, ubytearray* que, salign* al);
            void setParam(SeqSearchParam* p);
            void reset();
        };


        class SLIB_DLL SeqSearch {
            SeqSearchParam* _par;
            Array<AlignExtend> _extenders;
            Array<SLock> _locks;
            SWork* _threads;

        public:
            smath::Matrix<RArray<AlignPair>> aligns;
            //smath::Matrix<RecycleArray<salign>> aligns;


        public:
            SeqSearch();
            SeqSearch(SeqSearchParam* p);
            ~SeqSearch();

            void resize(size_t r, size_t q);
            void reserve(const size_t msz, const size_t asz);


            void searchAt(int r, Sequence* ref, DNASeqTrie* trie);
            void searchAt(int r, Sequence* ref, DNASeqTrie2* trie);
            void search(Sequence& ref, DNASeqTrie& trie);
            void search(Sequence& ref, DNASeqTrie2& trie);
            void search(SeqList& ref, DNASeqTrie& trie);
            void search(SeqList& ref, DNASeqTrie2& trie);
            void setThreads(SWork* w);
            void setParam(SeqSearchParam* p);
            void reset();
        };        
    }
}

#endif