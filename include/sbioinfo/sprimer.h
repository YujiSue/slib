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
		extern bool checkPrimer(subyte* seq, size_t len, sprimer_param* par);
		namespace sseq {
			extern sfrac baseBias(subyte* seq, size_t len);
			extern double tmCalc(double* tm, subyte* seq, size_t len);
			extern sregion selfComp(subyte* seq, size_t len);
			extern sregion crossComp(subyte* seq1, size_t len1, subyte* seq2, size_t len2);
		}
		class SBIOINFO_DLL SPrimer {
			sint score;
			ubytearray seq;
			Array<sbpos> match[3];

			SPrimer();
			SPrimer(const char *s, bool dir = false);
			SPrimer(subyte* s, size_t len, bool dir = false);
			SPrimer(const SPrimer&p);
            ~SPrimer();

			size_t length() const;
			String sequence() const;
			double tm() const;
			sfrac bias() const;
			sint scoring(sprimer_param* par);			
			sobj summary() const;
        };
    }
}

#endif