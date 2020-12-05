#include "sbioinfo/sbsearch.h"

using namespace slib;
using namespace slib::sbio;

spma::spma() {}
spma::spma(size_t n) {
    child.resize(n+1); memset(child.ptr(), 0, sizeof(spma *)*child.size());
}
spma::spma(const spma &p) {
    resize(p.child.size());
    child = p.child; match = p.match;
}
spma::~spma() {}
spma &spma::operator=(const spma &p) {
    child = p.child; match = p.match; return *this;
}
void spma::add(int i1, int i2) { match.add(i1, i2); }
void spma::resize(size_t n) { child.resize(n + 1, 0); }
void spma::init() {
    memset(child.ptr(), 0, sizeof(spma *)*child.size());
    match.clear();
}

SBQuery::SBQuery() : _par(nullptr), _pma_size(0), _total_length(0) {}
SBQuery::SBQuery(sbsearch_param *p) : SBQuery() { setParam(p); }
SBQuery::~SBQuery() {}
spma *SBQuery::root() { return &_pmas[0]; }
ubytearray &SBQuery::query(int idx) { return _seqs[idx]; }
const ubytearray &SBQuery::query(int idx) const { return _seqs[idx]; }
size_t SBQuery::count() const { return _seqs.size(); }
void SBQuery::setSize(size_t s) { _seqs.resize(s); }
void SBQuery::addQuery(const char *seq) {
    auto size = strlen(seq);
    if (_par->ds_search) {
		_total_length += 2*size;
		_seqs.resize(_seqs.size() + 2);
		_seqs[-2].resize(size); _seqs[-1].resize(size);
		if (size) {
			converter((const uint8_t*)seq, 0, size, _seqs[-2].ptr());
			if (_par->ref_type & DNA_SEQ) sseq::dcpycompi(_seqs[-2], _seqs[-1]);
			else if (_par->ref_type & RNA_SEQ) sseq::rcpycompi(_seqs[-2], _seqs[-1]);
		}
	}
	else {
		_total_length += size;
		_seqs.resize(_seqs.size() + 1);
		_seqs.last().resize(size);
		if (size) converter((const uint8_t*)seq, 0, size, _seqs.last().ptr());
	}    
}
void SBQuery::addQuery(ubytearray* seq, size_t off, size_t len) {
	if (len == -1) len = seq->size() - off;
	if (_par->ds_search) {
		_total_length += 2 * len;
		_seqs.resize(_seqs.size() + 2);
		_seqs[-2].resize(len); _seqs[-1].resize(len);
		if (len) {
			rawcopy(seq->ptr(), off, len, _seqs[-2].ptr());
			if (_par->ref_type & DNA_SEQ) sseq::dcpycompi(_seqs[-2], _seqs[-1]);
			else if (_par->ref_type & RNA_SEQ) sseq::rcpycompi(_seqs[-2], _seqs[-1]);
		}
	}
	else {
		_total_length += len;
		_seqs.resize(_seqs.size() + 1);
		_seqs.last().resize(len);
		if (len) rawcopy(seq->ptr(), off, len, _seqs.last().ptr());
	}
}
/*
void SBQuery::addQuery(const char* seq) {
	auto size = strlen(seq);
	_total_length += size;
	_seqs.resize(_seqs.size() + 1);
	_seqs.last().resize(size);
	converter((const uint8_t*)seq, 0, size, _seqs.last().ptr());
}
*/
void SBQuery::addDSQuery(const char *seq) {
    addQuery(seq); addQuery(seq);
    if(_par->ref_type&DNA_SEQ) sseq::dcompi(_seqs.last());
    else if(_par->ref_type&RNA_SEQ) sseq::rcompi(_seqs.last());
}
void SBQuery::setTotalLength(size_t len) { _total_length = len; }
void SBQuery::makeTrie() {
    if(!_par) throw SBioInfoException(ERR_INFO, SLIB_NULL_ERROR, "_par");
    if (_seqs.empty()) return;
    _pmas.reserve(_total_length*_par->coded_seed_len);
    sforeach(_seqs) {
        if (E_.size() < _par->seed_len) continue;
        auto p = E_.ptr();
        auto len = E_.size()-_par->seed_len+1;
        sforin(l, 0, len) {
            spma *now = &_pmas[0];
            sforin(s, 0, _par->coded_seed_len) {
                auto q = encoder(p, s)+1;
                if (!(now->child[q])) {
                    _pmas.add(spma(_pma_size));
                    now->child[q] = &_pmas.last();
                }
                now = now->child[q];
            }
            now->add(it-_seqs.begin(), l);
            ++p;
        }
    }
    this->complete();
}
void SBQuery::makeStrictTrie() {
    if(!_par) throw SBioInfoException(ERR_INFO, SLIB_NULL_ERROR, "_par");
    if (_seqs.empty()) return;
    _pmas.reserve(_total_length*_par->coded_seed_len);
    sforeach(_seqs) {
        if (E_.size() < _par->seed_len) continue;
        auto p = E_.ptr();
        auto len = E_.size()-_par->seed_len+1;
        sforin(l, 0, len) {
            if (isAvailable(p, _par->seed_len)) {
                spma *now = &_pmas[0];
                sforin(s, 0, _par->coded_seed_len) {
                    auto q = encoder(p, s)+1;
                    if (!(now->child[q])) {
                        _pmas.add(spma(_pma_size));
                        now->child[q] = &_pmas.last();
                    }
                    now = now->child[q];
                }
                now->add((sint)(it-_seqs.begin()), l);
            }
            ++p;
        }
    }
    this->complete();
}
void SBQuery::setParam(sbsearch_param *p) {
    _par = p;
    switch (p->ref_type) {
        case DNA_SEQ:
            switch (_par->code_size) {
                case 0:
                    _pma_size = 256;
                    encoder = [](uint8_t *seq, int8_t pos)->short { return (short)sbio::DNA_BASE16[seq[pos]]; };
                    break;
                case 1:
                    _pma_size = 16;
                    encoder = [](uint8_t *seq, int8_t pos)->short { return (short)seq[pos]; };
                    break;
                case 2:
                    _pma_size = 256;
                    encoder = [](uint8_t *seq, int8_t pos)->short { return (short)((seq[2*pos]<<4)|seq[2*pos+1]); };
                    break;
                case 4:
                    _pma_size = 256;
                    encoder = [](uint8_t *seq, int8_t pos)->short {
                        return (short)((sseq::b24(seq[4*pos])<<6)|(sseq::b24(seq[4*pos+1])<<4)|
                                       (sseq::b24(seq[4*pos+2])<<2)|sseq::b24(seq[4*pos+3]));
                    };
                    break;
                default:
                    break;
            }
            converter = sseq::dencode1;
            isAvailable = [](uint8_t *seq, int8_t len)->bool {
                sforin(i, 0, len) {
                    if(*seq == 0 || *seq == 15) return false;
                    ++seq;
                }
                return true;
            };
            break;
        case RNA_SEQ:
        {
            switch (_par->code_size) {
                case 0:
                    _pma_size = 256;
                    encoder = [](uint8_t *seq, int8_t len)->short { return (short)sbio::RNA_BASE[seq[len]]; };
                    break;
                case 1:
                    _pma_size = 5;
                    encoder = [](uint8_t *seq, int8_t len)->short { return (short)seq[len]; };
                    break;
                default:
                    break;
            }
            converter = sseq::rencode;
            isAvailable = [](uint8_t *seq, int8_t len)->bool {
                sforin(i, 0, len) { if(seq[i] == 4) return false; }
                return true;
            };
            break;
        }
        case AA_SEQ:
        {
            switch (_par->code_size) {
                case 0:
                    _pma_size = 256;
                    encoder = [](uint8_t *seq, int8_t len)->short { return (short)sbio::AMINO_ACID[seq[len]]; };
                    break;
                case 1:
                    _pma_size = 26;
                    encoder = [](uint8_t *seq, int8_t len)->short { return (short)seq[len]; };
                    break;
                default:
                    break;
            }
            converter = sseq::aencode;
            isAvailable = [](uint8_t *seq, int8_t len)->bool {
                sforin(i, 0, len) { if(seq[i] == 23) return false; }
                return true;
            };
            break;
        }
        default:
        {
            _pma_size = 256;
            converter = rawcopy;
            encoder = [](uint8_t *seq, int8_t len)->short { return (short)seq[len]; };
            isAvailable = [](uint8_t *seq, int8_t len)->bool { return true; };
            break;
        }
    }
    reset();
}
void SBQuery::reset() {
    _pmas.clear();
    _pmas.add(spma(_pma_size));
    _pmas[0].child[0] = &_pmas[0];
    _seqs.clear();
    _total_length = 0;
}
void SBQuery::complete() {
    auto size = root()->child.size();
    std::queue<spma *> stack;
    sforin(i, 1, size) {
        if (!(_pmas[0].child[i])) _pmas[0].child[i] = &_pmas[0];
        else {
            _pmas[0].child[i]->child[0] = &_pmas[0];
            stack.push(_pmas[0].child[i]);
        }
    }
    while (!stack.empty()) {
        spma *now = stack.front();
        stack.pop();
        sforin(i, 1, size) {
            if (now->child[i]) {
                spma *next = now->child[0];
                while (!(next->child[i])) next = next->child[0];
                now->child[i]->child[0] = next->child[i];
                if (next->child[i]->match.size())
                    now->child[i]->match.append(next->child[i]->match);
                stack.push(now->child[i]);
            }
        }
    }
}
