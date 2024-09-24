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
        aligns.resize(q, r);
        sfor(aligns) $_.reserve(_par->max_match_count + 1);
    }
}
void slib::sbio::SeqSearch::reserve(const size_t msz, const size_t asz) {
    sfor(_matches) $_.reserve(msz);
    sfor(aligns) $_.reserve(asz);
}

/*
void _strictCompare(slib::sbio::Sequence* ref, slib::ubytearray* que,
    slib::smath::Vector3D<int>* vec, slib::sbio::CigarArray* cigars,
    slib::sbio::SeqSearchParam* par) {
    subyte buffer[4];
    auto dp = par->depth();
    auto rseq = ref->data(vec->elements[0] / dp);
    if (dp == 2) sdna::ddec21(*rseq, buffer);
    else sdna::ddec41(*rseq, buffer);
    auto count = vec->elements[0] % dp;
    auto qseq = que->data(vec->elements[1]);
    sforin(l, 0, vec->elements[2]) {
        cigars->add(par->apar.compares[(int)buffer[count]][(int)*qseq]);
        if (count == dp - 1) {
            ++rseq;
            if (dp == 2) sdna::ddec21(*rseq, buffer);
            else sdna::ddec41(*rseq, buffer);
            count = 0;
        }
        else ++count;
        ++qseq;
    }
}
*/
/*
void _addAlign(int r, slib::sbio::Sequence* ref, slib::ubytearray* que, svec3i* vec, slib::sbio::AlignPair* al, bool strict) {
    al->ref.end = (*vec)[0] + (*vec)[2] - 1;
    al->query.end = (*vec)[1] + (*vec)[2] - 1;
    //if (strict) _strictCompare(ref, que, vec, &al->cigars, par);
    //else 
    al->cigars.add(Cigar(scigar::PMATCH, (*vec)[2]));
}
void _assemble(int r, int q, slib::sbio::Sequence* ref, ubytearray* que, 
    Array<smath::Vector3D<int>>* matches, RecycleArray<AlignPair>* aligns, 
    SeqSearchParam *par) {
    ubytearray buffer;
    bool extend = (par->max_gap || par->max_miss);
    //
    sforin(match, matches->begin(), matches->end()) {
        if (match->elements[0] < 0) continue;
        auto al = &aligns->next();

        _toAlign(r, &ref[r], que, match.ptr(), al, par);

        if (extend) {
            extender->extend(ref, que, al);
            auto nxt = match + 1;
            while (nxt < matches->end()) {
                if (al.query.end + _par->max_gap < nxt->elements[1]) break;
                rl = nxt->elements[0] - al.ref.end;
                ql = nxt->elements[1] - al.query.end;
                if (ql <= 0) {
                    rl -= ql;
                    if (0 < rl && rl <= _par->max_gap) {
                        al.cigars.add(Cigar(scigar::DELETION, rl));
                        //if (strict) {}
                        al.cigars.add(Cigar(scigar::PMATCH, nxt->elements[2] - rl));
                        al.ref.end = nxt->elements[0] + nxt->elements[2] - 1;
                        al.query.end = nxt->elements[1] + nxt->elements[2] - 1;
                        nxt->elements[0] = -1;
                    }
                }
                else if (rl <= 0) {
                    ql -= rl;
                    if (0 < ql && ql <= _par->max_gap) {
                        al.cigars.add(Cigar(scigar::INSERTION, rl));
                        //if (strict) {}
                        al.cigars.add(Cigar(scigar::PMATCH, nxt->elements[2] - ql));
                        al.ref.end = nxt->elements[0] + nxt->elements[2] - 1;
                        al.query.end = nxt->elements[1] + nxt->elements[2] - 1;
                        nxt->elements[0] = -1;
                    }
                }
                else {
                    _extender[0].extendTailTo(ref, nxt->elements[0], &$_1, nxt->elements[1], &al, nxt->elements[2]);
                    if (al.ref.end == nxt->elements[0] - 1 && al.query.end == nxt->elements[1] - 1) {
                        _addAlign(r, ref, &$_1, match.ptr(), &al, _par);
                        nxt->elements[0] = -1;
                    }
                }
                ++nxt;
            }
            _extender[0].extendTail(ref, &$_1, &al);
        }
        if (par->complement && (q % 2)) al->complement(que->size());
    }
}
*/

inline void _toAligns(int r, int q, srange *range, slib::sbio::Sequence* ref, ubytearray* query,
    slib::Array<Pair<int, int>>* matches, RecycleArray<AlignPair>* aligns,
    AlignExtend *extender, SLock *locker, slib::sbio::SeqSearchParam* par) {
    if (matches->empty()) return;
    // 
    int dp = par->depth();
    std::function<void(subyte, subyte *)> decorder = dp == 2 ? sdna::ddec21 : sdna::ddec41;
    subyte* qseq = nullptr;
    subyte buffer[4];
    srange rrange, qrange;
    bool extend = (par->max_gap || par->max_miss);
    // 
    sfor(*matches) {
        if ($_.first < 0) continue;
        rrange = srange($_.first, $_.first + par->seed);
        qrange = srange($_.second, $_.second + par->seed);
        //
        auto it_ = $ + 1;
        // Join neighboring match
        while (it_ < matches->end()) {
            // 
            if (rrange.end + dp < it_->first + par->seed) break;
            // 
            if (rrange.end + dp == it_->first + par->seed
                && qrange.end + dp == it_->second + par->seed) {
                rrange.end += dp; qrange.end += dp; it_->first = -1;
            }
            ++it_;
        }
        //
        if (1 < dp) {
            // Extend head
            if (0 < rrange.begin && 0 < qrange.begin) {
                decorder(ref->at(rrange.begin / dp - 1), buffer);
                qseq = query->data(qrange.begin - 1);
                srforin(i, 0, dp) {
                    if (qseq < query->data()) break;
                    if (buffer[i] == *qseq) {
                        --rrange.begin; --qrange.begin; --qseq;
                    }
                    else break;
                }
            }
            // Cut masked region
            if (rrange.begin < range->begin) {
                auto dif = range->begin - rrange.begin;
                rrange.begin += dif; qrange.begin += dif;
            }
            // Extend tail
            if (rrange.end < ref->length() && qrange.end < query->size()) {
                decorder(ref->at((rrange.end) / dp), buffer);
                qseq = query->data(qrange.end);
                sforin(i, 0, dp) {
                    if (query->data(-1) < qseq) break;
                    if (buffer[i] == *qseq) { ++rrange.end; ++qrange.end; ++qseq; }
                    else break;
                }
            }
            // Cut masked region
            if (range->end < rrange.end) {
                auto dif = rrange.end - range->end;
                rrange.end -= dif; qrange.end -= dif;
            }
        }
        // Filter
        if (par->min_match <= rrange.length()) {
            // Spin lock for async search
            // if (locker) locker->lock();
            salign* al = &aligns->next();
            al->ref = sbpos(r, rrange.begin, rrange.end - 1);
            al->query = srange(qrange.begin, qrange.end - 1);
            al->cigars.add(Cigar(scigar::PMATCH, rrange.length()));
            if (extend) {
                extender->extend(ref, query, al);
                auto it_ = $ + 1;
                while (it_ < matches->end()) {
                    if (-1 < it_->first &&
                        al->ref.include(srange(it_->first, it_->first + par->seed - 1)) &&
                        al->query.include(srange(it_->second, it_->second + par->seed - 1))) {
                        it_->first = -1;
                    }
                    ++it_;
                }
            }
            al->scoring(&par->apar);
            if (par->complement && (q % 2)) al->complement(query->size());
            //if (locker) locker->unlock();
        }
    }
}
inline bool _checkUniqueAlign(slib::RecycleArray<sbio::AlignPair>& aligns, int rpos, int qpos) {
    sfor(aligns) {
        if ($_.ref.include(rpos) && $_.query.include(qpos)) return false;
    }
    return true;
}
inline void _setRegion(slib::sbio::Sequence* ref, sregion& region) {
    if (ref->isMasked() && !ref->mask.empty()) region = srange(0, (int)ref->length() - 1) ^ ref->mask;
    else region.add(srange(0, (int)ref->length() - 1));
}
void slib::sbio::SeqSearch::searchAt1(int i, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie* trie) {
    // Set search region 
    sregion region;
    _setRegion(ref, region);
    //
    auto refseq = ref->data();
    auto matches = _matches[i];
    auto now = trie->root();
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
            if (!(now->matches.empty())) {
                sforeach(m, now->matches) {
                    matches[m.first].add(Pair<int,int>($_.begin - _par->seed + 1, m.second));
                }
            }
            ++$_.begin; ++refseq;
        }
        sforin(q, 0, trie->qcount()) {
            _toAligns(i, q, &$_, ref, &trie->queries[q], &matches[q], &aligns[q][i], &_extenders[i], (_threads ? &_locks[q] : nullptr), _par);
            matches[q].clear();
        }
        
    }
}
void slib::sbio::SeqSearch::searchAt2(int i, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie2* trie) {
    // Set search region 
    sregion region;
    srange range;
    _setRegion(ref, region);
    //
    auto refseq = ref->data();
    auto matches = _matches[i];
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
                    if (_checkUniqueAlign(aligns[m.first][i], rpos, m.second)) {
                        auto& al = aligns[m.first][i].next();
                        auto l = _par->seed - 1;
                        if ($_.end < rpos + l) l = rpos - $_.end;
                        al.ref = sbio::RefPos(i, rpos, rpos + l);
                        al.query = srange(m.second, m.second + l);
                        al.cigars.add(Cigar(scigar::PMATCH, l + 1));
                        _extenders[i].extendExact(ref, &trie->queries[m.first], &al);
                        if (al.cigars.refSize() < _par->min_match) {
                            aligns[m.first][i].resize(aligns[m.first][i].size() - 1);
                            continue;
                        }
                        if (extend) _extenders[i].extend(ref, &trie->queries[m.first], &al);
                        aligns[m.first][i].sort([](const AlignPair& a1, const AlignPair& a2) { return a2.score < a1.score; });
                        if (_par->max_match_count <= aligns[m.first][i].size()) {
                            aligns[m.first][i].resize(_par->max_match_count);
                        }
                    }
                }
            }
            ++range.begin; ++refseq;
        }
    }
    auto als = &aligns[0][i];
    auto ques = trie->queries.data();
    sforin(q, 0, trie->qcount()) {
        sforeach(al, *als) {
            if (_par->complement && (q % 2)) al.complement(ques->size());
        }
        als += aligns.col; ++ques;
    }
}
inline void _runSearchAt1(SeqSearch* ss, int i, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie* trie) { ss->searchAt1(i, ref, trie); }
inline void _runSearchAt2(SeqSearch* ss, int i, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie2* trie) { ss->searchAt2(i, ref, trie); }
void slib::sbio::SeqSearch::search(Sequence& ref, DNASeqTrie& trie) {
    resize(1, trie.qcount());
    searchAt1(0, &ref, &trie);
}
void slib::sbio::SeqSearch::search(Sequence& ref, DNASeqTrie2& trie) {
    resize(1, trie.qcount());
    searchAt2(0, &ref, &trie);
}
void slib::sbio::SeqSearch::search(SeqList& ref, DNASeqTrie& trie) {
    resize(ref.size(), trie.qcount());
    sfori(ref) {
        if (_threads) _threads->addTask(_runSearchAt1, this, i, &ref[i], &trie);
        else searchAt1(i, &ref[i], &trie);
    }
    if (_threads) _threads->complete();
}
void slib::sbio::SeqSearch::search(SeqList& ref, DNASeqTrie2& trie) {
    resize(ref.size(), trie.qcount());
    sfori(ref) {
        if (_threads) _threads->addTask(_runSearchAt2, this, i, &ref[i], &trie);
        else searchAt2(i, &ref[i], &trie);
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
    sfor(_matches) $_.clear();
    sfor(aligns) $_.clear();
}