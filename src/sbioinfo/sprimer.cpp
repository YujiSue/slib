#include "sbioinfo/sprimer.h"

using namespace slib;
using namespace slib::sbio;

primer_pair_param_t::primer_pair_param_t() {
    amplification = srange(DEFAULT_MIN_AMP_SIZE, DEFAULT_MAX_AMP_SIZE);
    max_cross_comp = DEFAULT_MAX_CROSS_COMP;
    max_dif_temp = DEFAULT_MAX_DIF_TEMP;
}
primer_pair_param_t::~primer_pair_param_t() {}

void primer_pair_param_t::set(sobj &obj) {
    if (obj["amp"]) amplification = srange(obj["amp"]["min"], obj["amp"]["max"]);
    if (!obj["comp"].isNull()) max_cross_comp = obj["comp"];
    if (!obj["temp"].isNull()) max_dif_temp = obj["temp"];
}
sobj primer_pair_param_t::toObj() {
    return sdict({
        kv("amp", sdict({ kv("min", amplification.begin), kv("max", amplification.end) })),
        kv("comp", max_cross_comp), kv("temp", max_dif_temp)
    });
}

primer_score_t::primer_score_t() {}
primer_score_t::~primer_score_t() {}

void primer_score_t::set(sobj &obj) {}
sobj primer_score_t::toObj() {
    return sdict({
        kv("threshold", score_threshold)
    });
}

primer_param_t::primer_param_t() {
    length = DEFAULT_PRIMER_LENGTH;
    space = DEFAULT_MIN_SPACE_SIZE;
    max_self_comp = DEFAULT_MAX_SELF_COMP;
    max_match = DEFAULT_MAX_MATCH;
    primer_range = srange(DEFAULT_PRIMER_RANGE_BEG, DEFAULT_PRIMER_RANGE_END);
    primer_temp[0] = DEFAULT_PRIMER_MIN_TEMP;
    primer_temp[1] = DEFAULT_PRIMER_MAX_TEMP;
    primer_gc[0] = DEFAULT_PRIMER_MIN_GC;
    primer_gc[1] = DEFAULT_PRIMER_MAX_GC;
    max_bias = DEFAULT_PRIMER_BIAS;
    nested = false;
    three_gc = false;
    three_t_except = false;
}
primer_param_t::~primer_param_t() {}

void primer_param_t::set(sobj &obj) {

}
sobj primer_param_t::toObj() {
    return sdict({
        kv("length", length),
        kv("space", space)
        
        
        
        
    });
}

inline double nnEnthalpy(const sbseq &seq) {
    double enthalpy = 0.0;
    for(int i = 0; i < seq->length()-1; i++) {
        if(seq[i] == 1) {
            if(seq[i+1] == 1) enthalpy -= 9.1;
            else if(seq[i+1] == 2) enthalpy -= 6.5;
            else if(seq[i+1] == 4) enthalpy -= 7.8;
            else if(seq[i+1] == 8) enthalpy -= 8.6;
            else return 0.0;
        }
        else if(seq[i] == 2) {
            if(seq[i+1] == 1) enthalpy -= 5.8;
            else if(seq[i+1] == 2) enthalpy -= 11.0;
            else if(seq[i+1] == 4) enthalpy -= 11.9;
            else if(seq[i+1] == 8) enthalpy -= 7.8;
            else return 0.0;
        }
        else if(seq[i] == 4) {
            if(seq[i+1] == 1) enthalpy -= 5.6;
            else if(seq[i+1] == 2) enthalpy -= 11.1;
            else if(seq[i+1] == 4) enthalpy -= 11.0;
            else if(seq[i+1] == 8) enthalpy -= 6.5;
            else return 0.0;
        }
        else if(seq[i] == 8) {
            if(seq[i+1] == 1) enthalpy -= 6.0;
            else if(seq[i+1] == 2) enthalpy -= 5.6;
            else if(seq[i+1] == 4) enthalpy -= 5.8;
            else if(seq[i+1] == 8) enthalpy -= 9.1;
            else return 0.0;
        }
        else return 0.0;
    }
    return enthalpy;
}
inline double nnEntropy(const sbseq &seq) {
    double entropy = 0.0;
    for(int i = 0; i < seq->length()-1; i++) {
        if(seq[i] == 1) {
            if(seq[i+1] == 1) entropy -= 24.0;
            else if(seq[i+1] == 2) entropy -= 17.3;
            else if(seq[i+1] == 4) entropy -= 20.8;
            else if(seq[i+1] == 8) entropy -= 23.9;
            else return 0.0;
        }
        else if(seq[i] == 2) {
            if(seq[i+1] == 1) entropy -= 12.9;
            else if(seq[i+1] == 2) entropy -= 26.6;
            else if(seq[i+1] == 4) entropy -= 27.8;
            else if(seq[i+1] == 8) entropy -= 20.8;
            else return 0.0;
        }
        else if(seq[i] == 4) {
            if(seq[i+1] == 1) entropy -= 13.5;
            else if(seq[i+1] == 2) entropy -= 26.7;
            else if(seq[i+1] == 4) entropy -= 26.6;
            else if(seq[i+1] == 8) entropy -= 17.3;
            else return 0.0;
        }
        else if(seq[i] == 8) {
            if(seq[i+1] == 1) entropy -= 16.9;
            else if(seq[i+1] == 2) entropy -= 13.5;
            else if(seq[i+1] == 4) entropy -= 12.9;
            else if(seq[i+1] == 8) entropy -= 24.0;
            else return 0.0;
        }
        else return 0.0;
    }
    return entropy;
}
inline void tmCalc(double *tm, const sbseq &seq) {
    int gc_count = slib::sbio::gcCounti(seq->), length = seq->length();
    tm[0] = 16.6*log10(0.05)-273.15+(1000.0*nnEnthalpy(seq))/(-10.8+nnEntropy(seq)+1.987*(log(0.125)-6*log(10.0)));
    tm[1] = 4*gc_count+2*(length-gc_count);
    tm[2] = 81.5+16.6*log10(0.05) + 41.0*gc_count/length-500.0/length;
}
inline int selfComplement(const sbseq&seq) {
    int selfcomp = 0;
    std::string raw = seq.raw(), sub;
    size_t length = seq.length()-3;
    for(int i = 0; i < length; ++i) {
        size_t pos = 0;
        sub = seq.decode(i,3);
        dcomp(&sub[0]);
        while ((pos = raw.find(sub, pos)) != std::string::npos) { ++selfcomp; ++pos; }
    }
    return selfcomp;
}
inline int crossComplement(const sbioseq &seq1, const sbioseq &seq2) {
    int crosscomp = 0;
    std::string cstr = seq2.raw(), sub;
    dcomp(cstr);
    size_t length = seq1.length()-3, pos = 0;
    for(int i = 0; i < length; ++i) {
        sub = seq1.decode(i,3);
        while ((pos = cstr.find(sub, pos)) != std::string::npos) { ++crosscomp; ++pos; }
        pos = 0;
    }
    return crosscomp;
}
inline int countBias(const sbioseq &seq) {
    size_t length = seq.length()-3;
    int bias = 0;
    for(int i = 0; i < length; ++i) {
        std::string sub = seq.decode(i ,3);
        size_t gc_count = gcCount(sub.c_str());
        if(!gc_count || gc_count == 3) ++bias;
    }
    return bias;
}
inline int matchedLength(char *s1, char *s2, int l) {
    int m = 0;
    for(int i = 0; i < l; ++i) { if(s1[i] == s2[i]) ++m; }
    return m;
}

primer_t::primer_t() {
    sequence = "";
    memset(tm_value, 0, sizeof(double)*3);
    score = 0.0;
    self_comp = 0;
    bias = 0;
    three_g = false;
    three_t = false;
}
primer_t::primer_t(const char *s) : primer_t() { setSeq(s); }
primer_t::primer_t(const sbpos_t &p, const char *s) : primer_t() { pos = p; setSeq(s); }
primer_t::primer_t(const primer_t &p) {
    sequence = p.sequence;
    pos = p.pos;
    memcpy(tm_value, p.tm_value, sizeof(double)*3);
    gc_ratio = p.gc_ratio;
    self_comp = p.self_comp;
    bias = p.bias;
    for(int i = 0; i < 3; ++i) match[i] = p.match[i];
    three_g = p.three_g;
    three_t = p.three_t;
    score = p.score;
}
primer_t::~primer_t() {}

void primer_t::setSeq(const char *s) {
    sequence = sbioseq(SB_DNA_SEQ, s);
    gc_ratio = sfrac(gcCounti(sequence.seq()), sequence.length());
    tmCalc(tm_value, sequence);
    self_comp = selfComplement(sequence);
    bias = countBias(sequence);
    three_g = sequence[sequence.length()-1]==2||sequence[sequence.length()-1]==4?true:false;
    three_t = sequence[sequence.length()-1]==8?true:false;
    score = 0.0;
}
double primer_t::melttemp() {
    return tm_value[0]<20.0?tm_value[1]:(tm_value[0]<80.0?tm_value[0]:tm_value[2]);
}
void primer_t::calcScore(primer_param_t *par) {
    score += par->score_par.self_comp_score*self_comp +
    par->score_par.bias_score*bias +
    par->score_par.match_score*match[0].size() +
    par->score_par.patial_match_score*match[1].size() +
    par->score_par.three_match_score*match[2].size() +
    (three_g?par->score_par.three_gc_score:(three_t?par->score_par.three_t_score:0));
}
bool primer_t::isAvailable(primer_param_t *par) {
    if(gc_ratio < par->primer_gc[0] || par->primer_gc[1] < gc_ratio ||
       melttemp() < par->primer_temp[0] || par->primer_temp[1] < melttemp() ||
       par->max_self_comp < self_comp ||
       par->max_bias < sfrac(bias, sequence.length()) ||
       (par->three_gc && !three_g) || (par->three_t_except && three_t)) return false;
    return true;
}
bool primer_t::scoreCheck(primer_param_t *par) {
    if(par->max_match < match[0].size()+match[1].size()+match[2].size() ||
       par->score_par.score_threshold < score) return false;
    return true;
}
void primer_t::matchCount(int q, sdnafile *ref, std::vector<std::vector<align_vec>> *vec) {
    std::string rseq, qseq = sequence.raw();
    size_t size = sequence.length();
    for (int i = 0; i < vec->size(); ++i) {
        align_vec &aligns = vec->at(i)[q];
        for(auto it = aligns.begin(); it < aligns.end(); ++it) {
            if(it->pos==0 && it->ref.len == size) match[0].add(it->ref);
            else {
                rseq = ref->getSequence(it->ref.idx, it->ref.pos-it->pos, size, it->ref.dir);
                int m1 = matchedLength(&rseq[0], &qseq[0], size/2),
                m2 = matchedLength(&rseq[size/2], &qseq[size/2], size-size/2);
                if (size*4/5 <= (m1+m2)) match[1].add(it->ref.idx, it->ref.pos-it->pos, size, it->ref.dir);
                else if(size/2 <= m2) match[2].add(it->ref.idx, it->ref.pos-it->pos, size, it->ref.dir);
            }
        }
    }
}

bool primer_t::operator<(const primer_t &p) const {
    if(pos != p.pos) return pos < p.pos;
    else if(sequence.length() != p.sequence.length()) return sequence.length() < p.sequence.length();
    return false;
}
bool primer_t::operator==(const primer_t &p) const {
    return pos == p.pos && sequence == p.sequence;
}
