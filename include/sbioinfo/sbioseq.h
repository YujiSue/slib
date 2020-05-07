#ifndef SBIO_SBIOSEQ_H
#define SBIO_SBIOSEQ_H

#include "sio/sfile.h"
#include "sbioinfo/sbioinfoutil.h"
#include "sbioinfo/sbsequtil.h"
#include "sbioinfo/sbseqio.h"

namespace slib {
    namespace sbio {
        
        class SBIOINFO_DLL SBioSeq;
        class SBIOINFO_DLL SBSeqList;
        
        #define sbseq sptr<SBioSeq>
        
        constexpr size_t MAX_FIND_SEQ_LENGTH = 1<<14;
        
        class SBIOINFO_DLL SBioSeq : public ubytearray {
			friend SBSeqIO;
            friend SBSeqList;
                        
        protected:
            sushort _type;
            sint _length;
			sregion _mask;
			SEQ_CONVERTER _dec, _enc;

		public:
            String name;
            sdict attribute;
			Array<seqannot> annotation;
            
        private:
            void _init();
            void _setType();
            
        public:
            SBioSeq();
            SBioSeq(sushort t, const char *name = nullptr, const char *seq = nullptr);
			SBioSeq(const char* seq);
            SBioSeq(const SBioSeq &seq);
            ~SBioSeq();
            
            SBioSeq &operator=(const SBioSeq &seq);
            
            void load(sushort t, const char *path);
            void save(const char *path);
            
            bool isCircular() const;
            bool isMasked() const;
            bool isScored() const;
            bool isAnnotated() const;
			bool isAttributed() const;

            sushort type() const;
            subyte seqtype() const;
            subyte compress() const;
            
            sint length() const;
            const sregion &mask() const;
            
            void setSeq(const char *seq);
            void setLength(const size_t &l, bool alias = false);
            void addMask(const srange &range);
			void removeMask(const srange& range);
			void clearAll();
            
            void encode(const char *seq, size_t off = 0, size_t len = -1, bool dir = false);
            void decode(char *seq, size_t off = 0, size_t len = -1, bool dir = false);
            void recode(subyte c, ubytearray &seq, size_t off = 0, size_t len = -1, bool dir = false);
            
            void convert(sushort t);

            SBioSeq subseq(const sbpos &pos);
            SBioSeq subseq(size_t off, size_t len, bool dir = false);
            String raw(const sbpos &pos) const;
            String raw(size_t off = 0, size_t len = -1, bool dir = false) const;
            /*
            void append(const char *seq);
            void append(const SBioSeq &seq);
            void insert(size_t idx, const char *seq);
            void insert(size_t idx, const SBioSeq &seq);
            */
            void circulate();
            void complement();
            void splice(const sregion &region);
            void transcribe();
            void rtranscribe();
            void translate(const CODON_TABLE &code = DEFAULT_CODON);
            
            SBioSeq transcript();
            SBioSeq rtranscript();
            SBioSeq translated(const CODON_TABLE &code = DEFAULT_CODON);
        };
        
        #define sdna(X) sbseq(COMPRESS1|DNA_SEQ, nullptr, (X))
        #define srna(X) sbseq(COMPRESS1|RNA_SEQ, nullptr, (X))
        #define saa(X) sbseq(COMPRESS1|AA_SEQ, nullptr, (X))
        
        class SBIOINFO_DLL SBSeqList : public Array<sbseq> {
			friend SBSeqIO;
			friend SBioSeq;
            
        protected:
			bool _loaded;
            intarray _length;

		public:
			sindex index;
			sdict attribute;
            
        public:
            SBSeqList();
            SBSeqList(const char *s, bool l = true);
            virtual ~SBSeqList();
            
			bool isLoaded() const;
            size_t seqIdx(const char *name);
			intarray& lengthList();
			const intarray &lengthList() const;
            suinteger total() const;

			void load(const char *path);
            void save(const char *path);
            void makeIndex(const char *path);

            SBioSeq getSeq(sushort type, const sbpos &pos);
            SBioSeq getSeq(sushort type, int idx, size_t pos, size_t len, bool dir = false);
            
			void clearAll();
        };
        
    }
}

#endif
