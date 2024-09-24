#ifndef SBIO_SBICONST_H
#define SBIO_SBICONST_H
#include "sconfig.h"
namespace slib {
	namespace sbio {
		namespace sseq {
			constexpr subyte MISC = 0x00;
			constexpr subyte DNA = 0x01;
			constexpr subyte RNA = 0x02;
			constexpr subyte AA = 0x04;
			constexpr subyte MASKED = 0x10;
			constexpr subyte CIRCULAR = 0x20;
			constexpr subyte ANNOTATED = 0x40;
			constexpr subyte ATTRIBUTED = 0x80;

		}
		//
		constexpr sushort UNCLEAR_HEAD = 0x01;
		constexpr sushort UNCLEAR_TAIL = 0x02;

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

		// DNA genome/vector
		constexpr sushort CHROMOSOME = 0x0001;
		constexpr sushort AUTOSOME = 0x0002;
		constexpr sushort SEX_CHR = 0x0004;
		constexpr sushort UNLINKAGED = 0x0008;
		constexpr sushort EXTRA_CHR = 0x0010;
		constexpr sushort NUCLEOTIDE_GENOME = 0x0100;
		constexpr sushort MITO_GENOME = 0x0200;
		constexpr sushort CHLORO_GENOME = 0x0400;
		constexpr sushort VIRAL_GENOME = 0x0800;
		constexpr sushort ORGANELLAR_DNA = 0x1000;
		constexpr sushort PLASMID = 0x2000;

/*
		constexpr sushort NUCLEAR_CHR = 0x1000;
		constexpr sushort AUTOSOME = 0x1001;
		constexpr sushort SEX_CHR = 0x1F00;

		constexpr sushort EXT_CHR = 0x2000;
		constexpr sushort MT_GENOME = 0x2001;
		constexpr sushort PLASMID = 0x2002;
		constexpr sushort PLASTID = 0x2003;
*/
		constexpr sushort ARTIFICIAL_CHR = 0x4000;
		//constexpr sushort BAC = 0x4001;
		//constexpr sushort YAC = 0x4002;

		//constexpr sushort BALANCER_CHR = 0x4100;

		// Linkage group type
		enum class LG_TYPE : subyte {
			CHROMOSOME = 0x01,
			AUTOSOME = 0x11,
			SEX_CHROMOSOME = 0x21,
			MT_GENOME = 0x02,		// Mitochondria
			PL_GENOME = 0x04,		// Plastid
			PLASMID = 0x08,
			MISC_LG = 0x80,
		};

		//Gene type
		enum class GENE_TYPE : subyte {
			MISC_GENE = 0x00,
			PROTEIN_CODING = 0x01,
			NON_CODING = 0x02,
			PSEUDO_GENE = 0x04,
			TRANSPOSON = 0x08,
			UNAVAILABLE = 0x80,
		};

		enum class TRANSCRIPT_TYPE : sushort {
			MISC_RNA = 0x0000,
			M_RNA = 0x0001,
			NC_RNA = 0x0002,
			PSEUDO_GENE_TRANSCRIPT = 0x0003,
			CIRC_NC_RNA = 0x1002,
			LNC_RNA = 0x0012,
			LINC_RNA = 0x0022,
			T_RNA = 0x0004,
			R_RNA = 0x0008,
			SMALL_RNA = 0x0100,
			MI_RNA = 0x0101,
			AS_RNA = 0x0102,
			PI_RNA = 0x0103,
			VT_RNA = 0x0104,
			Y_RNA = 0x0105,
			SN_RNA = 0x0111,
			SNO_RNA = 0x0112,
			SC_RNA = 0x0113,
			RIBOZYME = 0x0200,
			RNASE_P = 0x0201,
			RNASE_MRP = 0x0202,
			TEROMERASE_RNA = 0x0800
		};

		/*
		constexpr sushort PROTEIN_CODING = 0x1000;
		constexpr sushort NON_CODING = 0x2000;
		constexpr sushort PSEUDOGENE = 0x4000;
		constexpr sushort TRANSPOSON = 0x8000;
		*/
		//Transcript type
		constexpr sushort M_RNA = 0x1000;

		constexpr sushort NC_RNA = 0x2000;
		constexpr sushort T_RNA = 0x2001;
		constexpr sushort R_RNA = 0x2002;
		constexpr sushort AS_RNA = 0x2003;
		constexpr sushort LNC_RNA = 0x2004;
		constexpr sushort LINC_RNA = 0x2005;
		constexpr sushort CIRC_RNA = 0x2006;
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
		constexpr subyte CDS = 0x01;
		constexpr subyte UTR = 0x02;
		constexpr subyte UTR5 = 0x12;
		constexpr subyte UTR3 = 0x22;
		constexpr subyte EXON = 0x04;
		constexpr subyte INTRON = 0x08;
		constexpr subyte SPLICE_SITE = 0x48;
		constexpr subyte PROCESSING = 0x80;

		//Genetic mutation type
		constexpr subyte SILENT = 0x00;
		constexpr subyte SUBSTITUTION = 0x01;
		constexpr subyte SYNONYMOUS = 0x01;
		constexpr subyte MISSENSE = 0x11;
		constexpr subyte NONSENSE = 0x21;
		constexpr sushort FMET_LOSS = 0x41;
		constexpr sushort TERM_LOSS = 0x81;

		constexpr sushort INDEL = 0x02;
		constexpr sushort IN_FRAME = 0x12;
		constexpr sushort FRAME_SHIFT = 0x22;
		constexpr sushort HEAD_LESION = 0x42;
		constexpr sushort TAIL_LESION = 0x82;

		constexpr sushort COPY_MUT = 0x04;
		constexpr sushort NULL_MUT = 0x14;
		constexpr sushort MULTI_COPY = 0x24;
		constexpr sushort TRIPLET_REPEAT = 0x44;

		constexpr sushort REARRANGE_MUT = 0x08;
		constexpr sushort GENE_FUSION = 0x18;
		constexpr sushort GENE_SPLIT = 0x28;

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

		constexpr sushort MI_RNA_BINDING = 0x0441;


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

		// Ploidy type
		constexpr sushort HAPLOID = 0x0001;
		constexpr sushort DIPLOID = 0x0002;
		constexpr sushort POLYPLOID = 0x0004; // For practical use, this value should be 0xXX04 (XX:Number of copies of chromosome) 
		constexpr sushort ANEUPLOID = 0x0008;

		
		
	}
}

#endif 