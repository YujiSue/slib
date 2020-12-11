#ifndef SBIO_SPRIMER_H
#define SBIO_SPRIMER_H

#include "sbioinfo/sbioseq.h"

namespace slib {
    namespace sbio {
        
        #define DEFAULT_PRIMER_LENGTH 20
        #define DEFAULT_MIN_SPACE_SIZE 20
        #define DEFAULT_MAX_SELF_COMP 5
        #define DEFAULT_MAX_MATCH 100
        #define DEFAULT_PRIMER_RANGE_BEG 100
        #define DEFAULT_PRIMER_RANGE_END 1000
        #define DEFAULT_PRIMER_MIN_TEMP 50.0
        #define DEFAULT_PRIMER_MAX_TEMP 80.0
        #define DEFAULT_PRIMER_MIN_GC 45.0
        #define DEFAULT_PRIMER_MAX_GC 60.0
        #define DEFAULT_PRIMER_BIAS 0.5
        #define DEFAULT_MIN_AMP_SIZE 200
        #define DEFAULT_MAX_AMP_SIZE 10000
        #define DEFAULT_MAX_CROSS_COMP 5
        #define DEFAULT_MAX_DIF_TEMP 10.0
        
        struct sprimer_pair_param {
            srange min_ampl;
            sint max_cross_comp;
            float max_dif_temp;
            
			sprimer_pair_param();
			sprimer_pair_param(const sobj& obj);
            ~sprimer_pair_param();
            sobj toObj();
        };
        struct sprimer_score {
            float score_threshold, self_comp_score, cross_comp_score,
            match_score, patial_match_score, three_match_score,
            three_gc_score, three_t_score, bias_score, cross_amp_score;
            
			sprimer_score();
			sprimer_score(const sobj& obj);
            ~sprimer_score();
			sobj toObj();
        };
        struct SBIOINFO_DLL sprimer_param {
            sint length, space, max_self_comp, max_match;;
            srange primer_range;
            float primer_temp[2];
            sfrac primer_gc[2], max_bias;
            bool nested, three_gc, three_t_except;
			sprimer_pair_param pair_par;
			sprimer_score score_par;
            
			sprimer_param();
			sprimer_param(const sobj& obj);
            ~sprimer_param();
            sobj toObj();
        };

		extern double nnEnthalpy(SBioSeq* seq);
		extern double nnEntropy(SBioSeq* seq);
		extern void tmCalc(double* tm, SBioSeq* seq);
		extern sint crossComplement(SBioSeq* seq1, SBioSeq* seq2);
		/*{
			int crosscomp = 0;
			std::string cstr = seq2.raw(), sub;
			dcomp(cstr);
			size_t length = seq1.length() - 3, pos = 0;
			for (int i = 0; i < length; ++i) {
				sub = seq1.decode(i, 3);
				while ((pos = cstr.find(sub, pos)) != std::string::npos) { ++crosscomp; ++pos; }
				pos = 0;
			}
			return crosscomp;
		}
		*/
		class SBIOINFO_DLL SPrimer {
			sushort flag;
			SBioSeq* seq;
			srange range;
			bool dir;
			Array<sbpos> match[3];

			SPrimer();
			SPrimer(SBioSeq* s, sint p, sint l, bool d);
			SPrimer(const char *seq);
			SPrimer(const SPrimer&p);
            ~SPrimer();

			double score(sprimer_param* par) const;
			double tm() const;
			sint bias() const;
			sint selfcomp() const;
			String toString() const;
        };
		extern bool checkPrimer(SPrimer* p, sprimer_param* par);
		
    }
}

#endif