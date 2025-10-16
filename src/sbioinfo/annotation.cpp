#include "sbioinfo/gff.h"
#include "sbioinfo/variant.h"

inline slib::String rangeCondition(const slib::sbio::RefPos& pos) {
    return S("chromosome=") << pos.idx << " and start<=" << pos.end << " and end>=" << pos.begin;
}

slib::sbio::AnnotInfo::AnnotInfo() : type(0), record(0) {}
slib::sbio::AnnotInfo::AnnotInfo(const slib::suint t, const slib::String& n, const RefPos& p) : 
    slib::sbio::RefPos(p), type(t), name(n), record(0) {}
slib::sbio::AnnotInfo::AnnotInfo(const AnnotInfo& info) :
    slib::sbio::RefPos(info), type(info.type), name(info.name), record(info.record) {}
slib::sbio::AnnotInfo::~AnnotInfo() {}
slib::sbio::AnnotInfo& slib::sbio::AnnotInfo::operator=(const AnnotInfo& info) {
    idx = info.idx; begin = info.begin; end = info.end; dir = info.dir;
    record = info.record; type = info.type; name = info.name; return *this;
}
void slib::sbio::AnnotInfo::clear() {
    slib::sbio::RefPos::init(); type = 0; name.clear();
}
slib::sbio::GeneInfo::GeneInfo() : slib::sbio::AnnotInfo() {}
slib::sbio::GeneInfo::GeneInfo(const GeneInfo& ginfo) : slib::sbio::AnnotInfo(ginfo) {
    geneid = ginfo.geneid; description = ginfo.description; synonym = ginfo.synonym;
    transcripts = ginfo.transcripts; attribute = ginfo.attribute;
}
slib::sbio::GeneInfo::~GeneInfo() {}
slib::sbio::GeneInfo& slib::sbio::GeneInfo::operator=(const slib::sbio::GeneInfo& ginfo) {
    idx = ginfo.idx; begin = ginfo.begin; end = ginfo.end; dir = ginfo.dir;
    record = ginfo.record; type = ginfo.type; name = ginfo.name;
    geneid = ginfo.geneid; description = ginfo.description; synonym = ginfo.synonym;
    transcripts = ginfo.transcripts; attribute = ginfo.attribute; return *this;
}
void slib::sbio::GeneInfo::clear() {
    AnnotInfo::clear();
    geneid.clear(); description.clear(); synonym.clear();
    transcripts.clear(); attribute.clear();
}
slib::sbio::TranscriptInfo::TranscriptInfo() : gene(nullptr), product(nullptr), slib::sbio::AnnotInfo() {}
slib::sbio::TranscriptInfo::TranscriptInfo(const TranscriptInfo& tinfo) : gene(nullptr), product(nullptr), slib::sbio::AnnotInfo(tinfo) {
    gene = tinfo.gene; structures = tinfo.structures;
}
slib::sbio::TranscriptInfo::~TranscriptInfo() {}
slib::sbio::TranscriptInfo& slib::sbio::TranscriptInfo::operator=(const TranscriptInfo& tinfo) {
    idx = tinfo.idx; begin = tinfo.begin; end = tinfo.end; dir = tinfo.dir;
    record = tinfo.record; type = tinfo.type; name = tinfo.name;
    gene = tinfo.gene; structures = tinfo.structures; return *this;
}
sregion slib::sbio::TranscriptInfo::exons() const {
    sregion reg;
    sfor(structures) { if ($_.type == EXON) reg.add($_); }
    return reg;
}
sregion slib::sbio::TranscriptInfo::coding() const {
    sregion reg;
    sfor(structures) { if ($_.type == CDS) reg.add($_); }
    return reg;
}
void slib::sbio::TranscriptInfo::clear() {
    AnnotInfo::clear(); gene = nullptr; structures.clear();
}
slib::sbio::VariantInfo::VariantInfo() : slib::sbio::AnnotInfo() {}
slib::sbio::VariantInfo::VariantInfo(const slib::sbio::VariantInfo& vinfo) : slib::sbio::AnnotInfo(vinfo) {
    varid = vinfo.varid; attribute = vinfo.attribute;
}
slib::sbio::VariantInfo::~VariantInfo() {}
slib::sbio::VariantInfo& slib::sbio::VariantInfo::operator=(const slib::sbio::VariantInfo& vinfo) {
    idx = vinfo.idx; begin = vinfo.begin; end = vinfo.end; dir = vinfo.dir; type = vinfo.type; name = vinfo.name;
    varid = vinfo.varid; attribute = vinfo.attribute; return *this;
}
void slib::sbio::VariantInfo::clear() {
    AnnotInfo::clear(); varid.clear(); attribute.clear();
}
slib::sbio::MotifInfo::MotifInfo() : slib::sbio::AnnotInfo() {}
slib::sbio::MotifInfo::MotifInfo(const slib::sbio::MotifInfo& minfo) {
    idx = minfo.idx; begin = minfo.begin; end = minfo.end; dir = minfo.dir; type = minfo.type; name = minfo.name;
    motid = minfo.motid; program = minfo.program;
}
slib::sbio::MotifInfo::~MotifInfo() {}
slib::sbio::MotifInfo& slib::sbio::MotifInfo::operator=(const slib::sbio::MotifInfo& minfo) {
    idx = minfo.idx; begin = minfo.begin; end = minfo.end; dir = minfo.dir; type = minfo.type; name = minfo.name;
    motid = minfo.motid; program = minfo.program; return *this;
}
void slib::sbio::MotifInfo::clear() {
    AnnotInfo::clear(); motid.clear(); program.clear();
}

slib::sbio::AnnotDB::AnnotDB(const char* path) : SDataBase() { 
    sforin(i, 0, 4) _load[i] = false;
    if(path) open(path);
}
slib::sbio::AnnotDB::~AnnotDB() {}
inline void toChrInfo(slib::sbio::AnnotInfo& info, slib::SRow& row) {
    info.record = row["id"];
    info.name = row["name"];
    info.type = row["type"];
    info.idx = info.record;
    info.begin = row["start"];
    info.end = row["end"];
}
void slib::sbio::AnnotDB::open(const char* path) {
    SDataBase::open(path);
    auto chrtable = SDataBase::table("chromosome");
    chromosomes.resize(chrtable.count());
    chrtable.select({ "*" });
    sfor2(chromosomes, chrtable) {
        toChrInfo($_1, $_2);
        chrindex.set($_1.name, $_1.idx);
    }
    auto infoTbl = SDataBase::table("info");
    infoTbl.select({ "*" });
    sfor(infoTbl) attribute.set($_[0], $_[1]);
    auto cnstTbl = SDataBase::table("const");
    cnstTbl.select({ "*" });
    attribute["flag"] = cnstTbl.rows();
}
void slib::sbio::AnnotDB::load(const stringarray& li) {
    sfor(li) {
        if ($_.match(REG("/gene/i"))) loadGenes();
        else if ($_.match(REG("/mut/i"))) loadMutations();
        else if ($_.match(REG("/var/i"))) loadVariants();
        //else if ($_.match(REG("/feat/i"))) loadFeatures();
    }
}
bool slib::sbio::AnnotDB::isOpened() { return chromosomes.size(); }
bool slib::sbio::AnnotDB::isLoaded(const char* key) const {
    String k(key);
    if (k.match(REG("/gene/i"))) return _load[0];
    else if (k.match(REG("/mut/i"))) return _load[1];
    else if (k.match(REG("/var/i"))) return _load[2];
    //else if (k.match(REG("/feat/i"))) return _load[3];
    return false;
}

inline void toGeneInfo(slib::sbio::GeneInfo& info, slib::SRow& row) {
    info.clear();
    info.record = row.hasColumn("id") ? row["id"].intValue() : -1;
    info.name = row.hasColumn("name") ? row["name"] : "";
    info.type = row.hasColumn("type") ? row["type"].intValue() : 0;
    info.idx = row.hasColumn("chromosome") ? row["chromosome"].intValue() : -1;
    info.begin = row.hasColumn("start") ? row["start"].intValue() : -1;
    info.end = row.hasColumn("end") ? row["end"].intValue() : -1;
    info.dir = row.hasColumn("strand") ? row["strand"].intValue() == 1 : false;
    info.geneid = row.hasColumn("gid") ? row["gid"] : "";
    info.description = row.hasColumn("description") ? row["description"] : "";
    info.attribute = row.hasColumn("attribute") ? row["attribute"].dict() : slib::SDictionary();
}
inline void toTranscriptInfo(slib::sbio::TranscriptInfo& info, slib::SRow& row) {
    info.clear();
    info.record = row.hasColumn("id") ? row["id"].intValue() : 0;
    info.name = row.hasColumn("name") ? row["name"] : "";
    info.type = row.hasColumn("type") ? row["type"].intValue() : 0;
    info.idx = row.hasColumn("gene") ? row["gene"].intValue() : -1;
    info.begin = row.hasColumn("start") ? row["start"].intValue() :-1;
    info.end = row.hasColumn("end") ? row["end"].intValue() : -1;
}
inline void toStructureInfo(slib::sbio::AnnotInfo& info, slib::SRow& row) {
    info.clear();
    info.type = row.hasColumn("type") ? row["type"].intValue() : 0;
    info.idx = row.hasColumn("transcript") ? row["transcript"].intValue() : -1;
    info.begin = row.hasColumn("start") ? row["start"].intValue() : -1;
    info.end = row.hasColumn("end") ? row["end"].intValue() : -1;
}
void slib::sbio::AnnotDB::loadGenes() {
    AnnotInfo tmp;
    auto geneTable = table("gene");
    auto count = geneTable.count();
    _genes.resize(count);
    _geneIndex.resize(chromosomes.size());
    geneTable.sortBy("id", ORDER::ASC).select({ "id","name","type","chromosome","start","end","strand","gid" });
    sfor2(_genes, geneTable) {
        toGeneInfo($_1, $_2);
        _geneIndex[$_1.idx].add(&$_1);
    }
    sfori(_geneIndex) {
        _geneIndex[(int)i].sort([](const GeneInfo* g1, const GeneInfo* g2) {
            if (g1->begin == g2->begin) {
                if (g1->end == g2->end) return g2->dir;
                else return g1->end < g2->end;
            }
            else return g1->begin < g2->begin;
            });
    }
    auto trsTable = table("transcript");
    _transcripts.resize(trsTable.count());
    trsTable.sortBy("id", ORDER::ASC).select({ "id","name","type","gene","start","end" });
    sfor2(_transcripts, trsTable) {
        toTranscriptInfo($_1, $_2);
        auto idx = $_2["gene"].intValue() - 1;
        $_1.gene = &_genes[idx];
        _genes[idx].transcripts.add(&$_1);
    }
    auto strctTable = table("structure");
    strctTable.sortBy("transcript", ORDER::ASC).select({ "*" });
    sfor(strctTable) {
        auto idx = $_["transcript"].intValue() - 1;
        toStructureInfo(tmp, $_);
        _transcripts[idx].structures.add(tmp);
    }
    sfor(_transcripts) $_.structures.sort();
    _load[0] = true;
}
inline void toVarInfo(slib::sbio::VariantInfo& info, slib::SRow row) {
    info.clear();
    info.record = row.hasColumn("id") ? row["id"].intValue() : -1;
    info.name = row.hasColumn("name") ? row["name"] : "";
    info.type = row.hasColumn("type") ? row["type"].intValue() : 0;
    info.idx = row.hasColumn("chromosome") ? row["chromosome"].intValue() : -1;
    info.begin = row.hasColumn("start") ? row["start"].intValue() : -1;
    info.end = row.hasColumn("end") ? row["end"].intValue() : -1;
    info.varid = row.hasColumn("vid") ? row["vid"] : "";
    info.attribute = row.hasColumn("attribute") ? row["attribute"].dict() : slib::SDictionary();
}
void slib::sbio::AnnotDB::loadMutations() {
    auto mutTable = table("mutation");
    auto count = mutTable.count();
    _mutations.resize(count);
    _varIndex.resize(chromosomes.size());
    mutTable.select({ "*" });
    sfor2(_mutations, mutTable) {
        toVarInfo($_1, $_2);
        _varIndex[$_1.idx].add(&$_1);
    }
    sfori(_varIndex) {
        _varIndex[(int)i].sort([](const VariantInfo* v1, const VariantInfo* v2) {
            if (v1->begin == v2->begin) {
                if (v1->end == v2->end) return !v1->dir;
                return v1->end < v2->end;
            }
            return v1->begin < v2->begin;
            });
    }
    _load[1] = true;
}
void slib::sbio::AnnotDB::loadVariants() {
    auto varTable = table("variant");
    auto count = varTable.count();
    _variants.resize(count);
    _varIndex.resize(chromosomes.size());
    varTable.select({ "*" });
    sfor2(_variants, varTable) {
        toVarInfo($_1, $_2);
        _varIndex[$_1.idx].add(&$_1);
    }
    sfori(_varIndex) {
        _varIndex[(int)i].sort([](const VariantInfo* v1, const VariantInfo* v2) {
            if (v1->begin == v2->begin) {
                if (v1->end == v2->end) return !v1->dir;
                return v1->end < v2->end;
            }
            return v1->begin < v2->begin;
            });
    }
    _load[2] = true;
}

inline void toFeatureInfo(slib::sbio::AnnotInfo& info, slib::SRow row) {
    info.clear();
    info.name = row.hasColumn("name") ? row["name"] : "";
    info.type = row.hasColumn("type") ? row["type"].intValue() : 0;
    info.idx = row.hasColumn("chromosome") ? row["chromosome"].intValue() : -1;
    info.begin = row.hasColumn("start") ? row["start"].intValue() : -1;
    info.end = row.hasColumn("end") ? row["end"].intValue() : -1;
    info.dir = row.hasColumn("strand") ? row["strand"].intValue() : 0;
}
void slib::sbio::AnnotDB::loadFeatures() {
    /*
    */
}
inline void toCtgInfo(slib::sbio::AnnotInfo& info, slib::SRow row) {
    info.name = row["name"] ? row["name"] : "";
    info.idx = row["chromosome"] ? row["chromosome"].intValue() : -1;
    info.begin = row["start"] ? row["start"].intValue() : -1;
    info.end = row["end"] ? row["end"].intValue() : -1;
}
slib::Array<slib::sbio::AnnotInfo>& slib::sbio::AnnotDB::getContigs(const slib::sbio::RefPos& pos, Array<AnnotInfo>* container) {
    auto& contigs = container ? *container : _contigs;
    auto ctgTable = table("contig");
    ctgTable.where(rangeCondition(pos)).select({ "*" });
    contigs.resize(ctgTable.nrow());
    sfor2(contigs, ctgTable) toCtgInfo($_1, $_2);
    return contigs;
}
slib::Array<slib::sbio::AnnotInfo>& slib::sbio::AnnotDB::getContigs(const char* query, slib::MATCH match, Array<AnnotInfo>* container) {
    auto& contigs = container ? *container : _contigs;
    auto ctgTable = table("contig");
    ctgTable.where(sdb::textQuery("name", query, match)).select({ "*" });
    contigs.resize(ctgTable.nrow());
    sfor2(contigs, ctgTable) toCtgInfo($_1, $_2);
    return contigs;
}

intarray &slib::sbio::AnnotDB::searchGenes(const slib::sbio::RefPos& pos, sobj opts, intarray* container) {
    intarray& indice = container ? (*container) : _indice;
    indice.clear();
    auto geneTable = table("gene");
    String condition = rangeCondition(pos);
    if (opts) {
        slib::subyte gtype = 0;
        if (opts["protein-coding"]) gtype |= (subyte)GENE_TYPE::PROTEIN_CODING;
        if (opts["non-codng"]) gtype |= (subyte)GENE_TYPE::NON_CODING;
        if (opts["pseudogene"]) gtype |= (subyte)GENE_TYPE::PROTEIN_CODING;
        if (0 < gtype) condition << " AND 0<(type&" << (int)gtype << ")";
    }
    geneTable.where(condition).select({ "id" });
    indice.resize(geneTable.nrow());
    sfor2(indice, geneTable) $_1 = $_2[0];
    return indice;
}
intarray &slib::sbio::AnnotDB::searchGenes(const char* query, slib::MATCH match, sobj opts, intarray* container) {
    if (!query) throw NullException(nullErrorText("Query (gene symbol, synoym, or ID)"));
    intarray& indice = container ? (*container) : _indice;
    indice.clear();
    auto geneTable = table("gene");
    String condition = sdb::textQuery("name", query, match) + " OR " + sdb::textQuery("gid", query, match);
    if (opts) {
        if (match == MATCH::PARTIAL && opts["search-description"])
            condition << " OR " << sdb::textQuery("description", query, match);
        //
        slib::subyte gtype = 0;
        if (opts["protein-coding"]) gtype |= (subyte)GENE_TYPE::PROTEIN_CODING;
        if (opts["non-codng"]) gtype |= (subyte)GENE_TYPE::NON_CODING;
        if (opts["pseudogene"]) gtype |= (subyte)GENE_TYPE::PROTEIN_CODING;
        if (0 < gtype) condition << " AND 0<(type&" << (int)gtype << ")";
        //
        if (opts["search-synonym"]) {
            auto tbl1 = table("synonym");
            tbl1.where(sdb::textQuery("name", query, match)).select({ "gene" });
            sfor(tbl1) indice.add($_[0]);
        }
        //
        if (opts["search-xref"]) {
            auto tbl2 = table("xref");
            tbl2.where("type=1", "AND", "(", sdb::textQuery("ref", query, match), "OR", sdb::textQuery("refid", query, match), ")").select({"dbid"});
            sfor(tbl2) indice.add($_[0]);
        }
    }
    //
    geneTable.where(condition).select({ "id" });
    sfor(geneTable) indice.add($_[0]);
    indice.unique();
    return indice;
}
slib::Array<slib::sbio::GeneInfo>& slib::sbio::AnnotDB::getGenes(const RefPos& pos, sobj opts) { return geneInfos(searchGenes(pos, opts), opts); }
slib::Array<slib::sbio::GeneInfo>& slib::sbio::AnnotDB::getGenes(const char* query, slib::MATCH match, sobj opts) { return geneInfos(searchGenes(query, match, opts), opts); }
slib::Array<slib::sbio::GeneInfo*>& slib::sbio::AnnotDB::selectGenes(const RefPos& pos, Array<GeneInfo*>* container) {
    if (!container) container = &_pgenes;
    container->clear();
    if (!_load[0]) loadGenes();
    sfor(_geneIndex[pos.idx]) {
        if (pos.end < $_->begin) break;
        else if ($_->overlap(pos)) container->add($_);
    }
    return *container;
}
slib::sbio::GeneInfo& slib::sbio::AnnotDB::geneInfo(int record, sobj opts) {
    geneInfos({ record }, opts);
    if (_genes.empty()) throw NotFoundException(nofoundErrorText("ID:" + S(record), "Gene database"));
    return _genes[0];
}
slib::Array<slib::sbio::GeneInfo>& slib::sbio::AnnotDB::geneInfos(const intarray& records, sobj opts) {
    if (_load[0]) {
        _genes.clear(); _load[0] = false;
    }
    if (records.empty()) return _genes;
    stringarray select;
    if (opts && opts["select"]) select = opts["select"].split(",");
    else select = { "*" };
    auto geneTable = table("gene");
    geneTable
        .where(sdb::inQuery("id", records))
        .select(select);
    Map<int, GeneInfo*> gmap;
    _genes.resize(geneTable.nrow());
    sfor2(_genes, geneTable) {
        toGeneInfo($_1, $_2);
        gmap[$_1.record] = &$_1;
    }
    //
    if (opts && opts["synonym"]) {
        auto synTable = table("synonym");
        synTable
            .where(sdb::inQuery("gene", records))
            .select({ "gene", "name"});
        sfor(synTable) {
            gmap[$_[0].intValue()]->synonym.add($_[1]);
        }
    }
    //
    if (opts && opts["xref"]) {
        auto xrefTable = table("xref");
        xrefTable
            .where(sdb::inQuery("dbid", records), "AND", "type=1")
            .select({ "dbid", "ref", "refid" });
        sfor(xrefTable) {
            gmap[$_[0].intValue()]->attribute["xref:" + $_[1].toString()] = $_[2];
        }
    }
    //
    if (opts && opts["transcript"]) {
        auto trsTable = table("transcript");
        trsTable.where(sdb::inQuery("gene", records))
            .sortBy("name", ORDER::ASC)
            .select({ "*" });
        _transcripts.resize(trsTable.nrow());
        Map<int, TranscriptInfo*> tmap;
        sfor2(_transcripts, trsTable) {
            toTranscriptInfo($_1, $_2);
            tmap[$_1.record] = &$_1;
            $_1.gene = gmap[$_1.idx];
            $_1.dir = $_1.gene->dir;
            gmap[$_1.idx]->transcripts.add(&$_1);
        }
        auto tindice = tmap.keyset();
        auto strctTable = table("structure");
        strctTable.where(sdb::inQuery("transcript", tindice))
            .sortBy("start", ORDER::ASC)
            .select({ "*" });
        sfor(strctTable) {
            toStructureInfo(_ainfo, $_);
            tmap[$_["transcript"]]->structures.add(_ainfo);
        }
    }
    return _genes;
}
slib::Array<slib::sbio::TranscriptInfo>& slib::sbio::AnnotDB::transcriptsOf(int gene, sobj opts) {
    _transcripts.clear();
    String condition = "gene=" + S(gene);
    //
    stringarray select;
    if (opts && opts["select"]) select = opts["select"].split(",");
    else select = { "*" };
    //
    auto trsTable = table("transcript")
        .where(condition)
        .sortBy("name", ORDER::ASC)
        .select(select);
    Map<int, TranscriptInfo*> tmap;
    _transcripts.resize(trsTable.nrow());
    sfor2(_transcripts, trsTable) {
        toTranscriptInfo($_1, $_2);
        tmap[$_1.record] = &$_1;
    }
    auto tindice = tmap.keyset();
    auto strctTable = table("structure");
    strctTable.where(sdb::inQuery("transcript", tindice))
        .sortBy("start", ORDER::ASC)
        .select({ "*" });
    sfor(strctTable) {
        toStructureInfo(_ainfo, $_);
        tmap[$_["transcript"]]->structures.add(_ainfo);
    }
    if (opts["gene"]) {
        auto geneTable = table("gene");
        geneTable.where("id=" + S(gene)).select({ "*" });
        _genes.resize(1);
        auto row = geneTable.row(0);
        toGeneInfo(_genes[0], row);
        sforeach(rna, _transcripts) {
            rna.gene = &_genes[0];
            _genes[0].transcripts.add(&rna);
        }
    }
    return _transcripts;
}
slib::Array<slib::sbio::TranscriptInfo>& slib::sbio::AnnotDB::getTranscripts(const RefPos& pos, sobj opts) {
    _transcripts.clear();


    auto trsTable = table("transcript");
    searchGenes(pos);
    trsTable
        .where(sdb::inQuery("gene", _indice),
            "and", "transcript.start<="+S(pos.end),
            "and", "transcript.end>=" + S(pos.begin))
        .select({"*"});
    _transcripts.resize(trsTable.nrow());
    sfor2(_transcripts, trsTable)  toTranscriptInfo($_1, $_2);
    return _transcripts;
}
slib::Array<slib::sbio::TranscriptInfo>& slib::sbio::AnnotDB::getTranscripts(const char* query, slib::MATCH match, sobj opts) {
    if (!query) throw NullException(nullErrorText("Query"));
    auto trsTable = table("transcript");
    trsTable
        .join("xref", sdb::JOIN::INNER, "(xref.dbid=transcript.id and xref.type=2)")
        .where(sdb::textQuery("transcript.name", query, match), "or",
            sdb::textQuery("xref.refid", query, match))
        .grouping({ "transcript.id" })
        .select({ "*" });
    _transcripts.resize(trsTable.nrow());
    sfor2(_transcripts, trsTable)  toTranscriptInfo($_1, $_2);
    return _transcripts;
}
intarray& slib::sbio::AnnotDB::searchMutations(const slib::sbio::RefPos& pos, sobj opts, intarray* container) {
    intarray& indice = container ? (*container) : _indice;
    indice.clear();
    auto mutTable = table("mutation");
    String condition = rangeCondition(pos);
    if (opts) {
        sint mtype = 0;
        if (opts["snv"]) mtype |= sbio::SNV;
        if (opts["deleton"]) mtype |= sbio::DELETION;
        if (opts["duplicaton"]) mtype |= sbio::DUPLICATION;
        if (opts["insertion"]) mtype |= sbio::INSERTION;
        if (0 < mtype) condition << " AND 0<(type&" << mtype << ")";
        //
        if (opts["cds-only"]) condition << " AND (type&" << (sbio::CDS << 8) << ")=" << (sbio::CDS << 8);
    }
    //
    mutTable.where(condition).select({ "id" });
    sfor(mutTable) indice.add($_[0]);
    indice.unique();
    return indice;
}
intarray& slib::sbio::AnnotDB::searchMutations(const char* query, slib::MATCH match, sobj opts, intarray* container) {
    if (!query) throw NullException(nullErrorText("Query (gene symbol, synoym, or ID)"));
    intarray& indice = container ? (*container) : _indice;
    indice.clear();
    auto mutTable = table("mutation");
    String condition = sdb::textQuery("name", query, match) + " OR " + sdb::textQuery("vid", query, match);
    if (opts) {
        sint mtype = 0;
        if (opts["snv"]) mtype |= sbio::SNV;
        if (opts["deleton"]) mtype |= sbio::DELETION;
        if (opts["duplicaton"]) mtype |= sbio::DUPLICATION;
        if (opts["insertion"]) mtype |= sbio::INSERTION;
        if (0 < mtype) condition << " AND 0<(type&" << mtype << ")";
        //
        if (opts["cds-only"]) condition << " AND (type&" << (sbio::CDS << 8) << ")=" << (sbio::CDS << 8);
    }
    //
    mutTable.where(condition).select({ "id" });
    sfor(mutTable) indice.add($_[0]);
    indice.unique();
    return indice;
}
intarray& slib::sbio::AnnotDB::searchVariants(const RefPos& pos, sobj opts, intarray* container) {
    intarray& indice = container ? (*container) : _indice;
    indice.clear();
    auto varTable = table("variant");
    String condition = rangeCondition(pos);
    if (opts) {
        if (opts["type"]) {
            int vtype = 0;
            auto types = opts["type"].split(",");
            sfor(types) {
                if ($_=="snv") vtype |= sbio::SNV;
                if ($_ == "del") vtype |= sbio::DELETION;
                if ($_ == "dup") vtype |= sbio::DUPLICATION;
                if ($_ == "ins") vtype |= sbio::INSERTION;
            }
            if (0 < vtype) condition << " AND 0<(type&" << vtype << ")";
        }
        //
        if (opts["cds-only"]) condition << " AND (type&" << (sbio::CDS << 8) << ")=" << (sbio::CDS << 8);
    }
    //
    varTable.where(condition).select({ "id" });
    sfor(varTable) indice.add($_[0]);
    indice.unique();
    return indice;
}
intarray& slib::sbio::AnnotDB::searchVariants(const char* query, slib::MATCH match, sobj opts, intarray* container) {
    if (!query) throw NullException(nullErrorText("Query (gene symbol, synoym, or ID)"));
    intarray& indice = container ? (*container) : _indice;
    indice.clear();
    auto varTable = table("variant");
    String condition = sdb::textQuery("name", query, match) + " OR " + sdb::textQuery("vid", query, match);
    if (opts) {
        if (opts["type"]) {
            int vtype = 0;
            auto types = opts["type"].split(",");
            sfor(types) {
                if ($_ == "snv") vtype |= sbio::SNV;
                if ($_ == "del") vtype |= sbio::DELETION;
                if ($_ == "dup") vtype |= sbio::DUPLICATION;
                if ($_ == "ins") vtype |= sbio::INSERTION;
            }
            if (0 < vtype) condition << " AND 0<(type&" << vtype << ")";
        }
        //
        if (opts["cds-only"]) condition << " AND (type&" << (sbio::CDS << 8) << ")=" << (sbio::CDS << 8);
    }
    //
    varTable.where(condition).select({ "id" });
    sfor(varTable) indice.add($_[0]);
    indice.unique();
    return indice;
}
slib::Array<slib::sbio::VariantInfo>& slib::sbio::AnnotDB::getMutations(const RefPos& pos, sobj opts) {
    return mutationInfos(searchMutations(pos, opts), opts);
}
slib::Array<slib::sbio::VariantInfo>& slib::sbio::AnnotDB::getMutations(const char* query, slib::MATCH match, sobj opts) {
    return mutationInfos(searchMutations(query, match, opts), opts);
}
slib::Array<slib::sbio::VariantInfo>& slib::sbio::AnnotDB::getVariants(const RefPos& pos, sobj opts) {
    return variantInfos(searchVariants(pos, opts), opts);
}
slib::Array<slib::sbio::VariantInfo>& slib::sbio::AnnotDB::getVariants(const char* query, slib::MATCH match, sobj opts) {
    return variantInfos(searchVariants(query, match, opts), opts);
}
slib::Array<slib::sbio::VariantInfo>& slib::sbio::AnnotDB::mutationInfos(const intarray& records, sobj opts) {
    _mutations.clear();
    stringarray select;
    if (opts && opts["select"]) select = opts["select"].split(",");
    else select = { "*" };
    auto mutTable = table("mutation");
    mutTable
        .where(sdb::inQuery("id", records))
        .select(select);
    _mutations.resize(mutTable.nrow());
    sfor2(_mutations, mutTable) toVarInfo($_1, $_2);
    return _mutations;
}
slib::Array<slib::sbio::VariantInfo>& slib::sbio::AnnotDB::variantInfos(const intarray& records, sobj opts) {
    _variants.clear();
    stringarray select;
    if (opts && opts["select"]) select = opts["select"].split(",");
    else select = { "*" };
    auto varTable = table("variant");
    varTable
        .where(sdb::inQuery("id", records))
        .select(select);
    _variants.resize(varTable.nrow());
    sfor2(_variants, varTable) toVarInfo($_1, $_2);
    return _variants;
}
slib::Array<slib::sbio::VariantInfo>& slib::sbio::AnnotDB::variantsOf(int gene, sobj opts) {
    auto& ginfo = geneInfo(gene, { D_("select", "chromosome,start,end") });
    auto& mut = getMutations(ginfo, opts);
    auto& var = getVariants(ginfo, opts);
    _variants.append(_mutations);
    return _variants;
}
bool slib::sbio::AnnotDB::verifyVariant(slib::sbio::Variant& var, const VarParam& par) {
    bool verified = false;
    String cond;
    if (var.flag & SMALL_VARIANT) {
        cond << "(type&" << var.type << ")=" << var.type << " and " <<
            "chromosome=" << var.pos[0].idx << " and " <<
            "start=" << var.pos[0].begin << " and " <<
            "end=" << var.pos[0].end;
        //if (var.alt.size()) cond << " and " << "attribute like '%\"alt\":\"" << var.alt << "\"%'";
    }
    else {
        cond << "(type&" << var.type << ")=" << var.type << " and " <<
            "chromosome=" << var.pos[0].idx << " and " <<
            "start<=" << var.pos[0].end << " and " <<
            "end>=" << var.pos[0].begin;
    }
    if (_load[1] || _load[2]) {
        sfor(_varIndex[var.pos[0].idx]) {
            if (var.pos[0].end < $_->begin) break;
            if (var.pos[0].overlap(*$_)) {
                if (var.varid.empty() || var.varid == ".") {
                    if (var.flag & SMALL_VARIANT) {
                        if (var.type == ($_->type & 0xFF) &&
                            var.pos[0].begin == $_->begin &&
                            var.pos[0].end == $_->end &&
                            var.alt == $_->attribute["alt"].toString()) {
                            var.varid = $_->name;
                            verified = true;
                        }
                    }
                    else if (var.flag & SR_VARIANT) {
                        if ((var.type & ($_->type & 0xFF)) &&
                            std::abs(var.pos[0].begin - $_->begin) <= par.svp.max_gap &&
                            std::abs(var.pos[0].length() - $_->length()) <= par.svp.max_gap) {
                            var.varid = $_->name;
                            verified = true;
                        }
                    }
                }
            }
        }
    }
    else {
        if (!_load[1]) {
            auto mutTable = table("mutation");
            if (var.flag & SMALL_VARIANT) {
                mutTable.where(cond).limit(1).select({ "name" });
                if (0 < mutTable.nrow()) {
                    var.varid = mutTable[0][0];
                    verified = true;
                }
            }
            else {
                mutTable.where(cond).select({ "name", "start", "end" });
                sforeach(row, mutTable) {
                    if (std::abs(var.pos[0].begin - row["start"].intValue()) <= par.svp.break_site_len &&
                        std::abs(var.pos[0].length() - (row["end"].intValue() - row["start"].intValue())) <= par.svp.break_site_len) {
                        var.varid = row["name"];
                        verified = true;
                    }
                }
            }
        }
        if (!_load[2] && !verified) {
            auto varTable = table("variant");
            if (var.flag & SMALL_VARIANT) {
                varTable.where(cond).limit(1).select({ "name" });
                if (0 < varTable.nrow()) {
                    var.varid = varTable[0][0];
                    verified = true;
                }
            }
            else {
                varTable.where(cond).select({ "name", "start", "end" });
                sforeach(row, varTable) {
                    if (std::abs(var.pos[0].begin - row["start"].intValue()) <= par.svp.break_site_len &&
                        std::abs(var.pos[0].length() - (row["end"].intValue() - row["start"].intValue())) <= par.svp.break_site_len) {
                        var.varid = row["name"];
                        verified = true;
                    }
                }
            }
        }
    }
    return verified;
}

slib::Array<slib::sbio::AnnotInfo>& slib::sbio::AnnotDB::getFeatures(const RefPos& pos, sobj opts) {
    _features.clear();
    auto ftrTable = table("feature");
    //
    String condition = rangeCondition(pos);
    if (opts && opts["type"]) {
        String tmp;
        auto types = opts["type"].split(",");
        sfor(types) {
            if ($_ == "tf") tmp << "type=" << (int)sbio::TF_SITE << " OR ";
            if ($_ == "tss") tmp << "type=" << (int)sbio::TSS_SITE << " OR ";
            if ($_ == "promoter") tmp << "type=" << (int)sbio::PROMOTER << " OR ";
            if ($_ == "enhancer") tmp << "type=" << (int)sbio::ENHANCER << " OR ";
            if ($_ == "repressor") tmp << "type=" << (int)sbio::REPRESSOR << " OR ";

            if ($_ == "operon") tmp << "type=" << (int)sbio::OPERON << " OR ";

            if ($_ == "centromere") tmp << "type=" << (int)sbio::CENTROMERE << " OR ";
            if ($_ == "teromere") tmp << "type=" << (int)sbio::TEROMERE << " OR ";


            if ($_ == "balancer") tmp << "type=" << (int)sbio::BALANCED_SITE << " OR ";
        }
        if (tmp.size()) {
            tmp.resize(tmp.size() - 4);
            condition << " AND (" << tmp << ")";
        }
    }
    //
    stringarray select;
    if (opts && opts["select"]) select = opts["select"].split(",");
    else select = { "*" };
    //
    ftrTable
        .where(condition)
        .select(select);
    _features.resize(ftrTable.nrow());
    sfor2(_features, ftrTable) toFeatureInfo($_1, $_2);
    return _features;
}
slib::Array<slib::sbio::AnnotInfo>& slib::sbio::AnnotDB::getFeatures(const char* query, slib::MATCH match, sobj opts) {
    if (!query) throw NullException(nullErrorText("Query (gene symbol, synoym, or ID)"));
    _features.clear();
    auto ftrTable = table("feature");
    String condition = sdb::textQuery("name", query, match);
    if (opts && opts["type"]) {
        String tmp;
        auto types = opts["type"].split(",");
        sfor(types) {
            if ($_ == "tf") tmp << "type=" << (int)sbio::TF_SITE << " OR ";
            if ($_ == "tss") tmp << "type=" << (int)sbio::TSS_SITE << " OR ";
            if ($_ == "promoter") tmp << "type=" << (int)sbio::PROMOTER << " OR ";
            if ($_ == "enhancer") tmp << "type=" << (int)sbio::ENHANCER << " OR ";
            if ($_ == "repressor") tmp << "type=" << (int)sbio::REPRESSOR << " OR ";

            if ($_ == "operon") tmp << "type=" << (int)sbio::OPERON << " OR ";

            if ($_ == "centromere") tmp << "type=" << (int)sbio::CENTROMERE << " OR ";
            if ($_ == "teromere") tmp << "type=" << (int)sbio::TEROMERE << " OR ";


            if ($_ == "balancer") tmp << "type=" << (int)sbio::BALANCED_SITE << " OR ";
        }
        if (tmp.size()) {
            tmp.resize(tmp.size() - 4);
            condition << " AND (" << tmp << ")";
        }
    }
    //
    stringarray select;
    if (opts && opts["select"]) select = opts["select"].split(",");
    else select = { "*" };
    //
    ftrTable
        .where(condition)
        .select(select);
    _features.resize(ftrTable.nrow());
    sfor2(_features, ftrTable) toFeatureInfo($_1, $_2);
    return _features;
}
inline void toProteinInfo(slib::sbio::AnnotInfo& info, slib::SRow row) {
    info.clear();
    info.record = row.hasColumn("id") ? row["id"].intValue() : 0;
    info.name = row.hasColumn("name") ? row["name"] : "";
    info.type = row.hasColumn("type") ? row["type"].intValue() : 0;
    info.idx = row.hasColumn("gene") ? row["gene"].intValue() : -1;
    info.begin = row.hasColumn("start") ? row["start"].intValue() : -1;
    info.end = row.hasColumn("end") ? row["end"].intValue() : -1;
}
slib::Array<slib::sbio::AnnotInfo>& slib::sbio::AnnotDB::proteinsOf(int gene, sobj opts) {
    stringarray select;
    if (opts && opts["select"]) select = opts["select"].split(",");
    else select = { "*" };
    auto prtTable = table("protein");
    prtTable.where(sdb::inQuery("gene", _indice)).select(select);
    _proteins.resize(prtTable.nrow());
    sfor2(_proteins, prtTable) toProteinInfo($_1, $_2);
    return _proteins;
}
inline void toMotifInfo(slib::sbio::MotifInfo& info, slib::SRow row) {
    info.clear();
    info.name = row.hasColumn("name") ? row["name"] : "";
    info.type = row.hasColumn("type") ? row["type"].intValue() : 0;
    info.idx = row.hasColumn("protein") ? row["protein"].intValue() : -1;
    info.begin = row.hasColumn("start") ? row["start"].intValue() : -1;
    info.end = row.hasColumn("end") ? row["end"].intValue() : -1;
    info.motid = row.hasColumn("mid") ? row["mid"] : "";
    info.program = row.hasColumn("source") ? row["source"] : "";
}
slib::Array<slib::sbio::MotifInfo>& slib::sbio::AnnotDB::motifsOf(int prot, sobj opts) {
    auto motTable = table("motif");
    String condition = "protein=" + S(prot);
    if (opts["source"]) {
        auto sources = opts["source"].split(",");
        condition << " AND " << sdb::inQuery("source", sources);
    }
    motTable.where(condition).select({ "*" });
    _motifs.resize(motTable.nrow());
    sfor2(_motifs, motTable) toMotifInfo($_1, $_2);
    return _motifs;
}

/*
intarray slib::sbio::AnnotDB::searchMotifs(const RefPos& pos) {
    auto motifTable = table("motif");



    intarray array(motifTable.nrow());
    sfor2(array, motifTable) $_1 = $_2[0];
    return array;
}
intarray slib::sbio::AnnotDB::searchMotifs(const char* query, slib::MATCH match) {
    auto motifTable = table("motif");



    intarray array(motifTable.nrow());
    sfor2(array, motifTable) $_1 = $_2[0];
    return array;
}
slib::sbio::MotifInfo& slib::sbio::AnnotDB::motifInfo(int id) {
    auto motifTable = table("motif");
    motifTable
        .where("id=" + S(id))
        .select({ "*" });


}
*/

void slib::sbio::AnnotDB::annotate(Sequence& seq, const RefPos& pos, const sushort types) {
    if (types & (sushort)ANNOT_CATEGORY::GENE || types & (sushort)ANNOT_CATEGORY::TRANSCRIPT) {
        auto& genes = getGenes(pos, {
            D_("transcript", types & (sushort)ANNOT_CATEGORY::TRANSCRIPT)
            });
        sforeach(gene, genes) {
            if (types & (sushort)ANNOT_CATEGORY::GENE) {
                SeqNote gnote(gene);
                gnote.category = (sushort)ANNOT_CATEGORY::GENE;
                gnote.note = "gid=" + gene.geneid + "&" + gene.attribute.toString("url");
                seq.annotation.addNote(gnote);
            }
            if (types & (sushort)ANNOT_CATEGORY::TRANSCRIPT) {
                sforeach(rna, gene.transcripts) {
                    SeqNote tnote(*rna);
                    tnote.category = (sushort)ANNOT_CATEGORY::TRANSCRIPT;
                    seq.annotation.addNote(tnote);
                    //
                    if (rna->type == (int)TRANSCRIPT_TYPE::M_RNA) {
                        SeqNote snote(rna->coding(), gene.dir, (subyte)ANNOT_CATEGORY::STRUCTURE, sbio::CDS, rna->name + "-cds");
                        snote.shift(-1);
                        seq.annotation.addNote(snote);
                    }
                    else {
                        SeqNote snote(rna->exons(), gene.dir, (subyte)ANNOT_CATEGORY::STRUCTURE, sbio::EXON, rna->name + "-exon");
                        snote.shift(-1);
                        seq.annotation.addNote(snote);
                    }
                }
            }
        }
    }
    if (types & (sushort)ANNOT_CATEGORY::MUTATION) {
        auto& vars = getVariants(pos);
        auto& muts = getMutations(pos);
        sforeach(var, vars) {
            SeqNote vnote(var);
            vnote.category = (sushort)ANNOT_CATEGORY::MUTATION;
            seq.annotation.addNote(vnote);
        }
        sforeach(mut, muts) {
            SeqNote vnote(mut);
            vnote.category = (sushort)ANNOT_CATEGORY::MUTATION;
            seq.annotation.addNote(vnote);
        }
    }
}
void slib::sbio::AnnotDB::annotate(Sequence& seq, const GeneInfo& gene, const sushort types) {
    if (types & (sushort)ANNOT_CATEGORY::GENE) {
        SeqNote gnote(gene);
        gnote.category = (sushort)ANNOT_CATEGORY::GENE;
        gnote.note = "gid=" + gene.geneid + "&" + gene.attribute.toString("url");
        seq.annotation.addNote(gnote);
    }
    if (types & (sushort)ANNOT_CATEGORY::TRANSCRIPT) {
        sforeach(rna, gene.transcripts) {
            SeqNote tnote(*rna);
            tnote.category = (sushort)ANNOT_CATEGORY::TRANSCRIPT;
            seq.annotation.addNote(tnote);
            //
            if (rna->type == (int)TRANSCRIPT_TYPE::M_RNA) {
                SeqNote snote(rna->coding(), gene.dir, (subyte)ANNOT_CATEGORY::STRUCTURE, sbio::CDS, rna->name + "-cds");
                snote.shift(-1);
                seq.annotation.addNote(snote);
            }
            else {
                SeqNote snote(rna->exons(), gene.dir, (subyte)ANNOT_CATEGORY::STRUCTURE, sbio::EXON, rna->name + "-exon");
                snote.shift(-1);
                seq.annotation.addNote(snote);
            }
        }
    }
    if (types & (sushort)ANNOT_CATEGORY::MUTATION) {
        auto& vars = variantsOf(gene.record);
        sforeach(var, vars) {
            SeqNote vnote(var);
            vnote.category = (sushort)ANNOT_CATEGORY::MUTATION;
            seq.annotation.addNote(vnote);
        }
    }
}


inline void _splice(slib::sbio::TranscriptAnnotData& rna, const sregion& reg, bool dir) {
    srange tpos(-1, -1);
    slib::String tmp[2];
    int len = 0;
    sforeach(range, reg) {
        // Set pos on spliced rna
        if (range.include(rna.pos.begin)) tpos.begin = len + (rna.pos.begin - range.begin);
        else if (tpos.begin == -1 && rna.pos.begin < range.begin) tpos.begin = len;
        if (range.include(rna.pos.end)) tpos.end = len + (rna.pos.end - range.begin);
        else if (tpos.end == -1 && rna.pos.end < range.begin) tpos.end = len - 1;
        // spliced seq
        tmp[0] += rna.substitution.ori.substring(range.begin, range.length(true));
        tmp[1] += rna.substitution.alt.substring(range.begin, range.length(true));
        len += range.length(true);
    }
    if (tpos.end == -1) tpos.end = len - 1;
    rna.substitution.ori = tmp[0];
    rna.substitution.alt = tmp[1];
    if (dir) {
        // Rev. pos
        rna.pos.begin = len - tpos.end - 1;
        rna.pos.end = len - tpos.begin - 1;
        // Complement
        slib::sbio::sna::toComplement(rna.substitution.ori);
        slib::sbio::sna::toComplement(rna.substitution.alt);
    }
    else rna.pos = tpos;
}
inline void _translate(slib::sbio::AASubstitution& subst, const slib::sbio::CODON_TABLE& codon) {
    subst.ori = slib::sbio::sdna::transcript(subst.ori);
    subst.ori = slib::sbio::srna::protein(subst.ori, codon);
    subst.alt = slib::sbio::sdna::transcript(subst.alt);
    subst.alt = slib::sbio::srna::protein(subst.alt, codon);
}
// SNV|MNV
// nonsense | missense | synonymous
inline void checkSubstitution(slib::sbio::TranscriptAnnotData& data) {
    data.substitution.pos = srange(data.pos.begin / 3, data.pos.end / 3);
    if (data.substitution.pos.begin == 0 && data.substitution.alt[0] != 'M') 
        data.mutation = slib::sbio::FMET_LOSS;
    else {
        auto o = &data.substitution.ori[data.substitution.pos.begin],
            a = &data.substitution.alt[data.substitution.pos.begin];
        int i = 0;
        bool aachange = false;
        while(i < data.substitution.pos.length(true)) {
            if (*o != *a) { aachange = true; break; }
            ++o; ++a; ++i;
        }
        if (aachange) {
            data.substitution.ori.clip(data.substitution.pos.begin + i, data.substitution.pos.length(true) - i);
            data.substitution.alt.clip(data.substitution.pos.begin + i, data.substitution.pos.length(true) - i);
            // Check stop codon
            auto stop = data.substitution.alt.find("*");
            if (stop == slib::NOT_FOUND) data.mutation = slib::sbio::MISSENSE;
            else {
                data.mutation = slib::sbio::NONSENSE;
                data.substitution.ori.resize(stop);
                data.substitution.alt.resize(stop);
            }
        }
        else {
            data.mutation = slib::sbio::SYNONYMOUS;
            data.substitution.ori.clear();
            data.substitution.alt.clear();
        }
    }
    data.pos.shift(1);
    data.substitution.pos.shift(1);
}
// DEL | INS
// inframe | frameshift
inline void checkInDel(slib::sbio::TranscriptAnnotData& data, bool frame) {
    data.substitution.pos = srange(data.pos.begin / 3, data.pos.end / 3);
    if (data.mutation == slib::sbio::HEAD_LESION) {
        data.substitution.ori.clear();
        data.substitution.alt.clear();
    }
    else {
        //data.substitution.ori.clip(data.substitution.pos.begin);
        //data.substitution.alt.clip(data.substitution.pos.begin);

        if (frame) {
            data.mutation = slib::sbio::IN_FRAME;
            auto aend = data.substitution.alt.size() - (data.substitution.ori.size() - data.substitution.pos.end);
            
            if (aend < data.substitution.pos.begin) {

                SPrint("debug");

            }
            
            data.substitution.ori.clip(data.substitution.pos.begin, data.substitution.pos.length(true));
            data.substitution.alt.clip(data.substitution.pos.begin, aend - data.substitution.pos.begin + 1);
        }
        else {
            data.mutation = slib::sbio::FRAME_SHIFT;
            data.substitution.ori.clip(data.substitution.pos.begin);
            data.substitution.alt.clip(data.substitution.pos.begin);
            if (2 < data.substitution.ori.size()) data.substitution.ori.resize(2);
        }
        sfor(data.substitution.alt) {
            if ($_ == '*') {
                data.substitution.alt.resize($INDEX(data.substitution.alt) + 1);
                data.mutation = slib::sbio::FRAME_SHIFT;
                break;
            }
        }
    }
    data.pos.shift(1);
    data.substitution.pos.shift(1);
}
inline void setAAVar(slib::sbio::Variant& var, slib::sbio::TranscriptAnnotData& data, slib::sbio::TranscriptInfo& rna,
    const slib::sbio::SeqList& ref, const slib::sbio::CODON_TABLE& codon, const slib::sbio::CODON_TABLE& codon2) {
    auto cds = rna.coding();
    //
    data.substitution.prefix = 'p';
    // Raw genomic seq
    data.substitution.ori = ref[rna.gene->idx].raw(srange(cds[0].begin - 1, cds[-1].end - 1));
    data.substitution.alt = data.substitution.ori;
    // 
    data.pos = var.pos[0];
    slib::String alt = var.alt;
    // 
    if ((var.type == slib::sbio::SNV) || (var.type == slib::sbio::MNV)) {
        // Remove UTR variation
        if (var.pos[0].begin < cds[0].begin) {
            data.pos.begin = cds[0].begin; alt.remove(0, cds[0].begin - var.pos[0].begin);
        }
        if (cds[-1].end < var.pos[0].end) {
            data.pos.end = cds[-1].end; alt.resize(alt.size() - (var.pos[0].end - cds[-1].end));
        }
        // CDS(+1) = 0
        data.pos.shift(-cds[0].begin);
        // Replace genomic fragment
        data.substitution.alt.replace(data.pos.begin, data.pos.length(true), alt);
        // Splicing
        cds.shift(-cds[0].begin);
        _splice(data, cds, rna.gene->dir);
        // Translate with normal/mito. codons
        if (ref[rna.gene->idx].attribute["type"].intValue() & slib::sbio::MITO_GENOME) _translate(data.substitution, codon2);
        else _translate(data.substitution, codon);
        // Mutation type
        checkSubstitution(data);
    }
    else if (var.type == slib::sbio::DELETION) {
        // Null mut
        if (var.pos[0].include(srange(cds[0].begin, cds[-1].end))) {
            data.pos = cds.range();
            data.mutation = slib::sbio::NULL_MUT;
        }
        else {
            // Remove UTR variation
            if (var.pos[0].begin < cds[0].begin) data.pos.begin = cds[0].begin;
            if (cds[-1].end < var.pos[0].end) data.pos.end = cds[-1].end;
            // CDS(+1) = 0
            data.pos.shift(-cds[0].begin);
            // Splicing
            cds.shift(-cds[0].begin);
            _splice(data, cds, rna.gene->dir);
            // Check 1st and last base
            if (data.pos.begin == 0) data.mutation = slib::sbio::HEAD_LESION;
            else if (data.pos.end == cds.length(true) - 1) data.mutation = slib::sbio::TAIL_LESION;
            // Deletion and insertion 
            if (rna.gene->dir) data.substitution.alt.replace(data.pos.begin, data.pos.length(true), slib::sbio::sna::complement(var.alt));
            else data.substitution.alt.replace(data.pos.begin, data.pos.length(true), var.alt);
            // Translate with normal/mito. codons
            if (ref[rna.gene->idx].attribute["type"].intValue() & slib::sbio::MITO_GENOME) _translate(data.substitution, codon2);
            else _translate(data.substitution, codon);
            // Mutation type
            checkInDel(data, (std::abs(data.pos.length(true) - (int)var.alt.size()) % 3) == 0);
        }
    }
    else if (var.type == slib::sbio::INSERTION) {
        // CDS(+1) = 0
        cds.shift(-cds[0].begin);
        // Splicing
        _splice(data, cds, rna.gene->dir);
        // Insertion 
        if (rna.gene->dir) data.substitution.alt.insert(data.pos.begin, slib::sbio::sna::complement(var.alt));
        else data.substitution.alt.insert(data.pos.begin, var.alt);
        // Translate with normal/mito. codons
        if (ref[rna.gene->idx].attribute["type"].intValue() & slib::sbio::MITO_GENOME) _translate(data.substitution, codon2);
        else _translate(data.substitution, codon);
        // Mutation type
        checkInDel(data, ((int)var.alt.size() % 3) == 0);
    }
    else if (var.type == slib::sbio::DUPLICATION) {
        if (var.pos[0].include(cds.range())) data.mutation = slib::sbio::MULTI_COPY;
        else {
            if (var.flag & slib::sbio::SMALL_VARIANT) {
                // CDS(+1) = 0
                cds.shift(-cds[0].begin);
                // Splicing
                _splice(data, cds, rna.gene->dir);
                if (rna.gene->dir) 
                    data.substitution.alt.insert(data.pos.begin - 1, slib::sbio::sna::complement(var.alt) + ref[rna.gene->idx].raw(var.pos[0].begin, var.pos[0].length(true), rna.gene->dir));
                else
                    data.substitution.alt.insert(data.pos.begin - 1, var.alt + ref[rna.gene->idx].raw(var.pos[0].begin, var.pos[0].length(true), rna.gene->dir));
                // Translate with normal/mito. codons
                if (ref[rna.gene->idx].attribute["type"].intValue() & slib::sbio::MITO_GENOME) _translate(data.substitution, codon2);
                else _translate(data.substitution, codon);
                // Mutation type
                checkInDel(data, (((int)var.alt.size() + var.pos[0].length(true)) % 3) == 0);
            }
            else data.mutation = slib::sbio::COPY_MUT;
        }
    }
}
inline void setVarSite(slib::sbio::Variant& var, slib::sbio::TranscriptAnnotData &data, slib::sbio::TranscriptInfo &rna, int sz) {
    // Exon/CDS/UTR check
    sforeach(structure, rna.structures) {
        if (structure.overlap(var.pos[0])) data.site |= structure.type;
    }
    // Intron check
    auto introns = rna ^ rna.exons();
    sforeach(intron, introns) {
        if (intron.overlap(var.pos[0])) {
            if (var.pos[0].overlap(srange(intron.begin, intron.begin + sz)) ||
                var.pos[0].overlap(srange(intron.end - sz, intron.end))) {
                data.site |= slib::sbio::SPLICE_SITE;
            }
            else data.site |= slib::sbio::INTRON;
        }
    }
}

void slib::sbio::AnnotDB::annotate(slib::sbio::Variant& var, const slib::sbio::SeqList& ref, const slib::sbio::VarParam& par,
    const slib::sbio::CODON_TABLE& codon, const slib::sbio::CODON_TABLE& codon2) {
    if (var.annotation.empty()) {
        auto& genes1 = selectGenes(var.pos[0]);
        var.annotation.resize(genes1.size());
        sfor2(var.annotation, genes1) {
            $_1.gid = $_2->geneid;
            $_1.name = $_2->name;
            $_1.type = (GENE_TYPE)$_2->type;
            sforeach(rna, $_2->transcripts) {
                if (var.pos[0].overlap(*rna)) {
                    TranscriptAnnotData dat;
                    dat.name = rna->name;
                    dat.type = (TRANSCRIPT_TYPE)rna->type;
                    setVarSite(var, dat, *rna, par.splice_reg);
                    // Simple variant
                    if (var.pos[1].idx == -1) {
                        // CDS
                        if (dat.site & CDS) setAAVar(var, dat, *rna, ref, codon, codon2);
                        // Non CDS
                        else {
                            // In mRNA
                            if (dat.type == TRANSCRIPT_TYPE::M_RNA) {
                                dat.substitution.prefix = 'c';
                            }
                            // In ncRNA
                            else {
                                dat.substitution.prefix = 'g';
                            }
                            //
                            if (var.type == SNV || var.type == MNV) {
                                dat.mutation = SUBSTITUTION;
                                dat.substitution.ori = var.attribute.hasKey("_ref_") ? var.attribute["_ref_"].toString() : ref[var.pos[0].idx].raw(var.pos[0].begin - 1, var.pos[0].length(true));
                                dat.substitution.alt = var.alt;
                            }
                            else if (var.type == DELETION) {
                                if (var.pos[0].include(*rna)) dat.mutation = NULL_MUT;
                                else if (var.pos[0].include(rna->begin))
                                    dat.mutation = ($_2->dir ? TAIL_LESION : HEAD_LESION);
                                else if (var.pos[0].include(rna->end))
                                    dat.mutation = ($_2->dir ? HEAD_LESION : TAIL_LESION);
                                else dat.mutation = INDEL;
                            }
                            else if (var.type == DUPLICATION || var.type == MULTIPLICATION) {
                                if (var.pos[0].include(*rna)) dat.mutation = MULTI_COPY;
                                else dat.mutation = INDEL;
                            }
                            else if (var.type == INSERTION) dat.mutation = INDEL;
                        }
                    }
                    else dat.mutation |= GENE_SPLIT;
                    $_1.transcripts.add(dat);
                }
            }
        }
        if (-1 < var.pos[1].idx) {
            auto& genes2 = selectGenes(var.pos[1]);
            sfor(genes2) {
                GeneAnnotData gdat;
                gdat.gid = $_->geneid;
                gdat.name = $_->name;
                gdat.type = (GENE_TYPE)$_->type;
                sforeach(rna, $_->transcripts) {
                    if (var.pos[1].overlap(*rna)) {
                        TranscriptAnnotData tdat;
                        tdat.name = rna->name;
                        tdat.type = (TRANSCRIPT_TYPE)rna->type;
                        setVarSite(var, tdat, *rna, par.splice_reg);
                        if (var.type & INSERTION) tdat.mutation |= REARRANGE_MUT;
                        else if (var.type & TRANSLOCATION) tdat.mutation |= GENE_SPLIT;
                        else if (var.type & INVERSION) tdat.mutation |= REARRANGE_MUT;
                        gdat.transcripts.add(tdat);
                    }
                }
                var.annotation.add(gdat);
            }
        }
    }
    //
    if (!_load[1]) loadMutations();
    if (!_load[2]) loadVariants();
    var.attribute["mutant"] = SArray();
    sfor(_varIndex[var.pos[0].idx]) {
        if (var.pos[0].end < $_->begin) break;
        if (var.pos[0].overlap(*$_)) {
            var.attribute["mutant"].add($_->name);
            if (var.varid.empty() || var.varid == ".") {
                if (var.flag & SMALL_VARIANT) {
                    if (var.type == ($_->type & 0xFF) &&
                        var.pos[0].begin == $_->begin &&
                        var.pos[0].end == $_->end &&
                        var.alt == $_->attribute["alt"].toString()) {
                        var.varid = $_->name;
                        break;
                    }
                }
                else if (var.flag & SR_VARIANT) {
                    if ((var.type & ($_->type & 0xFF)) &&
                        std::abs(var.pos[0].begin-$_->begin) <= par.svp.max_gap &&
                        //par.svp.max_dist <= $_->length() &&
                        std::abs(var.pos[0].length() - $_->length()) <= par.svp.max_gap) {
                        var.varid = $_->name;
                    }
                }
            }
        }
    }
}

/*

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
            _trs_index[E_.idx][_bin_order[E_.idx][(int)sbiutil::getBin(E_)]].add(&E_);
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
            stringarray tids = String::dequot(row["TRANSCRIPT_ID"]).split(",");
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
            _mut_index[E_.idx][_bin_order[E_.idx][(int)sbiutil::getBin(E_)]].add(&E_);
            _mut_name_index.add(r);
            ++r;
        }
        commit();
        std::sort(_mut_name_index.begin(), _mut_name_index.end(),
                  [this](const sint &i1, const sint &i2) {
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
            _var_index[E_.idx][_bin_order[E_.idx][(int)sbiutil::getBin(E_)]].add(&E_);
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
            _ftr_index[E_.idx][_bin_order[E_.idx][(int)sbiutil::getBin(E_)]].add(&E_);
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
}template<class InfoVec>
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
inline void searchPos(const sbpos &pos, Array<Array<CArray<Info *>>> &index, Map<sint, suint> &order, CArray<Info *> &array) {
    ushortarray bins;
    sbiutil::getBins(bins, pos);
    sforeachi(bins) {
        if (index[pos.idx][order[(int)bins[i]]].empty()) continue;
        sforeach(index[pos.idx][order[(int)bins[i]]]) {
            auto info = static_cast<Info *>(E_);
            if (info->overlap(pos)) array.add(info);
        }
    }
	array.sort([](const Info* i1, const Info *i2) {
		if (i1->idx != i2->idx) return i1->idx < i2->idx;
		else if (i1->begin != i2->begin) return i1->begin < i2->begin;
		else if (i1->end != i2->end) return i1->end < i2->end;
		return i2->dir ? true : false;
		});
}
void SBAnnotDB::ctgInfo(ctgparray &array, const sbpos &pos) {
    if (!append) array.clear();
    if (_mode&LOAD_CTG) searchPos(pos, _ctg_index, _bin_order[pos.idx], array);
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
void SBAnnotDB::ctgInfo(ctgparray &array, const char *name, subyte match) {
	if (!append) array.clear();
    if (_mode&LOAD_CTG) {
        if (match == EXACT_MATCH) {
            srange range(0, (sint)_ctg_name_index.size());
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
            auto condition = NAME_CONDITION(sql::value(name, true, match));
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
void SBAnnotDB::geneInfo(geneparray &array, const sbpos &pos, bool trans) {
	if (!append) array.clear();
    if (_mode&LOAD_GENE) {
        if (trans && !(_mode&LOAD_TRANS)) setMode(_mode|LOAD_TRANS);
        searchPos(pos, _gene_index, _bin_order[pos.idx], array);
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
                            auto struct_ids = String::dequot(E_["STRUCTURE"]).split(",");
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
void SBAnnotDB::geneInfo(geneparray &array, const char *name, bool trans, subyte match) {
	if (!append) array.clear();
    if (_mode&LOAD_GENE) {
        if (!(_mode&LOAD_TRANS) && trans) setMode(_mode|LOAD_TRANS);
        if(match == EXACT_MATCH) {
            srange range(0, (sint)_gene_name_index.size());
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
            auto que = sql::value(name, true, match),
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
                            auto struct_ids = String::dequot(E_["STRUCTURE"]).split(",");
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
void SBAnnotDB::nearestGeneInfo(geneparray& array, const sbpos& pos, size_t range, bool trans) {
	if (!append) array.clear();
	sbpos pos_ = pos; pos_.begin -= range; pos_.end += range;
	if (_mode & LOAD_GENE) {
		if (trans && !(_mode & LOAD_TRANS)) setMode(_mode | LOAD_TRANS);
		searchPos(pos_, _gene_index, _bin_order[pos_.idx], array);
	}
	else {
		try {
			auto condition = RANGE_CONDITION(pos_);
			genes.resize((*this)["GENE"].count(condition));
			if (genes.empty()) return;
			integerarray gene_ids;
			(*this)["GENE"].getRecordPrepare({ "*" }, condition, START_ASC_QUE);
			sforeach(genes) {
				auto& row = getRow();
				toGeneInfo(&E_, row);
				gene_ids.add(row["ID"]);
				array.add(&E_);
			}
			commit();
			if (trans) {
				transcripts.clear();
				auto condition = sql::condition("GENE_ID IN (" + slib::toString(gene_ids, ",") + ")");
				transcripts.resize((*this)["TRANSCRIPT"].count(condition));
				if (transcripts.empty()) return;
				int t = 0;
				sforeachi(gene_ids) {
					auto& tarray = (*this)["TRANSCRIPT"].getRecords({ "*" }, sql::condition(String("GENE_ID=") << gene_ids[i]), START_ASC_QUE);
					if (tarray.empty()) continue;
					sforeach(tarray) {
						toAnnotInfo(&transcripts[t], E_.dict());
						transcripts[t].gene = &genes[i];
						genes[i].addTranscript(&transcripts[t]);
						if (E_["STRUCTURE"]) {
							auto struct_ids = String::dequot(E_["STRUCTURE"]).split(",");
							sforeachi_(s, struct_ids) {
								auto& row = (*this)["STRUCTURE"].getRecordAt(struct_ids[s].intValue());
								struct_info si;
								toAnnotInfo(&si, row);
								transcripts[t].structures.add(si);
							}
						}
						++t;
					}
				}
			}
		}
		catch (SDBException de) {
			de.print();
		}
	}
}
void SBAnnotDB::transcriptInfo(trsparray &array, const sbpos &pos, bool gene) {
	if (!append) array.clear();
    if (_mode&LOAD_TRANS) {
        if (gene & !(_mode&LOAD_GENE)) setMode(_mode|LOAD_GENE);
        searchPos(pos, _trs_index, _bin_order[pos.idx], array);
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
                        auto struct_ids = String::dequot(E_["STRUCTURE"]).split(",");
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
void SBAnnotDB::transcriptInfo(trsparray &array, const char *name, bool gene, subyte match) {
	if (!append) array.clear();
    if (_mode&LOAD_TRANS) {
        if (match == EXACT_MATCH) {
            srange range(0,(sint) _trs_name_index.size());
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
				auto condition = NAME_CONDITION(sql::value(name, true, match));
                transcripts.resize((*this)["TRANSCRIPT"].count(condition));
                if (transcripts.empty()) return;
                auto &tarray = (*this)["TRANSCRIPT"].getRecords({"*"}, condition, ID_ASC_QUE);
                if (tarray.empty()) return;
                int t = 0;
                sforeach(tarray) {
                    toAnnotInfo(&transcripts[t], E_.dict());
                    if (E_["STRUCTURE"]) {
                        auto struct_ids = String::dequot(E_["STRUCTURE"]).split(",");
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
void SBAnnotDB::mutantInfo(mutparray &array, const sbpos &pos) {
	if (!append) array.clear();
    if (_mode&LOAD_MUT) searchPos(pos, _mut_index, _bin_order[pos.idx], array);
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
void SBAnnotDB::mutantInfo(mutparray &array, const char *name, subyte match) {
	if (!append) array.clear();
    if (_mode&LOAD_MUT) {
        if (match == EXACT_MATCH) {
            srange range(0, (sint)_mut_name_index.size());
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
			auto condition = NAME_CONDITION(sql::value(name, true, match));
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
void SBAnnotDB::variationInfo(mutparray&array, const sbpos &pos) {
	if (!append) array.clear();
    if (_mode&LOAD_VAR) searchPos(pos, _var_index, _bin_order[pos.idx], array);
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
void SBAnnotDB::variationInfo(mutparray &array, const char *name, subyte match) {
    array.clear();
    if (_mode&LOAD_VAR) {
        if (match == EXACT_MATCH) {
            srange range(0, (sint)_var_name_index.size());
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
			auto condition = NAME_CONDITION(sql::value(name, true, match));
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
void SBAnnotDB::featureInfo(ftrparray &array, const sbpos &pos) {
	if (!append) array.clear();
    if (_mode&LOAD_FTR) searchPos(pos, _ftr_index, _bin_order[pos.idx], array);
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
void SBAnnotDB::featureInfo(ftrparray &array, const char *name, subyte match) {
	if (!append) array.clear();
    if (_mode&LOAD_FTR) {
		if (match == EXACT_MATCH) {
            srange range(0, _ftr_name_index.size());
            searchNameIndex<ftrarray>(range, name, _ftr_name_index, features);
			if (range.end - range.begin) return array.add(&features[_ftr_name_index[range.begin]]);
        }
        else {
            Regex reg("/"+String::lower(name)+"/i");
            sforeach(features) { if (String(E_.name).match(reg)) array.add(&E_); }
        }
    }
    else {
        features.clear();
        try {
			auto condition = NAME_CONDITION(sql::value(name, true, match));
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
*/

slib::sbio::AASubstitution::AASubstitution() : prefix('\0') {}
slib::sbio::AASubstitution::~AASubstitution() {}
slib::sbio::AASubstitution& slib::sbio::AASubstitution::operator=(const slib::sbio::AASubstitution& sub) {
    prefix = sub.prefix; pos = sub.pos; ori = sub.ori; alt = sub.alt;
    return *this;
}
slib::sbio::TranscriptAnnotData::TranscriptAnnotData() { 
    type = TRANSCRIPT_TYPE::MISC_RNA; site = 0; mutation = 0; score = -1.f; effect = VARIANT_EFFECT::UNKNOWN;
}
slib::sbio::TranscriptAnnotData::TranscriptAnnotData(TranscriptInfo* ti) : TranscriptAnnotData() {
    type = (TRANSCRIPT_TYPE)ti->type;
    name = ti->name;
}
slib::sbio::TranscriptAnnotData::~TranscriptAnnotData() {}
slib::sbio::TranscriptAnnotData& slib::sbio::TranscriptAnnotData::operator=(const TranscriptAnnotData& data) {
    type = data.type;
    pos = data.pos;
    name = data.name;
    site = data.site;
    mutation = data.mutation;
    score = data.score;
    effect = data.effect;
    substitution = data.substitution;
    return *this;
}
slib::String slib::sbio::TranscriptAnnotData::mutType() const {
    if (mutation == NONSENSE) return "nonsense";
    else if (mutation == MISSENSE) return "missense";
    else if (mutation == FMET_LOSS) return "missense (1st Met)";
    else if (mutation == TERM_LOSS) return "missense (Term. codon)";
    else if (mutation & SUBSTITUTION) return (site & CDS) ? "synonymous" : "substitution";
    else if (mutation == FRAME_SHIFT) return "frame shift";
    else if (mutation == IN_FRAME) return site & INTRON ? "frame shift? (exon/intron junction mutation)" : "in frame";
    else if (mutation == HEAD_LESION) return site & CDS ? "Lesion of 1st Met" : "Lesion of 5' end";
    else if (mutation == TAIL_LESION) return site & CDS ? "Lesion of term. codon" : "Lesion of 3' end";
    else if (mutation & INDEL) return "indel";
    else if (mutation == NULL_MUT) return "null mutation";
    else if (mutation & COPY_MUT) return "copy number variant";
    else if (0 < mutation) return "complex";
    else return "";
}
slib::String slib::sbio::TranscriptAnnotData::mutFormat() const {
    if (substitution.prefix == 'p') {
        if (mutation == NONSENSE || mutation == MISSENSE || mutation == FMET_LOSS || mutation == TERM_LOSS)
            return S("p.") << substitution.ori << substitution.pos.begin << substitution.alt;
        else if (mutation == FRAME_SHIFT) 
            return S("p.") << substitution.ori << substitution.pos.begin << substitution.alt.substring(0, sstat::getMin(2, (int)substitution.alt.size())) << "fs";
        else if (mutation == IN_FRAME && !(site & INTRON)) {
            if (substitution.ori.size() < substitution.alt.size()) {
                return S("p.") << substitution.ori << substitution.pos.begin << substitution.alt.substring(0, sstat::getMin(2, (int)substitution.alt.size())) << "ins";
            }
            else {
                return S("p.") << substitution.ori << substitution.pos.begin << substitution.alt.substring(0, sstat::getMin(2, (int)substitution.alt.size())) << "del";
            }
        }
    }
    return "";
}
slib::sbio::GeneAnnotData::GeneAnnotData() { type = GENE_TYPE::MISC_GENE; }
slib::sbio::GeneAnnotData::GeneAnnotData(GeneInfo* gi) {
    type = (GENE_TYPE)gi->type;
    gid = gi->geneid;
    name = gi->name;
    sfor(gi->transcripts) transcripts.add($_);
}
slib::sbio::GeneAnnotData::~GeneAnnotData() {}
slib::sbio::GeneAnnotData& slib::sbio::GeneAnnotData::operator=(const GeneAnnotData& data) {
    type = data.type;
    gid = data.gid;
    name = data.name;
    transcripts = data.transcripts;
    return *this;
}
slib::String slib::sbio::GeneAnnotData::gtype() const {
    switch (type) {
    case GENE_TYPE::PROTEIN_CODING:
        return "protein coding";
    case GENE_TYPE::PSEUDO_GENE:
        return "pseudo gene";
    case GENE_TYPE::TRANSPOSON:
        return "transposon";
    case GENE_TYPE::NON_CODING:
    {
        sforeach(rna, transcripts) {
            if (rna.type == TRANSCRIPT_TYPE::T_RNA) return "tRNA";
            else if (rna.type == TRANSCRIPT_TYPE::R_RNA) return "rRNA";
            else if (rna.type == TRANSCRIPT_TYPE::MI_RNA) return "miRNA";
            else if (rna.type == TRANSCRIPT_TYPE::PI_RNA) return "piRNA";
        }
        return "non-coding";
    }
    default:
        return "misc.";
    }
}
slib::String slib::sbio::GeneAnnotData::varSite() const {
    subyte site = 0;
    sfor(transcripts) site |= $_.site;
    if (site & CDS) return "CDS";
    else if (site & UTR) {
        if ((site & UTR3) == UTR3) return "3'UTR";
        else if ((site & UTR3) == UTR5) return "5'UTR";
        return "UTR";
    }
    else if (site & EXON) return "exon";
    else if (site & INTRON) {
        if ((site & SPLICE_SITE) == SPLICE_SITE) return "intron(splice donor/acceptor)";
        else return "intron";
    }
    else return "intergenic";
}
