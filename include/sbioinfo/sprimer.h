#ifndef __SPRIMER_H__
#define __SPRIMER_H__

#include "sobj/sobject.h"
#include "sbioinfo/sbioseq.h"
#include "sbioinfo/sbsearch.h"

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
        
        struct primer_t {
            sbpos_t pos;
            sbioseq sequence;
            double tm_value[3], score;
            sfraction gc_ratio;
            int self_comp, bias;
            slist<sbpos_t> match[3];
            bool three_g, three_t;
            
            primer_t();
            primer_t(const char *s);
            primer_t(const sbpos_t &p, const char *s);
            primer_t(const primer_t &p);
            ~primer_t();
            
            void setSeq(const char *s);
            double melttemp();
            
            void calcScore(primer_param_t *par);
            bool isAvailable(primer_param_t *par);
            bool scoreCheck(primer_param_t *par);
            void matchCount(int q, sdnafile *ref, std::vector<std::vector<align_vec>> *vec);
            
            bool operator < (const primer_t &p) const;
            bool operator == (const primer_t &p) const;
        };
        
        struct primer_pair_t {
            primer_t *primer1, *primer2;
            int amp_size, cross_comp;
            double dif_temp, cross_score;
            slist<std::pair<salign_t *, salign_t *>> false_positive;
            
            primer_pair_t(primer_t *p1, primer_t *p2);
            primer_pair_t(const primer_pair_t &pp);
            ~primer_pair_t();
            
            void calcScore(primer_param_t *par);
            
            bool operator<(const primer_pair_t &pp) const;
            bool operator==(const primer_pair_t &pp) const;
        };
    }
}

#endif