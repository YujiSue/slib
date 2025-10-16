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
void slib::sbio::SeqSearch::resize(const size_t r, const size_t q) {
    if (_matched.row < r || _matched.col < q) {
        _matched.resize(r, q);
        sfor(_matched) $_.reserve(_par->max_match_count + 1);
        _locker.resize(r, q);
    }
    auto q_ = q / (_par->complement ? 2 : 1);
    if (aligns.row < r || aligns.col < q_) {
        _extender.resize(r, q_);
        sfor(_extender) $_.setParam(_par);
        aligns.resize(q_, r);
        sfor(aligns) $_.reserve(_par->max_match_count + 1);
    }
}

inline bool _checkUniqueAlign(slib::Array<sbio::AlignPair>& aligns, sbio::AlignPair& al) {
    sfor(aligns) {
        if ($_.ref.include(al.ref) && $_.query.include(al.query)) return false;
    }
    return true;
}
/*
inline int _checkBack(subyte* ref, subyte* que, int num) {
    int count = 0;
    srforin(i, 0, num) {
        if (ref[i] == *(que)) { ++count; --que; }
    }
    return count;
}
inline int _checkNext(subyte* ref, subyte* que, int num) {
    int count = 0;
    srforin(i, 0, num) {
        if (ref[i] == (*que)) { ++count; ++que; }
    }
    return count;
}
*/

void slib::sbio::SeqSearch::searchAt(int i, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie* trie) {
    if (ref->mask.empty()) searchAt(i, srange(0, ref->length() - 1), ref, trie);
    else {
        auto region = srange(0, (int)ref->length() - 1) ^ ref->mask;
        sforeach(range, region) searchAt(i, range, ref, trie);
    }
}
void slib::sbio::SeqSearch::searchAt(int i, const srange &rng, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie* trie) {
    //
    srange range = rng;
    //
    Match tmp;
    auto* matchrow = _matched[i];
    auto* lockers = _locker[i];
    bool mt = _threads != nullptr;
    //
    auto refseq = ref->data(range.begin);
    // Reset trie position
    auto now = trie->root();
    //
    while (range.begin < range.end) {
        // Pattern match
        auto next = (int)(*refseq) + 1;
        while (!(now->children[next])) now = now->children[0];
        now = now->children[next];
        // 
        if (now->matches.size()) {
            auto rpos = range.begin - _par->seed + 1;
            sforeach(m, now->matches) {
                auto& queseq = trie->queries[m.first];
                tmp = Match(rpos, m.second, _par->seed);
                if (0 < tmp.ref && 0 < tmp.que) {
                    auto refs = refseq - _par->seed;
                    auto ques = queseq.data(tmp.que) - 1;
                    if ((*refs) == (*ques)) continue;
                }
                if (tmp.que + tmp.len < queseq.size()) {
                    auto rend = tmp.ref + tmp.len - 1, qend = tmp.que + tmp.len;
                    auto refp = refseq;
                    auto ques = queseq.data(qend);
                    while (rend < rng.end && qend < queseq.size()) {
                        if (*(refp + 1) == *ques) { ++rend; ++qend; ++refp; ++ques; }
                        else break;
                    }
                    tmp.len = qend - tmp.que;
                }
                // Ignore if matched size is less than min. threshold
                if (tmp.len < _par->min_match) continue;
                // 
                // Spin lock
                if (mt) lockers[m.first].lock();
                // 
                auto& matches = matchrow[m.first];
                if (matches.size() == _par->max_match_count &&
                    matches[-1].len < tmp.len) matches.resize(_par->max_match_count - 1);
                if (matches.size() < _par->max_match_count) {
                    if (matches.empty()) matches.add(tmp);
                    else {
                        auto ins = bisearch<Match, Array<Match>>(matches, tmp,
                            [](const Match& m1, const Match& m2) { return m1.len < m2.len; });
                        if (ins == matches.end()) matches.add(tmp);
                        else matches.insert(ins - matches.begin(), tmp);
                    }
                }
                // Unlock
                if (mt) lockers[m.first].unlock();
            }
        }
        ++range.begin; ++refseq;
    }
}

void slib::sbio::SeqSearch::searchAt(int i, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie2* trie) {
    if (ref->mask.empty()) searchAt(i, srange(0, ref->length() - 1), ref, trie);
    else {
        auto region = srange(0, (int)ref->length() - 1) ^ ref->mask;
        sforeach(range, region) searchAt(i, range, ref, trie);
    }
}
void slib::sbio::SeqSearch::searchAt(int i, const srange& rng, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie2* trie) {
    // Depth
    int dp = _par->depth();
    auto decoder = dp == 2 ? sdna::ddec21 : sdna::ddec41;
    //
    srange range(rng.begin / dp, rng.end / dp + 1);
    //
    Match tmp;
    auto* matchrow = _matched[i];
    auto* lockers = _locker[i];
    bool mt = _threads != nullptr;
    //
    subyte refs[4];
    auto refseq = ref->data(range.begin);
    // Reset trie position
    auto now = trie->root();
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
                auto &queseq = trie->queries[m.first];
                tmp = Match(rpos, m.second, _par->seed2 * dp);
                if (tmp.ref < rng.begin) {
                    auto dif = rng.begin - tmp.ref;
                    tmp.ref += dif; tmp.que += dif; tmp.len -= dif;
                }
                else if (dp <= tmp.ref) {
                    auto ques = queseq.data(tmp.que);
                    decoder(*(refseq - _par->seed2), refs);

                    /* DEBUG
                    if (i == 1 && m.first/2 == 9) {
                        sforin(d, 0, dp) {
                            std::cout << DNA_BASE16[refs[d]];
                            if (d < dp - 1) std::cout << ":";
                        }
                        std::cout << std::endl;
                        auto qp = ques - dp;
                        auto qi = tmp.que - dp;
                        sforin(d, 0, dp) {
                            if (qi > 0) std::cout << DNA_BASE16[qp[0]];
                            else std::cout << "-";
                            ++qp; ++qi;
                            if (d < dp - 1) std::cout << ":";
                        }
                        std::cout << std::endl;
                    }
                    */
                    auto dif = 0;
                    auto len = sstat::getMin(dp, tmp.ref, tmp.que);
                    sforin(l, 0, len) {
                        if (refs[dp - l - 1] == (*(ques - l - 1))) ++dif;
                        else break;
                    }
                    //auto dif = _checkBack(refs, ques - 1, sstat::getMin(dp, tmp.ref, tmp.que));
                    if (dif == dp) continue;
                    else { tmp.ref -= dif; tmp.que -= dif; tmp.len += dif; }
                }
                if (rng.end < (tmp.ref + tmp.len - 1)) {
                    auto dif = tmp.ref + tmp.len - rng.end - 1;
                    tmp.len -= dif;
                }
                else if (tmp.que + tmp.len < queseq.size()) {
                    auto count = 0;
                    auto ques = queseq.data(tmp.que + tmp.len);
                    auto refp = refseq + 1;
                    auto rl = rng.end - (tmp.ref + tmp.len - 1);
                    auto ql = (int)queseq.size() - (tmp.que + tmp.len);
                    while (true) {
                        decoder(*refp, refs);

                        /* DEBUG
                        if (i == 1 && m.first/2 == 9) {
                            sforin(d, 0, dp) {
                                std::cout << DNA_BASE16[refs[d]];
                                if (d < dp - 1) std::cout << ":";
                            }
                            std::cout << std::endl;
                            auto qp = ques;
                            sforin(d, 0, dp) {
                                if (d < ql) std::cout << DNA_BASE16[qp[0]];
                                else std::cout << "-";
                                ++qp;
                                if (d < dp - 1) std::cout << ":";
                            }
                            std::cout << std::endl;
                        }
                        */
                        auto dif = 0;
                        auto len = sstat::getMin(dp, rl, ql);
                        sforin(l, 0, len) {
                            if (refs[l] == ques[l]) ++dif;
                            else break;
                        }
                        count += dif;
                        if (dif < dp) break;
                        else { ++refp; rl -= dp; ql -= dp; ques += dp; }
                    }
                    tmp.len += count;
                    /*
                    auto refp = refseq;
                    auto ques = queseq.data(qend);
                    while (true) {
                        auto num = sstat::getMin(4, rng.end - rend, (int)queseq.size() - qend);
                        if (!num) break;
                        auto ext = true;
                        decoder(*(refp + 1), refs);
                        sforin(p, 0, num) {
                            if (refs[p] == *ques) { ++rend; ++qend; ++ques; }
                            else { ext = false; break; }
                        }
                        if (ext) ++refp;
                        else break;
                    }
                    tmp.len = qend - tmp.que;
                    */
                }
                // Ignore if matched size is less than min. threshold
                if (tmp.len < _par->min_match) continue;
                // 
                // Spin lock
                if (mt) lockers[m.first].lock();
                //
                


                /* DEBUG
                if (i == 3) {

                    SPrint("  ", m.first, "::", tmp.ref, "/", tmp.que, "|", tmp.len);

                }
                */


                // 
                auto& matches = matchrow[m.first];
                if (matches.size() == _par->max_match_count &&
                    matches[-1].len < tmp.len) matches.resize(_par->max_match_count - 1);
                if (matches.size() < _par->max_match_count) {
                    if (matches.empty()) matches.add(tmp);
                    else {
                        auto ins = bisearch<Match, Array<Match>>(matches, tmp,
                            [](const Match& m1, const Match& m2) { return m1.len < m2.len; });
                        if (ins == matches.end()) matches.add(tmp);
                        else matches.insert(ins - matches.begin(), tmp);
                    }
                }
                // Unlock
                if (mt) lockers[m.first].unlock();
            }
        }
        ++range.begin; ++refseq;
    }
}
void slib::sbio::SeqSearch::makeAlign(int r, int q, Sequence* ref, ubytearray* que) {
    AlignPair al;
    auto* matches = &_matched[r][(_par->complement ? 2 : 1) * q];
    auto& als = aligns[q][r];
    auto& ext = _extender[r][q];
    // Fwd.
    sforeach(match, *matches) {
        al.clear();
        al.ref = sbio::RefPos(r, match.ref, match.ref + match.len - 1);
        al.query = srange(match.que, match.que + match.len - 1);
        al.cigars.add(Cigar(scigar::PMATCH, match.len));
        al.scoring(&_par->apar);
        ext.extend(ref, que, &al);


        /* DEBUG
        if (r == 1 && q == 9) {

            String rs = ref->raw(al.ref);
            String qs; qs.resize(al.query.length(true));
            sdna::decode(que->data(), al.query.begin, al.query.length(true), (subyte*)&qs[0]);

            std::cout << al.alref(rs) << std::endl;
            std::cout << al.match() << std::endl;
            std::cout << al.alque(qs) << std::endl;

        }
        */


        if (al.score < _par->min_score) continue;
        bool add = true;
        if (als.size()) {
            sforeach(align, als) {
                if (align.ref.include(al.ref)) {
                    add = false; break;
                }
            }
        }
        /*
        if (als.size() && als[-1].score < al.score) {
            auto ins = bisearch<AlignPair, Array<AlignPair>>(als, al,
                [](const AlignPair& p1, const AlignPair& p2) { return p1.score < p2.score; });
            als.insert(ins - als.begin(), al);
        }
        */
        if(add) als.add(al);
    }
    ++matches; ++que;
    if (_par->complement) {
        // Rev.
        sforeach(match, *matches) {
            al.clear();
            al.ref = sbio::RefPos(r, match.ref, match.ref + match.len - 1);
            al.query = srange(match.que, match.que + match.len - 1);
            al.cigars.add(Cigar(scigar::PMATCH, match.len));
            al.scoring(&_par->apar);
            
            /* DEBUG
            if (r == 1 && q == 9) {

                String rs = ref->raw(al.ref);
                String qs; qs.resize(al.query.length(true));
                sdna::decode(que->data(), al.query.begin, al.query.length(true), (subyte*)&qs[0]);

                std::cout << al.ref.begin << "-" << al.ref.end << std::endl;


                std::cout << al.alref(rs) << std::endl;
                std::cout << al.match() << std::endl;
                std::cout << al.alque(qs) << std::endl;

            }
            */

            ext.extend(ref, que, &al);

            /* DEBUG
            if (r == 1 && q == 9) {

                String rs = ref->raw(al.ref);
                String qs; qs.resize(al.query.length(true));
                sdna::decode(que->data(), al.query.begin, al.query.length(true), (subyte*)&qs[0]);

                std::cout << al.alref(rs) << std::endl;
                std::cout << al.match() << std::endl;
                std::cout << al.alque(qs) << std::endl;

            }
            */


            if (al.score < _par->min_score) continue;
            al.complement(que->size());
            bool add = true;
            if (als.size()) {
                sforeach(align, als) {
                    if (align.ref.include(al.ref)) {
                        add = false; break;
                    }
                }
            }
            if (add) als.add(al);
            /*
            if (als.size() && als[-1].score < al.score) {
                auto ins = bisearch<AlignPair, Array<AlignPair>>(als, al,
                    [](const AlignPair& p1, const AlignPair& p2) { return p1.score < p2.score; });
                als.insert(ins - als.begin(), al);
            }
            else als.add(al);
            */
        }
        ++matches; ++que;
    }
    als.sort([](const AlignPair& p1, const AlignPair& p2) { return p2.score < p1.score; });
}
inline void _runSearchAt1(SeqSearch* ss, int i, const srange& rng, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie* trie) { ss->searchAt(i, rng, ref, trie); }
inline void _runSearchAt2(SeqSearch* ss, int i, const srange &rng, slib::sbio::Sequence* ref, slib::sbio::DNASeqTrie2* trie) { ss->searchAt(i, rng, ref, trie); }
inline void _runMakeAlign(SeqSearch* ss, int r, int q, slib::sbio::Sequence* ref, ubytearray* que) { ss->makeAlign(r, q, ref, que); }
void slib::sbio::SeqSearch::search(Sequence& ref, DNASeqTrie& trie) {
    resize(1, trie.qcount());
    if (ref.mask.empty()) {
        if (_threads) _threads->addTask(_runSearchAt1, this, 0, srange(0, ref.length() - 1), &ref, &trie);
        else searchAt(0, srange(0, ref.length() - 1), &ref, &trie);
    }
    else {
        auto unmask = srange(0, ref.length() - 1) ^ ref.mask;
        sforeach(loc, unmask) {
            if (_threads) _threads->addTask(_runSearchAt1, this, 0, loc, &ref, &trie);
            else searchAt(0, loc, &ref, &trie);
        }
    }
    auto* que = trie.queries.data();
    sforin(q, 0, aligns.row) {
        if (_threads) _threads->addTask(_runMakeAlign, this, 0, q, &ref, que);
        else makeAlign(0, q, &ref, que);
        que += (_par->complement ? 2 : 1);
    }
}
void slib::sbio::SeqSearch::search(SeqList& ref, DNASeqTrie& trie) {
    resize(ref.size(), trie.qcount());
    sfori(ref) {
        if (ref[i].mask.empty()) {
            if (_threads) _threads->addTask(_runSearchAt1, this, i, srange(0, ref[i].length() - 1), &ref[i], &trie);
            else searchAt(i, srange(0, ref[i].length() - 1), &ref[i], &trie);
        }
        else {
            auto unmask = srange(0, ref[i].length() - 1) ^ ref[i].mask;
            sforeach(loc, unmask) {
                if (_threads) _threads->addTask(_runSearchAt1, this, i, loc, &ref[i], &trie);
                else searchAt(i, loc, &ref[i], &trie);
            }
        }
    }
    if (_threads) _threads->complete();
    sforin(r, 0, aligns.col) {
        auto* que = trie.queries.data();
        sforin(q, 0, aligns.row) {
            if (_threads) _threads->addTask(_runMakeAlign, this, r, q, &ref[r], que);
            else makeAlign(r, q, &ref[r], que);
            que += (_par->complement ? 2 : 1);
        }
    }
    if (_threads) _threads->complete();
}
void slib::sbio::SeqSearch::search(Sequence& ref, DNASeqTrie2& trie) {
    resize(1, trie.qcount());
    if (ref.mask.empty()) {
        if (_threads) _threads->addTask(_runSearchAt2, this, 0, srange(0, ref.length() - 1), &ref, &trie);
        else searchAt(0, srange(0, ref.length() - 1), &ref, &trie);
    }
    else {
        auto unmask = srange(0, ref.length() - 1) ^ ref.mask;
        sforeach(loc, unmask) {
            if (_threads) _threads->addTask(_runSearchAt2, this, 0, loc, &ref, &trie);
            else searchAt(0, loc, &ref, &trie);
        }
    }
    auto* que = trie.queries.data();
    sforin(q, 0, aligns.row) {
        if (_threads) _threads->addTask(_runMakeAlign, this, 0, q, &ref, que);
        else makeAlign(0, q, &ref, que);
        que += (_par->complement ? 2 : 1);
    }
}
void slib::sbio::SeqSearch::search(SeqList& ref, DNASeqTrie2& trie) {
    resize(ref.size(), trie.qcount());
    sfori(ref) {
        if (ref[i].mask.empty()) {
            if (_threads) _threads->addTask(_runSearchAt2, this, i, srange(0, ref[i].length() - 1), &ref[i], &trie);
            else searchAt(i, srange(0, ref[i].length() - 1), &ref[i], &trie);
        }
        else {
            auto unmask = srange(0, ref[i].length() - 1) ^ ref[i].mask;
            sforeach(loc, unmask) {
                if (_threads) _threads->addTask(_runSearchAt2, this, i, loc, &ref[i], &trie);
                else searchAt(i, loc, &ref[i], &trie);
            }
        }
    }
    if (_threads) _threads->complete();
    sforin(r, 0, aligns.col) {
        auto* que = trie.queries.data();
        sforin(q, 0, aligns.row) {
            if (_threads) _threads->addTask(_runMakeAlign, this, r, q, &ref[r], que);
            else makeAlign(r, q, &ref[r], que);
            que += (_par->complement ? 2 : 1);
        }
    }
    if (_threads) _threads->complete();
}
void slib::sbio::SeqSearch::setThreads(SWork* w) { _threads = w; }
void slib::sbio::SeqSearch::setParam(SeqSearchParam* p) { 
    _par = p; 
    if (1 < _par->nthread && !_threads) _threads = &_par->threads;
}
void slib::sbio::SeqSearch::reset() {
    sfor(_extender) $_.reset();
    sfor(_matched) $_.clear();
    sfor(aligns) $_.clear();
}