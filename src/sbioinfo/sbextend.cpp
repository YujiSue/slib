#include "sbioinfo/sbsearch.h"

using namespace slib;
using namespace slib::sbio;

SBExtend::SBExtend() : _par(nullptr) {}
SBExtend::SBExtend(sbsearch_param *p) : SBExtend() { setParam(p); }
SBExtend::~SBExtend() {}

void SBExtend::_extendExactHead(salign *al) {
    sbyte cmp = _par->aln_par.compare_table[*_ref][*_que];
    if (cmp == scigar::PMATCH || cmp == scigar::MATCH) {
        do {
            al->cigars.put(scigar(cmp, 1));
            al->score += _par->aln_par.score_table[*_ref][*_que];
            --al->ref.begin; --al->aligned.begin;
            --_ref; --_que; --_rlen; --_qlen;
            cmp = _par->aln_par.compare_table[*_ref][*_que];
        }
        while (0 < _rlen && 0 < _qlen && (cmp == scigar::PMATCH || cmp == scigar::MATCH));
    }
}
void SBExtend::_extendExactTail(salign *al) {
    sbyte cmp = _par->aln_par.compare_table[*_ref][*_que];
    if (cmp == scigar::PMATCH || cmp == scigar::MATCH) {
        do {
            al->cigars.add(scigar(cmp, 1));
            al->score += _par->aln_par.score_table[*_ref][*_que];
            ++al->ref.end; ++al->aligned.end;
            ++_ref; ++_que; --_rlen; --_qlen;
            cmp = _par->aln_par.compare_table[*_ref][*_que];
        }
        while (0 < _rlen && 0 < _qlen && (cmp == scigar::PMATCH || cmp == scigar::MATCH));
    }
}
void SBExtend::_extendHead(salign *al) {
    int rl_, ql_;
    while (true) {
        if (_rlen == 0 || _qlen == 0) break;
        _extendExactHead(al);
        rl_ = _rlen < _par->aln_par.align_length?_rlen:_par->aln_par.align_length;
        ql_ = _qlen < _par->aln_par.align_length?_qlen:_par->aln_par.align_length;
        align.align(_ref-rl_+1, rl_, _que-ql_+1, ql_);
        if (align.cigars.empty()) break;
        _len = 0; _rl = 0; _ql = 0;
        _score = align.scores.ptr()+align.scores.size();
        _s = 0.0f; _ext = false;
        auto it = align.cigars.end()-1, it_ = it+1;
        while(align.cigars.begin() <= it) {
            _len += E_.length; _score -= E_.length;
            if ((_rlen == 0 || _qlen == 0) ||
                (E_.option == scigar::MMATCH && _par->max_miss < E_.length) ||
                ((E_.option == scigar::DELETION || E_.option == scigar::INSERTION) &&
                 _par->max_gap < E_.length)) break;
            else if (E_.option == scigar::DELETION) _rl += E_.length;
            else if (E_.option == scigar::INSERTION) _ql += E_.length;
            else if (E_.option == scigar::MMATCH) { _rl += E_.length; _ql += E_.length; }
            else {
                _rl += E_.length;
                _ql += E_.length;
                _s = ((*_score)-_s);
                if (_s/_len < _par->extend_threshold) break;
                _ref -= _rl; _que -= _ql;
                al->ref.begin -= _rl; al->aligned.begin -= _ql;
                al->score += _s; _rlen -= _rl; _qlen -= _ql;
                srforin(cit, it_, it) al->cigars.put(*cit);
                _rl = 0; _ql = 0; _len = 0; _s = (*_score); _ext = true; it_ = it;
            }
            PREV_;
        }
        if (!_ext) break;
    }
}
void SBExtend::_extendTail(salign *al) {
    while (true) {
        if (_rlen == 0 || _qlen == 0) break;
        _extendExactTail(al);
        align.ralign(_ref, _rlen < _par->aln_par.align_length?_rlen:_par->aln_par.align_length,
                     _que, _qlen < _par->aln_par.align_length?_qlen:_par->aln_par.align_length);
        if (align.cigars.empty()) break;
        _len = 0; _rl = 0; _ql = 0; _score = align.scores.ptr()-1; _s = 0.0f; _ext = false;
        auto it = align.cigars.begin(), it_ = it;
        while(it < align.cigars.end()) {
            _len += E_.length; _score += E_.length;
            if ((_rlen == 0 || _qlen == 0) ||
                (E_.option == scigar::MMATCH && _par->max_miss < E_.length) ||
                ((E_.option == scigar::DELETION || E_.option == scigar::INSERTION) &&
                 _par->max_gap < E_.length)) break;
            if (E_.option == scigar::DELETION) _rl += E_.length;
            else if (E_.option == scigar::INSERTION) _ql += E_.length;
            else if (E_.option == scigar::MMATCH) { _rl += E_.length; _ql += E_.length; }
            else {
                _rl += E_.length;
                _ql += E_.length;
                _s = ((*_score)-_s);
                if (_s/_len < _par->extend_threshold) break;
                al->ref.end += _rl; al->aligned.end += _ql;
                _ref += _rl; _que += _ql;
                al->score += _s; _rlen -= _rl; _qlen -= _ql;
                sforin(cit, it_, it+1) al->cigars.add(*cit);
                _rl = 0; _ql = 0; _len = 0; _s = (*_score); _ext = true; it_ = it+1;
            }
            NEXT_;
        }
        if (!_ext) break;
    }
}

void SBExtend::extendHead(SBioSeq *ref, ubytearray *que, salign *al) {
    _qlen = al->aligned.begin;
    _rlen = _qlen*(1+_par->max_gap);
    if (!_qlen || !_rlen) return;
    if (al->ref.begin < _rlen) _rlen = al->ref.begin;
    _ref_seq.resize(_rlen);
    ref->recode(COMPRESS1, _ref_seq, al->ref.begin-_rlen, _rlen);
    _ref = _ref_seq.ptr(_rlen-1);
    _que = que->ptr(_qlen-1);
    _extendHead(al);
}
void SBExtend::extendTail(SBioSeq *ref, ubytearray *que, salign *al) {
    _qlen = que->size()-al->aligned.end-1;
    _rlen = _qlen*(1+_par->max_gap);
    if (!_qlen || !_rlen) return;
    if (ref->length() < al->ref.end + _rlen + 1) _rlen = ref->length()-al->ref.end-1;
    _ref_seq.resize(_rlen);
    ref->recode(COMPRESS1, _ref_seq, al->ref.end+1, _rlen);
    _ref = _ref_seq.ptr(); _que = que->ptr(al->aligned.end+1);
    _extendTail(al);
}
void SBExtend::extend(SBioSeq *ref, ubytearray *que, salign *al) {
    extendHead(ref, que, al);
    extendTail(ref, que, al);
}

inline void _join(salign *a1, salign *a2) {
    a1->cigars.append(a2->cigars);
    a1->ref.end = a2->ref.end;
    a1->aligned.end = a2->aligned.end;
    a1->score += a2->score;
}
inline void _fillGap(subyte type, int size, salign *a1, salign *a2, salign_param *par) {
    a1->ref.end = a2->ref.end;
    a1->aligned.end = a2->aligned.end;
    a1->cigars.add(scigar(type, size));
    a1->cigars.append(a2->cigars);
    a1->score += par->gap_score+par->gap2_score*(size-1)+a2->score;
}
bool SBExtend::joint(SBioSeq *ref, ubytearray *que, salign *a1, salign *a2) {
    int rgap = a2->ref.begin-a1->ref.end-1,
    qgap = a2->aligned.begin-a1->aligned.end-1;
    if (rgap <= 0) {
        if (abs(qgap-rgap) <= _par->max_gap && -rgap < a2->cigars[0].length) {
            a2->cigars[0].length+=rgap; a2->ref.begin-=rgap; a2->aligned.begin-=rgap;
            _fillGap(scigar::INSERTION, abs(qgap-rgap), a1, a2, &_par->aln_par);
            return true;
        }
        else return false;
    }
    else if (qgap <= 0) {
        if (abs(rgap-qgap) < _par->max_gap && -qgap < a2->cigars[0].length) {
            a2->cigars[0].length+=qgap; a2->ref.begin-=qgap; a2->aligned.begin-=qgap;
            _fillGap(scigar::DELETION, abs(rgap-qgap), a1, a2, &_par->aln_par);
            return true;
        }
        else return false;
    }
    else {
        _qlen = qgap;
        _rlen = qgap*(_par->max_gap+1);
        if (rgap < _rlen) _rlen = rgap;
        _ref_seq.resize(_rlen);
        ref->recode(COMPRESS1, _ref_seq, a1->ref.end+1, _rlen);
        _ref = _ref_seq.ptr(); _que = que->ptr(a1->aligned.end+1);
        if (_par->aln_par.align_length < _rlen || _par->aln_par.align_length < _qlen) _extendTail(a1);
        if (_rlen <= _par->aln_par.align_length && _qlen <= _par->aln_par.align_length) {
            align.ralign(_ref, _rlen, _que, _qlen);
            if (!align.cigars.empty()) {
                _ext = true; _len = 0;
                sforeach(align.cigars) {
                    if ((E_.option == scigar::MMATCH && _par->max_miss < E_.length) ||
                        ((E_.option == scigar::DELETION || E_.option == scigar::INSERTION) &&
                         _par->max_gap < E_.length)) { _ext = false; break; }
                    _len += E_.length;
                }
                if (_ext && _par->extend_threshold*(_len+a2->ref.length(true)) <= align.scores.last()+a2->score) {
                    a1->ref.end = a2->ref.end; a1->aligned.end = a2->aligned.end;
                    a1->cigars.append(align.cigars); a1->cigars.append(a2->cigars);
                    a1->score+=align.scores.last()+a2->score;
                    return true;
                }
            }
        }
    }
    return false;
}
void SBExtend::assemble(SBioSeq *ref, ubytearray *que, Array<salign> *aligns) {
    if (aligns->empty()) return;
    auto size = aligns->size();
    if (size == 1) extend(ref, que, &aligns->first());
    else {
        sforeach(*aligns) {
            if (E_.ref.idx < 0) continue;
            extendHead(ref, que, &E_);
            auto it_ = it+1;
            while (it_ < aligns->end()) {
                if (joint(ref, que, &E_, &(*it_))) {
                    it_->ref.idx = -1; --size; ++it_;
                }
                else break;
            }
            if (it_ == it+1) extendTail(ref, que, &E_);
            it = it_-1;
        }
    }
    /*
    aligns->sort([](const salign &a1, const salign &a2) {
        if (a1.ref.idx < 0) return false;
        if (a2.ref.idx < 0) return true;
        return a1 < a2;
    });
    aligns->resize(size);
     */
}
void SBExtend::setParam(sbsearch_param *p) {
    _par = p; align.set(&_par->aln_par);
}
void SBExtend::reset() { align.reset(); }