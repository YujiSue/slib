#ifndef SBIO_SVARUTIL_H
#define SBIO_SVARUTIL_H
#include "sio/stream.h"
#include "sutil/sthread.h"
#include "sbioinfo/annotation.h"
#include "sbioinfo/seq.h"

namespace slib {
	using namespace smath;
	namespace sbio {
		class SLIB_DLL SVar;
		class SLIB_DLL Variant;
		class SLIB_DLL VarList;

#define DEFAULT_MIN_CVLEN 20
#define DEFAULT_BG_DEPTH 3.0
#define DEFAULT_HOMO_DEL 0.35
#define DEFAULT_DELCP 0.75
#define DEFAULT_DUPCP 1.35
#define DEFAULT_HOMO_DUPCP 1.75
#define DEFAULT_MULCP 2.5


#define DEFAULT_MIN_QUAL 1.0

#define DEFAULT_DIST 50
#define DEFAULT_DIFF 100
#define DEFAULT_FREQ_BIN 20

#define SV_TYPE_COUNT 6
		enum class VAR_CATEGORY : subyte {
			SMALL = 0x01,
			SV = 0x02,
			CNV = 0x04,
		};

		enum class SVREAD_TYPE : subyte {
			DEL = 0,
			DUP = 1,
			INS = 2,
			INV = 3,
			TRS = 4,
			TRINV = 5,
		};

		// Variant type
		constexpr subyte SNV = 0x01;
		constexpr subyte MNV = 0x02;
		constexpr subyte INSERTION = 0x04;
		constexpr subyte DELETION = 0x08;
		constexpr subyte DUPLICATION = 0x10;
		constexpr subyte MULTIPLICATION = 0x20;
		constexpr subyte INVERSION = 0x40;
		constexpr subyte TRANSLOCATION = 0x80;

		// Variant genotype
		constexpr subyte GERM_VAR = 0x10;
		constexpr subyte SOMA_VAR = 0x20;
		constexpr subyte HETERO_VAR = 0x01;
		constexpr subyte HOMO_VAR = 0x02;
		constexpr subyte TRANS_HETERO_VAR = 0x04;

		// Method flag
		constexpr sushort SMALL_VARIANT = 0x0001;
		constexpr sushort CN_VARIANT = 0x0002;
		constexpr sushort SR_VARIANT = 0x0004;
		constexpr sushort CNSR_VARIANT = 0x0008;
		

		// State flag
		constexpr sushort COMPLEX_VAR_FLAG = 0x1000;
		constexpr sushort MULTI_CANDIDATE_FLAG = 0x2000;
		constexpr sushort NOT_USE_FLAG = 0x4000;
		constexpr sushort UNAVAILABLE_FLAG = 0x8000;


		/*
		
		/**
		* Variant caller param (for small variants)
		*/
#define DEFAULT_VC_DP 3
#define DEFAULT_VC_VDP 1
#define DEFAULT_VC_FREQ 0.1
#define DEFAULT_VC_HOMO_FREQ 0.75
#define DEFAULT_VC_QUAL 10.0

		class SLIB_DLL VCParam {
		public:
			//0:SNV/MNV, 1:DEL, 3:INS/DUP
			int min_depth[3], min_vdepth[3];
			float min_freq[3], homo_freq[3] , min_qual[3];

		public:
			VCParam();
			VCParam(const VCParam& p);
			~VCParam();
			VCParam& operator=(const VCParam& par);
			void set(const sobj& obj);
			sobj toObj();
		};

		enum class CN_METHOD : subyte {
			RAW = 0x01,
			GC_BIAS = 0x02,
		};

		enum class CNV_TYPE : subyte {
			NON_CNV = 0x00,
			DEL_CNV = 0x01,
			DUP_CNV = 0x02,
			HETERO_CNV = 0x10,
			HOMO_CNV = 0x20,

			RARE_CNV = 0x80,
		};

		class SLIB_DLL CNVParam {
		public:
			CN_METHOD method;
			bool use_ml;
			bool gc_correct;
			float min_bg;
			float border[5];
			//copy: [0, border[0]), [border[0], border[1]), [border[1], border[2]), [border[2], border[3]), [border[3], border[4]), [border[4], +inf)
			sint min_length;
			double min_qual;
			smatd emission, transition;
		public:

			CNVParam();
			CNVParam(const CNVParam& p);
			~CNVParam();
			CNVParam& operator=(const CNVParam& par);
			sushort evaluate(float cp);
			void set(const sobj& obj);
			sobj toObj();
		};

#define DEFAULT_SR_READ 3
#define DEFAULT_MIN_DEL_SIZE 1
#define DEFAULT_MIN_INS_SIZE 1
#define DEFAULT_MIN_INV_SIZE 1
#define DEFAULT_FREQ_BIN 20
#define DEFAULT_MIN_FREQ 0.1
#define DEFAULT_FR_BIAS 1.0
#define DEFAULT_COMP_BIAS 1.0
#define DEFAULT_HOMO_FREQ 0.65
		class SLIB_DLL SVParam {
		public:
			// 
			int min_read, comp_min_read;


			sint freq_bin, min_length[3];// 0:del, 1:dup/ins, 2:inv
			double min_freq, homo_freq;
			int break_site_len, max_dist;
			srange size_range[3];
			
			
			sint max_gap;


			//
			float read_bias, comb_bias;
			//
			float min_qual;

		public:
			SVParam();
			SVParam(const SVParam& p);
			~SVParam();
			SVParam& operator=(const SVParam& par);
			void set(const sobj& obj);
			sobj toObj();
		};
		class SLIB_DLL VarParam {
		public:
			bool homo_select;
			sushort annot;
			int splice_reg;
			CODON_TABLE codon, mtcodon;

		public:
			VCParam vcp;
			CNVParam cnvp;
			SVParam svp;
		public:
			VarParam();
			VarParam(const VarParam& p);
			~VarParam();
			VarParam& operator=(const VarParam& par);
			void set(const sobj& obj);
			sobj toObj();
		};

		namespace sio {
			extern SLIB_DLL void writeRefInfo(const slib::Array<slib::Pair<slib::String, int>>& ref, slib::IOStream& strm);
			extern SLIB_DLL void writeHead(const stringarray& cols, slib::IOStream& strm);
			extern SLIB_DLL void writeVariant(const slib::sbio::Variant& var, const stringarray& cols, const slib::Array<slib::Pair<slib::String, int>>& ref, slib::IOStream& strm);
			extern SLIB_DLL void writeAnnotGeneIDs(const slib::sbio::Variant& var, slib::IOStream& strm);
			extern SLIB_DLL void writeAnnotGenes(const slib::sbio::Variant& var, slib::IOStream& strm);
			extern SLIB_DLL void writeAnnotGeneTypes(const slib::sbio::Variant& var, slib::IOStream& strm);
			extern SLIB_DLL void writeVariantSites(const slib::sbio::Variant& var, slib::IOStream& strm);
			extern SLIB_DLL void writeMutationType(const slib::sbio::Variant& var, slib::IOStream& strm);
			extern SLIB_DLL void writeSubstitution(const slib::sbio::Variant& var, slib::IOStream& strm);
			extern SLIB_DLL void writeVarEffect(const slib::sbio::Variant& var, slib::IOStream& strm);
		}
		namespace sutil {
			extern SLIB_DLL float readBias(const sushort* r);
			extern SLIB_DLL float combBias(sushort* r1, sushort* r2);
			
			extern SLIB_DLL Range<int> transcriptSite(const srange& rng, TranscriptInfo* trs, bool rev);
			extern SLIB_DLL Range<int> codingSite(const srange& rng, TranscriptInfo* trs, bool rev);
			extern SLIB_DLL Range<Pair<int, int>> proteinSite(const srange& rng, TranscriptInfo* trs, bool rev);

			extern SLIB_DLL sushort varType(const char *type, const char *sep = "+");
			extern SLIB_DLL String varTypeStr(const sushort type, const char* sep = "+");
			extern SLIB_DLL String varTypeDesc(const sushort type);
			
			extern SLIB_DLL SVREAD_TYPE toSVIndex(const sushort type);

			extern SLIB_DLL std::function<bool(const SVar&, const SVar&)> svsorter(const subyte type, bool counter = false);
			extern SLIB_DLL bool delsorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2);
			extern SLIB_DLL bool delsorterc(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2);
			extern SLIB_DLL bool dupsorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2);
			extern SLIB_DLL bool inssorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2);
			extern SLIB_DLL bool invsorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2);
			extern SLIB_DLL bool trssorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2);
			extern SLIB_DLL bool trinvsorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2);
		}
		class SLIB_DLL VarFilter {
		public:
			SeqList* reference;
			AnnotDB* annotdb;
			VarParam* param;
			Array<sregion>* target;
			String buffer[2];

		public:
			VarFilter(SeqList* ref = nullptr, AnnotDB* db = nullptr, VarParam* param = nullptr, Array<sregion>* target = nullptr);
			~VarFilter();
			VarFilter& operator=(const VarFilter& filter);
			//void subtract(Array<SVar>& variants1, Array<SVar>& variants2);
			
			//void makeIndex(Array<SVar>& variants, Array<ArrayIterator<SVar>>& index);

			void merge(slib::sbio::VarList& vl1, slib::sbio::VarList& vl2);
			void subtract(slib::sbio::VarList& vl1, slib::sbio::VarList& vl2);
			void common(slib::sbio::VarList& vl1, slib::sbio::VarList& vl2);
			void unique(slib::sbio::VarList& uni, slib::sbio::VarList& vl1, slib::sbio::VarList& vl2);
			void setRefName(slib::sbio::VarList* vl);
		private:
			void delAnnot(slib::sbio::Variant* var);
			void dupAnnot(slib::sbio::Variant* var);
			void insAnnot(slib::sbio::Variant* var);
			void invAnnot(slib::sbio::Variant* var);
			void trsAnnot(slib::sbio::Variant* var);
			void complexAnnot(slib::sbio::Variant* var);

		public:
			void annotate(slib::sbio::Variant* var);
			void annotate(slib::Array<slib::SPointer<slib::sbio::Variant>>& variants, SWork* threads = nullptr);
			//void annotate(slib::sbio::VarList& vl, SWork* threads = nullptr);
			void checkRepeat(slib::sbio::VarList *list, SeqList* ref);
			

			void check(slib::sbio::Variant* var);
			
			void filter(slib::sbio::VarList& list);


			
			//void sortBy();
			void linkGene(slib::sbio::Variant* var);
			void linkGenes(slib::sbio::VarList* list);

			void setReference(SeqList* ref);
			void setDB(AnnotDB* db);
			void setParam(VarParam* p);
			void setTarget(Array<sregion>* reg);
		};
	}
}

#endif