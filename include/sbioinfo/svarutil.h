#ifndef SBIO_SVARUTIL_H
#define SBIO_SVARUTIL_H

#include "sutil/sthread.h"
#include "sbioinfo/sbannot.h"
#include "sbioinfo/sbioseq.h"

namespace slib {
	using namespace smath;
	namespace sbio {
		struct SBIOINFO_DLL svar_data;
		class SBIOINFO_DLL SVariant;
		class SBIOINFO_DLL SVarList;

#define DEFAULT_VC_DP 5
#define DEFAULT_VC_VDP 3
#define DEFAULT_VC_FREQ 0.1
#define DEFAULT_VC_HOMO_FREQ 0.75
#define DEFAULT_VC_QUAL 10.0

#define DEFAULT_MIN_CVLEN 20
#define DEFAULT_BG_DEPTH 3.0
#define DEFAULT_HOMO_DEL 0.35
#define DEFAULT_DELCP 0.75
#define DEFAULT_DUPCP 1.35
#define DEFAULT_HOMO_DUPCP 1.75
#define DEFAULT_MULCP 2.5

#define DEFAULT_SR_READ 3
#define DEFAULT_MIN_DEL_SIZE 1
#define DEFAULT_MIN_INS_SIZE 1
#define DEFAULT_MIN_INV_SIZE 1
#define DEFAULT_FREQ_BIN 20
#define DEFAULT_MIN_FREQ 0.1
#define DEFAULT_FR_BIAS 1.0
#define DEFAULT_COMP_BIAS 1.0
#define DEFAULT_HOMO_FREQ 0.65

#define DEFAULT_MIN_QUAL 1.0

#define DEFAULT_DIST 50
#define DEFAULT_DIFF 100
#define DEFAULT_FREQ_BIN 20

		struct svc_param {
			//0:SNV, 1:MNV, 2:DEL, 3:INS
			sint min_depth[4], min_vdepth[4];
			double min_freq[4], homo_freq[4] , min_qual[4];
			
			svc_param();
			svc_param(const svc_param& p);
			~svc_param();

			svc_param& operator=(const svc_param& par);
			void set(const sobj& obj);
			sobj toObj();
		};

		typedef enum {
			HOMO_DEL_CNV = 0,
			HETERO_DEL_CNV = 1,
			NON_CNV = 2,
			HETERO_DUP_CNV = 3,
			HOMO_DUP_CNV = 4,
			MULTI_CNV = 5,
		} CNV_TYPE;

		struct scnv_param {
			

			//copy: [0, border[0]), [border[0], border[1]), [border[1], border[2]), [border[2], border[3]), [border[3], border[4]), [border[4], +inf)
			sint min_length;
			double min_bg, border[5], min_qual;
			matd emission, transition;

			scnv_param();
			scnv_param(const scnv_param& p);
			~scnv_param();

			scnv_param& operator=(const scnv_param& par);
			void set(const sobj& obj);
			sobj toObj();
		};

		struct ssrv_param {
			bool detect_var[5], detect_comp_var[4];
			sint freq_bin, min_sr[5], min_comp_sr[4], min_length[3];// 0:del, 1:dup/ins, 2:inv
			double min_freq, max_fr_bias, max_comp_bias, homo_freq, min_qual;

			ssrv_param();
			ssrv_param(const ssrv_param& p);
			~ssrv_param();

			ssrv_param& operator=(const ssrv_param& par);
			void set(const sobj& obj);
			sobj toObj();
		};

		struct svariant_param {
			bool homo_select;
			sushort annot;
			sint max_dist, max_diff;

			scnv_param cnv_par;
			ssrv_param srv_par;
			svc_param smv_par;

			svariant_param();
			svariant_param(const svariant_param& p);
			~svariant_param();

			svariant_param& operator=(const svariant_param& par);
			void set(const sobj& obj);
			sobj toObj();
		};

		class SBIOINFO_DLL SVarUtil {
		public:
			static float readBias(const sushort *r);
			static float combBias(sushort *r1, sushort *r2);
			static String vtype(sushort i);
			static sushort vtypeIdx(const char* s);
			static String vsite(sushort i);
			static subyte vsiteIdx(const char* s);
			static String mtype(sushort i);
			static subyte mtypeIdx(const char* s);
		};
		
		class SBIOINFO_DLL SVarIO {
		public:
			static const stringarray VCF_TABLE_COLUMNS;
			static const stringarray DEFAULT_TABLE_COLUMNS;

			static const sattribute ION_VAR_CALL;

			static void loadTxt(sio::SFile& file, SVarList* list, SBSeqList* ref);
			static void loadTSV(sio::SFile& file, SVarList* list, SBSeqList* ref);
			static void loadVCF(sio::SFile& file, SVarList* list, SBSeqList* ref, sattribute *converter);
			static void loadJSON(sio::SFile& file, SVarList* list);
			
			static void saveTxt(sio::SFile& file, SVarList* list);
			static void saveTSV(sio::SFile& file, SVarList* list, const stringarray& col = {""});
			static void saveVCF(sio::SFile& file, SVarList* list, SBSeqList* ref);
			static void saveJSON(sio::SFile& file, SVarList* list);

		};

		class SBIOINFO_DLL SVarFilter {
		private:
			SBSeqList* _ref;
			SBAnnotDB* _db;
			svariant_param* _par;
			Array<sregion>* _target;

		public:
			SVarFilter(SBSeqList* ref = nullptr, SBAnnotDB* db = nullptr, svariant_param* p = nullptr, Array<sregion>* t = nullptr);
			~SVarFilter();

			void consolidate(Array<svar_data>& variants);
			void subtract(Array<svar_data>& variants1, Array<svar_data>& variants2);
			void merge(SVarList& vl1, SVarList& vl2);
			void subtract(SVarList& vl1, SVarList& vl2);
			void common(SVarList& vl1, SVarList& vl2);
			void unique(SVarList& uni, SVarList& vl1, SVarList& vl2);
			
			void annotate(SVarList* vl, SWork* threads = nullptr);
			void checkRepeat(SVarList *list, SBSeqList* ref);
			//void filter(SVarList* list, const char *meht, sfunc<bool)1\);
			void filter(SVarList* list);
			//void sortBy();
			
			void check(SVariant* var);

			void setReference(SBSeqList* ref);
			void setDB(SBAnnotDB* db);
			void setParam(svariant_param* p);
			void setTarget(Array<sregion>* reg);
		};
	}
}

#endif