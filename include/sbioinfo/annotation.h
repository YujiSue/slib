#ifndef SBIO_ANNOTATION_H
#define SBIO_ANNOTATION_H
#include "sutil/sdb.h"
#include "sbioinfo/biutil.h"
namespace slib {
    namespace sbio {
        class SLIB_DLL Sequence;
        class SLIB_DLL GffData;
        class SLIB_DLL VarParam;
        class SLIB_DLL Variant;
        class SLIB_DLL AnnotInfo : public RefPos {
        public:
            int record;
            suint type;
            String name;
        public:
            AnnotInfo();
            AnnotInfo(const suint t, const String& n, const RefPos& p);
            AnnotInfo(const AnnotInfo& info);
            virtual ~AnnotInfo();
            AnnotInfo& operator=(const AnnotInfo& info);
            virtual void clear();
        };
        class SLIB_DLL TranscriptInfo;
        class SLIB_DLL GeneInfo : public AnnotInfo {
        public:
            String geneid, description;
            stringarray synonym;
            Array<TranscriptInfo*> transcripts;
            SDictionary attribute;
        public:
            GeneInfo();
            GeneInfo(const GeneInfo& g);
            ~GeneInfo();
            GeneInfo& operator=(const GeneInfo& g);
            void clear();
        };
        class SLIB_DLL ProteinInfo;
        class SLIB_DLL TranscriptInfo : public AnnotInfo {
        public:
            GeneInfo *gene;
            ProteinInfo* product;
            Array<AnnotInfo> structures;
        public:
            TranscriptInfo();
            TranscriptInfo(const AnnotInfo& info);
            TranscriptInfo(const TranscriptInfo& t);
            ~TranscriptInfo();
            TranscriptInfo& operator=(const TranscriptInfo& t);
            sregion exons() const;
            sregion coding() const;
            void clear();
        };
        class SLIB_DLL VariantInfo : public AnnotInfo {
        public:
            String varid;
            SDictionary attribute;
        public:
            VariantInfo();
            VariantInfo(const VariantInfo& v);
            ~VariantInfo();
            VariantInfo& operator=(const VariantInfo& v);
            void clear();
        };
        class SLIB_DLL MotifInfo : public AnnotInfo {
        public:
            String motid, program;
        public:
            MotifInfo();
            MotifInfo(const MotifInfo& m);
            ~MotifInfo();
            MotifInfo& operator=(const MotifInfo& m);
            void clear();
        };
        class SLIB_DLL ProteinInfo : public AnnotInfo {
        public:
            String protid;
            Array<TranscriptInfo*> transcripts;
            Array<MotifInfo> motifs;

        public:
            ProteinInfo();
            ProteinInfo(const ProteinInfo& p);
            ~ProteinInfo();
            ProteinInfo& operator=(const ProteinInfo& v);
            void clear();
        };
        class SLIB_DLL AASubstitution {
        public:
            char prefix;
            srange pos;
            String ori, alt;
        public:
            AASubstitution();
            ~AASubstitution();
            AASubstitution& operator=(const AASubstitution& sub);
        };

        enum class SLIB_DLL VARIANT_EFFECT : subyte {
            DELETERIOUS = 0x01, // severe effect : 1
            HIGH_IMPACT = 0x01,            
            PATHOGENIC = 0x11,
            MODERATE = 0x02,    // weak or middle effect : 2
            TOLERATED = 0x04,   // almost no effect: 4
            BENIGN = 0x14,
            UNKNOWN = 0x08,
        };

        class SLIB_DLL TranscriptAnnotData {
        public:
            TRANSCRIPT_TYPE type;
            srange pos;
            String name;
            subyte site;
            sushort mutation;
            
            float score;
            VARIANT_EFFECT effect;

            AASubstitution substitution; 

        public:
            TranscriptAnnotData();
            TranscriptAnnotData(TranscriptInfo *ti);
            ~TranscriptAnnotData();
            TranscriptAnnotData& operator=(const TranscriptAnnotData& data);
            String mutType() const;
            String mutFormat() const;
        };

        class SLIB_DLL GeneAnnotData {
        public:
            GENE_TYPE type;
            String gid, name;
            Array<TranscriptAnnotData> transcripts;

        public:
            GeneAnnotData();
            GeneAnnotData(GeneInfo *gi);
            ~GeneAnnotData();
            GeneAnnotData& operator=(const GeneAnnotData& data);
            String gtype() const;
            String varSite() const;
        };

        enum class SLIB_DLL ANNOT_CATEGORY : sushort {
            CONTIG = 0x01,
            GENE = 0x02,
            TRANSCRIPT = 0x04,
            STRUCTURE = 0x06,
            MUTATION = 0x08,
            FEATURE = 0x10,
            PROTEIN = 0x20,
            MOTIF = 0x40,
            USER_DEFINED = 0x8000,
        };

        class SLIB_DLL AnnotDB : public SDataBase {
            /**
            *   ------------------------------------------------------
            *  |idx | table      | columns                            |
            *  |------------------------------------------------------
            *  | 0  | info       | key, value                         |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 1  | const      | target, value, label, description  |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 2  | chromosome | id, name, type, start, end         |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 3  | contig     | name, chromosome, start, end       |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 4  | gene       | id, name, type, chromosome, start, |
            *  |    |            | end, strand, gid, description,     |
            *  |    |            | attribute                          |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 5  | synonym    | gene, name                         |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 6  | transcript | id, name, type, gene, start, end   |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 7  | structure  | type, transcript, start, end       |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 8  | mutation   | id, name, type, chromosome,        |
            *  |    |            | start, end, vid, attribute         |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 9  | variant    | id, name, type, chromosome,        | 
            *  |    |            | start, end, vid, attribute         |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 10 | feature    | name, type, chromosome, start, end | 
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 11 | protein    | id, name, gene, start, end         |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 12 | motif      | name, type, protein, start, end,   |
            *  |    |            | mid, source                        |
            *  | - - - - - - - - - - - - - - - - - - - - - - - - - - -|
            *  | 13 | xref       | type, dbid, ref, refid             |
            *   ------------------------------------------------------
            *  - mutation: Artificial mutation in model organisims
            *  - variant:  Polymorphisms or spontaneous variants
            *
            */
            
            
            // Buffer
            AnnotInfo _ainfo;
            GeneInfo _ginfo;
            TranscriptInfo _tinfo;
            VariantInfo _vinfo;
            MotifInfo _minfo;

            // Container
            intarray _indice;
            Array<AnnotInfo> _contigs, _features, _proteins;
            Array<GeneInfo> _genes;
            Array<Array<GeneInfo*>> _geneIndex;
            Array<GeneInfo*> _pgenes;
            Array<TranscriptInfo> _transcripts;
            Array<VariantInfo> _mutations, _variants;
            Array<Array<VariantInfo*>> _varIndex;
            Array<VariantInfo*> _pvars;
            Array<MotifInfo> _motifs;
            bool _load[4]; // gene, mut., var., feat.

        public:
            Array<AnnotInfo> chromosomes;
            sindex chrindex;
            SDictionary attribute;

        public:
            AnnotDB(const char* path = nullptr);
            ~AnnotDB();
            /**
            * \~english @brief Open database
            */
            void open(const char* path);
            /**
            * \~english @brief Load records
            */
            void load(const stringarray& li = {});
            bool isOpened();
            bool isLoaded(const char* key) const;
            void loadGenes();
            void loadMutations();
            void loadVariants();
            void loadFeatures();

            Array<AnnotInfo>& getContigs(const RefPos& pos, Array<AnnotInfo> *container = nullptr);
            Array<AnnotInfo>& getContigs(const char* query, slib::MATCH match = MATCH::EXACT, Array<AnnotInfo>* container = nullptr);
            /**
            * \~english @brief Get Gene IDs
            */
            intarray& searchGenes(const RefPos& pos, sobj opts = snull, intarray *container = nullptr);
            /**
            * \~english @brief Get Gene IDs
            */
            intarray& searchGenes(const char* query, slib::MATCH match = MATCH::EXACT, sobj opts = snull, intarray* container = nullptr);
            Array<GeneInfo>& getGenes(const RefPos& pos, sobj opts = snull);
            Array<GeneInfo>& getGenes(const char* query, slib::MATCH match = MATCH::EXACT, sobj opts = snull);
            Array<GeneInfo*>& selectGenes(const RefPos& pos, Array<GeneInfo*>* container = nullptr);
            GeneInfo& geneInfo(int record, sobj opts = snull);
            Array<GeneInfo>& geneInfos(const intarray &records, sobj opts = snull);

            Array<TranscriptInfo>& transcriptsOf(int gene, sobj opts = snull);
            Array<TranscriptInfo>& getTranscripts(const RefPos& pos, sobj opts = snull);
            Array<TranscriptInfo>& getTranscripts(const char* query, slib::MATCH match = MATCH::EXACT, sobj opts = snull);

            intarray& searchMutations(const RefPos& pos, sobj opts = snull, intarray* container = nullptr);
            intarray& searchMutations(const char* query, slib::MATCH match = MATCH::EXACT, sobj opts = snull, intarray* container = nullptr);
            intarray& searchVariants(const RefPos& pos, sobj opts = snull, intarray* container = nullptr);
            intarray& searchVariants(const char* query, slib::MATCH match = MATCH::EXACT, sobj opts = snull, intarray* container = nullptr);
            Array<VariantInfo>& getMutations(const RefPos& pos, sobj opts = snull);
            Array<VariantInfo>& getMutations(const char* query, slib::MATCH match = MATCH::EXACT, sobj opts = snull);
            Array<VariantInfo>& getVariants(const RefPos& pos, sobj opts = snull);
            Array<VariantInfo>& getVariants(const char* query, slib::MATCH match = MATCH::EXACT, sobj opts = snull);
            Array<VariantInfo>& mutationInfos(const intarray& records, sobj opts = snull);
            Array<VariantInfo>& variantInfos(const intarray& records, sobj opts = snull);
            Array<VariantInfo>& variantsOf(int gene, sobj opts = snull);
            bool verifyVariant(Variant& var, const VarParam& par);
            
            Array<AnnotInfo>& getFeatures(const RefPos& pos, sobj opts = snull);
            Array<AnnotInfo>& getFeatures(const char* query, slib::MATCH match = MATCH::EXACT, sobj opts = snull);

            Array<AnnotInfo>& proteinsOf(int gene, sobj opts = snull);

            Array<MotifInfo>& motifsOf(int prot, sobj opts = snull);

            void annotate(Sequence& seq, const RefPos& pos, const sushort types);
            void annotate(Sequence& seq, const GeneInfo& gene, const sushort types);
            void annotate(Variant& var, const SeqList& ref, const VarParam &par,
                const slib::sbio::CODON_TABLE &codon = DEFAULT_CODON, 
                const slib::sbio::CODON_TABLE& codon2 = DEFAULT_MT_CODON);
        };
        /*
        class SLIB_DLL SBioAnnotDB : public SDataBase<> {
            typedef SDataBase<> db;
            
        private:
            suint _mode;

            Array<AnnotInfo> _chromosomes, _contigs;
            Array<GeneInfo> _genes;
            Array<TranscriptInfo> transcripts;


            Array<VariantInfo> mutants, variants;
            Array<FeatureInfo> features;

        public:
            SBioAnnotDB(const char *path = nullptr);
            ~SBioAnnotDB();

            void open(const char* path);
            void load(const stringarray &li = {});

            const sattribute generalInfo() const;
            const Array<AnnotInfo>& chromosomes() const;
            const AnnotInfo& chrInfo(const sinteger idx) const;
            const sindex chrIndex() const;
            bool isLoaded(const char* key) const;
            //const Subset<AnnotInfo> findContig(const RefPos& pos, bool append = false);
            //const Subset<AnnotInfo> findContig(const char* query, slib::MATCH match = MATCH::EXACT, bool append = false);
            const Array<AnnotInfo>& searchContig(const RefPos& pos, bool append = false);
            const Array<AnnotInfo>& searchContig(const char* query, slib::MATCH match = MATCH::EXACT, bool append = false);

            const Array<GeneInfo>& searchGene(const RefPos& pos, bool append = false);
            const Array<GeneInfo>& searchGene(const char* query, slib::MATCH match = MATCH::EXACT, bool synonym_search = true, bool append = false);



        };
        */
        /*


		struct SBIOINFO_DLL transcript_site {
			transcript_info* info;
			sushort type, mutation, site;
			String name, ori, alt;
			sint pos;

			transcript_site();
			transcript_site(sobj& obj);
			transcript_site(transcript_info* ti);
			transcript_site(const transcript_site& trs);
			~transcript_site();

			transcript_site& operator=(const transcript_site& trs);
			void set(sobj obj);
			sobj toObj();
		};

		struct SBIOINFO_DLL gene_site {
			gene_info* info;
			sushort type, mutation;
			String gid, name;
			Array<transcript_site> transcripts;
			
			gene_site();
			gene_site(sobj& obj);
			gene_site(gene_info* gi);
			gene_site(gene_info* gi, sbpos*range);
			gene_site(const gene_site& g);
			~gene_site();

			gene_site& operator=(const gene_site& g);
			sushort annotatedSite() const;
			void check(sbpos* pos);
			void set(sobj obj);
			sobj toObj();
		};
        class SBIOINFO_DLL SBAnnotDB : public SDataBase {
			/*
             SQLite Table:
                INFO:
                    ID(KEY)
                    NAME(STRING)
                        e.g. creator
                             date
                             version
                             species
                             etc...
             
                    VALUE(STRING)
             
                CHROMOSOME:
                    ID(KEY)
                    NAME(STRING)
                    LENGTH(INTEGER)
                
                CONTIG:
                    ID(KEY)
                    NAME(STRING)
                    CHROMOSOME(INTEGER)
                    START(INTEGER)
                    END(INTEGER)
             
                GENE:
                    ID(KEY)
                    GENE_ID(STRING)
                    TYPE(INTEGER)
                    NAME(STRING)
                    CHROMOSOME(INTEGER)
                    START(INTEGER)
                    END(INTEGER)
                    STRAND(STRING)
                    OTHER_NAME(STRING)
                    TRANSCRIPT(ARRAY)
                    DESCRIPTION(STRING)
                    ATTRIBUTE(STRING)
                
                TRANSCRIPT:
                    ID(KEY)
                    GENE_ID(INTEGER)
                    TYPE(INTEGER)
                    CHROMOSOME(INTEGER)
                    NAME(STRING)
                    START(INTEGER)
                    END(INTEGER)
                    STRUCTURE(ARRAY)
             
                STRUCTURE:
                    ID(KEY)
                    TRANSCRIPT_ID(INTEGER)
                    TYPE(INTEGER)
                    START(INTEGER)
                    END(INTEGER)
                    
                MUTATION:
                    ID(KEY)
                    MUT_ID(STRING)
                    TYPE(INTEGER)
                    NAME(STRING)
                    CHROMOSOME(INTEGER)
                    START(INTEGER)
                    END(INTEGER)
                    STRAIN(STRING)
                    ATTRIBUTE(STRING)
             
                VARIATION:
                    ID(KEY)
                    VAR_ID(STRING)
                    TYPE(INTEGER)
                    NAME(STRING)
                    CHROMOSOME(INTEGER)
                    START(INTEGER)
                    END(INTEGER)
                    STRAIN(STRING)
                    ATTRIBUTE(STRING)

                FEATURE:
                    ID(KEY)
                    TYPE(INTEGER)
                    NAME(STRING)
                    START(INTEGER)
                    END(INTEGER)
                    ATTRIBUTE(ARRAY)
             */
            /*
        public:
            
            
            typedef Array<chr_info> chrarray;
            typedef Array<contig_info> ctgarray;
            typedef CArray<contig_info *> ctgparray;
            typedef Array<gene_info> genearray;
            typedef CArray<gene_info *> geneparray;
            typedef Array<transcript_info> trsarray;
            typedef CArray<transcript_info *> trsparray;
            typedef Array<mut_info> mutarray;
            typedef CArray<mut_info *> mutparray;
            typedef Array<feature_info> ftrarray;
            typedef CArray<feature_info *> ftrparray;
            
            typedef std::pair<String *, sint> name_pair;
            
        private:
            sushort _mode;
            Array<Map<sint, suint>> _bin_order;
            sindex _chr_index;
            Array<name_pair> _gene_name_index;
            Array<Array<ctgparray>> _ctg_index;
            Array<Array<geneparray>> _gene_index;
            Array<Array<trsparray>> _trs_index;
            Array<Array<mutparray>> _mut_index, _var_index;
            Array<Array<ftrparray>> _ftr_index;
            
            intarray _ctg_name_index, _trs_name_index, _mut_name_index, _var_name_index, _ftr_name_index;
            
        public:
			chrarray chromosomes;
			ctgarray contigs;
			genearray genes;
			trsarray transcripts;
			mutarray mutants, variations;
			ftrarray features;
            
        private:
            void _initIdx();
            void _loadChrInfo();
            void _loadContigInfo();
            void _loadGeneInfo();
            void _loadTranscriptInfo();
            void _loadMutantInfo();
            void _loadVariationInfo();
            void _loadFeatureInfo();
            
        public:
            SBAnnotDB ();
            SBAnnotDB (const char *path, int m = LOAD_CHR);
            ~SBAnnotDB ();
            
            void open(const char *path);
            void load(int m = LOAD_CHR);
            int mode() const;
            void setMode(int m);
            
            String species();
            String version();
            
            size_t chrNum() const;
            chr_info chrInfo(int idx) const;
            size_t chrIndex(const char *name) const;
            const sindex &nameIdx() const;
            
            void ctgInfo(ctgparray &array, const sbpos &pos, bool append = false);
            void ctgInfo(ctgparray& array, const char *name, subyte match = EXACT_MATCH, bool append = false);
            
            GeneInfo searchGene(const char *que, subyte match = EXACT_MATCH);
            //Array<GeneInfo>& searchGenes(const char* que, subyte match = );
            TranscriptInfo searchTranscript(const char* que);

            void geneInfo(geneparray & array, const sbpos &pos, bool trans = true, bool append = false);
            void geneInfo(geneparray& array, const char *name, bool trans = true, subyte match = EXACT_MATCH, bool append = false);
			void nearestGeneInfo(geneparray& array, const sbpos& pos, size_t range, bool trans = true, bool append = false);
            void transcriptInfo(trsparray & array, const sbpos &pos, bool gene = false, bool append = false);
            void transcriptInfo(trsparray& array, const char *name, bool gene = false, subyte match = EXACT_MATCH, bool append = false);
            


			void mutantInfo(mutparray& array, const sbpos& pos, bool append = false);
            void mutantInfo(mutparray& array, const char *name, subyte match = EXACT_MATCH, bool append = false);
            
            void variationInfo(mutparray& array, const sbpos &pos, bool append = false);
            void variationInfo(mutparray& array, const char *name, subyte match = EXACT_MATCH, bool append = false);
            
            void featureInfo(ftrparray & array, const sbpos &pos, bool append = false);
            void featureInfo(ftrparray& array, const char *name, subyte match = EXACT_MATCH, bool append = false);
        };
        */
    }
}
#endif
