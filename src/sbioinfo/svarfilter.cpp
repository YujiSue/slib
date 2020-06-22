#include "sbioinfo/svariant.h"

using namespace slib;
using namespace slib::sbio;

inline void _annotCodingGene(SVariant* var, transcript_site& trs, transcript_info* ti) {
	auto structure = ti->messenger();
	sforeach(structure) {
		if (E_.overlap(var->pos[0])) trs.site |= E_.type;
		if (it < structure.end() - 1) {
			if (var->pos[0].overlap(srange(it->end + 1, (it + 1)->begin - 1))) {
				trs.site |= INTRON;
				if (var->pos[0].include(it->end + 1) || var->pos[0].include(it->end + 2) ||
					var->pos[0].include((it + 1)->begin - 2) || var->pos[0].include((it + 1)->begin - 1))
					trs.site |= SPLICE_SITE;
			}
		}
	}
}
inline void _annotNonCodingGene(SVariant* var, transcript_site& trs, transcript_info* ti) {
	auto region = ti->exonRegion();
	sforeach(region) {
		if (E_.overlap(var->pos[0])) trs.site |= EXON;
		if (it < region.end() - 1) {
			if (var->pos[0].overlap(srange(it->end + 1, (it + 1)->begin - 1))) {
				trs.site |= INTRON;
				if (var->pos[0].include(it->end + 1) || var->pos[0].include(it->end + 2) ||
					var->pos[0].include((it + 1)->begin - 2) || var->pos[0].include((it + 1)->begin - 1))
					trs.site |= SPLICE_SITE;
			}
		}
	}
}
inline void _splice(String& ori, String& alt, bool dir, sregion& reg, int& pos) {
	String otmp, atmp;
	otmp.reserve(ori.length() + 1);
	atmp.reserve(alt.length() + 1);
	sforeach(reg) {
		if (E_.include(pos)) pos = otmp.length() + pos - E_.begin;
		otmp += ori.substring(E_.begin, E_.length(true));
		atmp += alt.substring(E_.begin, E_.length(true));
	}
	ori.swap(otmp); alt.swap(atmp);
	if (dir) {
		pos = ori.length() - pos - 1;
		sseq::dcomp(ori.ptr(), ori.size());
		sseq::dcomp(alt.ptr(), alt.size());
	}
}
inline srange _removeN(String& seq) {
	srange removed;
	bool in = false;
	sforeach(seq) {
		if (!in && E_ == '-') { removed.begin = it - seq.begin(); in = true; }
		if (in && E_ != '-') { removed.end = it - seq.begin(); break; }
	}
	seq.remove(removed);
	return removed;
}
inline void _translate(String& ori, String& alt, CODON_TABLE* codon = &DEFAULT_CODON) {
	sseq::dencode1((subyte*)ori.ptr(), 0, ori.size(), (subyte*)ori.ptr());
	sseq::dencode1((subyte*)alt.ptr(), 0, alt.size(), (subyte*)alt.ptr());
	sseq::dtransi((subyte*)ori.ptr(), 0, ori.size(), (subyte*)ori.ptr());
	sseq::dtransi((subyte*)alt.ptr(), 0, alt.size(), (subyte*)alt.ptr());
	sseq::atrans((subyte*)ori.ptr(), 0, ori.size(), (subyte*)ori.ptr(), *codon);
	sseq::atrans((subyte*)alt.ptr(), 0, alt.size(), (subyte*)alt.ptr(), *codon);
	sseq::adecode((subyte*)ori.ptr(), 0, ori.size(), (subyte*)ori.ptr());
	sseq::adecode((subyte*)alt.ptr(), 0, alt.size(), (subyte*)alt.ptr());
	ori.resize(ori.length() / 3);
	if (alt.length() % 3) alt.resize(alt.length() / 3 + 1);
	else alt.resize(alt.length() / 3);
}

inline void _compareMNV(String& ori, String& alt, transcript_site& trs, int offset, int len) {
	auto o_ = ori.ptr(offset), a_ = alt.ptr(offset);
	trs.pos = offset + 1;
	int i = 0;
	while (i < len) {
		if (*o_ == *a_) { ++i; ++trs.pos; ++o_; ++a_; }
		else break;
	}
	if (i < len) {
		sforin(j, i, len) {
			trs.ori.add(*o_); trs.alt.add(*a_);
			if (*a_ == '*') { trs.type = NONSENSE; break; }
			else trs.type = MISSENSE;
			++o_; ++a_;
		}
	}
}
inline void _compareDEL(String& ori, String& alt, transcript_site& trs, int offset, int len) {
	auto o_ = ori.ptr(offset), a_ = alt.ptr(offset);
	trs.pos = offset + 1;
	if (trs.type == IN_FRAME) {
		if (*o_ != *a_ || *(o_ + len + 1) != *(a_ + 1)) trs.type = FRAME_SHIFT;
	}
	trs.ori.add(*o_); trs.alt.add(*a_);
	++o_; ++a_;
	trs.ori.add(*o_); trs.alt.add(*a_);
}
inline void _compareINS(String& ori, String& alt, transcript_site& trs, int offset, int len) {
	auto o_ = ori.ptr(offset), a_ = alt.ptr(offset);
	trs.pos = offset + 1;
	int i = 0;
	trs.ori.add(*o_);
	while (i < len) {
		trs.alt.add(*a_);
		if (*a_ == '*') {
			if (trs.type == IN_FRAME) trs.type = FRAME_SHIFT;
			break;
		}
		++a_; ++i;
	}
}
inline void _annotateMNV(CArray<gene_info*>& genes, String& ori, String& alt, SVariant* var, SBSeqList* ref) {
	sforin(i, 0, genes.size()) {
		var->genes[i] = gene_site(genes[i]);
		auto& gene = var->genes[i];
		sforeach(genes[i]->transcripts) {
			gene.transcripts.add(E_);
			auto& trs = gene.transcripts.last();
			if (E_->type == M_RNA) {
				_annotCodingGene(var, trs, E_);
				if (trs.site & CDS) {
					auto region = E_->codingRegion(); region.shift(-E_->begin);
					ori = ref->at(var->pos[0].idx)->raw(E_->begin - 1, E_->length(true) + 1);
					alt = ori;
					int off = var->pos[0].begin - E_->begin;
					sforin(i, 0, var->alt.size()) {
						if (off + i < 0 || E_->length(true) < off + i) continue;
						alt[off + i] = var->alt[i];
					}
					_splice(ori, alt, gene.dir, region, off);
					_translate(ori, alt);
					auto last = off + var->alt.length() - 1;
					if (off < 0) off = 0;
					_compareMNV(ori, alt, trs, off / 3, (last / 3 - off / 3) + 1);
				}
			}
			else {
				_annotNonCodingGene(var, trs, E_);
				if (trs.site & EXON) trs.type = SUBSTITUTION;
			}
			gene.type |= trs.type;
		}
	}
}
inline void _annotateDEL(CArray<gene_info*>& genes, String& ori, String& alt, SVariant* var, SBSeqList* ref) {
	sforin(i, 0, genes.size()) {
		var->genes[i] = gene_site(genes[i]);
		auto& gene = var->genes[i];
		sforeach(genes[i]->transcripts) {
			gene.transcripts.add(E_);
			auto& trs = gene.transcripts.last();
			if (E_->type == M_RNA) {
				_annotCodingGene(var, trs, E_);
				if (trs.site & CDS) {
					auto region = E_->codingRegion(); region.shift(-E_->begin);
					ori = ref->at(var->pos[0].idx)->raw(E_->begin - 1, E_->length(true) + 1);
					alt = ori;
					int off = var->pos[0].begin - E_->begin;
					sforin(i, 0, var->pos[0].length(true)) {
						if ((off + i) < 0 || alt.length() <= (off + i)) continue;
						alt[off + i] = '-';
					}
					_splice(ori, alt, gene.dir, region, off);
					auto del = _removeN(alt);
					auto len = ori.length() - alt.length();
					if (alt.size()) {
						if (var->pos[0].begin < E_->begin) trs.type = NTERM_DEL;
						else if (E_->end < var->pos[0].end) trs.type = CTERM_DEL;
						else {
							if (len % 3) {
								trs.type = FRAME_SHIFT;
								_translate(ori, alt);
								_compareDEL(ori, alt, trs, off / 3, len / 3 + 1);
							}
							else {
								trs.type = IN_FRAME;
								_translate(ori, alt);
								_compareDEL(ori, alt, trs, off / 3, len / 3);
							}
						}
					}
					else trs.type = NULL_MUT;
				}
			}
			else {
				_annotNonCodingGene(var, trs, E_);
				trs.type = INDEL;
			}
			gene.type |= trs.type;
		}
	}
}
inline void _annotateMUL(CArray<gene_info*>& genes, String& ori, String& alt, SVariant* var, SBSeqList* ref) {
	sforin(i, 0, genes.size()) {
		var->genes[i] = gene_site(genes[i]);
		auto& gene = var->genes[i];
		sforeach(genes[i]->transcripts) {
			gene.transcripts.add(E_);
			auto& trs = gene.transcripts.last();
			if (E_->type == M_RNA) {
				_annotCodingGene(var, trs, E_);
				if (trs.site & CDS) {
					auto region = E_->codingRegion(); region.shift(-E_->begin);
					ori = ref->at(var->pos[0].idx)->raw(E_->begin - 1, E_->length(true) + 1);
					alt = ori;
					int off = var->pos[0].begin - E_->begin;
					_splice(ori, alt, gene.dir, region, off);
					auto mul = ref->at(var->pos[0].idx)->raw(var->pos[0].begin, var->pos[0].length(true));
					if (gene.dir) alt.insert(off, sseq::dcompseq(mul));
					else alt.insert(off, mul);
					auto len = alt.length() - ori.length();
					if (len % 3) {
						trs.type = FRAME_SHIFT;
						_translate(ori, alt);
						_compareINS(ori, alt, trs, off / 3, len / 3 + 1);
					}
					else {
						trs.type = IN_FRAME;
						_translate(ori, alt);
						_compareINS(ori, alt, trs, off / 3, len / 3);
					}
				}
			}
			else {
				_annotNonCodingGene(var, trs, E_);
				trs.type = INDEL;
			}
			gene.type |= trs.type;
		}
	}
}
inline void _annotateINS(CArray<gene_info*>& genes, String& ori, String& alt, SVariant* var, SBSeqList* ref) {
	sforin(i, 0, genes.size()) {
		var->genes[i] = gene_site(genes[i]);
		auto& gene = var->genes[i];
		sforeach(genes[i]->transcripts) {
			gene.transcripts.add(E_);
			auto& trs = gene.transcripts.last();
			if (E_->type == M_RNA) {
				_annotCodingGene(var, trs, E_);
				if (trs.site & CDS) {
					auto region = E_->codingRegion(); region.shift(-E_->begin);
					ori = ref->at(var->pos[0].idx)->raw(E_->begin - 1, E_->length(true) + 1);
					alt = ori;
					int off = var->pos[0].begin - E_->begin;
					_splice(ori, alt, gene.dir, region, off);
					if (gene.dir) alt.insert(off, sseq::dcompseq(var->alt));
					else alt.insert(off, var->alt);
					auto len = alt.length() - ori.length();
					if (len % 3) {
						trs.type = FRAME_SHIFT;
						_translate(ori, alt);
						_compareINS(ori, alt, trs, off / 3, len / 3 + 1);
					}
					else {
						trs.type = IN_FRAME;
						_translate(ori, alt);
						_compareINS(ori, alt, trs, off / 3, len / 3);
					}
				}
			}
			else {
				_annotNonCodingGene(var, trs, E_);
				trs.type = INDEL;
			}
			gene.type |= trs.type;
		}
	}
}

inline void _annotate(SVariant* var, SBSeqList* ref, SBAnnotDB* db) {
	CArray<gene_info*> genes;
	CArray<mut_info*> mutants;
	String ori, alt;
	if (var->type & INSERTION && -1 < var->pos[1].idx) {  // complex var.
		db->geneInfo(genes, var->pos[0]);
		var->genes.resize(genes.size());

		sforin(i, 0, genes.size()) {
			var->genes[i] = gene_site(genes[i]);
			auto& gene = var->genes[i];
			sforeach(genes[i]->transcripts) {
				gene.transcripts.add(E_);
				auto& trs = gene.transcripts.last();
				if (E_->type == M_RNA) {
					_annotCodingGene(var, trs, E_);
					if (trs.site & CDS) {
						auto region = E_->codingRegion(); region.shift(-E_->begin);
						ori = ref->at(var->pos[0].idx)->raw(E_->begin - 1, E_->length(true) + 1);
						alt = ori;
						int off = var->pos[0].begin - E_->begin;
						sforin(i, 0, var->pos[0].length(true)) {
							if ((off + i) < 0 || alt.length() <= (off + i)) continue;
							alt[off + i] = '-';
						}
						_splice(ori, alt, gene.dir, region, off);
						auto del = _removeN(alt);
						auto len = ori.length() - alt.length();
						if (alt.size()) {
							if (var->pos[0].begin < E_->begin) trs.type = NTERM_DEL;
							else if (E_->end < var->pos[0].end) trs.type = CTERM_DEL;
							else {
								if (len % 3) {
									trs.type = FRAME_SHIFT;
									_translate(ori, alt);
									_compareDEL(ori, alt, trs, off / 3, len / 3 + 1);
								}
								else {
									trs.type = IN_FRAME;
									_translate(ori, alt);
									_compareDEL(ori, alt, trs, off / 3, len / 3);
								}
							}
						}
						else trs.type = NULL_MUT;
					}
				}
				else {
					_annotNonCodingGene(var, trs, E_);
					trs.type = INDEL;
				}
				gene.type |= trs.type;
			}
		}

		db->mutantInfo(mutants, var->pos[0]);
		if (!mutants.empty()) { sforeach_(mit, mutants) var->mutants.add((*mit)->name); }
	}
	else if (var->type < 0x40) {  // simple var.
		db->geneInfo(genes, var->pos[0]);
		var->genes.resize(genes.size());
		if (var->type == SNV || var->type == MNV)
			_annotateMNV(genes, ori, alt, var, ref);
		else if (var->type == DELETION)
			_annotateDEL(genes, ori, alt, var, ref);
		else if (var->type == DUPLICATION || var->type == MULTIPLICATION)
			_annotateMUL(genes, ori, alt, var, ref);
		else if (var->type == INSERTION)
			_annotateINS(genes, ori, alt, var, ref);
		db->mutantInfo(mutants, var->pos[0]);
		if (!mutants.empty()) { sforeach_(mit, mutants) var->mutants.add((*mit)->name); }
	}
	else {  // rearrangement
		if (var->type & TRANSLOCATION) {

		}
		else if (var->type & INVERSION) {

		}
	}
}


SVarFilter::SVarFilter(SBSeqList* ref, SBAnnotDB* db, svariant_param* p, Array<sregion>* t) : _ref(ref), _db(db), _par(p), _target(t) {}
SVarFilter::~SVarFilter() {}
void SVarFilter::consolidate(Array<svar_data>& variants) {
	if (variants.empty()) return;
	auto size = variants.size();
	variants.sort();
	sforeach(variants) {
		if (!(E_.type)) continue;
		auto it_ = it + 1;
		while (it_ < variants.end()) {
			if (!(E_.lt(*it_, _par->max_dist))) break;
			if (E_.equal(*it_, _par->max_dist)) {
				E_ += (*it_); it_->type = 0; --size;
			}
			++it_;
		}
	}
	variants.sort([](const svar_data& v1, const svar_data& v2) {
			if (!(v1.type)) return false;
			if (!(v2.type)) return true;
			return v1 < v2;
		});
	variants.resize(size);
}
void SVarFilter::subtract(Array<svar_data>& variants1, Array<svar_data>& variants2) {
	Array<std::pair<sbyte, svar_data*>> vec;
	size_t size = variants1.size();
	sforeach(variants1) vec.add(0, &E_);
	sforeach(variants2) vec.add(1, &E_);
	vec.sort([](const std::pair<sbyte, svar_data*>& p1, const std::pair<sbyte, svar_data*>& p2) {
		return *(p1.second) < *(p2.second);
		});
	sforeach(vec) {
		if (!E_.second) continue;
		auto it_ = it + 1;
		svar_data* vptr = nullptr;
		while (it_ < vec.end()) {
			if (E_.second->equal(*it_->second, _par->max_dist)) {
				if (!E_.first) vptr = E_.second;
				else if (!it_->first) vptr = it_->second;
				it_->second = nullptr;
			}
			else if (!(E_.second->lt(*it_->second, _par->max_dist))) break;
			++it_;
		}
		if (vptr) { vptr->type = 0; --size; }
	}
	variants1.sort([](const svar_data& v1, const svar_data& v2) {
		if (!(v1.type)) return false;
		if (!(v2.type)) return true;
		return v1 < v2;
		});
	variants1.resize(size);
}
void SVarFilter::merge(SVarList& vl1, SVarList& vl2) {
	auto dist = _par ? _par->max_dist : 0;
	sforeach(vl2.attribute) {
		if (!vl1.attribute.hasKey(E_.key())) vl1.attribute.insert(E_);
	}
	vl1.name += "_" + vl2.name + "_merged";
	vl1.append(vl2);
	auto size = vl1.size();
	vl1.sort();
	sforeach(vl1) {
		if (E_->flag & UNAVAILABLE_FLAG) continue;
		sforin(vit, it + 1, vl1.end()) {
			if ((*vit)->flag & UNAVAILABLE_FLAG) continue;
			if (E_->comparable(*vit) && (*vit)->lt(E_, dist)) {
				if (E_->equal(*vit, dist)) { (*vit)->flag |= UNAVAILABLE_FLAG; --size; }
			}
			else break;
		}
	}
	vl1.tidy(size);
}
void SVarFilter::subtract(SVarList& vl1, SVarList& vl2) {
	auto dist = _par ? _par->max_dist : 0;
	auto size = vl1.size();
	sforeach(vl2) E_->flag |= NOT_USE_FLAG;
	vl1.append(vl2);
	vl1.sort();
	sforeach(vl1) {
		if (E_->flag & UNAVAILABLE_FLAG) continue;
		sforin(vit, it + 1, vl1.end()) {
			if ((*vit)->flag & UNAVAILABLE_FLAG) continue;
			if (!((*vit)->lt(E_, dist))) break;
			if (E_->equal(*vit, dist)) {
				if (!(E_->flag & NOT_USE_FLAG)) { E_->flag |= UNAVAILABLE_FLAG; --size; }
				else if (!((*vit)->flag & NOT_USE_FLAG)) { (*vit)->flag |= UNAVAILABLE_FLAG; --size; }
			}
		}
	}
	vl1.tidy(size);
	sforeach(vl2) E_->flag -= NOT_USE_FLAG;
}
void SVarFilter::common(SVarList& vl1, SVarList& vl2) {
	auto dist = _par ? _par->max_dist : 0;
	auto size = vl1.size();
	sforeach(vl1) E_->flag |= UNAVAILABLE_FLAG;
	sforeach(vl2) E_->flag |= NOT_USE_FLAG;
	vl1.append(vl2);
	vl1.sort();
	sint count = 0;
	sforeach(vl1) {
		sforin(vit, it + 1, vl1.end()) {
			if (!((*vit)->lt(E_, dist))) break;
			if (E_->equal(*vit, dist)) {
				if (E_->flag & UNAVAILABLE_FLAG) { E_->flag -= UNAVAILABLE_FLAG; ++count; }
				else if ((*vit)->flag & UNAVAILABLE_FLAG) { (*vit)->flag -= UNAVAILABLE_FLAG; ++count; }
			}
		}
	}
	vl1.tidy(count);
	sforeach(vl2) E_->flag -= NOT_USE_FLAG;
}
void SVarFilter::unique(SVarList& uni, SVarList& vl1, SVarList& vl2) {
	uni = vl1;
	uni.name += "_" + vl1.name + "_uniq";
	SVarFilter::subtract(uni, vl2);
}

void SVarFilter::annotate(SVarList* vl, SWork* threads) {
	sforeach(*vl) {
		if (threads) threads->addTask(_annotate, E_, _ref, _db);
		else _annotate(E_, _ref, _db);
	}
	if (threads) threads->complete();
}
void SVarFilter::checkRepeat(SVarList* list, SBSeqList* ref) {
	sforin(i, 0, list->size()) {
		auto& vi = list->at(i);
		if (!(vi->flag & SMALL_VARIANT)) continue;
		String prev, post, mut, mut_;
		if (vi->pos[0].length(true) == 1) {
			if (vi->type == INSERTION) {
				prev = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 4, 3);
				post = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 1, 3);
				mut = String(vi->alt) * 3;
				if (prev == mut || post == mut) vi->attribute["repeat"] = true;
			}
			else if (vi->type == DELETION) {
				prev = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 3, 3);
				post = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 1, 3);
				mut = vi->attribute["Ref"].string() * 3;
				if (prev == mut || post == mut) vi->attribute["repeat"] = true;
			}
			else if (vi->type == SNV) {
				prev = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 4, 3);
				post = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin, 3);
				mut = String(vi->alt) * 3;
				if (prev == mut || post == mut) vi->attribute["repeat"] = true;
			}
		}
		else if (vi->pos[0].length(true) == 2) {
			if (vi->type == INSERTION) {
				if (vi->alt[0] == vi->alt[1]) {
					prev = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 6, 5);
					post = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 1, 5);
					mut = String(vi->alt[0]) * 5;
					if (prev == mut || post == mut) vi->attribute["repeat"] = true;
				}
				else {
					prev = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 4, 3);
					post = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 1, 3);
					mut = String(vi->alt[0]) * 3, mut_ = String(vi->alt[1]) * 3;
					if (prev == mut && post == mut_) vi->attribute["repeat"] = true;
				}
			}
			else if (vi->type == DELETION) {
				String seq = vi->attribute["Ref"];
				if (seq[0] == seq[1]) {
					prev = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 5, 5);
					post = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 1, 5);
					mut = String(seq[0]) * 5;
					if (prev == mut || post == mut) vi->attribute["repeat"] = true;
				}
				else {
					prev = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin - 3, 3);
					post = ref->at(vi->pos[0].idx)->raw(vi->pos[0].begin, 3);
					mut = String(seq[0]) * 3, mut_ = String(seq[1]) * 3;
					if (prev == mut && post == mut_) vi->attribute["repeat"] = true;
				}
			}
		}
	}
}

void SVarFilter::filter(SVarList* list) {
	size_t s = list->size();
	sforeach(*list) {
		check(E_);
		if (E_->flag & UNAVAILABLE_FLAG) --s;
	}
	list->tidy(s);
}
void SVarFilter::check(SVariant* var) {
	if (_ref && _ref->at(var->pos[0].idx)->isMasked()) {
		auto& mask = _ref->at(var->pos[0].idx)->mask();
		if (mask.include(var->pos[0])) var->flag |= UNAVAILABLE_FLAG;
	}
	if (_target && !_target->empty() && _target->at(var->pos[0].idx).include(var->pos[0])) var->flag |= UNAVAILABLE_FLAG;
}
void SVarFilter::setReference(SBSeqList* ref) { _ref = ref; }
void SVarFilter::setDB(SBAnnotDB* db) { _db = db; }
void SVarFilter::setParam(svariant_param* p) { _par = p; }
void SVarFilter::setTarget(Array<sregion>* reg) { _target = reg; }