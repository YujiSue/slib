#ifndef SBIO_SBIOINFOUTIL_H
#define SBIO_SBIOINFOUTIL_H

#include "sobj.h"

#ifndef SBIOINFO_DLL
#if defined(_WINDLL)
#define SBIOINFO_DLL __declspec(dllexport)
#else
#define SBIOINFO_DLL  
#endif
#endif

namespace slib {
	namespace sbio {
		class SBIOINFO_DLL SBioInfoException : public SException {
        public:
            SBioInfoException(const char* f, sint l, const char* func, sint e = 0, const char* target = nullptr, const char* note = nullptr);
            ~SBioInfoException();
        };
		
		constexpr sushort NOT_USE_FLAG = 0x4000;
		constexpr sushort UNAVAILABLE_FLAG = 0x8000;

		//File type
		// 0x0000 bin
		// 0x0001 txt
		// 0x0010 seq
		// 0x0020 pos
		// 0x0100 gene
		// 0x0200 var
		// 0x1000 score
		// 0x2000 index
		constexpr sushort PLAIN_TEXT = 0x0001;
		
		constexpr sushort FASTA = 0x0011;
		constexpr sushort FASTQ = 0x1011;
		constexpr sushort GENBANK = 0x0111;
		constexpr sushort ABI = 0x1010;

		constexpr sushort BED_TEXT = 0x0021;
		constexpr sushort GTF = 0x0121;
		constexpr sushort GFF3 = 0x1121;

		constexpr sushort VCF = 0x0221;

		constexpr sushort SAM = 0x1021;
		constexpr sushort BAM = 0x1020;
		constexpr sushort BAI = 0x3020;

		//Annotation category
		constexpr sushort MISC_FEATURE = 0x0000;
		constexpr sushort CHROMOSOMIC_FEATURE = 0x0001;
		constexpr sushort GENOMIC_FEATURE = 0x0002;
		constexpr sushort GENE_FEATURE = 0x0003;
		constexpr sushort TRANSPOSON_FEATURE = 0x0004;
		constexpr sushort NUCLEOTIDE_FEATURE = 0x0005;
		constexpr sushort TRANSCRIPT_FEATURE = 0x0006;
		constexpr sushort PROTEIN_FEATURE = 0x0007;
		constexpr sushort VARIATION_FEATURE = 0x0008;

		//Chromosome type
		constexpr sushort NUCLEAR_CHR = 0x1000;
		constexpr sushort AUTOSOME = 0x1001;
		constexpr sushort SEX_CHR = 0x1F00;

		constexpr sushort EXT_CHR = 0x2000;
		constexpr sushort MT_GENOME = 0x2001;
		constexpr sushort PLASMID = 0x2002;
		constexpr sushort PLASTID = 0x2003;

		constexpr sushort ART_CHR = 0x4000;
		constexpr sushort BAC = 0x4001;
		constexpr sushort YAC = 0x4002;

		constexpr sushort BALANCER_CHR = 0x4100;

		//Gene type
		constexpr sushort PROTEIN_CODING = 0x1000;
		constexpr sushort NON_CODING = 0x2000;
		constexpr sushort PSEUDOGENE = 0x4000;
		constexpr sushort TRANSPOSON = 0x8000;

		//Transcript type
		constexpr sushort M_RNA = 0x1000;

		constexpr sushort NC_RNA = 0x2000;
		constexpr sushort T_RNA = 0x2001;
		constexpr sushort R_RNA = 0x2002;
		constexpr sushort AS_RNA = 0x2003;
		constexpr sushort LNC_RNA = 0x2004;
		constexpr sushort LINC_RNA = 0x2005;

		constexpr sushort SMALL_RNA = 0x2100;
		constexpr sushort MI_RNA = 0x2101;
		constexpr sushort SI_RNA = 0x2102;
		constexpr sushort SN_RNA = 0x2103;
		constexpr sushort SNO_RNA = 0x2104;
		constexpr sushort PI_RNA = 0x2105;
		constexpr sushort SC_RNA = 0x2106;
		constexpr sushort SCA_RNA = 0x2107;
		constexpr sushort Y_RNA = 0x2108;
		constexpr sushort VT_RNA = 0x2109;
		constexpr sushort P_RNA = 0x210A;
		constexpr sushort SL_RNA = 0x210B;

		constexpr sushort EX_RNA = 0x21A0;

		constexpr sushort RIBOZYME = 0x2200;
		constexpr sushort RNASE_P = 0x2201;
		constexpr sushort RN_MRP = 0x2202;

		constexpr sushort TERC = 0x22A0;

		constexpr sushort VIRAL_RNA = 0x2400;
		constexpr sushort VA_RNA = 0x2401;

		constexpr sushort SH_RNA = 0x2800;

		constexpr sushort CRISPR_RNA = 0x2810;
		constexpr sushort GUIDE_RNA = 0x2811;
		constexpr sushort TRACER_RNA = 0x2812;

		//Genetic region type
		constexpr sushort INTER_GENE = 0x0100;
		constexpr sushort CDS = 0x0001;
		constexpr sushort UTR = 0x0002;
		constexpr sushort UTR5 = 0x0012;
		constexpr sushort UTR3 = 0x0022;
		constexpr sushort EXON = 0x0004;
		constexpr sushort INTRON = 0x0008;
		constexpr sushort SPLICE_SITE = 0x0048;
		constexpr sushort PROCESSED = 0x0080;

		//Genomic feature type
		constexpr sushort CENTROMERE = 0x0001;
		constexpr sushort TEROMERE = 0x0002;

		constexpr sushort SATELLITE = 0x0010;

		constexpr sushort BALANCED_SITE = 0x0080;

		constexpr sushort REGULATOR = 0x0101;
		constexpr sushort PROMOTER = 0x0102;
		constexpr sushort ENHANCER = 0x0103;
		constexpr sushort REPRESSOR = 0x0104;
		constexpr sushort INSULATOR = 0x0105;
		constexpr sushort TERMINATOR = 0x0106;
		constexpr sushort SILENCER = 0x0107;

		constexpr sushort OPERON = 0x010A;
		constexpr sushort OPERATOR = 0x010B;

		constexpr sushort TSS_SITE = 0x0111;
		constexpr sushort TST_SITE = 0x0112;
		constexpr sushort TATA_BOX = 0x0113;

		constexpr sushort SPLICE_LEADER = 0x01A0;

		constexpr sushort LOW_COMPLEX = 0x0200;
		constexpr sushort POLY_A_SITE = 0x0201;
		constexpr sushort AT_RICH = 0x0202;
		constexpr sushort GC_RICH = 0x0203;

		constexpr sushort REPEAT_SITE = 0x0210;
		constexpr sushort TANDEM_REPEAT = 0x0211;
		constexpr sushort PALINDROME = 0x0212;

		constexpr sushort BINDING_SITE = 0x0400;
		constexpr sushort TF_SITE = 0x0401;


		constexpr sushort HETERO_CHR = 0x0801;
		constexpr sushort IMPRINT_SITE = 0x0802;

		constexpr sushort DNA_METHYL_SITE = 0x0810;

		constexpr sushort HISTONE_SITE = 0x0820;
		constexpr sushort HIS_METHYL_SITE = 0x0821;
		constexpr sushort HIS_ACETHYL_SITE = 0x0822;
		constexpr sushort HIS_PHOS_SITE = 0x0823;
		constexpr sushort HIS_UBIQ_SITE = 0x0824;

		constexpr sushort RESTRICTION_SITE = 0x1000;

		constexpr sushort PAM_SITE = 0x1F00;
		constexpr sushort CAS9_PAM = 0x1F01;
		constexpr sushort CAS12_PAM = 0x1F02;
		constexpr sushort CAS13_PAM = 0x1F03;

		//Variant type
		constexpr subyte SNV = 0x01;
		constexpr subyte MNV = 0x02;
		constexpr subyte INSERTION = 0x04;
		constexpr subyte DELETION = 0x08;
		constexpr subyte DUPLICATION = 0x10;
		constexpr subyte MULTIPLICATION = 0x20;
		constexpr subyte INVERSION = 0x40;
		constexpr subyte TRANSLOCATION = 0x80;

		//Variant flag
		constexpr sushort SMALL_VARIANT = 0x0001;
		constexpr sushort CN_VARIANT = 0x0002;
		constexpr sushort SR_VARIANT = 0x0004;

		//Genetic mutation type
		constexpr sushort SILENT_MUT = 0x0000;
		constexpr sushort SUBSTITUTION = 0x1000;
		constexpr sushort MISSENSE = 0x1100;
		constexpr sushort NONSENSE = 0x1200;

		constexpr sushort INDEL_MUT = 0x2000;
		constexpr sushort IN_FRAME = 0x2100;
		constexpr sushort FRAME_SHIFT = 0x2200;
		constexpr sushort NTERM_LESION = 0x2400;
		constexpr sushort CTERM_LESION = 0x2800;

		constexpr sushort HEAD_LESION = 0x2400;
		constexpr sushort TAIL_LESION = 0x2800;

		constexpr sushort COPYNUM_MUT = 0x4000;
		constexpr sushort NULL_MUT = 0x4100;
		constexpr sushort MULTI_COPY = 0x4200;
		constexpr sushort TRIPLET_REPEAT = 0x4400;

		constexpr sushort REARRANGE_MUT = 0x8000;
		constexpr sushort SPLIT_MUT = 0x8100;
		constexpr sushort SELF_REARRANGEMENT = 0x8200;
		constexpr sushort ECTOPIC_MUT = 0x8400;
		/*
		class SBIOINFO_DLL sbiutil {
		public:
			static double scoreVal(double v);
			static double phredVal(double v);

			static suint countBin(Map<sint, suint>& order, srange range);
			static sushort getBin(srange range);
			static void getBins(ushortarray& bins, srange range);
			static void getBins(ushortarray& bins, const sregion& region);

			static String vartype(sushort i);
			static sushort vartypeIdx(const char* s);
			static sushort codeVarType(sobj& obj);
			
			static String vsite(sushort i);
			static subyte vsiteIdx(const char* s);
			static String mtype(sushort i);
			static subyte mtypeIdx(const char* s);

		};
		*/
		namespace sbiutil {
			extern double scoreVal(double v);
			extern double phredVal(double v);

			extern suint countBin(Map<sint, suint>& order, srange range);
			extern sushort getBin(srange range);
			extern void getBins(ushortarray& bins, srange range);
			extern void getBins(ushortarray& bins, const sregion& region);

			extern String geneType(sushort i);
			extern sushort geneTypeIdx(const char* s);
			extern sobj decodeGeneType(sushort i);
			extern sushort encodeGeneType(sobj obj);
			extern String transcriptType(sushort i);
			extern sushort transcriptTypeIdx(const char* s);
			extern sobj decodeTranscriptType(sushort i);
			extern sushort encodeTranscriptType(sobj obj);
			extern stringarray geneSite(sushort i);
			extern sushort geneSiteIdx(const char* s);
			extern sushort geneSiteIdx(const stringarray& s);
			extern sobj decodeGeneSite(sushort i);
			extern sushort encodeGeneSite(sobj obj);
			extern String varType(sushort i);
			extern stringarray varTypes(sushort i);
			extern sushort varTypeIdx(const char* s);
			extern sushort varTypeIdx(const stringarray &s);
			extern sobj decodeVarType(sushort i);
			extern sushort encodeVarType(sobj obj);
			extern stringarray mutType(sushort i);
			extern sushort mutTypeIdx(const char* s);
			extern sushort mutTypeIdx(const stringarray& s);
			extern sobj decodeMutType(sushort i);
			extern sushort encodeMutType(sobj obj);
		}

		struct SBIOINFO_DLL sbpos : public srange {
			sint idx;
			bool dir;

			sbpos();
			sbpos(sint i, sint b, sint e, bool d = false);
			sbpos(const char* s, const sindex* namei = nullptr);
			sbpos(const sbpos& p);
			sbpos& operator = (const sbpos& p);
			~sbpos();

			void set(const char* s, const sindex* namei = nullptr);
			void set(double d, const intarray* length);
			double absolute(const intarray* length);

			String toString(stringarray* names = nullptr) const;
			void init();

			bool operator < (const sbpos& p) const;
			bool operator == (const sbpos& p) const;
			bool operator != (const sbpos& p) const;
		};
	}
}

#endif 