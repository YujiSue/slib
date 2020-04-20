#include "sbioinfo/sbannot.h"

using namespace slib;
using namespace slib::sbio;

inline String RANGE_CONDITION(const sbpos &pos) {
    return sql::condition(String("CHROMOSOME=")<<pos.idx<<
                    " AND START<="<<pos.end<<
                    " AND END>="<<pos.begin);
}
inline String NAME_CONDITION(const String &s) {
    return sql::condition("NAME LIKE", s);
}
const String ID_ASC_QUE = sql::orderQue({ std::pair<String, ORDER>("ID", ASC) });
const String START_ASC_QUE = sql::orderQue({ std::pair<String, ORDER>("START", ASC) });
const String POS_ASC_QUE = sql::orderQue({ std::pair<String, ORDER>("CHROMOSOME", ASC), std::pair<String, ORDER>("START", ASC) });

annot_info::annot_info() : sbpos(), _id(0), type(0) {}
annot_info::annot_info(const suint&t, const String &n, const sbpos &p) : type(t), name(n), sbpos(p) {}
annot_info::annot_info(const annot_info &info) : _id(info._id), type(info.type), name(info.name), sbpos(info) {}
annot_info::~annot_info() {}
chr_info::chr_info() : annot_info() {}
chr_info::chr_info(const char *s, const sbpos &pos) : annot_info() {}
chr_info::chr_info(const chr_info &c) : annot_info(c) {}
chr_info::~chr_info() {}
chr_info &chr_info::operator=(const chr_info &c) {
    idx = c.idx; begin = c.begin; end = c.end; dir = c.dir;
    _id = c._id; type = c.type; name = c.name; return *this;
}
contig_info::contig_info() : annot_info() {}
contig_info::contig_info(const contig_info &c) : annot_info(c) {}
contig_info::~contig_info() {}
contig_info &contig_info::operator=(const contig_info &c) {
    idx = c.idx; begin = c.begin; end = c.end; dir = c.dir;
    _id = c._id; type = c.type; name = c.name; return *this;
}
struct_info::struct_info() : annot_info() {}
struct_info::struct_info(const struct_info &s) : annot_info(s) {}
struct_info::~struct_info() {}
struct_info &struct_info::operator=(const struct_info &s) {
    idx = s.idx; begin = s.begin; end = s.end; dir = s.dir;
    _id = s._id; type = s.type; name = s.name; return *this;
}
transcript_info::transcript_info() : annot_info(), gene(nullptr) {}
transcript_info::transcript_info(const transcript_info &t) : annot_info(t) {
    structures = t.structures;
    gene = t.gene;
}
transcript_info::~transcript_info() {}
transcript_info &transcript_info::operator=(const transcript_info &t) {
    idx = t.idx; begin = t.begin; end = t.end; dir = t.dir;
    _id = t._id; type = t.type; name = t.name; structures = t.structures; gene = t.gene; return *this;
}
void transcript_info::addStructure(struct_info &&s) { structures.add(s); }
void transcript_info::setGene(gene_info *g) { gene = g; }
sregion transcript_info::exonRegion() {
    sregion region;
    sforeach(structures) { if(E_.type==EXON) region.add(E_); }
    return region;
}
sregion transcript_info::codingRegion() {
    sregion region;
    sforeach(structures) { if(E_.type==CDS) region.add(E_); }
    return region;
}
Array<struct_info> transcript_info::messenger() {
    Array<struct_info> region;
    sforeach(structures) { if(E_.type==CDS || E_.type&UTR) region.add(E_); }
    return region;
}
gene_info::gene_info() : annot_info() {}
gene_info::gene_info(const gene_info &g) : annot_info(g) {
    gene_id = g.gene_id;
    description = g.description;
    other_names = g.other_names;
    transcripts = g.transcripts;
    attribute = g.attribute;
}
gene_info::~gene_info() {}
gene_info &gene_info::operator=(const gene_info &g) {
    idx = g.idx; begin = g.begin; end = g.end; dir = g.dir;
    _id = g._id; type = g.type; name = g.name; gene_id = g.gene_id; description = g.description;
    other_names = g.other_names; transcripts = g.transcripts; attribute = g.attribute;
    return *this;
}
void gene_info::setDescription(String *str) { description = *str; }
void gene_info::addTranscript(transcript_info *t) { transcripts.add(t); }
mut_info::mut_info() : annot_info() {}
mut_info::mut_info(const mut_info &v) : annot_info(v) {
    mut_id = v.mut_id;
    strain = v.strain;
    attribute = v.attribute;
}
mut_info::~mut_info() {}
mut_info &mut_info::operator=(const mut_info &v) {
    idx = v.idx; begin = v.begin; end = v.end; dir = v.dir;
    _id = v._id; type = v.type; name = v.name; mut_id = v.mut_id; strain = v.strain;
    attribute = v.attribute; return *this;
}
feature_info::feature_info() : annot_info() {}
feature_info::feature_info(const feature_info &f) : annot_info(f) {}
feature_info::~feature_info() {}
feature_info &feature_info::operator=(const feature_info &f) {
    idx = f.idx; begin = f.begin; end = f.end; dir = f.dir;
    _id = f._id; type = f.type; name = f.name; return *this;
}

transcript_site::transcript_site() : type(0), site(0), pos(0) {}
transcript_site::transcript_site(transcript_info* ti) : transcript_site() {
	name = ti->name;
}
transcript_site::transcript_site(const transcript_site& trs) {
	type = trs.type; site = trs.site; name = trs.name;
	ori = trs.ori; alt = trs.alt; pos = trs.pos;
}
transcript_site::~transcript_site() {}
transcript_site& transcript_site::operator=(const transcript_site& trs) {
	type = trs.type; site = trs.site; name = trs.name;
	ori = trs.ori; 	alt = trs.alt; pos = trs.pos; return *this;
}

gene_site::gene_site() : type(0), dir(false) {}
gene_site::gene_site(const gene_info* gi) : gene_site() {
	name = gi->name; dir = gi->dir;
	transcripts.reserve(gi->transcripts.size() + 1);
}
gene_site::gene_site(const gene_site& g) {
	type = g.type; name = g.name; dir = g.dir;
	transcripts = g.transcripts;
}
gene_site::~gene_site() {}
gene_site& gene_site::operator=(const gene_site& g) {
	type = g.type; name = g.name; dir = g.dir;
	transcripts = g.transcripts;

	return *this;
}

void SBAnnotDB::_initIdx() {
    auto num = chrNum();
    bin_order.resize(num);
    _ctg_index.resize(num);
    _gene_index.resize(num);
    _trs_index.resize(num);
    _mut_index.resize(num);
    _var_index.resize(num);
    _ftr_index.resize(num);
    sforin(i, 0, num) {
        auto count = SBIUtil::countBin(bin_order[i], srange(chrInfo(i).begin, chrInfo(i).end-1));
        _ctg_index[i].resize(count);
        _gene_index[i].resize(count);
        _trs_index[i].resize(count);
        _mut_index[i].resize(count);
        _var_index[i].resize(count);
        _ftr_index[i].resize(count);
    }
}
inline void toAnnotInfo(annot_info *info, SDictionary &dict) {
    if(dict["ID"]) info->_id = dict["ID"];
    if(dict["TYPE"]) info->type = dict["TYPE"];
    if(dict["NAME"]) info->name = dict["NAME"];
    info->idx = dict["CHROMOSOME"];
    info->begin = dict["START"];
    info->end = dict["END"];
    if(dict["STRAND"]) info->dir = (dict["STRAND"]=="-");
}
void SBAnnotDB::_loadChrInfo() {
    chromosomes.clear();
    try {
        chromosomes.resize((*this)["CHROMOSOME"].count());
        (*this)["CHROMOSOME"].getRecordPrepare();
        sforeachi(chromosomes) {
            auto &row = getRow();
            _chr_index[row["NAME"]] = i;
            chromosomes[i].name = row["NAME"];
            chromosomes[i].idx = row["ID"];
            chromosomes[i].begin = 1;
            chromosomes[i].end = row["LENGTH"];
        }
    } catch (SDBException de) {
		de.print();
    }
}
void SBAnnotDB::_loadContigInfo() {
    contigs.clear();
    int r = 0;
    try {
        contigs.resize((*this)["CONTIG"].count());
        if (contigs.empty()) return;
        (*this)["CONTIG"].getRecordPrepare({ "*" }, nullptr, POS_ASC_QUE);
        sforeach(contigs) {
            auto &row = getRow();
            toAnnotInfo(&E_, row);
            _ctg_index[E_.idx][bin_order[E_.idx][SBIUtil::getBin(E_)]].add(&E_);
            _ctg_name_index.add(r);
            ++r;
        }
        commit();
        std::sort(_ctg_name_index.begin(), _ctg_name_index.end(),
                  [this](const int32_t &i1, const int32_t &i2) {
                      return contigs[i1].name < contigs[i2].name;
                  });
    } catch (SDBException de) {
		de.print();
    }
}
inline void toGeneInfo(gene_info *info, SDictionary &dict) {
    toAnnotInfo(info, dict);
    if(dict["GENE_ID"]) info->gene_id = dict["GENE_ID"];
    if(dict["OTHER_NAME"]) {
        auto list = dict["OTHER_NAME"].split(",");
        if (!list.empty()) { sforeach(list) info->other_names.add(E_.cstr()); }
    }
    if(dict["DESCRIPTION"]) info->description = dict["DESCRIPTION"];
    if(dict["ATTRIBUTE"]) info->attribute = dict["ATTRIBUTE"].parse(";", "=");
}
void SBAnnotDB::_loadGeneInfo() {
    genes.clear();
    int r = 0;
    try {
        genes.resize((*this)["GENE"].count());
        if (genes.empty()) return;
        (*this)["GENE"].getRecordPrepare({"*"}, nullptr, ID_ASC_QUE);
        sforeach(genes) {
            auto &row = getRow();
            toGeneInfo(&E_, row);
            if ((_mode&LOAD_TRANS) && !transcripts.empty() && row["TRANSCRIPT"]) {
                auto translist = row["TRANSCRIPT"].split(",");
                sforeachi(translist) {
                    auto &trans = transcripts[translist[i].intValue()-1];
                    E_.transcripts.add(&trans);
                    trans.gene = &E_;
                }
            }
            _gene_index[E_.idx][bin_order[E_.idx][SBIUtil::getBin(E_)]].add(&E_);
            if (E_.gene_id.size()) _gene_name_index.add(name_pair(&E_.gene_id, r));
            if (E_.name.size()) _gene_name_index.add(name_pair(&E_.name, r));
            if (!E_.other_names.empty()) {
                sforeach_(oit, E_.other_names) _gene_name_index.add(name_pair(&(*oit), r));
            }
            ++r;
        }
        commit();
        std::sort(_gene_name_index.begin(), _gene_name_index.end(),
                  [](const name_pair &n1, const name_pair &n2) {
                      return *n1.first < *n2.first;
                  });
    } catch (SDBException de) {
		de.print();
    }
}
void SBAnnotDB::_loadTranscriptInfo() {
    transcripts.clear();
    int r = 0;
    try {
        transcripts.resize((*this)["TRANSCRIPT"].count());
        if (transcripts.empty()) return;
        (*this)["TRANSCRIPT"].getRecordPrepare({ "*" }, nullptr, ID_ASC_QUE);
        sforeach(transcripts) {
            auto &row = getRow();
            toAnnotInfo(&E_, row);
            if ((_mode&LOAD_GENE) && !genes.empty() && row["GENE_ID"]) {
                auto &gene = genes[row["GENE_ID"]-1];
                E_.gene = &gene;
                gene.transcripts.add(&E_);
            }
            _trs_index[E_.idx][bin_order[E_.idx][SBIUtil::getBin(E_)]].add(&E_);
            _trs_name_index.add(r);
            ++r;
        }
        commit();
        (*this)["STRUCTURE"].getRecordPrepare({ "*" }, nullptr, START_ASC_QUE);
        while (true) {
            auto &row = getRow();
            if (row.empty()) break;
            struct_info si;
            toAnnotInfo(&si, row);
            stringarray tids = row["TRANSCRIPT_ID"].split(",");
            sforeach(tids) transcripts[E_.intValue()-1].structures.add(si);
        }
        commit();
        std::sort(_trs_name_index.begin(), _trs_name_index.end(),
                  [this](const int32_t &i1, const int32_t &i2) {
                      return transcripts[i1].name < transcripts[i2].name;
                  });
    } catch (SDBException de) {
		de.print();
    }
}
inline void toMutInfo(mut_info *info, SDictionary &dict) {
    toAnnotInfo(info, dict);
    if(dict["VAR_ID"]) info->mut_id = dict["VAR_ID"];
    if(dict["MUT_ID"]) info->mut_id = dict["MUT_ID"];
    if(dict["STRAIN"]) info->strain = dict["STRAIN"];
    if(!dict["ATTRIBUTE"].isNull()) info->attribute = dict["ATTRIBUTE"].parse(";", "=");
}
void SBAnnotDB::_loadMutantInfo() {
    mutants.clear();
    int r = 0;
    try {
        mutants.resize((*this)["MUTATION"].count());
        if (mutants.empty()) return;
        (*this)["MUTATION"].getRecordPrepare({ "*" }, nullptr, POS_ASC_QUE);
        sforeach(mutants) {
            auto &row = getRow();
			toMutInfo(&E_, row);
            _mut_index[E_.idx][bin_order[E_.idx][SBIUtil::getBin(E_)]].add(&E_);
            _mut_name_index.add(r);
            ++r;
        }
        commit();
        std::sort(_mut_name_index.begin(), _mut_name_index.end(),
                  [this](const int32_t &i1, const int32_t &i2) {
                      return mutants[i1].name < mutants[i2].name;
                  });
    } catch (SDBException de) {
		de.print();
    }
}
void SBAnnotDB::_loadVariationInfo() {
    variations.clear();
    int r = 0;
    try {
        variations.resize((*this)["VARIATION"].count());
        if (variations.empty()) return;
        (*this)["VARIATION"].getRecordPrepare({ "*" }, nullptr, POS_ASC_QUE);
        sforeach(variations) {
            auto &row = getRow();
			toMutInfo(&E_, row);
            _var_index[E_.idx][bin_order[E_.idx][SBIUtil::getBin(E_)]].add(&E_);
            _var_name_index.add(r);
            ++r;
        }
        commit();
        std::sort(_var_name_index.begin(), _var_name_index.end(),
                  [this](const int32_t &i1, const int32_t &i2) {
                      return variations[i1].name < variations[i2].name;
                  });
    } catch (SDBException de) {
		de.print();
    }
}
void SBAnnotDB::_loadFeatureInfo() {
    features.clear();
    int r = 0;
    try {
        features.resize((*this)["FEATURE"].count());
        if (features.empty()) return;
        (*this)["FEATURE"].getRecordPrepare({ "*" }, nullptr, POS_ASC_QUE);
        sforeach(features) {
            auto &row = getRow();
            toAnnotInfo(&E_, row);
            _ftr_index[E_.idx][bin_order[E_.idx][SBIUtil::getBin(E_)]].add(&E_);
            _ftr_name_index.add(r);
            ++r;
        }
        commit();
        std::sort(_ftr_name_index.begin(), _ftr_name_index.end(),
                  [this](const int32_t &i1, const int32_t &i2) {
                      return features[i1].name < features[i2].name;
                  });
    } catch (SDBException de) {
		de.print();
    }
}

SBAnnotDB::SBAnnotDB() : SDataBase(), _mode(0) {}
SBAnnotDB::SBAnnotDB(const char *path, int m) : SDataBase() {
    open(path); load(m);
}
SBAnnotDB::~SBAnnotDB() {}
void SBAnnotDB::open(const char *path) {
    SDataBase::open(path);
    _loadChrInfo();
}
void SBAnnotDB::load(int m) {
    _mode = m;
    _initIdx();
    if (_mode&LOAD_CTG) _loadContigInfo();
    if (_mode&LOAD_GENE) _loadGeneInfo();
    if (_mode&LOAD_TRANS) _loadTranscriptInfo();
    if (_mode&LOAD_MUT) _loadMutantInfo();
    if (_mode&LOAD_VAR) _loadVariationInfo();
    if (_mode&LOAD_FTR) _loadFeatureInfo();
}
int SBAnnotDB::mode() const { return _mode; }
void SBAnnotDB::setMode(int m) {
    if (m&LOAD_CTG && !(_mode&LOAD_CTG)) _loadContigInfo();
    if (_mode&LOAD_CTG && !(m&LOAD_CTG)) {
        contigs.clear();
        sforeachi(_ctg_index) {
            sforeachi_(j, _ctg_index[i])  _ctg_index[i][j].clear();
        }
        _ctg_name_index.clear();
    }
    if (m&LOAD_GENE && !(_mode&LOAD_GENE)) _loadGeneInfo();
    if (_mode&LOAD_GENE && !(m&LOAD_GENE)) {
        genes.clear();
        sforeachi(_gene_index) {
            sforeachi_(j, _gene_index[i]) _gene_index[i][j].clear();
        }
        _gene_name_index.clear();
    }
    if (m&LOAD_TRANS && !(_mode&LOAD_TRANS)) _loadTranscriptInfo();
    if (_mode&LOAD_TRANS && !(m&LOAD_TRANS)) {
        transcripts.clear();
        sforeachi(_trs_index) {
            sforeachi_(j, _trs_index[i])  _trs_index[i][j].clear();
        }
        _trs_name_index.clear();
    }
    if (m&LOAD_MUT && !(_mode&LOAD_MUT)) _loadMutantInfo();
    if (_mode&LOAD_MUT && !(m&LOAD_MUT)) {
        mutants.clear();
        sforeachi(_mut_index) {
            sforeachi_(j, _mut_index[i])  _mut_index[i][j].clear();
        }
        _mut_name_index.clear();
    }
    if (m&LOAD_VAR && !(_mode&LOAD_VAR)) _loadVariationInfo();
    if (_mode&LOAD_VAR && !(m&LOAD_VAR)) {
        variations.clear();
        sforeachi(_var_index) {
            sforeachi_(j, _var_index[i])  _var_index[i][j].clear();
        }
        _var_name_index.clear();
    }
    if (m&LOAD_FTR && !(_mode&LOAD_FTR)) _loadFeatureInfo();
    if (_mode&LOAD_FTR && !(m&LOAD_FTR)) {
        features.clear();
        sforeachi(_mut_index) {
            sforeachi_(j, _ftr_index[i])  _ftr_index[i][j].clear();
        }
        _ftr_name_index.clear();
    }
    _mode = m;
}
String SBAnnotDB::species() {
    auto &res = (*this)["INFO"].getRecord({"VALUE"}, sql::condition("NAME='species'"));
    return res.empty()?"":res.begin()->value;
}
String SBAnnotDB::version() {
    auto &res = (*this)["INFO"].getRecord({"VALUE"}, sql::condition("NAME='version'"));
    return res.empty()?"":res.begin()->value;
}
size_t SBAnnotDB::chrNum() const { return chromosomes.size(); }
chr_info SBAnnotDB::chrInfo(int idx) const { return chromosomes[idx]; }
size_t SBAnnotDB::chrIndex(const char *name) const { return _chr_index[name]; }
const sindex &SBAnnotDB::nameIdx() const { return _chr_index; }
template<class InfoVec>
inline void searchNameIndex(srange &range, const String &que, intarray &index, InfoVec &array) {
    if (range.end-range.begin < 2) return;
    if (range.length() < 20) {
        sforin(i, range.begin, range.end) {
            if(que == array[index[i]].name) { range = srange(i, i+1); break; }
        }
        if (range.begin+1 < range.end) range.begin = range.end;
    }
    else {
        auto mid = (range.begin+range.end)/2;
        if (que < array[index[mid]].name) range.end = mid;
        else range.begin = mid;
        searchNameIndex<InfoVec>(range, que, index, array);
    }
}
inline void searchGeneNameIndex(srange &range, const String &que, Array<SBAnnotDB::name_pair> &index) {
    if (range.end-range.begin < 2) return;
    if (range.length() < 20) {
        auto namep = &index[range.begin];
        sforin(i, 0, range.length()+1) {
            if(que == *namep->first) { range = srange(range.begin+i, range.begin+i+1); return; }
            ++namep;
        }
        range.begin = range.end;
    }
    if (range.begin+1 < range.end) {
        auto mid = (range.begin+range.end)/2;
        if (que < *index[mid].first) range.end = mid;
        else range.begin = mid;
        searchGeneNameIndex(range, que, index);
    }
}
template<class Info>
inline void searchPos(const sbpos &pos, Array<Array<CArray<Info *>>> &index, sorder &order, CArray<Info *> &array) {
    sizearray bins;
    SBIUtil::getBins(bins, pos);
    sforeachi(bins) {
        if (index[pos.idx][order[bins[i]]].empty()) continue;
        sforeach(index[pos.idx][order[bins[i]]]) {
            auto info = static_cast<Info *>(E_);
            if (info->overlap(pos)) array.add(info);
        }
    }
}
void SBAnnotDB::ctgInfo(ctgparray &array, const sbpos &pos, bool append) {
    if (!append) array.clear();
    if (_mode&LOAD_CTG) searchPos(pos, _ctg_index, bin_order[pos.idx], array);
    else {
        contigs.clear();
        try {
            auto condition = RANGE_CONDITION(pos);
            contigs.resize((*this)["CONTIG"].count(condition));
            if (contigs.empty()) return;
            (*this)["CONTIG"].getRecordPrepare({"*"}, condition, START_ASC_QUE);
            sforeach(contigs) {
                auto &row = getRow();
                toAnnotInfo(&E_, row);
                array.add(&E_);
            }
            commit();
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::ctgInfo(ctgparray &array, const char *name, sql::MATCH_TYPE match, bool append) {
	if (!append) array.clear();
    if (_mode&LOAD_CTG) {
        if (match == sql::EXACT) {
            srange range(0, _ctg_name_index.size());
            searchNameIndex<ctgarray>(range, name, _ctg_name_index, contigs);
            if (range.end-range.begin) return array.add(&contigs[_ctg_name_index[range.begin]]);
        }
        else {
            Regex reg("/"+String::lower(name)+"/i");
            sforeach(contigs) { if (String(E_.name).match(reg)) array.add(&E_); }
        }
    }
    else {
        contigs.clear();
        try {
            auto que = sql::textQue(name, match), condition = NAME_CONDITION(que);
            contigs.resize((*this)["CONTIG"].count(condition));
            if (contigs.empty()) return;
            (*this)["CONTIG"].getRecordPrepare({"*"}, condition, START_ASC_QUE);
            sforeach(contigs) {
                auto &row = getRow();
                toAnnotInfo(&E_, row);
                array.add(&E_);
            }
            commit();
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::geneInfo(geneparray &array, const sbpos &pos, bool trans, bool append) {
	if (!append) array.clear();
    if (_mode&LOAD_GENE) {
        if (trans && !(_mode&LOAD_TRANS)) setMode(_mode|LOAD_TRANS);
        searchPos(pos, _gene_index, bin_order[pos.idx], array);
    }
    else {
        try {
            auto condition = RANGE_CONDITION(pos);
            genes.resize((*this)["GENE"].count(condition));
            if (genes.empty()) return;
            integerarray gene_ids;
            (*this)["GENE"].getRecordPrepare({"*"}, condition, START_ASC_QUE);
            sforeach(genes) {
                auto &row = getRow();
                toGeneInfo(&E_, row);
                gene_ids.add(row["ID"]);
                array.add(&E_);
            }
            commit();
            if (trans) {
                transcripts.clear();
                auto condition = sql::condition("GENE_ID IN ("+slib::toString(gene_ids, ",")+")");
                transcripts.resize((*this)["TRANSCRIPT"].count(condition));
                if (transcripts.empty()) return;
                int t = 0;
                sforeachi(gene_ids) {
                    auto &tarray = (*this)["TRANSCRIPT"].getRecords({"*"}, sql::condition(String("GENE_ID=")<<gene_ids[i]), START_ASC_QUE);
                    if (tarray.empty()) continue;
                    sforeach(tarray) {
                        toAnnotInfo(&transcripts[t], E_.dict());
                        transcripts[t].gene = &genes[i];
                        genes[i].addTranscript(&transcripts[t]);
                        if (E_["STRUCTURE"]) {
                            auto struct_ids = E_["STRUCTURE"].split(",");
                            sforeachi_(s, struct_ids) {
                                auto &row = (*this)["STRUCTURE"].getRecordAt(struct_ids[s].intValue());
                                struct_info si;
                                toAnnotInfo(&si, row);
                                transcripts[t].structures.add(si);
                            }
                        }
                        ++t;
                    }
                }
            }
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::geneInfo(geneparray &array, const char *name, bool trans, sql::MATCH_TYPE match, bool append) {
	if (!append) array.clear();
    if (_mode&LOAD_GENE) {
        if (!(_mode&LOAD_TRANS) && trans) setMode(_mode|LOAD_TRANS);
        if(match == sql::EXACT) {
            srange range(0, _gene_name_index.size());
            searchGeneNameIndex(range, name, _gene_name_index);
            if (range.end-range.begin) array.add(&genes[_gene_name_index[range.begin].second]);
        }
        else {
            Regex reg("/"+String::lower(name)+"/i");
            sforeach(_gene_name_index) {
                if(String(*E_.first).match(reg)) array.add(&genes[E_.second]);
            }
        }
    }
    else {
        genes.clear();
        try {
            auto que = sql::textQue(name, match),
            condition = sql::condition("GENE_ID LIKE", que,
                                "OR NAME LIKE", que,
                                "OR OTHER_NAME LIKE", que);
            genes.resize((*this)["GENE"].count(condition));
            if (genes.empty()) return;
            integerarray gene_ids;
            (*this)["GENE"].getRecordPrepare({"*"}, condition, START_ASC_QUE);
            sforeach(genes) {
                auto &row = getRow();
                toGeneInfo(&E_, row);
                gene_ids.add(row["ID"]);
                array.add(&E_);
            }
            if (trans) {
                transcripts.clear();
                auto condition = sql::condition("GENE_ID IN ("+slib::toString(gene_ids, ",")+")");
                transcripts.resize((*this)["TRANSCRIPT"].count(condition));
                if (transcripts.empty()) return;
                int t = 0;
                sforeachi(gene_ids) {
                    auto &tarray = (*this)["TRANSCRIPT"].getRecords({"*"}, sql::condition(String("GENE_ID=")<<gene_ids[i]), START_ASC_QUE);
                    if (tarray.empty()) continue;
                    sforeach(tarray) {
                        toAnnotInfo(&transcripts[t], E_.dict());
                        transcripts[t].gene = &genes[i];
                        genes[i].addTranscript(&transcripts[t]);
                        if (E_["STRUCTURE"]) {
                            auto struct_ids = E_["STRUCTURE"].split(",");
                            sforeachi_(s, struct_ids) {
                                auto &row = (*this)["STRUCTURE"].getRecordAt(struct_ids[s].intValue());
                                struct_info si;
                                toAnnotInfo(&si, row);
                                transcripts[t].structures.add(si);
                            }
                        }
                        ++t;
                    }
                }
            }
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::transcriptInfo(trsparray &array, const sbpos &pos, bool gene, bool append) {
	if (!append) array.clear();
    if (_mode&LOAD_TRANS) {
        if (gene & !(_mode&LOAD_GENE)) setMode(_mode|LOAD_GENE);
        searchPos(pos, _trs_index, bin_order[pos.idx], array);
    }
    else {
        transcripts.clear();
        try {
            if (gene) {
                geneparray garray;
                geneInfo(garray, pos, true);
                if (garray.empty()) return;
                sforeachi(garray) {
                    if (garray[i]->transcripts.empty()) continue;
                    sforeach(garray[i]->transcripts) {
                        if (E_->overlap(pos)) array.add(E_);
                    }
                }
            }
            else {
                auto condition = RANGE_CONDITION(pos);
                transcripts.resize((*this)["TRANSCRIPT"].count(condition));
                if (transcripts.empty()) return;
                auto &tarray = (*this)["TRANSCRIPT"].getRecords({"*"}, condition, ID_ASC_QUE);
                if (tarray.empty()) return;
                int t = 0;
                sforeach(tarray) {
                    toAnnotInfo(&transcripts[t], E_.dict());
                    if (E_["STRUCTURE"]) {
                        auto struct_ids = E_["STRUCTURE"].split(",");
                        sforeachi_(s, struct_ids) {
                            auto &row = (*this)["STRUCTURE"].getRecordAt(struct_ids[s].intValue());
                            struct_info si;
                            toAnnotInfo(&si, row);
                            transcripts[t].structures.add(si);
                        }
                    }
                    array.add(&transcripts[t]); ++t;
                }
            }
            
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::transcriptInfo(trsparray &array, const char *name, bool gene, sql::MATCH_TYPE match, bool append) {
	if (!append) array.clear();
    if (_mode&LOAD_TRANS) {
        if (match == sql::EXACT) {
            srange range(0, _trs_name_index.size());
            searchNameIndex<trsarray>(range, name, _trs_name_index, transcripts);
            if (range.end-range.begin) return array.add(&transcripts[_trs_name_index[range.begin]]);
        }
        else {
            Regex reg("/"+String::lower(name)+"/i");
            sforeach(transcripts) { if (String(E_.name).match(reg)) array.add(&E_); }
        }
    }
    else {
        transcripts.clear();
        try {
            if (gene) {
                geneparray garray;
                geneInfo(garray, name, true, match);
                if (garray.empty()) return;
                sforeachi(garray) {
                    if (garray[i]->transcripts.empty()) continue;
                    sforeach(garray[i]->transcripts) array.add(E_);
                }
            }
            else {
                auto que = sql::textQue(name, match), condition = NAME_CONDITION(que);
                transcripts.resize((*this)["TRANSCRIPT"].count(condition));
                if (transcripts.empty()) return;
                auto &tarray = (*this)["TRANSCRIPT"].getRecords({"*"}, condition, ID_ASC_QUE);
                if (tarray.empty()) return;
                int t = 0;
                sforeach(tarray) {
                    toAnnotInfo(&transcripts[t], E_.dict());
                    if (E_["STRUCTURE"]) {
                        auto struct_ids = E_["STRUCTURE"].split(",");
                        sforeachi_(s, struct_ids) {
                            auto &row = (*this)["STRUCTURE"].getRecordAt(struct_ids[s].intValue());
                            struct_info si;
                            toAnnotInfo(&si, row);
                            transcripts[t].structures.add(si);
                        }
                    }
                    array.add(&transcripts[t]); ++t;
                }
            }
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::mutantInfo(mutparray &array, const sbpos &pos, bool append) {
	if (!append) array.clear();
    if (_mode&LOAD_MUT) searchPos(pos, _mut_index, bin_order[pos.idx], array);
    else {
        mutants.clear();
        try {
            auto condition = RANGE_CONDITION(pos);
            mutants.resize((*this)["MUTATION"].count(condition));
            if (mutants.empty()) return;
            (*this)["MUTATION"].getRecordPrepare({"*"}, condition, START_ASC_QUE);
            sforeach(mutants) {
                auto &row = getRow();
                toMutInfo(&E_, row);
                array.add(&E_);
            }
            commit();
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::mutantInfo(mutparray &array, const char *name, sql::MATCH_TYPE match, bool append) {
	if (!append) array.clear();
    if (_mode&LOAD_MUT) {
        if (match == sql::EXACT) {
            srange range(0, _mut_name_index.size());
            searchNameIndex<mutarray>(range, name, _mut_name_index, mutants);
            if (range.end-range.begin) return array.add(&mutants[_mut_name_index[range.begin]]);
        }
        else {
            Regex reg("/"+String::lower(name)+"/i");
            sforeach(mutants) { if (String(E_.name).match(reg)) array.add(&E_); }
        }
    }
    else {
        mutants.clear();
        try {
            auto que = sql::textQue(name, match), condition = NAME_CONDITION(que);
            mutants.resize((*this)["MUTATION"].count(condition));
            if (mutants.empty()) return;
            (*this)["MUTATION"].getRecordPrepare({"*"}, condition, START_ASC_QUE);
            sforeach(mutants) {
                auto &row = getRow();
                toMutInfo(&E_, row);
                array.add(&E_);
            }
            commit();
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::variationInfo(mutparray&array, const sbpos &pos, bool append) {
	if (!append) array.clear();
    if (_mode&LOAD_VAR) searchPos(pos, _var_index, bin_order[pos.idx], array);
    else {
        variations.clear();
        try {
            auto condition = RANGE_CONDITION(pos);
            variations.resize((*this)["VARIATION"].count(condition));
            if (variations.empty()) return;
            (*this)["VARIATION"].getRecordPrepare({"*"}, condition, START_ASC_QUE);
            sforeach(variations) {
                auto &row = getRow();
				toMutInfo(&E_, row);
                array.add(&E_);
            }
            commit();
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::variationInfo(mutparray &array, const char *name, sql::MATCH_TYPE match, bool append) {
    array.clear();
    if (_mode&LOAD_VAR) {
        if (match == sql::EXACT) {
            srange range(0, _var_name_index.size());
            searchNameIndex<mutarray>(range, name, _var_name_index, variations);
            if (range.end-range.begin) return array.add(&variations[_var_name_index[range.begin]]);
        }
        else {
            Regex reg("/"+String::lower(name)+"/i");
            sforeach(variations) { if (String(E_.name).match(reg)) array.add(&E_); }
        }
    }
    else {
        variations.clear();
        try {
            auto que = sql::textQue(name, match), condition = NAME_CONDITION(que);
            variations.resize((*this)["VARIATION"].count(condition));
            if (variations.empty()) return;
            (*this)["VARIATION"].getRecordPrepare({"*"}, condition, START_ASC_QUE);
            sforeach(variations) {
                auto &row = getRow();
                toMutInfo(&E_, row);
                array.add(&E_);
            }
            commit();
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::featureInfo(ftrparray &array, const sbpos &pos, bool append) {
	if (!append) array.clear();
    if (_mode&LOAD_FTR) searchPos(pos, _ftr_index, bin_order[pos.idx], array);
    else {
        features.clear();
        try {
            auto condition = RANGE_CONDITION(pos);
            features.resize((*this)["FEATURE"].count(condition));
            if (features.empty()) return;
            (*this)["FEATURE"].getRecordPrepare({"*"}, condition, START_ASC_QUE);
            sforeach(features) {
                auto &row = getRow();
                toAnnotInfo(&E_, row);
                array.add(&E_);
            }
            commit();
        } catch (SDBException de) {
			de.print();
        }
    }
}
void SBAnnotDB::featureInfo(ftrparray &array, const char *name, sql::MATCH_TYPE match, bool append) {
	if (!append) array.clear();
    if (_mode&LOAD_FTR) {
		if (match == sql::EXACT) {
            srange range(0, _ftr_name_index.size());
            searchNameIndex<ftrarray>(range, name, _ftr_name_index, features);
            if (range.end-range.begin) return array.add(&features[_ftr_name_index[range.begin]]);
        }
        else {
            Regex reg("/"+String::lower(name)+"/i");
            sforeach(features) { if (String(E_.name).match(reg)) array.add(&E_); }
        }
    }
    else {
        features.clear();
        try {
            auto que = sql::textQue(name, match), condition = NAME_CONDITION(que);
            features.resize((*this)["FEATURE"].count(condition));
            if (features.empty()) return;
            (*this)["FEATURE"].getRecordPrepare({"*"}, condition, START_ASC_QUE);
            sforeach(features) {
                auto &row = getRow();
                toAnnotInfo(&E_, row);
                array.add(&E_);
            }
            commit();
        } catch (SDBException de) {
			de.print();
        }
    }
}