#include "sbioinfo/seqsearch.h"
inline void dnascore(slib::smat<slib::sbyte> &table, slib::sbyte pm, slib::sbyte am, slib::sbyte mm, slib::sbyte gp, slib::sbyte gs) {
    table = slib::smat<slib::sbyte>(16, 16, {
        ///// =  A  C  M  G  R  S  V  T  W  Y  H  K  D  B  N
        /*=*/gs,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,
        /*A*/gp,pm,mm,am,mm,am,mm,am,mm,am,mm,am,mm,am,mm,am,
        /*C*/gp,mm,pm,am,mm,mm,am,am,mm,mm,am,am,mm,mm,am,am,
        /*M*/gp,am,am,am,mm,mm,mm,mm,mm,mm,mm,mm,mm,mm,mm,am,
        /*G*/gp,mm,mm,mm,pm,am,am,am,mm,mm,mm,mm,am,am,am,am,
        /*R*/gp,am,mm,mm,am,am,mm,mm,mm,mm,mm,mm,mm,mm,mm,am,
        /*S*/gp,mm,am,mm,am,mm,am,mm,mm,mm,mm,mm,mm,mm,mm,am,
        /*V*/gp,am,am,mm,am,mm,mm,am,mm,mm,mm,mm,mm,mm,mm,am,
        /*T*/gp,mm,mm,mm,mm,mm,mm,mm,pm,am,am,am,am,am,am,am,
        /*W*/gp,am,mm,mm,mm,mm,mm,mm,am,am,mm,mm,mm,mm,mm,am,
        /*Y*/gp,mm,am,mm,mm,mm,mm,mm,am,mm,am,mm,mm,mm,mm,am,
        /*H*/gp,am,am,mm,mm,mm,mm,mm,am,mm,mm,am,mm,mm,mm,am,
        /*K*/gp,mm,mm,mm,am,mm,mm,mm,am,mm,mm,mm,am,mm,mm,am,
        /*D*/gp,am,mm,mm,am,mm,mm,mm,am,mm,mm,mm,mm,am,mm,am,
        /*B*/gp,mm,am,mm,am,mm,mm,mm,am,mm,mm,mm,mm,mm,am,am,
        /*N*/gp,am,am,am,am,am,am,am,am,am,am,am,am,am,am,am
    });
}
inline void rnascore(slib::smat<slib::sbyte>& table, slib::sbyte pm, slib::sbyte am, slib::sbyte mm, slib::sbyte gp, slib::sbyte gs) {
    table = slib::smat<slib::sbyte>(5, 5, {
        ///// -  A  C  G  T  N
        /*-*/gs,gp,gp,gp,gp,gp,
        /*A*/gp,pm,mm,mm,mm,am,
        /*C*/gp,mm,pm,mm,mm,am,
        /*G*/gp,mm,mm,pm,mm,am,
        /*T*/gp,mm,mm,mm,pm,am,
        /*N*/gp,am,am,am,am,am
    });
}
inline void aascore(slib::smat<slib::sbyte> &table, const slib::String& name) {
    table = slib::smat<slib::sbyte>(26, 26);
    if (name.beginWith("BROSSUM")) {
		/*
		*/
    }
}
inline void dnacompare(slib::smat<slib::sbyte>& table) {
    table = slib::smat<slib::sbyte>(16, 16, {
        /////= A C M G R S V T W Y H K D B N
        /*=*/7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
        /*A*/0,7,8,0,8,0,8,0,8,0,8,0,8,0,8,8,
        /*C*/0,8,7,0,8,8,0,0,8,8,0,0,8,8,0,8,
        /*M*/0,0,0,0,8,8,8,8,8,8,8,8,8,8,8,8,
        /*G*/0,8,8,8,7,0,0,0,8,8,8,8,0,0,0,8,
        /*R*/0,0,8,8,0,0,8,8,8,8,8,8,8,8,8,8,
        /*S*/0,8,0,8,0,8,0,8,8,8,8,8,8,8,8,8,
        /*V*/0,0,0,8,0,8,8,0,8,8,8,8,8,8,8,8,
        /*T*/0,8,8,8,8,8,8,8,7,0,0,0,0,0,0,8,
        /*W*/0,0,8,8,8,8,8,8,0,0,8,8,8,8,8,8,
        /*Y*/0,8,0,8,8,8,8,8,0,8,0,8,8,8,8,8,
        /*H*/0,0,0,8,8,8,8,8,0,8,8,0,8,8,8,8,
        /*K*/0,8,8,8,0,8,8,8,0,8,8,8,0,8,8,8,
        /*D*/0,0,8,8,0,8,8,8,0,8,8,8,8,0,8,8,
        /*B*/0,8,0,8,0,8,8,8,0,8,8,8,8,8,0,8,
        /*N*/7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0
    });
}
inline void nacompare(slib::smat<slib::sbyte>& table) {
    table = slib::smat<slib::sbyte>(5, 5, {
        /////A C G T N
        /*A*/8,0,8,0,0,
        /*C*/0,8,0,0,0,
        /*G*/0,0,8,0,0,
        /*T*/0,0,0,8,0,
        /*N*/0,0,0,0,7
        });
}

slib::sbio::AlignmentParam::AlignmentParam(subyte t, const char* s) {
    seqtype = t;
	max_length = 20;
    pm_score = DEFAULT_PM_SCORE;
    am_score = DEFAULT_AM_SCORE;
    mm_score = DEFAULT_MM_SCORE;
    gap_score = DEFAULT_GAP_SCORE;
    gap2_score = DEFAULT_GAP2_SCORE;
    score = s;
    makeTable();
}
slib::sbio::AlignmentParam::~AlignmentParam() {}
slib::sbio::AlignmentParam & slib::sbio::AlignmentParam::operator=(const AlignmentParam &par) {
    seqtype = par.seqtype;
    max_length = par.max_length;

    pm_score = par.pm_score;
    am_score = par.am_score;
    mm_score = par.mm_score;
    gap_score = par.gap_score;
    gap2_score = par.gap2_score;

    score = par.score;
    scores = par.scores;
    compares = par.compares;
    return *this;
}
void slib::sbio::AlignmentParam::setSeqType(subyte t) {
    seqtype = t;
    makeTable();
}
void slib::sbio::AlignmentParam::makeTable(const char *s) {
    if (s) score = s;
    switch (seqtype & 0x07) {
    case sseq::DNA:
        dnacompare(compares);
        dnascore(scores, pm_score, am_score, mm_score, gap_score, gap2_score);
        break;
    case sseq::RNA:
        nacompare(compares);
        rnascore(scores, pm_score, am_score, mm_score, gap_score, gap2_score);
        break;
    case sseq::AA:
        aascore(scores, score);
        break;
    default:
        break;
    }
}
void slib::sbio::AlignmentParam::set(const sobj &obj) {
    if (obj["type"]) seqtype = obj["type"];
    if (obj["length"]) max_length = obj["length"];

    if (!obj["pmscore"].isNull()) pm_score = obj["pmscore"];
    if (!obj["amscore"].isNull()) am_score = obj["amscore"];
    if (!obj["mmscore"].isNull()) mm_score = obj["mmscore"];
    if (!obj["gapscore"].isNull()) gap_score = obj["gapscore"];
    if (!obj["gap2score"].isNull()) gap2_score = obj["gap2score"];

    makeTable(obj["score"]);
}
sobj slib::sbio::AlignmentParam::toObj() {
    return { 
        D_("type", seqtype),
        D_("length", max_length),

        D_("pmscore", pm_score),
        D_("amscore", am_score),
        D_("mmscore", mm_score),
        D_("gapscore", gap_score),
        D_("gap2score", gap2_score),

        D_("score", score)
    };
}

slib::sbio::AlignPair::AlignPair() : score(0) {}
slib::sbio::AlignPair::AlignPair(const sbpos& r, const srange& q) : AlignPair() {
    ref = r; query = q;
}
slib::sbio::AlignPair::AlignPair(const slib::sbio::AlignPair &align) {
    ref = align.ref;
    query = align.query;
    score = align.score;
    cigars = align.cigars;
}
slib::sbio::AlignPair::~AlignPair() {}
slib::sbio::AlignPair & slib::sbio::AlignPair::operator = (const AlignPair &align) {
    ref = align.ref;
    query = align.query;
    score = align.score;
    cigars = align.cigars;
    return (*this);
}
bool slib::sbio::AlignPair::operator < (const AlignPair &align) const {
    if (ref != align.ref) return ref < align.ref;
    if (query != align.query) return query < align.query;
    return false;
}
bool slib::sbio::AlignPair::operator == (const AlignPair &align) const {
    return ref == align.ref && query == align.query && cigars == align.cigars;
}
void slib::sbio::AlignPair::scoring(AlignmentParam *par) {
    score = 0;
    sfor(cigars) {
		if ($_.option == sbio::scigar::PMATCH) score += $_.length * par->pm_score;
		else if ($_.option == sbio::scigar::MATCH) score += $_.length * par->am_score;
		else if ($_.option == sbio::scigar::MMATCH) score += $_.length * par->mm_score;
        else score += par->gap_score+($_.length-1)*par->gap2_score;
    }
}
void slib::sbio::AlignPair::complement(const size_t sz) {
    ref.dir = !ref.dir;
    sbio::sutil::reverse(query, sz);
    cigars.reverse();
}
void slib::sbio::AlignPair::clear() {
    ref = sbpos();
    query = srange();
    score = 0;
    cigars.clear();
}
slib::String slib::sbio::AlignPair::alref(const String &ref) {
    String str;
    int offset = 0;
    sfor(cigars) {
        if($_.option == sbio::scigar::INSERTION) { sforin(i, 0, $_.length) str += "-"; }
        else {
            str += ref.substring(offset, $_.length);
            offset += $_.length;
        }
    }
    return str;
}
slib::String slib::sbio::AlignPair::match() {
    String str = "";
    sfor(cigars) {
        if($_.option == sbio::scigar::PMATCH) { sforin(i, 0, $_.length) str += "*"; }
        else if($_.option == sbio::scigar::MATCH) { sforin(i, 0, $_.length) str += "+"; }
        else if($_.option == sbio::scigar::MMATCH) { sforin(i, 0, $_.length) str += "."; }
        else { sforin(i, 0, $_.length) str += " "; }
    }
    return str;
}
slib::String slib::sbio::AlignPair::consensus(const String& ref, const String& que) {
	String str;
	int roff = 0, qoff = 0;
    sfor(cigars) {
		if ($_.option == sbio::scigar::INSERTION) { 
			str += que.substring(qoff, $_.length);
			qoff += $_.length;
		}
		else if ($_.option == sbio::scigar::DELETION) { 
			str += ref.substring(roff, $_.length);
			roff += $_.length;
		}
		else {
			if ($_.option == sbio::scigar::PMATCH) str += ref.substring(roff, $_.length);
			else {
				auto r_ = ref.substring(roff, $_.length);
				auto q_ = que.substring(qoff, $_.length);
				String tmp($_.length, '\0');
				sforin(l, 0, $_.length) {
					tmp[l] = sbio::DNA_BASE16[(int)(sbio::sna::encBase16(r_[l]) | sbio::sna::encBase16(q_[l]))];
				}
				str += tmp;
			}
			roff += $_.length; qoff += $_.length;
		}
	}
	return str;
}
slib::String slib::sbio::AlignPair::alque(const String &que) {
    String str = "";
    int offset = 0;
    sfor(cigars) {
        if($_.option == sbio::scigar::DELETION) { sforin(i, 0, $_.length) str += "-"; }
        else {
            str += que.substring(offset, $_.length);
            offset += $_.length;
        }
    }
    return str;
}

slib::sbio::Alignment::Alignment() : _par(nullptr) {}
slib::sbio::Alignment::Alignment(AlignmentParam* p) : Alignment() { setParam(p); }
slib::sbio::Alignment::~Alignment() {}
void slib::sbio::Alignment::_align1(subyte* ref, int rlen, subyte* que, int qlen) {
    // Set byte pointer of ref (row) and que (column)
    auto rseq = ref, qseq = que;
    // Column count
    auto col = qlen + 1;
    sforin(r, 0, rlen) {
        // reset column pos.
        qseq = que;
        // Alignment path pointer
        auto path = &_path[r + 1][1];
        // Score matrix pointer of upper left and current position
        auto ul = &_score[r][0], current = &_score[r + 1][1];
        sforin(q, 0, qlen) {
            // Candidate zero (negative score is not allowed)
            _buffer[0] = 0;
            // Candidate lower right
            _buffer[1] = (*ul) + (int)_par->scores[(int)(*rseq)][(int)(*qseq)];
            // Candidate lower
            _buffer[2] = _score[_maxrow[q + 1]][q + 1] + (int)_par->gap_score + (int)_par->gap2_score * (r - _maxrow[q + 1]);
            // Candidate right
            _buffer[3] = _score[r + 1][_maxcol[r + 1]] + (int)_par->gap_score + (int)_par->gap2_score * (q - _maxcol[r + 1]);
            auto di = (int)sstat::argmax(_buffer);
            // Define current score and path
            *current = _buffer[di];
            if (di == 2) *path = sbio::scigar::DELETION;
            else if (di == 3) *path = sbio::scigar::INSERTION;
            else *path = _par->compares[(int)(*rseq)][(int)(*qseq)];
            // Update max score pos
            if (_score[_maxrow[q + 1]][q + 1] <= (*current)) _maxrow[q + 1] = r + 1;
            if (_score[r + 1][_maxcol[r + 1]] <= (*current)) _maxcol[r + 1] = q + 1;
            ++current;
            ++ul;
            ++path;
            ++qseq;
        }
        ++rseq;
    }
    // Retrace
    cigars.setOffset(2 * _par->max_length - 1);
    auto path = _path[rlen][qlen];
    while (0 < rlen && 0 < qlen) {
        cigars.add(Cigar(path, 1), DIRECTION::HEAD);
        if (path == scigar::MATCH ||
            path == scigar::MMATCH ||
            path == scigar::PMATCH) {
            --rlen; --qlen;
        }
        else {
            if (path == sbio::scigar::DELETION) --rlen;
            else --qlen;
        }
        path = _path[rlen][qlen];

    }
    if (0 < rlen)
        cigars.add(Cigar(sbio::scigar::DELETION, rlen), DIRECTION::HEAD);
    else if (0 < qlen)
        cigars.add(Cigar(sbio::scigar::INSERTION, qlen), DIRECTION::HEAD);
    scores.resize(cigars.size());
    sfor2(scores, cigars) {
        if ($_2.gap()) $_1 = _par->gap_score + _par->gap2_score * ($_2.length - 1);
        else {
            if ($_2.option == scigar::MMATCH) $_1 = _par->mm_score * $_2.length;
            else if ($_2.option == scigar::PMATCH) $_1 = _par->pm_score * $_2.length;
            else $_1 = _par->am_score * $_2.length;
        }
        if ($_2.gap()) $_1 = _par->gap_score + _par->gap2_score * ($_2.length - 1);
    }
}
void slib::sbio::Alignment::_align2(subyte* ref, int rlen, subyte* que, int qlen) {
    // Set byte pointer of ref (row) and que (column)
    auto rseq = ref + rlen - 1, qseq = que + qlen - 1;
    // Column count
    auto col = qlen + 1;
    sforin(r, 0, rlen) {
        // reset column pos.
        qseq = que + qlen - 1;
        // Alignment path pointer
        auto path = &_path[r + 1][1];
        // Score matrix pointer of upper left and current position
        auto ul = &_score[r][0], current = &_score[r + 1][1];
        sforin(q, 0, qlen) {
            // Candidate zero (negative score is not allowed)
            _buffer[0] = 0;
            // Candidate lower right
            _buffer[1] = (*ul) + (int)_par->scores[(int)(*rseq)][(int)(*qseq)];
            // Candidate lower
            _buffer[2] = _score[_maxrow[q + 1]][q + 1] + (int)_par->gap_score + (int)_par->gap2_score * (r - _maxrow[q + 1]);
            // Candidate right
            _buffer[3] = _score[r + 1][_maxcol[r + 1]] + (int)_par->gap_score + (int)_par->gap2_score * (q - _maxcol[r + 1]);
            auto di = (int)sstat::argmax(_buffer);
            // Define current score and path
            *current = _buffer[di];
            if (di == 2) *path = sbio::scigar::DELETION;
            else if (di == 3) *path = sbio::scigar::INSERTION;
            else *path = _par->compares[(int)(*rseq)][(int)(*qseq)];
            // Update max score pos
            if (_score[_maxrow[q + 1]][q + 1] <= (*current)) _maxrow[q + 1] = r + 1;
            if (_score[r + 1][_maxcol[r + 1]] <= (*current)) _maxcol[r + 1] = q + 1;
            ++current;
            ++ul;
            ++path;
            --qseq;
        }
        --rseq;
    }
    // Retrace
    cigars.setOffset(1);
    auto path = _path[rlen][qlen];
    while (0 < rlen && 0 < qlen) {
        cigars.add(Cigar(path, 1), DIRECTION::TAIL);
        if (path == scigar::MATCH ||
            path == scigar::MMATCH ||
            path == scigar::PMATCH) {
            --rlen; --qlen;
        }
        else {
            if (path == sbio::scigar::DELETION) --rlen;
            else --qlen;
        }
        path = _path[rlen][qlen];
    }
    if (0 < rlen)
        cigars.add(Cigar(sbio::scigar::DELETION, rlen), DIRECTION::TAIL);
    else if (0 < qlen)
        cigars.add(Cigar(sbio::scigar::INSERTION, qlen), DIRECTION::TAIL);
    scores.resize(cigars.size());
    sfor2(scores, cigars) {
        if ($_2.gap()) $_1 = _par->gap_score + _par->gap2_score * ($_2.length - 1);
        else {
            if ($_2.option == scigar::MMATCH) $_1 = _par->mm_score * $_2.length;
            else if ($_2.option == scigar::PMATCH) $_1 = _par->pm_score * $_2.length;
            else $_1 = _par->am_score * $_2.length;
        }
        if ($_2.gap()) $_1 = _par->gap_score + _par->gap2_score * ($_2.length - 1);
    }
}
void slib::sbio::Alignment::align(subyte* ref, int rlen, subyte* que, int qlen, bool dir) {
    if (!rlen || !qlen) return;
    if (dir) _align1(ref, rlen, que, qlen);
    else _align2(ref, rlen, que, qlen);
}
void slib::sbio::Alignment::setParam(AlignmentParam* p) {
    _par = p;
    _maxcol.resize(_par->max_length + 1);
    _maxrow.resize(_par->max_length + 1);
    _score.resize(_par->max_length + 1, _par->max_length + 1);
    _path.resize(_par->max_length + 1, _par->max_length + 1);
    cigars.reserve(2 * _par->max_length);
    scores.reserve(2 * _par->max_length);
    reset();
}
void slib::sbio::Alignment::reset() {
    _maxcol.reset(0);
    _maxrow.reset(0);
    _score.reset(0);
    _path.reset(0);
    cigars.clear();
    scores.clear();
}

slib::sbio::AlignExtend::AlignExtend() : 
    _par(nullptr), _rlimit(0), _rlen(0), _qlimit(0), _qlen(0), _maxlen(0), _score(0.f) {}
slib::sbio::AlignExtend::AlignExtend(slib::sbio::SeqSearchParam* p) : AlignExtend() { setParam(p); }
slib::sbio::AlignExtend::~AlignExtend() {}
/*
void slib::sbio::AlignExtend::_extendHead(salign* al) {
    int rl_, ql_;
    while (true) {
        if (_rlen == 0 || _qlen == 0) break;
        _extendExactHead(al);
        rl_ = _rlen < _par->apar.max_length ? _rlen : _par->apar.max_length;
        ql_ = _qlen < _par->apar.max_length ? _qlen : _par->apar.max_length;
        
        
        aligner.align(_ref - rl_ + 1, rl_, _que - ql_ + 1, ql_);
        
        
        if (align.cigars.empty()) break;
        _len = 0; _rl = 0; _ql = 0;
        _score = align.scores.data() + align.scores.size();
        _s = 0.0f; _ext = false;
        auto it = align.cigars.end() - 1, it_ = it + 1;
        while (align.cigars.begin() <= it) {
            _len += $_.length; _score -= $_.length;
            if ((_rlen == 0 || _qlen == 0) ||
                ($_.option == scigar::MMATCH && _par->max_miss < $_.length) ||
                (($_.option == scigar::DELETION || $_.option == scigar::INSERTION) &&
                    _par->max_gap < $_.length)) break;
            else if ($_.option == scigar::DELETION) _rl += $_.length;
            else if ($_.option == scigar::INSERTION) _ql += $_.length;
            else if ($_.option == scigar::MMATCH) { _rl += $_.length; _ql += $_.length; }
            else {
                _rl += $_.length;
                _ql += $_.length;
                _s = ((*_score) - _s);
                if (_s / _len < _par->ext_threshold) break;
                _ref -= _rl; _que -= _ql;
                al->ref.begin -= _rl; al->query.begin -= _ql;
                al->score += _s; _rlen -= _rl; _qlen -= _ql;
                srforin(cit, it, it_) al->cigars.add(*cit, DIRECTION::HEAD);
                _rl = 0; _ql = 0; _len = 0; _s = (*_score); _ext = true; it_ = it;
            }
            PREV_
        }
        if (!_ext) break;
    }
}
void slib::sbio::AlignExtend::_extendExactHead(salign* al) {
    sbyte cmp = _par->apar.compares[*_ref][*_que];
    if (cmp == scigar::PMATCH || cmp == scigar::MATCH) {
        do {
            al->cigars.add(Cigar(cmp, 1), DIRECTION::HEAD);
            al->score += _par->apar.scores[*_ref][*_que];
            --al->ref.begin; --al->query.begin;
            --_ref; --_que; --_rlen; --_qlen;
            cmp = _par->apar.compares[*_ref][*_que];
        } while (0 < _rlen && 0 < _qlen && (cmp == scigar::PMATCH || cmp == scigar::MATCH));
    }
}
*/
void slib::sbio::AlignExtend::_extend(Sequence* ref, ubytearray* que, salign* al, slib::DIRECTION dir) {
    auto ext = true;
    if (dir == slib::DIRECTION::HEAD) {
        while (ext) {
            // 
            _update(ref, que, al, dir);
            //
            if (!_rlen || !_qlen) break;
            //
            auto mlen = sstat::getMin(_rlen, _qlen);
            auto rseq = _buffer.data(-1), qseq = que->data(al->query.begin - 1);
            sforin(e, 0, mlen) {
                auto cmp = _par->apar.compares[(int)(*rseq)][(int)(*qseq)];
                if (cmp == scigar::MATCH || cmp == scigar::PMATCH) {
                    --rseq;
                    --qseq;
                    if (al->ref.dir) ++al->ref.end; 
                    else --al->ref.begin;
                    --al->query.begin;
                    al->cigars.add(Cigar(cmp, 1), dir);
                }
                else {
                    ext = false;
                    break;
                }
            }
        }
    }
    else {
        while (ext) {
            // 
            _update(ref, que, al, dir);
            //
            if (!_rlen || !_qlen) break;
            auto mlen = sstat::getMin(_rlen, _qlen);
            auto rseq = _buffer.data(), qseq = que->data(al->query.end + 1);
            sforin(e, 0, mlen) {
                auto cmp = _par->apar.compares[(int)(*rseq)][(int)(*qseq)];
                if (cmp == scigar::MATCH || cmp == scigar::PMATCH) {
                    ++rseq;
                    ++qseq;
                    if (al->ref.dir) --al->ref.begin; 
                    else ++al->ref.end;
                    ++al->query.end;
                    al->cigars.add(Cigar(cmp, 1), dir);
                }
                else {
                    ext = false;
                    break;
                }
            }
        }
    }
}
void slib::sbio::AlignExtend::_update(Sequence* ref, ubytearray* que, salign* al, slib::DIRECTION dir) {
    if (dir == slib::DIRECTION::HEAD) {
        // Set align length
        if (al->ref.dir) _rlen = al->ref.end + _maxlen < _rlimit ? _maxlen : _rlimit - al->ref.end - 1;
        else _rlen = _rlimit + _maxlen < al->ref.begin ? _maxlen : al->ref.begin - _rlimit;
        _qlen = _qlimit + _maxlen < al->query.begin ? _maxlen : al->query.begin - _qlimit;
        // Copy partial ref. to buffer 
        _buffer.resize(_rlen);
        if (_rlen) {
            if (al->ref.dir) {
                _encoder(ref->data(), al->ref.end + 1, _rlen, _buffer.data());
                slib::sbio::sna::toComplement16(_buffer);
            }
            else _encoder(ref->data(), al->ref.begin - _rlen, _rlen, _buffer.data());
        }
    }
    else {
        // Set align length
        if (al->ref.dir) _rlen = _rlimit + _maxlen < al->ref.begin ? _maxlen : al->ref.begin - _rlimit;
        else _rlen = al->ref.end + _maxlen < _rlimit ? _maxlen : _rlimit - al->ref.end - 1;
        _qlen = al->query.end + _maxlen < _qlimit ? _maxlen : _qlimit - al->query.end - 1;
        // Copy partial ref. to buffer 
        _buffer.resize(_rlen);
        if (_rlen) {
            if (al->ref.dir) {
                _encoder(ref->data(), al->ref.begin - _rlen, _rlen, _buffer.data());
                slib::sbio::sna::toComplement16(_buffer);
            }
            else _encoder(ref->data(), al->ref.end + 1, _rlen, _buffer.data());
        }
    }
}
void slib::sbio::AlignExtend::_deflimit(Sequence* ref, ubytearray* que, salign* al, slib::DIRECTION dir) {
    if (dir == slib::DIRECTION::HEAD) {
        if (al->ref.dir) {
            _rlimit = (int)ref->length();
            sfor(ref->mask) {
                if (al->ref.end < $_.begin) {
                    _rlimit = $_.begin;
                    break;
                }
            }
        }
        else {
            _rlimit = 0;
            sfor(ref->mask) {
                if (al->ref.begin < $_.begin) break;
                _rlimit = $_.end;
            }
        }
        _qlimit = 0;
    }
    else {
        if (al->ref.dir) {
            _rlimit = 0;
            sfor(ref->mask) {
                if (al->ref.begin < $_.begin) break;
                _rlimit = $_.end;
            }
        }
        else {
            _rlimit = (int)ref->length();
            sfor(ref->mask) {
                if (al->ref.end < $_.begin) {
                    _rlimit = $_.begin;
                    break;
                }
            }
        }
        _qlimit = (int)que->size();
    }
}
void slib::sbio::AlignExtend::extendHead(Sequence* ref, ubytearray* que, salign* al) {
    if (!al->query.begin) return;
    _deflimit(ref, que, al, DIRECTION::HEAD);
    while (true) {
        _extend(ref, que, al, DIRECTION::HEAD);
        _update(ref, que, al, DIRECTION::HEAD);
        if (!_rlen || !_qlen) break;
        aligner.reset();
        aligner.align(_buffer.data(), _rlen, que->data(al->query.begin - _qlen), _qlen);
        // Set update iter
        auto current = aligner.cigars.end();
        // Init score
        _score = 0;
        srfor2(aligner.cigars, aligner.scores) {
            // Add score
            _score += $_2;
            // Check break condition
            if (($_1.option == scigar::MMATCH && _par->max_miss < $_1.length) ||
                ($_1.gap() && _par->max_gap < $_1.length)) break;
            else if ($_1.option == scigar::MATCH || $_1.option == scigar::PMATCH) {
                // Update extend point
                if (aligner.cigars.queSize(it.first - aligner.cigars.begin()) * _par->ext_threshold < _score) {
                    current = it.first; _score = 0;
                }
            }
        }
        //
        if (current == aligner.cigars.end()) break;
        // Update aligned region
        if (al->ref.dir) al->ref.end += (int)aligner.cigars.refSize(current - aligner.cigars.begin());
        else al->ref.begin -= (int)aligner.cigars.refSize(current - aligner.cigars.begin());
        al->query.begin -= (int)aligner.cigars.queSize(current - aligner.cigars.begin());
        srforin(cig, current, aligner.cigars.end()) al->cigars.add(*cig, DIRECTION::HEAD);
    }
    al->scoring(&_par->apar);
    /*
    while (((al->ref.dir && al->ref.end + 1 < _rlimit) || 
        (!al->ref.dir && _rlimit < al->ref.begin)) && 
        _qlimit < al->query.begin) {
        // Simple comparation
        _extend(ref, que, al, DIRECTION::HEAD);
        if (((al->ref.dir && _rlimit == al->ref.begin) || 
            (!al->ref.dir && al->ref.end == _rlimit - 1)) &&
            _qlimit == al->query.begin) break;
        // Alignment
        _update(ref, que, al, DIRECTION::HEAD);
        aligner.reset();
        aligner.align(_buffer.data(), _rlen, que->data(al->query.begin - _qlen), _qlen);
        // Set update iter
        auto current = aligner.cigars.end();
        // Init score
        _score = 0;
        srfor2(aligner.cigars, aligner.scores) {
            // Add score
            _score += $_2;
            // Check break condition
            if ($_1.option == scigar::MMATCH && _par->max_miss < $_1.length ||
                $_1.gap() && _par->max_gap < $_1.length) {
                break;
            }
            else if ($_1.option == scigar::MATCH || $_1.option == scigar::PMATCH) {
                // Update extend point
                if (aligner.cigars.queSize(it.first - aligner.cigars.begin()) * _par->ext_threshold < _score) {
                    current = it.first;
                    _score = 0;
                }
            }
        }
        //
        if (current == aligner.cigars.end()) break;
        // Update aligned region
        if (al->ref.dir) al->ref.end += (int)aligner.cigars.refSize(current - aligner.cigars.begin());
        else al->ref.begin -= (int)aligner.cigars.refSize(current - aligner.cigars.begin());
        al->query.begin -= (int)aligner.cigars.queSize(current - aligner.cigars.begin());
        srforin(cig, current, aligner.cigars.end()) al->cigars.add(*cig, DIRECTION::HEAD);
    }
    al->scoring(&_par->apar);
    */
}
void slib::sbio::AlignExtend::extendHeadTo(Sequence* ref, ubytearray* que, salign* al, salign* lim) {
    if (!al->query.begin) return;
    _deflimit(ref, que, al, DIRECTION::HEAD);
    if (al->ref.dir == lim->ref.dir) {
        if (al->ref.dir) {
            if (lim->ref.begin - 1 < _rlimit) _rlimit = lim->ref.begin - 1;
        }
        else {
            if (_rlimit < lim->ref.end + 1) _rlimit = lim->ref.end + 1;
        }
    }
    if (_qlimit < lim->query.end + 1) _qlimit = lim->query.end + 1;
    //
    while (true) {
        _extend(ref, que, al, DIRECTION::HEAD);
        _update(ref, que, al, DIRECTION::HEAD);
        if (!_rlen || !_qlen) break;
        aligner.reset();
        aligner.align(_buffer.data(), _rlen, que->data(al->query.begin - _qlen), _qlen);
        // Set update iter
        auto current = aligner.cigars.end();
        // Init score
        _score = 0;
        srfor2(aligner.cigars, aligner.scores) {
            // Add score
            _score += $_2;
            // Check break condition
            if ($_1.option == scigar::MMATCH && _par->max_miss < $_1.length ||
                $_1.gap() && _par->max_gap < $_1.length) {
                break;
            }
            else if ($_1.option == scigar::MATCH || $_1.option == scigar::PMATCH) {
                // Update extend point
                if (aligner.cigars.queSize(it.first - aligner.cigars.begin()) * _par->ext_threshold < _score) {
                    current = it.first;
                    _score = 0;
                }
            }
        }
        //
        if (current == aligner.cigars.end()) break;
        // Update aligned region
        if (al->ref.dir) al->ref.end += (int)aligner.cigars.refSize(current - aligner.cigars.begin());
        else al->ref.begin -= (int)aligner.cigars.refSize(current - aligner.cigars.begin());
        al->query.begin -= (int)aligner.cigars.queSize(current - aligner.cigars.begin());
        srforin(cig, current, aligner.cigars.end()) al->cigars.add(*cig, DIRECTION::HEAD);
    }
    al->scoring(&_par->apar);

    /*
    _rlimit = rend; _qlimit = qend;
    
    while (_rlimit < al->ref.begin && _qlimit < al->query.begin) {
        // Simple comparation
        _extend(ref, que, al, DIRECTION::HEAD);
        if (_rlimit == al->ref.begin && _qlimit == al->query.begin) break;
        // Alignment
        _update(ref, que, al, DIRECTION::HEAD);
        aligner.reset();
        aligner.align(_buffer.data(), _rlen, que->data(al->query.begin - _qlen), _qlen);
        // Set update iter
        auto current = aligner.cigars.end();
        // Init score
        _score = 0;
        srfor2(aligner.cigars, aligner.scores) {
            // Add score
            _score += $_2;
            // Check break condition
            if ($_1.option == scigar::MMATCH && _par->max_miss < $_1.length ||
                $_1.gap() && _par->max_gap < $_1.length) {
                break;
            }
            else if ($_1.option == scigar::MATCH || $_1.option == scigar::PMATCH) {
                // Update extend point
                if (aligner.cigars.queSize(it.first - aligner.cigars.begin()) * _par->ext_threshold < _score)
                    current = it.first;
            }
            if (it.first == aligner.cigars.begin()) {
                
            }
        }
        //
        if (current == aligner.cigars.end()) break;
        // Update aligned region
        al->ref.begin -= (int)aligner.cigars.refSize(current - aligner.cigars.begin());
        al->query.begin -= (int)aligner.cigars.queSize(current - aligner.cigars.begin());
        srforin(cig, current, aligner.cigars.end()) al->cigars.add(*cig, DIRECTION::HEAD);
    }
    */
}
void slib::sbio::AlignExtend::extendTail(Sequence* ref, ubytearray* que, salign* al) {
    if (al->query.end == que->size() - 1) return;
    _deflimit(ref, que, al, DIRECTION::TAIL);
    while (true) {
        _extend(ref, que, al, DIRECTION::TAIL);
        _update(ref, que, al, DIRECTION::TAIL);
        if (!_rlen || !_qlen) break;
        aligner.reset();
        aligner.align(_buffer.data(), _rlen, que->data(al->query.end + 1), _qlen, false);
        // Set update iter
        auto current = aligner.cigars.end();
        // Init score
        _score = 0;
        sfor2(aligner.cigars, aligner.scores) {
            // Add score
            _score += $_2;
            // Check break condition
            if (($_1.option == scigar::MMATCH && _par->max_miss < $_1.length) ||
                ($_1.gap() && _par->max_gap < $_1.length)) break;
            else if ($_1.option == scigar::MATCH || $_1.option == scigar::PMATCH) {
                // Update extend point
                if (aligner.cigars.queSize(0, it.first - aligner.cigars.begin() + 1) * _par->ext_threshold < _score) {
                    current = it.first; _score = 0;
                }
            }
        }
        //
        if (current == aligner.cigars.end()) break;
        // Update aligned region
        if (al->ref.dir) al->ref.begin -= (int)aligner.cigars.refSize(current - aligner.cigars.begin() + 1);
        else al->ref.end += (int)aligner.cigars.refSize(0, current - aligner.cigars.begin() + 1);
        al->query.end += (int)aligner.cigars.queSize(0, current - aligner.cigars.begin() + 1);
        sforin(cig, aligner.cigars.begin(), current + 1) al->cigars.add(*cig, DIRECTION::TAIL);
    }
    al->scoring(&_par->apar);

    /*
    _deflimit(ref, que, al, DIRECTION::TAIL);
    while (((al->ref.dir && _rlimit < al->ref.begin)||
        (!al->ref.dir && al->ref.end + 1 < _rlimit)) && 
        al->query.end + 1 < _qlimit) {
        // Simple comparation
        _extend(ref, que, al, DIRECTION::TAIL);
        if (((al->ref.dir&& _rlimit == al->ref.begin) ||
            (!al->ref.dir&& al->ref.end == _rlimit - 1)) && 
            al->query.end == _qlimit - 1) break;
        // Alignment
        _update(ref, que, al, DIRECTION::TAIL);
        aligner.reset();
        aligner.align(_buffer.data(), _rlen, que->data(al->query.end + 1), _qlen, false);
        // Set update iter
        auto current = aligner.cigars.end();
        // Init score
        _score = 0;
        sfor2(aligner.cigars, aligner.scores) {
            // Add score
            _score += $_2;
            // Check break condition
            if ($_1.option == scigar::MMATCH && _par->max_miss < $_1.length ||
                $_1.gap() && _par->max_gap < $_1.length) {
                break;
            }
            else if ($_1.option == scigar::MATCH || $_1.option == scigar::PMATCH) {
                // Update extend point
                if (aligner.cigars.queSize(0, it.first - aligner.cigars.begin() + 1) * _par->ext_threshold < _score) {
                    current = it.first;
                    _score = 0;
                }
            }
        }
        //
        if (current == aligner.cigars.end()) break;
        // Update aligned region
        if (al->ref.dir) al->ref.begin -= (int)aligner.cigars.refSize(current - aligner.cigars.begin() + 1);
        else al->ref.end += (int)aligner.cigars.refSize(0, current - aligner.cigars.begin() + 1);
        al->query.end += (int)aligner.cigars.queSize(0, current - aligner.cigars.begin() + 1);
        sforin(cig, aligner.cigars.begin(), current + 1) al->cigars.add(*cig, DIRECTION::TAIL);
    }
    al->scoring(&_par->apar);
    */
}
void slib::sbio::AlignExtend::extendTailTo(Sequence* ref, ubytearray* que, salign* al, salign* lim) {
    if (al->query.end == que->size() - 1) return;
    _deflimit(ref, que, al, DIRECTION::TAIL);
    if (al->ref.dir == lim->ref.dir) {
        if (al->ref.dir) {
            if (_rlimit < lim->ref.end) _rlimit = lim->ref.end;
        }
        else {
            if (lim->ref.begin  < _rlimit) _rlimit = lim->ref.begin;
        }
    }
    if (lim->query.begin < _qlimit) _qlimit = lim->query.begin;
    //
    while (true) {
        _extend(ref, que, al, DIRECTION::TAIL);
        _update(ref, que, al, DIRECTION::TAIL);
        if (!_rlen || !_qlen) break;
        aligner.reset();
        aligner.align(_buffer.data(), _rlen, que->data(al->query.end + 1), _qlen, false);
        // Set update iter
        auto current = aligner.cigars.end();
        // Init score
        _score = 0;
        sfor2(aligner.cigars, aligner.scores) {
            // Add score
            _score += $_2;
            // Check break condition
            if (($_1.option == scigar::MMATCH && _par->max_miss < $_1.length) ||
                ($_1.gap() && _par->max_gap < $_1.length)) break;
            else if ($_1.option == scigar::MATCH || $_1.option == scigar::PMATCH) {
                // Update extend point
                if (aligner.cigars.queSize(0, it.first - aligner.cigars.begin() + 1) * _par->ext_threshold < _score) {
                    current = it.first; _score = 0;
                }
            }
        }
        //
        if (current == aligner.cigars.end()) break;
        // Update aligned region
        if (al->ref.dir) al->ref.begin -= (int)aligner.cigars.refSize(current - aligner.cigars.begin() + 1);
        else al->ref.end += (int)aligner.cigars.refSize(0, current - aligner.cigars.begin() + 1);
        al->query.end += (int)aligner.cigars.queSize(0, current - aligner.cigars.begin() + 1);
        sforin(cig, aligner.cigars.begin(), current + 1) al->cigars.add(*cig, DIRECTION::TAIL);
    }
    al->scoring(&_par->apar);

    /*
    _rlimit = (int)ref->length(); _qlimit = (int)que->size();
    while (al->ref.end + 1 < _rlimit && al->query.end + 1 < _qlimit) {
        // Simple comparation
        _extend(ref, que, al, DIRECTION::TAIL);
        if (al->ref.end == _rlimit - 1 && al->query.end == _qlimit - 1) break;
        // Alignment
        _update(ref, que, al, DIRECTION::TAIL);
        aligner.reset();
        aligner.align(_buffer.data(), _rlen, que->data(al->query.end + 1), _qlen, false);
        // Set update iter
        auto current = aligner.cigars.end();
        // Init score
        _score = 0;
        sfor2(aligner.cigars, aligner.scores) {
            // Add score
            _score += $_2;
            // Check break condition
            if ($_1.option == scigar::MMATCH && _par->max_miss < $_1.length ||
                $_1.gap() && _par->max_gap < $_1.length) {
                break;
            }
            else if ($_1.option == scigar::MATCH || $_1.option == scigar::PMATCH) {
                // Update extend point
                if (aligner.cigars.queSize(0, it.first - aligner.cigars.begin() + 1) * _par->ext_threshold < _score)
                    current = it.first;
            }
            if (it.first == aligner.cigars.end() - 1) {
                
            }
        }
        //
        if (current == aligner.cigars.end()) break;
        // Update aligned region
        al->ref.end += (int)aligner.cigars.refSize(0, current - aligner.cigars.begin() + 1);
        al->query.end += (int)aligner.cigars.queSize(0, current - aligner.cigars.begin() + 1);
        sforin(cig, aligner.cigars.begin(), current + 1) al->cigars.add(*cig, DIRECTION::TAIL);
    }
    */
}
void slib::sbio::AlignExtend::link(Sequence* ref, ubytearray* que, salign* al1, salign* al2) {
    extendHeadTo(ref, que, al2, al1);
    extendTailTo(ref, que, al1, al2);
    bool link = false;
    if (al1->ref.dir) _rlen = al1->ref.end - al2->ref.begin - 1;
    else _rlen = al2->ref.begin - al1->ref.end - 1;
    _qlen = al2->query.begin - al1->query.end - 1;

    if (!_qlen) {
        if (!_rlen) link = true;
        //DEL
        else if (0 < _rlen && _rlen <= _par->max_gap) {
            al1->cigars.add(Cigar(scigar::DELETION, _rlen), DIRECTION::TAIL);
            if (al1->ref.dir) al1->ref.begin -= _rlen;
            else al1->ref.end += _rlen;
            link = true;
        }
        //DUP
        else if (_rlen < 0 && (-_rlen) <= _par->max_gap && 
            (-_rlen) < al1->cigars[-1].length && al1->cigars[-1].option == scigar::PMATCH) {
            al1->cigars[-1].length -= (-_rlen);
            if (al1->ref.dir) al1->ref.begin += _rlen;
            else al1->ref.end -= _rlen;
            al1->cigars.add(Cigar(scigar::INSERTION, -_rlen), DIRECTION::TAIL);
            link = true;
        }
    }
    else {
        //INS
        if (!_rlen) {
            if (_qlen <= _par->max_gap) {
                al1->cigars.add(Cigar(scigar::INSERTION, _qlen), DIRECTION::TAIL);
                al1->query.end += _qlen;
                link = true;
            }
        }
        else {
            if (_rlen <= _par->apar.max_length && _qlen <= _par->apar.max_length) {
                _buffer.resize(_rlen);
                if (al1->ref.dir) {
                    _encoder(ref->data(), al1->ref.begin - _rlen, _rlen, _buffer.data());
                    slib::sbio::sna::toComplement16(_buffer);
                }
                else _encoder(ref->data(), al1->ref.end + 1, _rlen, _buffer.data());
                aligner.reset();
                aligner.align(_buffer.data(), _rlen, que->data(al1->query.end + 1), _qlen, false);
                link = true;
                sforeach(cigar, aligner.cigars) {
                    if ((cigar.gap() && _par->max_gap < cigar.length) ||
                        (cigar.option == scigar::MMATCH && _par->max_miss < cigar.length)) {
                        link = false; break;
                    }
                }
            }
        }
    }
    if (link) {
        if (al1->ref.dir) al1->ref.begin = al2->ref.begin;
        else al1->ref.end = al2->ref.end;
        al1->query.end = al2->query.end;
        al1->cigars.append(al2->cigars);
        al2->cigars.clear();
    }
}
void slib::sbio::AlignExtend::extend(Sequence* ref, ubytearray* que, salign* al) {
    extendHead(ref, que, al);
    extendTail(ref, que, al);
}
void slib::sbio::AlignExtend::extendExact(Sequence* ref, ubytearray* que, salign* al) {
    if (!al->query.begin) return;
    _deflimit(ref, que, al, DIRECTION::HEAD);
    _extend(ref, que, al, DIRECTION::HEAD);
    if (al->query.end == que->size() - 1) return;
    _deflimit(ref, que, al, DIRECTION::TAIL);
    _extend(ref, que, al, DIRECTION::TAIL);
    al->scoring(&_par->apar);
}
void slib::sbio::AlignExtend::setParam(SeqSearchParam* p) {
    _par = p; 
    aligner.setParam(&p->apar); 
    _maxlen = _par->apar.max_length;
    switch (p->reftype & 0xFF) {
    case sbio::sseq::DNA:
    {
        auto enc = p->reftype >> 8;
        if (enc == 1) _encoder = sbio::sseq::rawcopy;
        else if (enc == 2) _encoder = sbio::sdna::expand2;
        else if (enc == 4) _encoder = sbio::sdna::expand4;
        break;
    }
    default:
        _encoder = sbio::sseq::rawcopy;
        break;
    }
}
void slib::sbio::AlignExtend::reset() { 
    _rlimit = 0;
    _qlimit = 0;
    _rlen = 0;
    _qlen = 0;
    _score = 0.f;
    _buffer.clear();
    aligner.reset();
}

void slib::sbio::sutil::trimAlignHead(AlignPair& al, int rlim, int qlim) {
    if (rlim < 0 && qlim < 0) return;
    int len = (rlim < 0 ? (qlim - al.query.begin + 1) : (rlim - al.ref.begin + 1));
    bool mode = (qlim < 0 ? false : true);
    if (!mode && rlim < al.ref.begin) return;
    if (mode && qlim < al.query.begin) return;
    if (mode || (!mode && !al.ref.dir)) {
        auto current = al.cigars.begin();
        sfor(al.cigars) {
            if ($_.option == scigar::MATCH || $_.option == scigar::PMATCH) {
                if (len <= 0) break;
                // Trim matched region and break
                if (len < $_.length) {
                    $_.length -= len;
                    if (!al.ref.dir) al.ref.begin += len; else al.ref.end -= len;
                    al.query.begin += len;
                    break;
                }
                else {
                    len -= $_.length;
                    ++current;
                    if (!al.ref.dir) al.ref.begin += $_.length; else al.ref.end -= $_.length;
                    al.query.begin += $_.length;
                }
            }
            else {
                if ($_.option == scigar::MMATCH) {
                    len -= $_.length;
                    if (!al.ref.dir) al.ref.begin += $_.length; else al.ref.end -= $_.length;
                    al.query.begin += $_.length;
                }
                else if ($_.option == scigar::DELETION || $_.option == scigar::SKIP) {
                    if (!mode) len -= $_.length;
                    if (!al.ref.dir) al.ref.begin += $_.length; else al.ref.end -= $_.length;
                }
                else {
                    if (mode) len -= $_.length;
                    al.query.begin += $_.length;
                }
                ++current;
            }
        }
        // Remove cigars
        if (current != al.cigars.begin()) al.cigars.remove(0, current - al.cigars.begin());
    }
    else {
        auto current = al.cigars.end();
        srfor(al.cigars) {
            if ($_.option == scigar::MATCH || $_.option == scigar::PMATCH) {
                if (len <= 0) break;
                // Trim matched region and break
                if (len < $_.length) {
                    $_.length -= len; 
                    al.ref.begin += len; 
                    al.query.end -= len; break;
                }
                else {
                    len -= $_.length; --current;
                    al.ref.begin += $_.length; 
                    al.query.end -= $_.length;
                }
            }
            else {
                if ($_.option == scigar::MMATCH) {
                    len -= $_.length;
                    al.ref.begin += $_.length; 
                    al.query.end -= $_.length;
                }
                else if ($_.option == scigar::DELETION || $_.option == scigar::SKIP) {
                    len -= $_.length; al.ref.begin += $_.length;
                }
                else al.query.end -= $_.length;
                --current;
            }
        }
        // Remove cigars
        al.cigars.resize(current - al.cigars.begin());
    }
}
void slib::sbio::sutil::trimAlignTail(AlignPair& al, int rlim, int qlim) {
    if (rlim < 0 && qlim < 0) return;
    int len = (rlim < 0 ? (al.query.end - qlim + 1) : (al.ref.end - rlim + 1));
    bool mode = (qlim < 0 ? false : true);
    if (!mode && al.ref.end < rlim) return;
    if (mode && al.query.end < qlim) return;
    if (mode || (!mode && !al.ref.dir)) {
        auto current = al.cigars.end();
        srfor(al.cigars) {
            if ($_.option == scigar::MATCH || $_.option == scigar::PMATCH) {
                if (len <= 0) break;
                // Trim matched region and break
                if (len < $_.length) {
                    $_.length -= len;
                    if (!al.ref.dir) al.ref.end -= len; else al.ref.begin += len;
                    al.query.end -= len;
                    break;
                }
                else {
                    len -= $_.length; --current;
                    if (!al.ref.dir) al.ref.end -= len; else al.ref.begin += len;
                    al.query.end -= $_.length;
                }
            }
            else {
                if ($_.option == scigar::MMATCH) {
                    len -= $_.length;
                    if (!al.ref.dir) al.ref.end -= len; else al.ref.begin += len;
                    al.query.end -= $_.length;
                }
                else if ($_.option == scigar::DELETION || $_.option == scigar::SKIP) {
                    if (!mode) len -= $_.length;
                    if (!al.ref.dir) al.ref.end -= len; else al.ref.begin += len;
                }
                else {
                    if (mode) len -= $_.length;
                    al.query.end -= $_.length;
                }
                --current;
            }
        }
        // Remove cigars
        al.cigars.resize(current - al.cigars.begin());
    }
    else {
        auto current = al.cigars.begin();
        sfor(al.cigars) {
            if ($_.option == scigar::MATCH || $_.option == scigar::PMATCH) {
                if (len <= 0) break;
                // Trim matched region and break
                if (len < $_.length) {
                    $_.length -= len; 
                    al.ref.end -= len;
                    al.query.begin += len; 
                    break;
                }
                else {
                    len -= $_.length; ++current;
                    al.ref.end -= $_.length; 
                    al.query.begin += $_.length;
                }
            }
            else {
                if ($_.option == scigar::MMATCH) {
                    len -= $_.length;
                    al.ref.end -= $_.length; 
                    al.query.begin += $_.length;
                }
                else if ($_.option == scigar::DELETION || $_.option == scigar::SKIP) {
                    len -= $_.length; 
                    al.ref.end -= $_.length;
                }
                else al.query.begin += $_.length;
                ++current;
            }
        }
        // Remove cigars
        if (current != al.cigars.begin()) al.cigars.remove(0, current - al.cigars.begin());
    }
}



void slib::sbio::sutil::trimHeadTo(srange& ref, srange& que, CigarArray& cigs, int lim, bool rev) {
    if (lim < ref.begin) return;
    // overlap length
    auto len = lim - ref.begin + 1;
    if (!rev) {
        auto current = cigs.begin();
        sfor(cigs) {
            if ($_.option == scigar::MATCH || $_.option == scigar::PMATCH) {
                if (len <= 0) break;
                // Trim matched region and break
                if (len < $_.length) {
                    $_.length -= len; ref.begin += len; que.begin += len; break;
                }
                else {
                    len -= $_.length; ++current;
                    ref.begin += $_.length; que.begin += $_.length;
                }
            }
            else {
                if ($_.option == scigar::MMATCH) {
                    len -= $_.length;
                    ref.begin += $_.length; que.begin += $_.length;
                }
                else if ($_.option == scigar::DELETION || $_.option == scigar::SKIP) {
                    len -= $_.length; ref.begin += $_.length;
                }
                else que.begin += $_.length;
                ++current;
            }
        }
        // Remove cigars
        if (current != cigs.begin()) cigs.remove(0, current - cigs.begin());
    }
    else {
        auto current = cigs.end();
        srfor(cigs) {
            if ($_.option == scigar::MATCH || $_.option == scigar::PMATCH) {
                if (len <= 0) break;
                // Trim matched region and break
                if (len < $_.length) {
                    $_.length -= len; ref.begin += len; que.end -= len; break;
                }
                else {
                    len -= $_.length; --current;
                    ref.begin += $_.length; que.end -= $_.length;
                }
            }
            else {
                if ($_.option == scigar::MMATCH) {
                    len -= $_.length;
                    ref.begin += $_.length; que.end -= $_.length;
                }
                else if ($_.option == scigar::DELETION || $_.option == scigar::SKIP) {
                    len -= $_.length; ref.begin += $_.length;
                }
                else que.end -= $_.length;
                --current;
            }
        }
        // Remove cigars
        cigs.resize(current - cigs.begin());
    }
}
void slib::sbio::sutil::trimTailTo(srange& ref, srange& que, CigarArray& cigs, int lim, bool rev) {
    if (ref.end < lim) return;
    // overlap length
    auto len = ref.end - lim + 1;
    if (!rev) {
        auto current = cigs.end();
        srfor(cigs) {
            if ($_.option == scigar::MATCH || $_.option == scigar::PMATCH) {
                if (len <= 0) break;
                // Trim matched region and break
                if (len < $_.length) {
                    $_.length -= len; ref.end -= len; que.end -= len; break;
                }
                else {
                    len -= $_.length; --current;
                    ref.end -= $_.length; que.end -= $_.length;
                }
            }
            else {
                if ($_.option == scigar::MMATCH) {
                    len -= $_.length;
                    ref.end -= $_.length; que.end -= $_.length;
                }
                else if ($_.option == scigar::DELETION || $_.option == scigar::SKIP) {
                    len -= $_.length; ref.end -= $_.length;
                }
                else que.end -= $_.length;
                --current;
            }
        }
        // Remove cigars
        cigs.resize(current - cigs.begin());
    }
    else {
        auto current = cigs.begin();
        sfor(cigs) {
            if ($_.option == scigar::MATCH || $_.option == scigar::PMATCH) {
                if (len <= 0) break;
                // Trim matched region and break
                if (len < $_.length) {
                    $_.length -= len; ref.end -= len; que.begin += len; break;
                }
                else {
                    len -= $_.length; ++current;
                    ref.end -= $_.length; que.begin += $_.length;
                }
            }
            else {
                if ($_.option == scigar::MMATCH) {
                    len -= $_.length;
                    ref.end -= $_.length; que.begin += $_.length;
                }
                else if ($_.option == scigar::DELETION || $_.option == scigar::SKIP) {
                    len -= $_.length; ref.end -= $_.length;
                }
                else que.begin += $_.length;
                ++current;
            }
        }
        // Remove cigars
        if (current != cigs.begin()) cigs.remove(0, current - cigs.begin());
    }
}
void slib::sbio::sutil::trimOver(srange& ref1, srange& que1, CigarArray& cigs1, srange& ref2, srange& que2, CigarArray& cigs2, bool rev) {
    if (!rev) {
        if (ref1.end < ref2.begin) return;
        // overlap length
        auto len = ref1.end - ref2.begin + 1;
        auto current = cigs1.end();
        srfor(cigs1) {
            if ($_.option == scigar::MATCH || $_.option == scigar::PMATCH) {
                if (len <= 0) break;
                // Trim matched region and break
                if (len < $_.length) {
                    $_.length -= len; ref1.end -= len; que1.end -= len; break;
                }
                else {
                    len -= $_.length; --current;
                    ref1.end -= $_.length; que1.end -= $_.length;
                }
            }
            else {
                if ($_.option == scigar::MMATCH) {
                    len -= $_.length;
                    ref1.end -= $_.length; que1.end -= $_.length;
                }
                else if ($_.option == scigar::DELETION || $_.option == scigar::SKIP) {
                    len -= $_.length; ref1.end -= $_.length;
                }
                else que1.end -= $_.length;
                --current;
            }
        }
        // Remove cigars
        cigs1.resize(current - cigs1.begin());
    }
    else {
        if (ref2.end < ref1.begin) return;
        // overlap length
        auto len = ref2.end - ref1.begin + 1;
        auto current = cigs2.begin();
        sfor(cigs2) {
            if ($_.option == scigar::MATCH || $_.option == scigar::PMATCH) {
                if (len <= 0) break;
                // Trim matched region and break
                if (len < $_.length) {
                    $_.length -= len; ref2.end -= len; que2.begin += len; break;
                }
                else {
                    len -= $_.length; ++current;
                    ref2.end -= $_.length; que2.begin += $_.length;
                }
            }
            else {
                if ($_.option == scigar::MMATCH) {
                    len -= $_.length;
                    ref2.end -= $_.length; que2.begin += $_.length;
                }
                else if ($_.option == scigar::DELETION || $_.option == scigar::SKIP) {
                    len -= $_.length; ref2.end -= $_.length;
                }
                else que2.begin += $_.length;
                ++current;
            }
        }
        // Remove cigars
        if (current != cigs2.begin()) cigs2.remove(0, current - cigs2.begin());
    }
}

