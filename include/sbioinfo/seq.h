#ifndef SBIO_SEQ_H
#define SBIO_SEQ_H
#include "sbasic/parray.h"
#include "sobj/sobjptr.h"
#include "sio/sfile.h"
#include "sbioinfo/sequtil.h"
#include "sbioinfo/fasta.h"
#include "sbioinfo/annotation.h"

namespace slib {
    namespace sbio {
        class SLIB_DLL Sequene;
        class SLIB_DLL SeqList;
        class SLIB_DLL AnnotDB;

        class SLIB_DLL SeqAnnotation {
            Array<SeqNote> _annot;
        public:
            SeqAnnotation();
            ~SeqAnnotation();

            size_t count() const;
            bool annotated(const int pos) const;
            bool annotated(const srange& rng) const;
            ArrayIterator<SeqNote> begin();
            ArrayCIterator<SeqNote> begin() const;
            ArrayIterator<SeqNote> end();
            ArrayCIterator<SeqNote> end() const;

            void addNote(const SeqNote& note);
            
            void shift(const int i);
            void clear();
        };
        class SLIB_DLL Sequence : public ubytearray {
            friend Fasta;
            friend SeqList;
        protected:
            sushort _type;
            size_t _length;

        public:
            String name;
            sregion mask;
            SeqAnnotation annotation;
            SDictionary attribute;

        public:
            Sequence();
            Sequence(const sushort t);
            Sequence(const char* seq, const sushort t = 0xFFFF);
            Sequence(Sequence&& seq) noexcept;
            Sequence(const Sequence& seq);
            ~Sequence();
            Sequence& operator=(const Sequence& seq);
            Sequence& operator<<(const char* seq);
            Sequence& operator<<(const Sequence& seq);
            
            bool isDNA() const;
            bool isRNA() const;
            bool isAA() const;
            bool isMisc() const;
            bool isCircular() const;
            bool isMasked() const;
            bool isAnnotated() const;
            bool loaded() const;

            subyte type() const;
            subyte compression() const;
            size_t length() const;
            SEQ_CONVERTER decoder() const;
            SEQ_CONVERTER encoder() const;
            void setSeq(const char* seq);
            void setSeqAs(const char* seq, const sushort t);
            void setLength(const size_t len, bool resize = true);
            void convert(const sushort t, const CODON_TABLE& codon = DEFAULT_CODON);
            void clearAll();

            Sequence& linearize();
            Sequence& circulate();
            Sequence& complement();
            Sequence& transcribe();
            Sequence& translate(const CODON_TABLE &codon = DEFAULT_CODON);
            
            void load(const char* path);
            void save(const char* path);

            Sequence subsequence(const size_t off = 0, const size_t len = -1) const;
            Sequence subsequence(const Range<sint>& range) const;
            Sequence subsequence(const Region<sint>& region) const;
            String raw(const size_t off = 0, size_t len = -1, bool dir = false) const;
            String raw(const DirRange& range) const;
            String raw(const srange& range, bool dir = false) const;
            String raw(const sregion& region, bool dir = false) const;
            void summary() const;
            SObjPtr toObj() const;
            String toString(const char* format = "txt") const;
        };
        extern SLIB_DLL Sequence splice(const Sequence& seq, const sregion& region, bool rev, bool zero = true);
        extern SLIB_DLL Sequence transcribe(const Sequence& seq);
        extern SLIB_DLL Sequence translate(const Sequence& seq, const CODON_TABLE& code = DEFAULT_CODON);

        class SLIB_DLL SeqList : public Array<Sequence> {
            friend Sequence;
            friend Fasta;
        protected:
            SFile _file;
            sizearray _offsets;

        public:
            SDictionary attribute;

        public:
            SeqList();
            SeqList(const char* s, bool l = true);
            ~SeqList();
            void open(const char* path);
            void load(const char* path = nullptr);
            void save(const char* path);
            bool loaded() const;
            String path() const;
            size_t total() const;
            size_t index(const char *name) const;
            sindex nameIndex() const;
            Sequence subsequence(const RefPos& pos) const;
            String raw(const RefPos& pos, bool zero = true) const;
            void clearAll();
        };
    }
    //using sbseq = sbio::Sequence;
    extern inline std::ostream& operator<<(std::ostream& os, const sbio::Sequence& seq);
}
using sbioseq = slib::SPointer<slib::sbio::Sequence>;
#define sdna(X) slib::sbio::Sequence((X),slib::sbio::DNA_SEQ)
#define srna(X) slib::sbio::Sequence((X),slib::sbio::RNA_SEQ)
#define sprotein(X) slib::sbio::Sequence((X),slib::sbio::AA_SEQ)


#endif
