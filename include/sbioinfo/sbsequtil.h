#ifndef SBIO_SBSEQUTIL_H
#define SBIO_SBSEQUTIL_H

#include "sobj.h"
#include "sbioinfo/sbioinfoutil.h"

namespace slib {
    using namespace smath;
    namespace sbio {
        //seq type
        constexpr sushort MISC_SEQ = 0x0000;
        constexpr sushort DNA_SEQ = 0x0001;
        constexpr sushort RNA_SEQ = 0x0002;
        constexpr sushort AA_SEQ = 0x0004;
        //seq coding
        constexpr sushort COMPRESS1 = 0x0010;
        constexpr sushort COMPRESS2 = 0x0020;
        constexpr sushort COMPRESS4 = 0x0040;
        //seq extra info
        constexpr sushort MASKED = 0x0100;
        constexpr sushort SCORED = 0x0200;
        constexpr sushort ANNOTATED = 0x0400;
		constexpr sushort ATTRIBUTED = 0x0800;
        //seq form
        constexpr sushort CIRCULAR = 0x1000;
        
        #define CIRC_DNA_SEQ CIRCULAR|DNA_SEQ
        #define DNA_SEQ1 COMPRESS1|DNA_SEQ
        #define CIRC_DNA_SEQ1 CIRCULAR|COMPRESS1|DNA_SEQ
        #define DNA_SEQ2 COMPRESS2|DNA_SEQ
        #define DNA_SEQ4 COMPRESS4|DNA_SEQ
        #define RNA_SEQ1 COMPRESS1|RNA_SEQ
        #define AA_SEQ1 COMPRESS1|AA_SEQ
        
        #define cc(X,Y) std::pair<char,char>((X),(Y))
        #define cu(X,Y) std::pair<char,subyte>((X),(Y))
        #define CODON_TABLE smat<svec4d<sbyte>>
        
		//------------------------------------------------------------

		extern void seqform(String& str);
		extern subyte seqtype(String& str);

		extern subyte maskByte(sushort type);
		extern char maskChar(sushort type);

        extern void rawcopy(const subyte *ori, size_t pos, size_t len, subyte *seq);
        
        #define BASE_CODER std::function<void(subyte &b, char *s)>
        #define SEQ_CONVERTER std::function<void(const subyte *, size_t, size_t, subyte *)>
        
        constexpr char DNA_BASE16[17] = "NACMGRSVTWYHKDBN";
        constexpr char DNA_BASE4[6] = "ACGTN";
        
        extern Map<char, subyte> DNA_BASE16_INDEX;
        extern Map<char, subyte> DNA_BASE4_INDEX;
        extern Map<char, char> DNA_COMPLEMENT_CHAR;
        extern bytearray DNA_COMPLEMENT_IDX;
        extern smat<SEQ_CONVERTER> DNA_CONVERTER;
        
        constexpr char RNA_BASE[6] = "ACGUN";
        
        extern Map<char, subyte> RNA_BASE_INDEX;
        extern Map<char, char> RNA_COMPLEMENT_CHAR;
        extern smat<SEQ_CONVERTER> RNA_CONVERTER;
        
        constexpr char AMINO_ACID[27] = "ARNDCQEGHILKMFPSTWYVBJZX*";
        
        extern Map<char, subyte> AMINO_ACID_INDEX;
        extern smat<SEQ_CONVERTER> AA_CONVERTER;
        extern CODON_TABLE DEFAULT_CODON;

		struct SBIOINFO_DLL sbseq_annot {
			suint type;
			String name;
			sbpos pos;
			sbseq_annot* prev, *next;
			sdict attribute;

			sbseq_annot();
			sbseq_annot(suint i, suint t, const char* n, const sbpos& p);
			sbseq_annot(const sbseq_annot& a);
			~sbseq_annot();

			sbseq_annot& operator=(const sbseq_annot& a);
			sbseq_annot& operator+=(const sbseq_annot& a);
			sbseq_annot& operator-=(const sbseq_annot& a);
			sbseq_annot& operator/=(const sbseq_annot& a);

			void join(sbseq_annot *annot)

			String toString(const char *format) const;

			bool operator<(const sbseq_annot& a) const;
			bool operator==(const sbseq_annot& a) const;
		};
        
        namespace sseq {
            /*
             * NA util
             */
            extern bool isATGC(const char &s);
            extern bool isATGCi(const subyte &b);
            extern bool isGC(const char &s);
            extern bool isGCi(const subyte &b);
            extern size_t gcCount(const char *s);
            extern size_t gcCounti(const ubytearray &s);
            extern bool containBase(const char &c, const char *s, size_t l);
            
            /*
             * DNA util
             */
            extern void draw(const subyte &b, char *s);
            extern void ddec10(const subyte &b, char *s);
            extern void ddec20(const subyte &b, char *s);
            extern void ddec40(const subyte &b, char *s);
            extern void denc02(subyte &b, const char *s);
            extern void denc04(subyte &b, const char *s);
            extern subyte b24(subyte s);
            extern subyte b42(subyte s);
            extern void ddec21(const subyte &b, subyte *s);
            extern void ddec41(const subyte &b, subyte *s);
            extern void denc12(subyte &b, const subyte *s);
            extern void denc14(subyte &b, const subyte *s);
            
            extern void ddecode1(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void ddecode2(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void ddecode4(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void dencode1(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void dencode2(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void dencode4(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void dexpand2(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void dexpand4(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void dcompress2(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void dcompress4(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void drecode22(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void drecode24(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void drecode42(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void drecode44(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void dcomp(char *seq, size_t s = -1);
            extern void dcomp(String &seq);
            //extern void dcomp(std::string &seq);
            extern void dcompi(ubytearray &seq);
            extern void dcpycomp(const char *seq, char *cseq);
            extern String dcompseq(const char *seq);
            extern void dcpycompi(ubytearray &seq, ubytearray &cseq);
            extern ubytearray dcompseqi(ubytearray &seq);
            
            /*
             * RNA util
             */
            extern void rdecode(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void rencode(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void rcomp(char *seq, size_t s = -1);
            extern void rcomp(String &seq);
            extern void rcompi(ubytearray &seq);
            extern void rcpycomp(const char *seq, char *cseq);
            extern String rcompseq(const char *seq);
            extern void rcpycompi(ubytearray &seq, ubytearray &cseq);
            extern ubytearray rcompseqi(ubytearray &seq);
            extern void dtrans(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void dtransi(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void rtrans(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void rtransi(const subyte *ori, size_t pos, size_t length, subyte *seq);
            
            /*
             * Amino acid util
             */
            extern void adecode(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void aencode(const subyte *ori, size_t pos, size_t length, subyte *seq);
            extern void atrans(const subyte *ori, size_t pos, size_t length, subyte *seq, const CODON_TABLE &codon);
        }
    }
}

#endif
