#include "sbioinfo/seqsearch.h"

slib::sbio::DNASeqTrie::DNASeqTrie() : SeqTrie<17>() {}
slib::sbio::DNASeqTrie::DNASeqTrie(SeqSearchParam* p) : DNASeqTrie() { setParam(p); }
slib::sbio::DNASeqTrie::~DNASeqTrie() {}
void slib::sbio::DNASeqTrie::addQuery(const char* seq) {}
void slib::sbio::DNASeqTrie::addQuery(const ubytearray& seq) {
    if (!_par) throw NullException(nullErrorText("parameter"));
    if (seq.size() < _par->seed) throw Exception();
    auto qs = (int)queries.size();
    queries.next() = seq;
    addQuery1(qs, queries[-1]);
    if (_par->complement) {
        queries.next().resize(seq.size());
        sna::complement1(queries[-2].data(), 0, queries[-1].size(), queries[-1].data());
        addQuery1(qs + 1, queries[-1]);
    }
}
void slib::sbio::DNASeqTrie::makeTrie() {
    sfor(queries) {
        addQuery1((int)$INDEX(queries), $_);
    }
}
void slib::sbio::DNASeqTrie::addQuery1(int q, const ubytearray& seq) {
    auto base = seq.data(), tmp = base;
    bool available = true;
    auto idx = 0;
    auto len = seq.size() - _par->seed + 1;
    sforin(l, 0, len) {
        available = true;
        sforin(s, 0, _par->seed) {
            if (base[s] == 15) { available = false; break; }
        }
        if (!available) { ++base; continue; }
        auto now = root();
        tmp = base;
        sforin(s, 0, _par->seed) {
            idx = (int)(*tmp) + 1;
            if (!(now->children[idx])) {
                auto nxt = Trie<Pair<int, int>, 17>::newPMA();
                now->children[idx] = nxt;
            }
            now = now->children[idx]; ++tmp;
        }
        now->add(Pair<int, int>(q, l)); ++base;
    }
}

slib::sbio::DNASeqTrie2::DNASeqTrie2() : SeqTrie<257>() {}
slib::sbio::DNASeqTrie2::DNASeqTrie2(SeqSearchParam* p) : DNASeqTrie2() { setParam(p); }
slib::sbio::DNASeqTrie2::~DNASeqTrie2() {}
void slib::sbio::DNASeqTrie2::addQuery(const char* seq) {
    if (!_par) throw NullException(nullErrorText("parameter"));
    auto sz = strlen(seq);
    auto qs = (int)queries.size();
    queries.next().resize(sz);
    sdna::encode((const subyte*)seq, 0, sz, queries[-1].data());
    auto dp = _par->depth();
    if (dp == 2) {
        addQuery2(qs, queries[-1]);
        if (_par->complement) {
            queries.next().resize(sz);
            sna::complement16(queries[-2].data(), 0, queries[-1].size(), queries[-1].data());
            addQuery2(qs + 1, queries[-1]);
        }
    }
    else {
        addQuery4(qs, queries[-1]);
        if (_par->complement) {
            queries.next().resize(sz);
            sna::complement16(queries[-2].data(), 0, queries[-1].size(), queries[-1].data());
            addQuery4(qs + 1, queries[-1]);
        }
    }
}
void slib::sbio::DNASeqTrie2::addQuery(const ubytearray& seq) {
    if (!_par) throw NullException(nullErrorText("parameter"));
    auto qs = (int)queries.size();
    queries.next() = seq;
    auto dp = _par->depth();
    if (dp == 2) {
        addQuery2(qs, queries[-1]);
        if (_par->complement) {
            queries.next().resize(seq.size());
            sna::complement4(queries[-2].data(), 0, queries[-1].size(), queries[-1].data());
            addQuery2(qs + 1, queries[-1]);
        }
    }
    else {
        addQuery4(qs, queries[-1]);
        if (_par->complement) {
            queries.next().resize(seq.size());
            sna::complement16(queries[-2].data(), 0, queries[-1].size(), queries[-1].data());
            addQuery4(qs + 1, queries[-1]);
        }
    }
}
void slib::sbio::DNASeqTrie2::makeTrie() {
    auto dp = _par->depth();
    if (dp == 2) { sfor(queries) { addQuery2((int)$INDEX(queries), $_); } }
    else { sfor(queries) { addQuery4((int)$INDEX(queries), $_); } }
}
slib::subyte encD2(const slib::subyte* b, int s) {
    return (b[2 * s] << 4) | b[2 * s + 1];
}
void slib::sbio::DNASeqTrie2::addQuery2(int q, const ubytearray& seq) {
    if (seq.size() < _par->seed) return;
    bool available = true;
    auto base = seq.data();
    auto idx = 0;
    auto len = seq.size() - _par->seed + 1;
    sforin(l, 0, len) {
        available = true;
        sforin(s, 0, _par->seed2 * 2) {
            if (base[s] == 15) { available = false; break; }
        }
        if (!available) { ++base; continue; }
        auto now = root();
        sforin(s, 0, _par->seed2) {
            idx = (int)encD2(base, s) + 1;
            if (!(now->children[idx])) {
                auto nxt = Trie<Pair<int, int>, 257>::newPMA();
                now->children[idx] = nxt;
            }
            now = now->children[idx];
        }
        now->add(Pair<int, int>(q, l)); ++base;
    }
}
slib::subyte encD4(const slib::subyte* b, int s) {
    return ((slib::sbio::sdna::b24(b[4 * s]) << 6) | (slib::sbio::sdna::b24(b[4 * s + 1]) << 4) |
        (slib::sbio::sdna::b24(b[4 * s + 2]) << 2) | slib::sbio::sdna::b24(b[4 * s + 3]));
}
void slib::sbio::DNASeqTrie2::addQuery4(int q, const ubytearray& seq) {
    if (seq.size() < _par->seed) return;
    bool available = true;
    auto base = seq.data();
    auto idx = 0;
    auto len = seq.size() - _par->seed + 1;
    sforin(l, 0, len) {
        available = true;
        sforin(s, 0, _par->seed2 * 4) {
            if (base[s] == 15) { available = false; break; }
        }
        if (!available) { ++base; continue; }
        auto now = root();
        sforin(s, 0, _par->seed2) {
            idx = (int)encD4(base, s) + 1;
            if (!(now->children[idx])) {
                auto nxt = Trie<Pair<int, int>, 257>::newPMA();
                now->children[idx] = nxt;
            }
            now = now->children[idx];
        }
        now->add(Pair<int, int>(q, l)); ++base;
    }
}

