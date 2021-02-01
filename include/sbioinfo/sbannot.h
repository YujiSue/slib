#ifndef SBIO_SBIOANNOT_H
#define SBIO_SBIOANNOT_H

#include "sutil/sdb.h"
#include "sbioinfo/sbioinfoutil.h"

namespace slib {
    using namespace sio;
    
    namespace sbio {

		struct SBIOINFO_DLL annot_info : public sbpos {
			sint _id;
			suint type;
			String name;

			annot_info();
			annot_info(const suint& t, const String& n, const sbpos& p);
			annot_info(const annot_info& info);
			~annot_info();
		};
		struct SBIOINFO_DLL chr_info : public annot_info {
			chr_info();
			chr_info(const char* s, const sbpos& pos);
			chr_info(const chr_info& c);
			~chr_info();
			chr_info& operator=(const chr_info& c);
		};
		struct SBIOINFO_DLL contig_info : public annot_info {
			contig_info();
			contig_info(const contig_info& c);
			~contig_info();
			contig_info& operator=(const contig_info& c);
		};
		struct SBIOINFO_DLL struct_info : public annot_info {
			struct_info();
			struct_info(const struct_info& s);
			~struct_info();
			struct_info& operator=(const struct_info& s);
		};
		struct SBIOINFO_DLL gene_info;
		struct SBIOINFO_DLL transcript_info : public annot_info {
			gene_info* gene;
			Array<struct_info> structures;

			transcript_info();
			transcript_info(const transcript_info& t);
			~transcript_info();
			transcript_info& operator=(const transcript_info& t);
			void addStructure(struct_info&& s);
			void setGene(gene_info* g);
			sregion exonRegion();
			sregion codingRegion();
			Array<struct_info> messenger();
		};
		struct SBIOINFO_DLL gene_info : public annot_info {
			String gene_id, description;
			stringarray other_names;
			Array<transcript_info*> transcripts;
			sattribute attribute;

			gene_info();
			gene_info(const gene_info& g);
			~gene_info();
			gene_info& operator=(const gene_info& g);
			void setDescription(String* str);
			void addTranscript(transcript_info* t);
		};
		struct SBIOINFO_DLL mut_info : public annot_info {
			String mut_id, strain;
			sattribute attribute;

			mut_info();
			mut_info(const mut_info& m);
			~mut_info();
			mut_info& operator=(const mut_info& m);
		};
		struct SBIOINFO_DLL feature_info : public annot_info {
			feature_info();
			feature_info(const feature_info& f);
			~feature_info();
			feature_info& operator=(const feature_info& f);
		};

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
            
        public:
            static const sushort LOAD_CHR = 0x01;
            static const sushort LOAD_CTG = 0x02;
            static const sushort LOAD_GENE = 0x04;
            static const sushort LOAD_TRANS = 0x08;
            static const sushort LOAD_MUT = 0x10;
            static const sushort LOAD_VAR = 0x20;
            static const sushort LOAD_FTR = 0x40;
            
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
    }
}
#endif
