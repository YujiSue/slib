#include "sbioinfo/sprimer.h"

using namespace slib;
using namespace slib::sbio;

sprimer_pair_param::sprimer_pair_param() {
	min_ampl = srange(DEFAULT_MIN_AMP_SIZE, DEFAULT_MAX_AMP_SIZE);
    max_cross_comp = DEFAULT_MAX_CROSS_COMP;
    max_dif_temp = DEFAULT_MAX_DIF_TEMP;
}
sprimer_pair_param::~sprimer_pair_param() {}
/*
void sprimer_pair_param::set(sobj &obj) {
    if (obj["amp"]) amplification = srange(obj["amp"]["min"], obj["amp"]["max"]);
    if (!obj["comp"].isNull()) max_cross_comp = obj["comp"];
    if (!obj["temp"].isNull()) max_dif_temp = obj["temp"];
}
*/
sobj sprimer_pair_param::toObj() {
    return {
        kv("amp", V({ kv("min", min_ampl.begin), kv("max", min_ampl.end) })),
        kv("comp", max_cross_comp), kv("temp", max_dif_temp)
    };
}

sprimer_score::sprimer_score() {}
sprimer_score::~sprimer_score() {}
//void sprimer_score::set(sobj &obj) {}
sobj sprimer_score::toObj() {
    return sdict({
        kv("threshold", score_threshold)
    });
}

sprimer_param::sprimer_param() {
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
sprimer_param::~sprimer_param() {}
//void sprimer_param::set(sobj &obj) {}
sobj sprimer_param::toObj() {
    return sdict({
        kv("length", length),
        kv("space", space)
        
        
        
        
    });
}

/*
inline int matchedLength(char *s1, char *s2, int l) {
    int m = 0;
    for(int i = 0; i < l; ++i) { if(s1[i] == s2[i]) ++m; }
    return m;
}

sprimer::sprimer() {
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
*/

inline double nnEnthalpy(subyte* seq, size_t len) {
	double enthalpy = 0.0;
	sforin(i, 0, len - 1) {
		if ((*seq) == 0x01) {
			if (*(seq + 1) == 0x01) enthalpy -= 9.1;
			else if (*(seq + 1) == 0x02) enthalpy -= 6.5;
			else if (*(seq + 1) == 0x04) enthalpy -= 7.8;
			else if (*(seq + 1) == 0x08) enthalpy -= 8.6;
			else return 0.0;
		}
		else if ((*seq) == 0x02) {
			if (*(seq + 1) == 0x01) enthalpy -= 5.8;
			else if (*(seq + 1) == 0x02) enthalpy -= 11.0;
			else if (*(seq + 1) == 0x04) enthalpy -= 11.9;
			else if (*(seq + 1) == 0x08) enthalpy -= 7.8;
			else return 0.0;
		}
		else if ((*seq) == 4) {
			if (*(seq + 1) == 0x01) enthalpy -= 5.6;
			else if (*(seq + 1) == 0x02) enthalpy -= 11.1;
			else if (*(seq + 1) == 0x04) enthalpy -= 11.0;
			else if (*(seq + 1) == 0x08) enthalpy -= 6.5;
			else return 0.0;
		}
		else if ((*seq) == 0x08) {
			if (*(seq + 1) == 0x01) enthalpy -= 6.0;
			else if (*(seq + 1) == 0x02) enthalpy -= 5.6;
			else if (*(seq + 1) == 0x04) enthalpy -= 5.8;
			else if (*(seq + 1) == 0x08) enthalpy -= 9.1;
			else return 0.0;
		}
		else return 0.0;
		++seq;
	}
	return enthalpy;
}
inline double nnEntropy(subyte* seq, size_t len) {
	double entropy = 0.0;
	sforin(i, 0, len - 1) {
		if ((*seq) == 0x01) {
			if (*(seq + 1) == 0x01) entropy -= 24.0;
			else if (*(seq + 1) == 0x02) entropy -= 17.3;
			else if (*(seq + 1) == 0x04) entropy -= 20.8;
			else if (*(seq + 1) == 0x08) entropy -= 23.9;
			else return 0.0;
		}
		else if ((*seq) == 0x02) {
			if (*(seq + 1) == 0x01) entropy -= 12.9;
			else if (*(seq + 1) == 0x02) entropy -= 26.6;
			else if (*(seq + 1) == 0x04) entropy -= 27.8;
			else if (*(seq + 1) == 0x08) entropy -= 20.8;
			else return 0.0;
		}
		else if ((*seq) == 0x04) {
			if (*(seq + 1) == 0x01) entropy -= 13.5;
			else if (*(seq + 1) == 0x02) entropy -= 26.7;
			else if (*(seq + 1) == 0x04) entropy -= 26.6;
			else if (*(seq + 1) == 0x08) entropy -= 17.3;
			else return 0.0;
		}
		else if ((*seq) == 0x08) {
			if (*(seq + 1) == 0x01) entropy -= 16.9;
			else if (*(seq + 1) == 0x02) entropy -= 13.5;
			else if (*(seq + 1) == 0x04) entropy -= 12.9;
			else if (*(seq + 1) == 0x08) entropy -= 24.0;
			else return 0.0;
		}
		else return 0.0;
		++seq;
	}
	return entropy;
}
sfrac slib::sbio::sseq::baseBias(subyte* seq, size_t len) {
	sint bias = 0, count = 0;
	sforin(i, 0, len - 1) {
		auto count = sseq::gcCounti(seq, i, 2);
		if (!count || count == 2) ++bias;
	}
	return sfrac(bias, len - 1);
}
double slib::sbio::sseq::tmCalc(double* tm, subyte* seq, size_t len) {
	sint gc_count = sseq::gcCounti(seq, 0, len);
	tm[0] = 16.6 * log10(0.05) - 273.15 + (1000.0 * nnEnthalpy(seq, len)) / (-10.8 + nnEntropy(seq, len) + 1.987 * (log(0.125) - 6 * log(10.0)));
	tm[1] = 4 * gc_count + 2 * (len - gc_count);
	tm[2] = 81.5 + 16.6 * log10(0.05) + 41.0 * gc_count / len - 500.0 / len;
	return tm[0] < 20.0 ? tm[1] : (tm[0] < 80.0 ? tm[0] : tm[2]);
}
sregion slib::sbio::sseq::selfComp(subyte* seq, size_t len) {
	sregion reg;
	ubytearray comp;
	comp.copy(seq, len);
	sseq::dcompi(comp);
	auto p1 = seq;
	sforin(i, 0, len - 2) {
		auto p2 = comp.ptr();
		sforin(j, 0, len - 2) {
			bool match = true;
			sforin(k, 0, 3) {
				if ((*(p1 + k)) != (*(p2 + k))) { match = false; break; }
			}
			if (match) reg.add(i, j);
			++p2;
		}
		++p1;
	}
	return reg;
}
sregion crossComp(subyte* seq1, size_t len1, subyte* seq2, size_t len2) {
	sregion reg;
	ubytearray comp;
	comp.copy(seq2, len2);
	sseq::dcompi(comp);
	auto p1 = seq1;
	sforin(i, 0, len1 - 2) {
		auto p2 = comp.ptr();
		sforin(j, 0, len2 - 2) {
			bool match = true;
			sforin(k, 0, 3) {
				if ((*(p1 + k)) != (*(p2 + k))) { match = false; break; }
			}
			if (match) reg.add(i, j);
			++p2;
		}
		++p1;
	}
	return reg;
}

SPrimer::SPrimer() : score(0) {}
SPrimer::SPrimer(const char* s, bool dir) : SPrimer() {
	seq.resize(strlen(s));
	sseq::dencode1((const subyte*)s, 0, seq.size(), seq.ptr());
	if (dir) sseq::dcompi(seq);
}
SPrimer::SPrimer(subyte* s, size_t len, bool dir) : SPrimer() {
	seq.resize(len);
	memcpy(seq.ptr(), s, len);
	if (dir) sseq::dcompi(seq);
}
SPrimer::SPrimer(const SPrimer& p) : SPrimer() {
	score = p.score;
	sforin(i, 0, 3) match[i] = p.match[i];
}
SPrimer::~SPrimer() {}
size_t SPrimer::length() const { return seq.size(); }
double SPrimer::tm() const  {
	if (seq.empty()) return 0;
	double values[3];
	return sseq::tmCalc(values, seq.ptr(), seq.size());
}
sfrac SPrimer::bias() const {
	if (seq.empty()) return 0;
	return sseq::baseBias(seq.ptr(), seq.size());
}
String SPrimer::sequence() const {
	String str(seq.size(), '\0');
	sseq::ddecode1(seq.ptr(), 0, seq.size(), (subyte*)str.ptr());
	return str;
}
sint SPrimer::scoring(sprimer_param* par) {
	score = 0;


	return score;
}
sobj SPrimer::summary() const {
	double temp[3];
	auto tm = sseq::tmCalc(temp, seq.ptr(), seq.size());
	sarray sc;
	auto reg = sseq::selfComp(seq.ptr(), seq.size());
	if (!reg.empty()) {
		sforeach(reg) sc.add(sarray({ E_.begin, E_.end }));
	}
	return {
		kv("seq", sequence()), 
		kv("gc", sseq::gcCounti(seq)),
		kv("temp", V({ temp[0], temp[1], temp[2] })),
		kv("tm", tm),
		kv("bias", bias()),
		kv("selfcomp", sc.empty()?snull:sc),
		kv("terminal", V({ kv("gc", sseq::isGCi(seq[-1])), kv("t", seq[-1]==sbio::DNA_BASE16_INDEX['T']) })),
		kv("score", score)
	};
}