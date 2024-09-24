#ifndef SBIO_ALIGN_H
#define SBIO_ALIGN_H
#include "smath/stat.h"
#include "smath/matrix.h"
#include "sobj/sobjptr.h"
#include "sobj/sdict.h"
#include "sbioinfo/seq.h"

namespace slib {
    namespace sbio {
        constexpr sbyte DEFAULT_PM_SCORE = 2;
        constexpr sbyte DEFAULT_AM_SCORE = 1;
        constexpr sbyte DEFAULT_MM_SCORE = -1;
        constexpr sbyte DEFAULT_GAP_SCORE = -2;
        constexpr sbyte DEFAULT_GAP2_SCORE = -1;
        
        struct SLIB_DLL AlignmentParam {
        public:
            subyte seqtype;

			sbyte pm_score, am_score, mm_score, gap_score, gap2_score;

            int max_length; // default: 20
            //String method;
            String score;
            smath::Matrix<sbyte> scores, compares;
        public:
            AlignmentParam(subyte t = sseq::MISC, const char* s = nullptr);
            ~AlignmentParam();
            AlignmentParam&operator=(const AlignmentParam&par);
            void setSeqType(subyte t);
            void makeTable(const char *s = nullptr);
            void set(const SObjPtr &obj);
            SObjPtr toObj();
        };
        /**
        * @namespace scigar
        */
        namespace scigar {
            constexpr subyte MATCH = 0;
            constexpr subyte INSERTION = 1;
            constexpr subyte DELETION = 2;
            constexpr subyte SKIP = 3;
            constexpr subyte SCLIP = 4;
            constexpr subyte HCLIP = 5;
            constexpr subyte PADDING = 6;
            constexpr subyte PMATCH = 7;
            constexpr subyte MMATCH = 8;
            constexpr char LABEL[10] = "MIDNSHP=X";
            extern inline subyte SLIB_DLL index(const char c);
        }
        /**
        * @class Cigar
        * ~\english @brief Cigar class for NGS alignment
        */
        class SLIB_DLL Cigar {
        public:
            subyte option;
            sint length;
        public:
            Cigar();
            Cigar(const subyte o, const sint l);
            Cigar(const suint i);
            Cigar(const Cigar&c);
            Cigar& operator = (const Cigar&c);
            bool ref() const;
            bool que() const;
            bool gap() const;
            bool clip() const;
            bool operator < (const Cigar&c) const;
            bool operator == (const Cigar&c) const;
            bool operator != (const Cigar&c) const;
        };
        /**
        * @class CigarArray
        */
        class SLIB_DLL CigarArray {
            Cigar* _ptr, * _offset, * _end;
            size_t _capacity;
        public:
            CigarArray();
            CigarArray(const size_t n);
            CigarArray(const char *s);
            CigarArray(const Cigar&c);
            CigarArray(const sint n, const suint *c);
            CigarArray(CigarArray&& array) noexcept;
            CigarArray(const CigarArray &c);
            ~CigarArray();
            CigarArray& operator = (CigarArray&& array) noexcept;
            CigarArray &operator = (const CigarArray &c);
            
            bool empty() const;
            size_t size() const;
            size_t capacity() const;
            size_t offset() const;
            void setOffset(const size_t off);
            void resize(const size_t sz);
            void expand(const size_t sz, slib::DIRECTION dir);
            void reserve(const size_t sz, const size_t off = -1);
            bool clipped(const size_t sz) const;
            ArrayIterator<Cigar> iterAt(const int i);
            ArrayCIterator<Cigar> iterAt(const int i) const;
            Cigar& at(const int i);
            const Cigar& at(const int i) const;
            Cigar& operator[](const int i);
            const Cigar& operator[](const int i) const;
            ArrayIterator<Cigar> begin();
            ArrayCIterator<Cigar> begin() const;
            ArrayIterator<Cigar> end();
            ArrayCIterator<Cigar> end() const;
            void add(const Cigar &c, DIRECTION dir = DIRECTION::TAIL);
            void append(const CigarArray &c, DIRECTION dir = DIRECTION::TAIL);
            void set(const int n, const suint *c);
            void remove(const size_t off, const size_t len = -1);
            void reverse();
            void clear();
            void swap(CigarArray& array);
            size_t refSize(size_t beg = 0, size_t len = -1) const;
            size_t queSize(size_t beg = 0, size_t len = -1) const;
            size_t countCigar(const subyte op);
            size_t sizeofOP(const subyte op);
            String toString() const;
            bool operator==(const CigarArray &array) const;
        };
        /**
        * @class AlignPair
        */
        class SLIB_DLL AlignPair {
        public:
            sbpos ref;
            srange query;
            float score;
            CigarArray cigars;
        public:
            AlignPair();
            AlignPair(const sbpos& ref, const srange &que);
            AlignPair(const AlignPair&align);
            ~AlignPair();
            AlignPair& operator = (const AlignPair& align);
            void scoring(AlignmentParam*par);
            void complement(const size_t sz);
            void clear();
            String alref(const String &ref);
			String match();
            String consensus(const String &ref, const String &que);
			String alque(const String &que);
            bool operator < (const AlignPair& align) const;
            bool operator == (const AlignPair& align) const;
        };
		#define alignarray Array<salign>
        /**
        * @class Alignment
        */
        class SLIB_DLL Alignment {
        private:
            AlignmentParam* _par;
            smatb _path;
            smati _score;
            sveci _maxcol, _maxrow;
            svec4i _buffer;
        public:
            sveci scores;
            CigarArray cigars;

        public:
            Alignment();
            Alignment(AlignmentParam* p);
            ~Alignment();
        private:
            void _align1(subyte* ref, int rlen, subyte* que, int qlen);
            void _align2(subyte* ref, int rlen, subyte* que, int qlen);
        public:
            void align(subyte* ref, int rlen, subyte* que, int qlen, bool dir = true);
            void setParam(AlignmentParam* p);
            void reset();
        };

        namespace sutil {
            extern SLIB_DLL void trimAlignHead(AlignPair &al, int rlim, int qlim = -1);
            extern SLIB_DLL void trimAlignTail(AlignPair& al, int rlim, int qlim = -1);
            extern SLIB_DLL void trimHeadTo(srange& ref, srange& que, CigarArray& cigs, int lim, bool rev);
            extern SLIB_DLL void trimTailTo(srange& ref, srange& que, CigarArray& cigs, int lim, bool rev);
            extern SLIB_DLL void trimTailTo(srange& ref, srange& que, CigarArray& cigs, int lim, bool rev);
            extern SLIB_DLL void trimOver(srange& ref1, srange& que1, CigarArray& cigs1, srange& ref2, srange& que2, CigarArray& cigs2, bool rev);
        }
    }
    using salign = sbio::AlignPair;
}

#endif