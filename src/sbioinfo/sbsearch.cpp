#include "sbioinfo/sbsearch.h"

using namespace slib;
using namespace slib::sbio;

sbsearch_param::sbsearch_param(sushort t) {
	ref_type = t & 0x0F;
	code_size = (t >> 4) & 0x0F;
    ds_search = ref_type==sbio::DNA_SEQ;
    multi_thread = true;
    min_match = DEFAULT_MIN_SIZE;
	seed_len = ref_type & AA_SEQ ? DEFAULT_AASEED : DEFAULT_SEED;
	coded_seed_len = seed_len / (code_size == 0 ? 1 : code_size);
    max_gap = DEFAULT_MAX_GAP;
    max_miss = DEFAULT_MAX_MISS;
    thread_count = DEFAULT_THREAD;
    extend_threshold = DEFAULT_THRESHOLD;
    aln_par = salign_param((subyte)ref_type);
}
sbsearch_param::~sbsearch_param() {}
sbsearch_param &sbsearch_param::operator=(const sbsearch_param &par) {
    ref_type = par.ref_type;
    code_size = par.code_size;
    ds_search = par.ds_search;
    multi_thread = par.multi_thread;
    min_match = par.min_match;
    seed_len = par.seed_len;
    coded_seed_len = par.coded_seed_len;
    max_gap = par.max_gap;
    max_miss = par.max_miss;
    thread_count = par.thread_count;
    extend_threshold = par.extend_threshold;
    aln_par = par.aln_par;
    return *this;
}
void sbsearch_param::setType(int t) {
	ref_type = t & 0x0F;
	code_size = (t >> 4) & 0x0F;
	coded_seed_len = seed_len / (code_size == 0 ? 1 : code_size);
	aln_par = salign_param((subyte)ref_type);
}
void sbsearch_param::setSeed(int s) {
    seed_len = s;
	coded_seed_len = seed_len / (code_size == 0 ? 1 : code_size);
}
void sbsearch_param::set(const sobj &obj) {
    if (obj["ref"]) ref_type = obj["ref"];
    if (obj["code"]) code_size = obj["code"];
    if (obj["ds"]) ds_search = obj["ds"]; else ds_search = false;
    if (obj["mt"]) multi_thread = obj["mt"]; else multi_thread = false;
    if (obj["match"]) min_match = obj["match"]; else max_gap = DEFAULT_MIN_SIZE;
    if (obj["seed"]) seed_len = obj["seed"];
    if (obj["cseed"]) coded_seed_len = obj["cseed"];
    if (obj["gap"]) max_gap = obj["gap"]; else max_gap = DEFAULT_MAX_GAP;
    if (obj["miss"]) max_miss = obj["miss"]; else max_miss = DEFAULT_MAX_MISS;
    if (obj["thread"]) thread_count = obj["thread"]; else thread_count = DEFAULT_THREAD;
    if (obj["threshold"]) extend_threshold = obj["threshold"];
    if (obj["align"]) aln_par.set(obj["align"]);
}
sobj sbsearch_param::toObj() {
    return {
        kv("ref", ref_type),
        kv("code", code_size),
        kv("ds", ds_search),
        kv("mt", multi_thread),
        kv("match", min_match),
        kv("seed", seed_len),
        kv("cseed", coded_seed_len),
        kv("gap", max_gap),
        kv("miss", max_miss),
        kv("thread", thread_count),
        kv("threshold", extend_threshold),
        kv("align", aln_par.toObj())
    };
}

SBSearch::SBSearch() : _par(nullptr), _qnum(0), _rnum(0) {}
SBSearch::SBSearch(sbsearch_param *lp) { setParam(lp); }
SBSearch::~SBSearch() {}
void SBSearch::_resize(size_t rn, size_t qn) {
    if (_matched.size() < rn*qn) { _matched.resize(rn*qn); aligns.resize(rn*qn); }
    _rnum = (sint)rn; _qnum = (sint)qn;
}
inline void assemble(int r, SBSearch::match_array *match, Array<salign> *vec, srange *range,
                     ubytearray *ref, ubytearray *que, bool dir, sbsearch_param *par) {
	if (match->empty()) return;
    size_t len;
    sforeach(*match) {
        if (E_.first < 0) continue;
        auto it_ = it+1;
        len = 0;
        while (it_ < match->end()) {
            if (E_.first + 1 < it_->first) break;
            if (E_.first + 1 == it_->first &&
                E_.second + par->code_size == it_->second) {
                E_.second += par->code_size; ++E_.first; it_->first = -1; ++len;
            }
            ++it_;
        }
        E_.first -= (sint)len; E_.first *= par->code_size; E_.first-=par->seed_len-par->code_size;
        len *= par->code_size; E_.second -= (sint)len; len += par->seed_len;
        if (range->end < E_.first+len) len = range->end-E_.first;
        if (len+2*((par->code_size?par->code_size:1)-1) < par->min_match) continue;
        if (1 < par->code_size) {
            switch (par->code_size) {
                case 2:
                {
                    subyte seq[2];
                    if (range->begin < E_.first && E_.second) {
                        sseq::ddec21(ref->at(E_.first/2-1), seq);
                        if (seq[1] == (char)que->at(E_.second-1)) { --E_.first; --E_.second; ++len; }
                    }
                    if (E_.first+len < range->end && E_.second+len < que->size()) {
                        sseq::ddec21(ref->at((E_.first+ (sint)len)/2), seq);
                        if (seq[0] == (char)que->at(E_.second+ (sint)len)) ++len;
                    }
                }
                case 4:
                {
                    subyte seq[4];
                    if (range->begin < E_.first && E_.second) {
                        sseq::ddec41(ref->at(E_.first/4-1), seq);
                        auto qseq = que->ptr(E_.second-1);
                        sforin(i, 0, 4) {
                            if (E_.second && seq[3-i] == *qseq) {
                                --E_.first; --E_.second; --qseq; ++len;
                            }
                            else break;
                        }
                    }
                    if (E_.first+len < range->end && E_.second+len < que->size()) {
                        sseq::ddec41(ref->at((E_.first+ (sint)len)/4), seq);
                        auto qseq = que->ptr(E_.second+len);
                        sforin(i, 0, 4) {
                            if (E_.first+len < range->end && E_.second+len < que->size() && seq[i] == *qseq) { ++qseq; ++len; }
                            else break;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
        if (par->min_match <= len) vec->add(sbpos(r, E_.first, E_.first+ (sint)len-1, dir), srange(E_.second, E_.second+(sint)len-1));
    }
    match->clear();
}
inline void seq_search(int r, ubytearray *ref, SBQuery *que, sregion *region, sbsearch_param *par,
                       SBSearch::match_array *matches, Array<salign> *vec) {
    srange range;
    subyte *refseq = ref->ptr();
    spma *root = que->root();
    short next;
    int current = 0;
    sforeach(*region) {
        range.begin = E_.begin/par->code_size;
        range.end = (E_.end/par->code_size)+1;
        if (current != range.begin) {
            refseq += (range.begin-current);
            current = range.begin;
        }
        while (current < range.end) {
            next = (short)(*refseq)+1;
            while (!(root->child[next])) root = root->child[0];
            root = root->child[next];
            if (!(root->match.empty())) {

                sforeach_(mit, root->match)
                    matches[mit->first].add(current, mit->second);
            }
            ++current; ++refseq;
        }
        sforin(q, 0, que->count()) assemble(r, &matches[q], &vec[q], &E_, ref, &que->query(q),
                                            (par->ds_search&&(q%2)), par);
    }
}
inline void search_i(int i, SBioSeq *ref, SBQuery *que, sbsearch_param *par,
                     SBSearch::match_array *matches, Array<salign> *vec) {
    sregion region;
    if (ref->isMasked() && !ref->mask().empty()) region = complement(srange(0, ref->length()-1), ref->mask());
    else region.add(srange(0, ref->length()-1));
    seq_search(i, ref, que, &region, par, matches, vec);
}
void SBSearch::search(SBioSeq *ref, SBQuery *que) {
    _resize(1, que->count()); _que = que;
    search_i(0, ref, que, _par, &_matched[0], &aligns[0]);
}
void SBSearch::search(SBSeqList *ref, SBQuery *que) {
    _resize(ref->size(), que->count()); _que = que;
    if (_par->multi_thread) {
        sforin(i, 0, _rnum) _threads.addTask(search_i, i, ref->at(i), que, _par, &_matched[i*_qnum], &aligns[i*_qnum]);
        _threads.complete();
    }
    else sforin(i, 0, _rnum) search_i(i, ref->at(i), que, _par, &_matched[i*_qnum], &aligns[i*_qnum]);
}
inline void _align(salign *al, sbsearch_param *par, SBQuery *que) {
    al->cigars.add(scigar(scigar::PMATCH, al->aligned.length()+1));
    if (par->ref_type&DNA_SEQ) al->score = par->aln_par.pm_score*(al->aligned.length()+1);
    else {
        /*
         */
    }
}
inline void align_q(int q, sint rn, sint qn, Array<Array<salign>> *aligns, sbsearch_param *par, SBQuery *que) {
    sforin(r, 0, rn) {
        auto &avec = aligns->at(r*qn+q);
        sforeach(avec) _align(&E_, par, que);
    }
}
void SBSearch::makeAlign() {
    if (_par->multi_thread) {
        sforin(q, 0, _qnum) _threads.addTask(align_q, q, _rnum, _qnum, &aligns, _par, _que);
        _threads.complete();
    }
    else { sforin(q, 0, _qnum) align_q(q, _rnum, _qnum, &aligns, _par, _que); }
}
void SBSearch::setParam(sbsearch_param *p) {
    _par = p;
    if (p->multi_thread) _threads.setSize(p->thread_count);
    _rnum = 0;
    _qnum = 0;
    _que = nullptr;
}
void SBSearch::reset() {
    _rnum = 0;
    _qnum = 0;
    _que = nullptr;
    sforeach(_matched) E_.clear();
    sforeach(aligns) E_.clear();
}
