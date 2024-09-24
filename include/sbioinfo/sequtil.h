#ifndef SBIO_SEQUTIL_H
#define SBIO_SEQUTIL_H
#include "smath/matrix.h"
#include "sobj/sdict.h"
#include "sbioinfo/biutil.h"
namespace slib {
    namespace sbio {
        class SLIB_DLL AnnotInfo;

        constexpr char DNA_BASE4[6] = "ACGTN";
        constexpr char DNA_BASE16[17] = "=ACMGRSVTWYHKDBN";
        constexpr char RNA_BASE[6] = "ACGUN";
        constexpr char AMINO_ACID[26] = "ARNDCQEGHILKMFPSTWYVBJZX*";

        constexpr sushort MISC_SEQ = (0x0100 | sseq::MISC);
        constexpr sushort DNA_SEQ = (0x0100 | sseq::DNA);
        constexpr sushort CIRCULAR_DNA_SEQ = (0x0100 | sseq::DNA | sseq::CIRCULAR);
        constexpr sushort DNA_SEQ2 = (0x0200 | sseq::DNA);
        constexpr sushort DNA_SEQ4 = (0x0400 | sseq::DNA);
        constexpr sushort RNA_SEQ = (0x0100 | sseq::RNA);
        constexpr sushort AA_SEQ = (0x0100 | sseq::AA);
        
        using SEQ_CONVERTER = std::function<void(const subyte*, const size_t, const size_t, subyte*)>;

        class SLIB_DLL SeqNote {
            slib::Region<sint> _region;
        public:
            bool dir;
            sushort category;
            suint type;
            String name, note;
            
        public:
            SeqNote();
            SeqNote(const Range<sint>& rng, bool dir = false, const subyte c = 0, const suint t = 0, const char* s = nullptr, const char* n = nullptr);
            SeqNote(const Region<sint>& reg, bool dir = false, const subyte c = 0, const suint t = 0, const char* s = nullptr, const char* n = nullptr);
            SeqNote(const AnnotInfo& info);
            SeqNote(const SeqNote &note);
            ~SeqNote();
            SeqNote& operator=(const SeqNote& note);
            size_t count() const;
            const Range<sint>& range() const;
            const Region<sint>& region() const;
            void setRange(const srange& range);
            void addRange(const srange& range);
            void setRegion(const sregion& region);
            void shift(const int i);
            void complement(const size_t sz);
            bool include(const int pos) const;
            bool overlap(const srange& range) const;
        };
        SeqNote shift(const SeqNote& sn, const int i);

        namespace sutil {
            extern SLIB_DLL String seqTypeName(const subyte type);
            extern SLIB_DLL String seqTypeUnit(const subyte type);

            extern SLIB_DLL String gbkKey(const SeqNote& note);
            extern SLIB_DLL String gbkPos(const srange& rng, const size_t sz = -1, bool dir = false);
            extern SLIB_DLL String gbkPos(const sregion& reg, const size_t sz = -1, bool dir = false);
            extern SLIB_DLL String gbkPos(const SeqNote& note, const size_t sz);
            extern SLIB_DLL String toGBK(const SeqNote &note, const size_t sz);

        }
        namespace sseq {
            // Util
            extern SLIB_DLL void format(String& seq);
            extern SLIB_DLL subyte checkType(String& seq);
            extern SLIB_DLL char maskChar(const subyte type);

            // Converter
            extern SLIB_DLL void rawcopy(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void reverse(const subyte* ori, const size_t pos, const size_t length, subyte* seq);

            extern SLIB_DLL void transcribe(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void rtranscribe(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void translate(const subyte* ori, const size_t pos, const size_t length, subyte* seq, const CODON_TABLE& codon = DEFAULT_CODON);
        }
        /**
        * @namespace sna
        * \~english @brief Nucleic acid related functions
        */
        namespace sna {
            constexpr char mask = 'N';
            extern SLIB_DLL size_t gcCount(const char* seq, const size_t length);
            extern SLIB_DLL size_t gcCount(const String& seq);
            extern SLIB_DLL size_t gcCount(const subyte* seq, const size_t length);
            extern SLIB_DLL size_t gcCount(const ubytearray& seq);

            extern SLIB_DLL subyte encBase4(const char c);
            extern SLIB_DLL subyte encBase16(const char c);
            extern SLIB_DLL char compBase(const char b);
            extern SLIB_DLL subyte comp4(const subyte b);
            extern SLIB_DLL subyte comp16(const subyte b);

            extern SLIB_DLL void complement1(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void complement4(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void complement16(const subyte* ori, const size_t pos, const size_t length, subyte* seq);

            extern SLIB_DLL String complement(const String& seq);
            extern SLIB_DLL void toComplement(String& seq);
            extern SLIB_DLL void toComplement4(ubytearray& seq);
            extern SLIB_DLL void toComplement16(ubytearray& seq);

        }
        /**
        * @namespace sdna
        * \~english @brief DNA specific functions
        */
        namespace sdna {
            extern SLIB_DLL void dec2(const subyte b, char* s);
            extern SLIB_DLL void dec4(const subyte b, char* s);
            extern SLIB_DLL subyte b24(subyte b);
            extern SLIB_DLL subyte b42(subyte b);
            extern SLIB_DLL subyte enc2(const char* s);
            extern SLIB_DLL subyte enc4(const char* s);
            extern SLIB_DLL void ddec21(const subyte b, subyte* s);
            extern SLIB_DLL void ddec41(const subyte b, subyte* s);
            extern SLIB_DLL void encode(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void encode2(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void encode4(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void decode(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void decode2(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void decode4(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void compress2(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void compress4(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void expand2(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void expand4(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL String transcript(const String& seq);
            extern SLIB_DLL Pair<int, svec3d> meltTemp(const String& seq);
            extern SLIB_DLL Pair<int, svec3d> meltTemp(const ubytearray& seq);
        }
        /**
        * @namespace sdna
        * \~english @brief RNA specific functions
        */
        namespace srna {
            extern SLIB_DLL void encode(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void decode(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL String rtranscript(const String& seq);
            extern SLIB_DLL String protein(const String& seq, const CODON_TABLE &codon = DEFAULT_CODON);
        }
        /**
        * @namespace saa
        * \~english @brief Amino acid specific functions
        */
        namespace saa {
            constexpr char mask = 'X';
            extern SLIB_DLL subyte encAA(const char c);
            extern SLIB_DLL void encode(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
            extern SLIB_DLL void decode(const subyte* ori, const size_t pos, const size_t length, subyte* seq);
        }

    }
}

#endif
