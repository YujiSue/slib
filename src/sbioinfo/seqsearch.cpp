#include "sbioinfo/seqsearch.h"
using namespace slib;
using namespace slib::sbio;
slib::sbio::SeqSearchParam::SeqSearchParam() {
    reftype = 0;
    seed = reftype & sseq::AA ? 3 : 12;
    seed2 = seed / depth();
    min_match = seed;
    max_gap = 0;
    max_miss = 0;
    min_score = 0.f;
    ext_threshold = 1.0f;
    complement = reftype & sseq::DNA ? true : false;
    strict = true;
    max_match_count = 100;
    nthread = 1;
}
slib::sbio::SeqSearchParam::SeqSearchParam(const sushort t) : SeqSearchParam() { setRefType(t); }
slib::sbio::SeqSearchParam::~SeqSearchParam() {}
slib::sbio::SeqSearchParam &SeqSearchParam::operator=(const SeqSearchParam &par) {
    reftype = par.reftype;
    seed = par.seed;
    seed2 = par.seed2;
    min_match = par.min_match;
    max_gap = par.max_gap;
    max_miss = par.max_miss;
    min_score = par.min_score;
    ext_threshold = par.ext_threshold;
    complement = par.complement;
    strict = par.strict;
    max_match_count = par.max_match_count;
    nthread = par.nthread;
    if (1 < nthread) threads.setSize(nthread);
    apar = par.apar;
    return *this;
}
int slib::sbio::SeqSearchParam::depth() const {
    auto dp = (reftype >> 8) & 0x0F;
    return dp == 0 ? 1 : dp;
}
void slib::sbio::SeqSearchParam::setRefType(const slib::sushort type) {
	reftype = type;
    seed = reftype & sseq::AA ? 3 : 12;
    seed2 = seed / depth();
    complement = reftype & sseq::DNA ? true : false;
    apar.setSeqType((subyte)(reftype & 0xFF));
}
void slib::sbio::SeqSearchParam::setSeed(int s) {
    seed = s;
    seed2 = seed / depth();
}
void slib::sbio::SeqSearchParam::setAsync(int n) {
    nthread = n;
    if (1 < n) threads.setSize(nthread);
}
void slib::sbio::SeqSearchParam::set(const sobj &obj) {
    if (obj["ref"]) reftype = obj["ref"];
    if (obj["seed"]) seed = obj["seed"];
    if (obj["seed2"]) seed2 = obj["seed2"];
    if (obj["match"]) min_match = obj["match"];
    if (obj["gap"]) max_gap = obj["gap"];
    if (obj["miss"]) max_miss = obj["miss"];
    if (obj["score"]) min_score = obj["score"];
    if (obj["threshold"]) ext_threshold = obj["threshold"];
    if (obj["complement"]) complement = true;
    if (obj["strict"]) strict = true;
    if (obj["maxcount"])max_match_count = obj["maxcount"];
    if (obj["thread"]) nthread = obj["thread"];
    if (1 < nthread) threads.setSize(nthread);
    if (obj["align"]) apar.set(obj["align"]);
}
sobj slib::sbio::SeqSearchParam::toObj() {
    return {
        D_("ref", reftype),
        D_("seed", seed),
        D_("seed2", seed2),
        D_("match", min_match),
        D_("gap", max_gap),
        D_("miss", max_miss),
        D_("score", min_score),
        D_("threshold", ext_threshold),
        D_("complement", complement),
        D_("strict", strict),        
        D_("thread", nthread),
        D_("maxcount", max_match_count),
        D_("align", apar.toObj())
    };
}
slib::sbio::SeqSearch::SeqSearch() : _par(nullptr), _threads(nullptr) {}
slib::sbio::SeqSearch::SeqSearch(SeqSearchParam* p) : SeqSearch() { setParam(p); }
slib::sbio::SeqSearch::~SeqSearch() {}
void slib::sbio::SeqSearch::resize(size_t r, size_t q) {
    if (_extenders.size() < r) {
        _extenders.resize(r);
        sfor(_extenders) $_.setParam(_par);
    }
    if (_locks.size() < q) _locks.resize(q);
    if (aligns.size() < r * q) {
        aligns.resize(r, q / 2);
        sfor(aligns) $_.reserve(_par->max_match_count + 1);
    }
}
void slib::sbio::SeqSearch::reserve(const size_t msz, const size_t asz) {
    sfor(aligns) $_.reserve(asz);
}

inline bool _checkUniqueAlign(slib::RArray<sbio::AlignPair> &aligns, int rpos, int qpos) {
    sfor(aligns) {
        if ($_.ref.include(rpos) && $_.query.include(qpos)) return false;
    }
    return true;
}
inline void _setRegion(slib::sbio::Sequence* ref, sregion& region) {
    if (ref->isMasked() && !ref->mask.empty()) region = srange(0, (int)ref->length() - 1) ^ ref->mask;
    else region.add(srange(0, (int)ref->length() - 1));
}
void slib::sbio::SeqSearch::searchAt(int i, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie* trie) {
    // Set search region 
    sregion region;
    AlignPair al;
    auto alist = aligns[i];
    _setRegion(ref, region);
    //
    auto refseq = ref->data();
    auto now = trie->root();
    bool extend = (_par->max_gap || _par->max_miss);
    // 
    sfor(region) {
        // Set range
        refseq = ref->data($_.begin);
        // Reset trie position
        now = trie->root();
        //
        while ($_.begin < $_.end) {
            // Pattern match
            auto next = (int)(*refseq) + 1;
            while (!(now->children[next])) now = now->children[0];
            now = now->children[next];
            // 
            if (now->matches.size()) {
                auto rpos = $_.begin - _par->seed + 1;
                sforeach(m, now->matches) {
                    auto& array = alist[m.first / 2];
                    // 
                    if (_checkUniqueAlign(array, rpos, m.second)) {
                        // Clear buffer
                        al.clear();
                        // Aligned site
                        auto l = _par->seed - 1;
                        if ($_.end < rpos + l) l = rpos - $_.end;
                        al.ref = sbio::RefPos(i, rpos, rpos + l);
                        al.query = srange(m.second, m.second + l);
                        al.cigars.add(Cigar(scigar::PMATCH, l + 1));
                        _extenders[i].extendExact(ref, &trie->queries[m.first], &al);
                        // Ignore if matched size is less than min. threshold
                        if (al.cigars.refSize() < _par->min_match) continue;
                        // Local extension
                        if (extend) _extenders[i].extend(ref, &trie->queries[m.first], &al);
                        // Check capacity
                        if (array.size() < _par->max_match_count) {
                            if (_par->complement && (m.first % 2)) al.complement(trie->queries[m.first].size());
                            if (array.empty() || al.score < array[-1].score) array.add(al);
                            else {
                                auto ins = bisearch<AlignPair, RArray<AlignPair>>(array, al,
                                    [](const AlignPair& p1, const AlignPair& p2) { return p1.score < p2.score; });
                                if (ins == array.end()) array.add(al);
                                else array.insert(ins - array.begin(), al);
                            }
                        }
                        // Check score
                        else if (array[-1].score < al.score) {
                            if (_par->complement && (m.first % 2)) al.complement(trie->queries[m.first].size());
                            array.resize(array.size() - 1);
                            auto ins = bisearch<AlignPair, RArray<AlignPair>>(array, al,
                                [](const AlignPair& p1, const AlignPair& p2) { return p1.score < p2.score; });
                            if (ins == array.end()) array.add(al);
                            else array.insert(ins - array.begin(), al);
                        }
                    }
                }
            }
            ++$_.begin; ++refseq;
        }
    }
}

void slib::sbio::SeqSearch::searchAt(int i, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie2* trie) {
    // Set search region 
    sregion region;
    srange range;
    AlignPair al;
    auto alist = aligns[i];
    _setRegion(ref, region);
    //
    auto refseq = ref->data();
    auto now = trie->root();
    int dp = _par->depth();
    bool extend = (_par->max_gap || _par->max_miss);
    // 
    sfor(region) {
        // Set range
        range.begin = $_.begin / dp;
        range.end = $_.end / dp + 1;
        refseq = ref->data(range.begin);
        // Reset trie position
        now = trie->root();
        //
        while (range.begin < range.end) {
            // Pattern match
            auto next = (int)(*refseq) + 1;
            while (!(now->children[next])) now = now->children[0];
            now = now->children[next];
            // 
            if (now->matches.size()) {
                auto rpos = (range.begin - _par->seed2 + 1) * dp;
                sforeach(m, now->matches) {
                    auto& array = alist[m.first / 2];
                    // 
                    if (_checkUniqueAlign(array, rpos, m.second)) {
                        // Clear buffer
                        al.clear();
                        // Aligned site
                        auto l = _par->seed - 1;
                        if ($_.end < rpos + l) l = rpos - $_.end;
                        al.ref = sbio::RefPos(i, rpos, rpos + l);
                        al.query = srange(m.second, m.second + l);
                        al.cigars.add(Cigar(scigar::PMATCH, l + 1));
                        _extenders[i].extendExact(ref, &trie->queries[m.first], &al);
                        // Ignore if matched size is less than min. threshold
                        if (al.cigars.refSize() < _par->min_match) continue;
                        // Local extension
                        if (extend) _extenders[i].extend(ref, &trie->queries[m.first], &al);
                        // Check capacity
                        if (array.size() < _par->max_match_count) {
                            if (_par->complement && (m.first % 2)) al.complement(trie->queries[m.first].size());
                            if (array.empty() || al.score < array[-1].score) array.add(al); 
                            else {
                                auto ins = bisearch<AlignPair, RArray<AlignPair>>(array, al,
                                    [](const AlignPair& p1, const AlignPair& p2) { return p1.score < p2.score; });
                                if (ins == array.end()) array.add(al);
                                else array.insert(ins - array.begin(), al);
                            }
                        }
                        // Check score
                        else if (array[-1].score < al.score) {
                            if (_par->complement && (m.first % 2)) al.complement(trie->queries[m.first].size());
                            array.resize(array.size() - 1);
                            auto ins = bisearch<AlignPair, RArray<AlignPair>>(array, al,
                                [](const AlignPair& p1, const AlignPair& p2) { return p1.score < p2.score; });
                            if (ins == array.end()) array.add(al);
                            else array.insert(ins - array.begin(), al);
                        }
                    }
                }
            }
            ++range.begin; ++refseq;
        }
    }
}
inline void _runSearchAt1(SeqSearch* ss, int i, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie* trie) { ss->searchAt(i, ref, trie); }
inline void _runSearchAt2(SeqSearch* ss, int i, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie2* trie) { ss->searchAt(i, ref, trie); }
void slib::sbio::SeqSearch::search(Sequence& ref, DNASeqTrie& trie) {
    resize(1, trie.qcount());
    searchAt(0, &ref, &trie);
}
void slib::sbio::SeqSearch::search(Sequence& ref, DNASeqTrie2& trie) {
    resize(1, trie.qcount());
    searchAt(0, &ref, &trie);
}
void slib::sbio::SeqSearch::search(SeqList& ref, DNASeqTrie& trie) {
    resize(ref.size(), trie.qcount());
    sfori(ref) {
        if (_threads) _threads->addTask(_runSearchAt1, this, i, &ref[i], &trie);
        else searchAt(i, &ref[i], &trie);
    }
    if (_threads) _threads->complete();
}
void slib::sbio::SeqSearch::search(SeqList& ref, DNASeqTrie2& trie) {
    resize(ref.size(), trie.qcount());
    sfori(ref) {
        if (_threads) _threads->addTask(_runSearchAt2, this, i, &ref[i], &trie);
        else searchAt(i, &ref[i], &trie);
    }
    if (_threads) _threads->complete();
}
void slib::sbio::SeqSearch::setThreads(SWork* w) { _threads = w; }
void slib::sbio::SeqSearch::setParam(SeqSearchParam* p) { 
    _par = p; 
    if (1 < _par->nthread) _threads = &_par->threads; 
}
void slib::sbio::SeqSearch::reset() {
    sfor(_extenders) $_.reset();
    sfor(aligns) $_.clear();
}