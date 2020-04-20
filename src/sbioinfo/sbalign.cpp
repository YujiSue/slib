#include "sbioinfo/sbalign.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::sbio;

inline void dnascore(matb &table, sbyte pm, sbyte am, sbyte mm, sbyte gp, sbyte gs) {
    table = matb(16, 16, {
        ///// -  A  C  M  G  R  S  V  T  W  Y  H  K  D  B  N
        /*-*/gs,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,gp,
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
inline void rnascore(matb &table, sbyte pm, sbyte am, sbyte mm, sbyte gp, sbyte gs) {
    table = matb(16, 16, {
        ///// -  A  C  G  T  N
        /*-*/gs,gp,gp,gp,gp,gp,
        /*A*/gp,pm,mm,mm,mm,am,
        /*C*/gp,mm,pm,mm,mm,am,
        /*G*/gp,mm,mm,pm,mm,am,
        /*T*/gp,mm,mm,mm,pm,am,
        /*N*/gp,am,am,am,am,am
    });
}
inline void aascore(matb &table, const SString &name) {
    table = matb(26, 26);
    if (name.beginWith("BROSSUM")) {
		/*
		*/
    }
}
inline void dnacompare(matb &table) {
    table = matb(16, 16, {
        /////N A C M G R S V T W Y H K D B N
        /*N*/7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
        /*A*/0,7,8,0,8,0,8,0,8,0,8,0,8,0,8,0,
        /*C*/0,8,7,0,8,8,0,0,8,8,0,0,8,8,0,0,
        /*M*/0,0,0,0,8,8,8,8,8,8,8,8,8,8,8,0,
        /*G*/0,8,8,8,7,0,0,0,8,8,8,8,0,0,0,0,
        /*R*/0,0,8,8,0,0,8,8,8,8,8,8,8,8,8,0,
        /*S*/0,8,0,8,0,8,0,8,8,8,8,8,8,8,8,0,
        /*V*/0,0,0,8,0,8,8,0,8,8,8,8,8,8,8,0,
        /*T*/0,8,8,8,8,8,8,8,7,0,0,0,0,0,0,0,
        /*W*/0,0,8,8,8,8,8,8,0,0,8,8,8,8,8,0,
        /*Y*/0,8,0,8,8,8,8,8,0,8,0,8,8,8,8,0,
        /*H*/0,0,0,8,8,8,8,8,0,8,8,0,8,8,8,0,
        /*K*/0,8,8,8,0,8,8,8,0,8,8,8,0,8,8,0,
        /*D*/0,0,8,8,0,8,8,8,0,8,8,8,8,0,8,0,
        /*B*/0,8,0,8,0,8,8,8,0,8,8,8,8,8,0,0,
        /*N*/0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7
    });
}
salign_param::salign_param(subyte t, const char* method) {
    seq_type = t;
	align_length = DEFAULT_ALIGN_LENGTH;
    pm_score = DEFAULT_PM_SCORE;
    am_score = DEFAULT_AM_SCORE;
    mm_score = DEFAULT_MM_SCORE;
    gap_score = DEFAULT_GAP_SCORE;
    gap2_score = DEFAULT_GAP2_SCORE;
    score_name = "";
    makeTable();
}
salign_param::~salign_param() {}
salign_param &salign_param::operator=(const salign_param &par) {
    seq_type = par.seq_type;
    align_length = par.align_length;
    pm_score = par.pm_score;
    am_score = par.am_score;
    mm_score = par.mm_score;
    gap_score = par.gap_score;
    gap2_score = par.gap2_score;
    score_name = par.score_name;
    score_table = par.score_table;
    compare_table = par.compare_table;
    return *this;
}
void salign_param::set(const sobj &obj) {
    if (obj["type"]) seq_type = obj["type"];
    if (obj["length"]) align_length = obj["length"];
    if (!obj["pmscore"].isNull()) pm_score = obj["pmscore"];
    if (!obj["amscore"].isNull()) am_score = obj["amscore"];
    if (!obj["mmscore"].isNull()) mm_score = obj["mmscore"];
    if (!obj["gapscore"].isNull()) gap_score = obj["gapscore"];
    if (!obj["gap2score"].isNull()) gap2_score = obj["gap2score"];
    if (obj["score"]) score_name = obj["score"];
    makeTable();
}
sobj salign_param::toObj() {
    return { kv("type", seq_type),
        kv("length", align_length),
        kv("pmscore", pm_score),
        kv("amscore", am_score),
        kv("mmscore", mm_score),
        kv("gapscore", gap_score),
        kv("gap2score", gap2_score),
        kv("score", score_name)
    };
}
void salign_param::makeTable() {
    switch (seq_type) {
        case DNA_SEQ:
            dnacompare(compare_table);
            dnascore(score_table, pm_score, am_score, mm_score, gap_score, gap2_score);
            break;
        case RNA_SEQ:
            rnascore(score_table, pm_score, am_score, mm_score, gap_score, gap2_score);
            break;
        case AA_SEQ:
            aascore(score_table, score_name);
            break;
        default:
            break;
    }
}

salign::salign() : score(0) {}
salign::salign(const sbpos &pos, const srange &range) : ref(pos), aligned(range), score(0) {}
salign::salign(const salign &align) {
    ref = align.ref;
    aligned = align.aligned;
    score = align.score;
    cigars = align.cigars;
}
salign::~salign() {}
salign &salign::operator = (const salign &align) {
    ref = align.ref;
    aligned = align.aligned;
    score = align.score;
    cigars = align.cigars;
    return (*this);
}
bool salign::operator < (const salign &align) const {
    if (ref != align.ref) return ref < align.ref;
    if (aligned != align.aligned) return aligned < align.aligned;
    return false;
}
bool salign::operator == (const salign &align) const {
    return ref == align.ref && aligned == align.aligned && cigars == align.cigars;
}
void salign::scoring(salign_param *par) {
    score = 0;
    sforeach(cigars) {
		if (E_.option == scigar::PMATCH) score += E_.length * par->pm_score;
		else if (E_.option == scigar::MATCH) score += E_.length * par->am_score;
		else if (E_.option == scigar::MMATCH) score += E_.length * par->mm_score;
        else score += par->gap_score+(E_.length-1)*par->gap2_score;
    }
}
void salign::init() {
    ref.init();
    aligned = srange();
    score = 0;
    cigars.clear();
}
String salign::alref(const String &ref) {
    String str;
    int offset = 0;
    sforeach(cigars) {
        if(E_.option == scigar::INSERTION) { sforin(i, 0, E_.length) str += "-"; }
        else {
            str += ref.substring(offset, E_.length);
            offset += E_.length;
        }
    }
    return str;
}
String salign::match() {
    String str = "";
    sforeach(cigars) {
        if(E_.option == scigar::PMATCH) { sforin(i, 0, E_.length) str += "*"; }
        else if(E_.option == scigar::MATCH) { sforin(i, 0, E_.length) str += "+"; }
        else if(E_.option == scigar::MMATCH) { sforin(i, 0, E_.length) str += "."; }
        else { sforin(i, 0, E_.length) str += " "; }
    }
    return str;
}
String consensus(const String &ref, const String &que);

String slib::sbio::salign::alque(const String &que) {
    String str = "";
    int offset = 0;
    sforeach(cigars) {
        if(E_.option == scigar::DELETION) { sforin(i, 0, E_.length) str += "-"; }
        else {
            str += que.substring(offset, E_.length);
            offset += E_.length;
        }
    }
    return str;
}
inline int aldir(sint *score) {
    int idx = 0;
    sforin(i, 1, 4) { if(score[idx] < score[i]) idx = i; }
    return idx;
}
SAlignment::SAlignment() : _par(nullptr) {}
SAlignment::SAlignment(salign_param *p) : SAlignment() { set(p); }
SAlignment::~SAlignment() {}
void SAlignment::align(subyte *ref, size_t rlen, subyte *que, size_t qlen) {
    reset(); if (!rlen || !qlen) return;
    auto r_ = ref, q_ = que;
    auto col = qlen+1;
    auto p = _path.ptr(col+1);
    auto _s = _score.ptr(), s = _score.ptr(col+1), s2 = _score2.ptr(col+1), sr = _score.ptr(col), sc = _score.ptr(1);
    auto mr = _maxrow.ptr(1), mc = _maxcol.ptr(1);
    sforin(i, 1, rlen+1) {
        q_ = que;
        mr = _maxrow.ptr(1);
        sc = _score.ptr((*mr)*col);
        sforin(j, 1, qlen+1) {
            _scr[0] = 0; *s2 = _par->score_table[*r_][*q_];
            _scr[1] = (*_s)+(*s2);
            _scr[2] = sc[j]+_par->gap_score+_par->gap2_score*(i-(*mr)-1);
            _scr[3] = sr[*mc]+_par->gap_score+_par->gap2_score*(j-(*mc)-1);
            int idx = aldir(_scr);
            *s = _scr[idx];
            if (idx == 2) *p = scigar::DELETION;
            else if (idx == 3) *p = scigar::INSERTION;
            else *p = _par->compare_table[*r_][*q_];
            if (sc[j] < *s) *mr = i;
            if (sr[*mc] < *s) *mc = j;
            ++q_; ++p; ++s; ++s2; ++_s; ++mr; sc = _score.ptr((*mr)*col);
        }
        ++r_; ++p; ++s; ++s2; ++_s; ++mc; sr += col;
    }
    p -= 2; s -= 2; s2 -= 2;
    bool _gap = false;
    while (0 < rlen && 0 < qlen) {
        cigars.put(scigar(*p, 1));
        if (!(*p) || scigar::PADDING < *p) {
            scores.add((scores.empty()?0:scores.last())+(*s2));
            p -= col+1; s -= col+1; s2 -= col+1; --rlen; --qlen;
            _gap = false;
        }
        else {
            if (_gap) scores.add((scores.empty()?0:scores.last())+_par->gap2_score);
            else scores.add((scores.empty()?0:scores.last())+_par->gap_score);
            if (*p == scigar::DELETION) {
                p -= col; s -= col; s2 -= col; --rlen;
            }
            else { --p; --s; --s2; --qlen; }
            _gap = true;
        }
    }
    if (0 < rlen) {
        cigars.put(scigar(scigar::DELETION, rlen));
        if (_gap) { sforin(l, 0, rlen) scores.add(scores.last()+_par->gap2_score); }
        else {
            scores.add(scores.last()+_par->gap_score);
            if (1 < rlen) { sforin(l, 1, rlen) scores.add(scores.last()+_par->gap2_score); }
        }
    }
    else if (0 < qlen) {
        cigars.put(scigar(scigar::INSERTION, qlen));
        if (_gap) { sforin(l, 0, qlen) scores.add(scores.last()+_par->gap2_score); }
        else {
            scores.add(scores.last()+_par->gap_score);
            if (1 < qlen) { sforin(l, 1, qlen) scores.add(scores.last()+_par->gap2_score); }
        }
    }
    auto beg = scores.begin(), end = scores.end()-1;
    while (beg < end) {
        auto tmp = *beg; *beg = *end; *end = tmp; ++beg; --end;
    }
}
void SAlignment::ralign(subyte *ref, size_t rlen, subyte *que, size_t qlen) {
    reset(); if (!rlen || !qlen) return;
    auto r_ = ref+rlen-1, q_ = que+qlen-1;
    auto col = qlen+1;
    auto p = _path.ptr(col+1);
    auto _s = _score.ptr(), s = _score.ptr(col+1), s2 = _score2.ptr(col+1), sr = _score.ptr(col), sc = _score.ptr(1);
    auto mr = _maxrow.ptr(1), mc = _maxcol.ptr(1);
    sforin(i, 1, rlen+1) {
        q_ = que+qlen-1;
        mr = _maxrow.ptr(1);
        sc = _score.ptr((*mr)*col);
        sforin(j, 1, qlen+1) {
            _scr[0] = 0; *s2 = _par->score_table[*r_][*q_];
            _scr[1] = (*_s)+(*s2);
            _scr[2] = sc[j]+_par->gap_score+_par->gap2_score*(i-(*mr)-1);
            _scr[3] = sr[*mc]+_par->gap_score+_par->gap2_score*(j-(*mc)-1);
            int idx = aldir(_scr);
            *s = _scr[idx];
            if (idx == 2) *p = scigar::DELETION;
            else if (idx == 3) *p = scigar::INSERTION;
            else *p = _par->compare_table[*r_][*q_];
            if (sc[j] < *s) *mr = i;
            if (sr[*mc] < *s) *mc = j;
            --q_; ++p; ++s; ++s2; ++_s; ++mr; sc = _score.ptr((*mr)*col);
        }
        --r_; ++p; ++s; ++s2; ++_s; ++mc; sr += col;
    }
    p -= 2; s -= 2; s2 -= 2;
    bool _gap = false;
    while (0 < rlen && 0 < qlen) {
        cigars.add(scigar(*p, 1));
        if (!(*p) || scigar::PADDING < *p) {
            scores.add((scores.empty()?0:scores.last())+(*s2));
            p -= col+1; s -= col+1; s2 -= col+1; --rlen; --qlen;
            _gap = false;
        }
        else {
            if (_gap) scores.add(scores.last()+_par->gap2_score);
            else scores.add((scores.empty()?0:scores.last())+_par->gap_score);
            if (*p == scigar::DELETION) {
                p -= col; s -= col; s2 -= col; --rlen;
            }
            else { --p; --s; --s2; --qlen; }
            _gap = true;
        }
    }
    if (0 < rlen) {
        cigars.add(scigar(scigar::DELETION, rlen));
        if (_gap) { sforin(l, 0, rlen) scores.add(scores.last()+_par->gap2_score); }
        else {
            scores.add(scores.last()+_par->gap_score);
            if (1 < rlen) { sforin(l, 1, rlen) scores.add(scores.last()+_par->gap2_score); }
        }
    }
    else if (0 < qlen) {
        cigars.add(scigar(scigar::INSERTION, qlen));
        if (_gap) { sforin(l, 0, qlen) scores.add(scores.last()+_par->gap2_score); }
        else {
            scores.add(scores.last()+_par->gap_score);
            if (1 < qlen) { sforin(l, 1, qlen) scores.add(scores.last()+_par->gap2_score); }
        }
    }
}
void SAlignment::set(salign_param *p) {
    _par = p;
    cigars.reserve(p->align_length, false);
    if (_par->score_table.empty()) _par->makeTable();
    init(); reset();
}
void SAlignment::init() {
    if(!_par) throw SException(ERR_INFO, SLIB_NULL_ERROR, "_par");
    _maxcol.resize(_par->align_length+1);
    _maxrow.resize(_par->align_length+1);
    _score.resize((_par->align_length+1)*(_par->align_length+1));
    _score2.resize((_par->align_length+1)*(_par->align_length+1));
    _path.resize((_par->align_length+1)*(_par->align_length+1));
}
void SAlignment::reset() {
    _maxcol.reset(0);
    _maxrow.reset(0);
    _score.reset(0);
    _score2.reset(0);
    _path.reset(0);
    scores.clear();
    cigars.clear();
}