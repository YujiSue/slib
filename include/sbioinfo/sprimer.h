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
        
        struct primer_pair_param_t {
            srange amplification;
            int32_t max_cross_comp;
            float max_dif_temp;
            
            primer_pair_param_t();
            ~primer_pair_param_t();
            
            void set(sobj &obj);
            sobj toObj();
        };
        struct primer_score_t {
            float score_threshold, self_comp_score, cross_comp_score,
            match_score, patial_match_score, three_match_score,
            three_gc_score, three_t_score, bias_score, cross_amp_score;
            
            primer_score_t();
            ~primer_score_t();
            
            void set(sobj &obj);
            sobj toObj();
        };
        struct primer_param_t {
            int32_t length, space, max_self_comp, max_match;;
            srange primer_range;
            float primer_temp[2];
            sfrac primer_gc[2], max_bias;
            bool nested, three_gc, three_t_except;
            primer_pair_param_t pair_par;
            primer_score_t score_par;
            
            primer_param_t();
            ~primer_param_t();
            
            void set(sobj &obj);
            sobj toObj();
        };
        
        struct sprimer {
            sbpos pos;
            sbseq sequence;
            double score;
            
			sprimer();
			sprimer(const char *s);
			sprimer(const sbpos&p, const char *s);
			sprimer(const sprimer&p);
            ~sprimer();
            
            void setSeq(const char *s);
            double melttemp();
            
            void calcScore(primer_param_t *par);
            bool isAvailable(primer_param_t *par);
            bool scoreCheck(primer_param_t *par);
            void matchCount(int q, sdnafile *ref, std::vector<std::vector<align_vec>> *vec);
            
            bool operator < (const primer_t &p) const;
            bool operator == (const primer_t &p) const;
        };
    }
}

#endif