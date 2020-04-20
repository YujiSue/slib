#ifndef SBIO_SBALIGN_H
#define SBIO_SBALIGN_H

#include "sobj.h"
#include "sbioinfo/sbsequtil.h"

namespace slib {
    namespace sbio {
        constexpr sshort DEFAULT_ALIGN_LENGTH = 20;
        constexpr sbyte DEFAULT_PM_SCORE = 2;
        constexpr sbyte DEFAULT_AM_SCORE = 1;
        constexpr sbyte DEFAULT_MM_SCORE = -1;
        constexpr sbyte DEFAULT_GAP_SCORE = -2;
        constexpr sbyte DEFAULT_GAP2_SCORE = -1;
        
        struct SBIOINFO_DLL salign_param {
			sbyte pm_score, am_score, mm_score, gap_score, gap2_score;
            sushort seq_type;
            sshort align_length;
            String method, score_name;
            matb score_table;
            matb compare_table;
            
            salign_param(subyte t = MISC_SEQ, const char *method = "default");
            ~salign_param();
            
            salign_param &operator=(const salign_param &par);
            
            void set(const sobj &obj);
            sobj toObj();
            void makeTable();
        };
        
        struct SBIOINFO_DLL scigar {
            static constexpr subyte MATCH = 0;
            static constexpr subyte INSERTION = 1;
            static constexpr subyte DELETION = 2;
            static constexpr subyte SKIP = 3;
            static constexpr subyte SCLIP = 4;
            static constexpr subyte HCLIP = 5;
            static constexpr subyte PADDING = 6;
            static constexpr subyte PMATCH = 7;
            static constexpr subyte MMATCH = 8;
            static const char *CIGAR_STRING;
            static const Map<char, subyte> CIGAR_INDEX;
            
            subyte option;
            sint length;
            
            scigar();
            scigar(subyte o, sint l);
            scigar(suint i);
            scigar(const scigar &c);
            
            scigar & operator = (const scigar &c);
            bool operator < (const scigar &c) const;
            bool operator == (const scigar &c) const;
            bool operator != (const scigar &c) const;
        };
        
        class SBIOINFO_DLL SCigarArray : public BiArray<scigar> {
        public:
            typedef BiArray<scigar> cigarray;
            
        public:
            SCigarArray();
            SCigarArray(size_t n);
            SCigarArray(const char *s);
            SCigarArray(const scigar &c);
            SCigarArray(sint n, suint *c);
            SCigarArray(std::initializer_list<scigar> li);
            SCigarArray(const SCigarArray &c);
            ~SCigarArray();
            
            SCigarArray &operator = (const SCigarArray &c);
            
            void add(const scigar &c);
            void append(const SCigarArray &c);
            void put(const scigar &c);
            void pile(const SCigarArray &c);
            void set(int n, suint *c);
            void reverse();
            
            size_t countRef(size_t beg = 0, size_t len = -1) const;
            size_t countQue(size_t beg = 0, size_t len = -1) const;
            size_t countCigar(subyte op);
            
			String toString() const;
            
            bool operator==(const SCigarArray &array) const;
        };
        
        struct SBIOINFO_DLL salign {
            sbpos ref;
            srange aligned;
            sint score;
            SCigarArray cigars;
            
            salign();
            salign(const sbpos &pos, const srange &range);
            salign(const salign &align);
            ~salign();
            
            salign &operator = (const salign &align);
            
            bool operator < (const salign &align) const;
            bool operator == (const salign &align) const;
            
            void scoring(salign_param *par);
            void init();
            
            String alref(const String &ref);
            String match();
            String consensus(const String &ref, const String &que);
            String alque(const String &que);
        };

		#define alignarray Array<salign>
        
        class SBIOINFO_DLL SAlignment {
            
        private:
            salign_param *_par;
            bytearray _path;
            intarray _score, _score2, _maxcol, _maxrow;
            sint _scr[4];
            
        public:
            intarray scores;
            SCigarArray cigars;
            
        public:
            SAlignment();
            SAlignment(salign_param *p);
            ~SAlignment();
            
            void align(subyte *ref, size_t rlen, subyte *que, size_t qlen);
            void ralign(subyte *ref, size_t rlen, subyte *que, size_t qlen);
            void set(salign_param *p);
            void init();
            void reset();
        };
        
        extern void calcScore(salign *al, matb *comparer, bytearray *score_list);
    }
}

#endif