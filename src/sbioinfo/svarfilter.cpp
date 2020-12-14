#include "sbioinfo/svariant.h"

using namespace slib;
using namespace slib::sbio;
inline void _splice(String* ori, String* alt, bool dir, sregion reg, int *pos, int *pos2 = nullptr) {
	String otmp, atmp;
	otmp.reserve(ori->size() + 1);
	atmp.reserve(alt->size() + 1);
	sforeach(reg) {
		if (E_.include(*pos)) (*pos) = otmp.length() + (*pos) - E_.begin;
		if (pos2 && E_.include(*pos2)) (*pos2) = otmp.length() + (*pos2) - E_.begin;
		otmp += ori->substring(E_.begin, E_.length(true));
		atmp += alt->substring(E_.begin, E_.length(true));
	}
	ori->swap(otmp); alt->swap(atmp);
	if (dir) {
		(*pos) = ori->size() - (*pos) - 1;
		if(pos2) (*pos2) = ori->size() - (*pos2) - 1;
		sseq::dcomp(ori->ptr(), ori->size());
		sseq::dcomp(alt->ptr(), alt->size());
	}
}
inline void _removeDeleted(String* seq) {
	sarr_iter<char> beg, end;
	sforeach(*seq) {
		if (E_ == '-') {
			beg = it; end = beg;
			while (E_ == '-' && it < seq->end()) { ++end; NEXT_; }
		}
	}
	seq->remove(beg - seq->begin(), end - beg);
}
inline void _translate(String* ori, String* alt, CODON_TABLE* codon = &DEFAULT_CODON) {
	subyte* op = (subyte*)ori->ptr(), * ap = (subyte*)alt->ptr();
	size_t os = ori->size(), as = alt->size();
	sseq::dencode1(op, 0, os, op); sseq::dencode1(ap, 0, as, ap);
	sseq::dtransi(op, 0, os, op); sseq::dtransi(ap, 0, as, ap);
	sseq::atrans(op, 0, os, op, *codon); sseq::atrans(ap, 0, as, ap, *codon);
	sseq::adecode(op, 0, os, op); sseq::adecode(ap, 0, as, ap);
	ori->resize(os / 3);
	if (as % 3) alt->resize(as / 3 + 1);
	else alt->resize(as / 3);
}
inline void _compareMNV(String* ori, String* alt, transcript_site* trs, sint off) {
	sint len = ori->size();
	char* oc = ori->ptr(off), * ac = alt->ptr(off);
	sforin(i, off, len) {
		if ((*oc) != (*ac)) {
			if (trs->pos < 0) { trs->mutation = MISSENSE; trs->pos = i + 1; }
			trs->ori << (*oc); trs->alt << (*ac);
			if ((*ac) == '*') { trs->mutation = NONSENSE; break; }
		}
		++oc; ++ac;
	}
}
inline void _compareDEL(String* ori, String* alt, transcript_site* trs, sint off) {
	if (trs->mutation == NULL_MUT) return;
	else if (trs->mutation == NTERM_LESION) {
		trs->pos = 1; trs->ori << ori->at(0); trs->alt << "-"; return;
	}
	else if (trs->mutation == CTERM_LESION) {
		trs->pos = off + 1; trs->ori << ori->at(off); trs->alt << "-"; return;
	}
	sint len = ori->size(), alen = alt->size();
	char* oc = ori->ptr(off), * ac = alt->ptr(off);
	srange range(off, alen - 1);
	while ((*oc) == (*ac) && range.begin < alen) { ++oc; ++ac; ++range.begin; }
	oc = &ori->last(); ac = &alt->last();
	while ((*oc) == (*ac) && off <= range.end) { --oc; --ac; --range.end; }
	trs->pos = range.begin + 1;
	if (range.end < range.begin) {
		trs->mutation |= IN_FRAME;
		trs->ori = ori->substring(range.begin, len - alen);
		trs->alt = "-";
	}
	else {
		trs->ori = ori->substring(range.begin, 1);
		trs->alt = alt->substring(range.begin, 1);
		if (range.begin == range.end && alt->at(range.begin) != '*') trs->mutation |= IN_FRAME;
		else trs->mutation |= FRAME_SHIFT;
	}
}
inline void _compareINS(String* ori, String* alt, transcript_site* trs, sint off) {
	sint len = ori->size(), alen = alt->size();
	char* oc = ori->ptr(off), * ac = alt->ptr(off);
	srange range(off, len - 1);
	while ((*oc) == (*ac) && range.begin < len) { ++oc; ++ac; ++range.begin; }
	oc = &ori->last(); ac = &alt->last();
	while ((*oc) == (*ac) && off <= range.end) { --oc; --ac; --range.end; }
	trs->pos = range.begin + 1;
	if (range.end < range.begin || (range.begin == range.end && alt->last() == '*')) {
		trs->ori = ori->substring(range.begin, 1);
		auto ac_ = alt->ptr(range.begin);
		bool term = false;
		while (ac_ <= ac) { 
			trs->alt << *ac_;
			if ((*ac_) == '*') { term = true; break; }
			++ac_;
		}
		if (term) trs->mutation |= FRAME_SHIFT;
		else trs->mutation |= IN_FRAME;
	}
	else {
		trs->mutation |= FRAME_SHIFT;
		trs->ori = ori->substring(range.begin, 1);
		trs->alt = alt->substring(range.begin, 1);
	}
}
inline void _annotComplexInsertion(SVariant* var, SBAnnotDB::geneparray* genes, SBSeqList* ref) {
	if (genes->empty()) return;
	SBAnnotDB::geneparray outs, ints, com;
	sforeach(*genes) {
		if (E_->idx == var->pos[0].idx && E_->overlap(var->pos[0])) {
			if (E_->idx == var->pos[1].idx && E_->overlap(var->pos[1])) com.add(E_);
			else outs.add(E_);
		}
		else ints.add(E_);
	}
	//Rearrangement of genes
	if (!com.empty()) {
		sforeach(com) {
			sbio::gene_site gs(E_, &var->pos[0]);
			gs.check(&var->pos[1]);
			gs.mutation = SELF_REARRANGEMENT;
			var->genes.add(gs);
		}
	}
	//Genes in the insertion sites
	if (!outs.empty()) {
		sforeach(outs) {
			sbio::gene_site gs(E_, &var->pos[0]);
			if (var->type & DELETION && var->pos[0].include(*E_)) gs.mutation = NULL_MUT;
			else if (E_->include(var->pos[0])) gs.mutation = SPLIT_MUT;
			else if (var->pos[0].include(E_->begin)) gs.mutation = E_->dir ? TAIL_LESION : HEAD_LESION;
			else if (var->pos[0].include(E_->end)) gs.mutation = E_->dir ? HEAD_LESION : TAIL_LESION;
			var->genes.add(gs);
		}
	}
	//Inserted genes
	if (!ints.empty()) {
		sforeach(ints) {
			sbio::gene_site gs(E_, &var->pos[1]);
			if (var->pos[1].include(*E_)) {
				if (var->type & DUPLICATION || var->type & MULTIPLICATION) gs.mutation = MULTI_COPY;
				else gs.mutation = ECTOPIC_MUT;
			}
			else if (!(var->pos[1].include(E_->begin))) gs.mutation =  E_->dir ? TAIL_LESION : HEAD_LESION;
			else if (!(var->pos[1].include(E_->end))) gs.mutation = E_->dir ? TAIL_LESION : HEAD_LESION;
			var->genes.add(gs);
		}
	}
}
inline void _annotTranslocation(SVariant* var, SBAnnotDB::geneparray* genes, SBSeqList* ref) {
	if (genes->empty()) return;
	SBAnnotDB::geneparray lows, highs;
	sforeach(*genes) {
		if (E_->idx == var->pos[0].idx) lows.add(E_);
		else highs.add(E_);
	}
	if (!lows.empty()) {
		sforeach(lows) {
			sbio::gene_site gs(E_, &var->pos[0]);
			if (var->type & DELETION && var->pos[0].include(*E_)) gs.mutation = NULL_MUT;
			else if (E_->include(var->pos[0])) gs.mutation = SPLIT_MUT;
			else if (var->pos[0].include(E_->begin)) gs.mutation = E_->dir ? TAIL_LESION : HEAD_LESION;
			else if (var->pos[0].include(E_->end)) gs.mutation = E_->dir ? HEAD_LESION : TAIL_LESION;
			var->genes.add(gs);
		}
	}
	if (!highs.empty()) {
		sforeach(highs) {
			sbio::gene_site gs(E_, &var->pos[1]);
			if (var->type & DELETION && var->pos[1].include(*E_)) gs.mutation = NULL_MUT;
			else if (E_->include(var->pos[1])) gs.mutation = SPLIT_MUT;
			else if (var->pos[1].include(E_->begin)) gs.mutation = E_->dir ? TAIL_LESION : HEAD_LESION;
			else if (var->pos[1].include(E_->end)) gs.mutation = E_->dir ? HEAD_LESION : TAIL_LESION;
			var->genes.add(gs);
		}
	}
}
inline void _annotInversion(SVariant* var, sbpos *left, sbpos *right, SBAnnotDB::geneparray* genes, SBSeqList* ref) {
	if (genes->empty()) return;
	SBAnnotDB::geneparray lefts, rights, com;
	sforeach(*genes) {
		if (E_->overlap(*left)) {
			if (E_->overlap(*left)) com.add(E_);
			else lefts.add(E_);
		}
		else rights.add(E_);
	}
	//Rearrangement of genes
	if (!com.empty()) {
		sforeach(com) {
			sbio::gene_site gs(E_, left);
			gs.check(right);
			gs.mutation = SELF_REARRANGEMENT;
			var->genes.add(gs);
		}
	}
	if (!lefts.empty()) {
		sforeach(lefts) {
			sbio::gene_site gs(E_, left);
			if (var->type & DELETION && left->include(*E_)) gs.mutation = NULL_MUT;
			else if (E_->include(*left)) gs.mutation = SPLIT_MUT;
			else if (left->include(E_->begin)) gs.mutation = E_->dir ? TAIL_LESION : HEAD_LESION;
			else if (left->include(E_->end)) gs.mutation = E_->dir ? HEAD_LESION : TAIL_LESION;
			var->genes.add(gs);
		}
	}
	if (!rights.empty()) {
		sforeach(rights) {
			sbio::gene_site gs(E_, right);
			if (var->type & DELETION && right->include(*E_)) gs.mutation = NULL_MUT;
			else if (E_->include(*right)) gs.mutation = SPLIT_MUT;
			else if (right->include(E_->begin)) gs.mutation = E_->dir ? TAIL_LESION : HEAD_LESION;
			else if (right->include(E_->end)) gs.mutation = E_->dir ? HEAD_LESION : TAIL_LESION;
			var->genes.add(gs);
		}
	}
}
inline void _annotSmallVar(SVariant* var, gene_site *gene, SBSeqList* ref, String *ori, String* alt) {
	sforeach_(tit, gene->transcripts) {
		if (tit->site & CDS) {
			auto region = tit->info->codingRegion();
			region.shift(-tit->info->begin);
			*ori = ref->at(var->pos[0].idx)->raw((size_t)tit->info->begin - 1, tit->info->length(true));
			*alt = *ori;
			sint off = var->pos[0].begin - tit->info->begin;
			sforin(i, 0, var->alt.size()) {
				if (off + i < 0 || tit->info->length(true) <= off + i) continue;
				alt->at(off + i) = var->alt[i];
			}
			_splice(ori, alt, gene->info->dir, region, &off);
			_translate(ori, alt);
			if (off < 0) off = 0;
			_compareMNV(ori, alt, &(*tit), off / 3);
		}
		else if (tit->site & EXON) tit->mutation = SUBSTITUTION;
		gene->mutation |= tit->mutation;
	}
}
inline void _annotSmallDEL(SVariant* var, gene_site* gene, SBSeqList* ref, String* ori, String* alt) {
	sforeach_(tit, gene->transcripts) {
		if (tit->site & CDS) {
			auto region = tit->info->codingRegion();
			region.shift(-tit->info->begin);
			*ori = ref->at(var->pos[0].idx)->raw((size_t)tit->info->begin - 1, tit->info->length(true));
			*alt = *ori;
			sint off = var->pos[0].begin - tit->info->begin;
			sforin(i, 0, var->pos[0].length(true)) {
				if ((off + i) < 0 || alt->length() <= (off + i)) continue;
				alt->at(off + i) = '-';
			}
			_splice(ori, alt, gene->info->dir, region, &off);
			if (alt->first() == '-') tit->mutation = NTERM_LESION;
			else if (alt->last() == '-') tit->mutation = CTERM_LESION;
			_removeDeleted(alt);
			if (-1 < off && var->alt.length()) alt->insert(off / 3, var->alt);
			_translate(ori, alt);
			if (off < 0) off = 0;
			_compareDEL(ori, alt, &(*tit), off / 3);
		}
		else if (tit->site & EXON) tit->mutation = INDEL_MUT;
		gene->mutation |= tit->mutation;
	}
}
inline void _annotSmallINS(SVariant* var, gene_site* gene, SBSeqList* ref, String* ori, String* alt) {
	sforeach_(tit, gene->transcripts) {
		if (tit->site & CDS) {
			tit->mutation = INDEL_MUT;
			auto region = tit->info->codingRegion();
			region.shift(-tit->info->begin);
			*ori = ref->at(var->pos[0].idx)->raw(tit->info->begin - 1, tit->info->length(true));
			*alt = *ori;
			sint off = var->pos[0].begin - tit->info->begin;
			_splice(ori, alt, gene->info->dir, region, &off);
			alt->insert(off / 3, var->alt);
			_translate(ori, alt);
			_compareINS(ori, alt, &(*tit), off / 3);
		}
		else if (tit->site & EXON) tit->mutation = INDEL_MUT;
		gene->mutation |= tit->mutation;
	}
}
inline void _annotSmallMUL(SVariant* var, gene_site* gene, SBSeqList* ref, String* ori, String* alt) {
	sforeach_(tit, gene->transcripts) {
		if (tit->site & CDS) {
			tit->mutation = INDEL_MUT;
			auto region = tit->info->codingRegion();
			region.shift(-tit->info->begin);
			*ori = ref->at(var->pos[0].idx)->raw(tit->info->begin - 1, tit->info->length(true));
			*alt = *ori;
			auto cp = var->homo ? var->copy.ratio[0] : var->copy.ratio[0] * 2.0 - 1.0;
			sint off = var->pos[0].begin - tit->info->begin,
				off2 = var->pos[0].end - tit->info->begin,
				copy = (cp - floor(cp)) < 0.5 ? floor(cp) : floor(cp) + 1;
			_splice(ori, alt, gene->info->dir, region, &off, &off2);


			alt->insert(off / 3, ori->substring(off < off2 ? off : off2, off < off2 ? (off2 - off + 1) : (off - off2 + 1)) * (copy - 1));
			
			
			_translate(ori, alt);
			if (off < 0) off = 0;
			_compareINS(ori, alt, &(*tit), off / 3);
		}
		else if (tit->site & EXON) tit->mutation = INDEL_MUT;
		gene->mutation |= tit->mutation;
	}
}
template<class T>
inline void _removeDuplicatedPtr(CArray<T*>* array) {
	CArray<T*> newarray;
	auto size = array->size();
	array->sort();
	auto it = array->begin();
	while (it < array->end()) {
		newarray.add(E_);
		if (it < (array->end() - 1) && (*(it + 1)) == (*it)) NEXT_;
		NEXT_;
	}
	newarray.sort([](const T* t1, const T* t2) {
		if (t1->idx != t2->idx) return t1->idx < t2->idx;
		else return t1->begin < t2->begin;
		}
	);
	array->swap(newarray);
}
inline void _makeMutList(stringarray* array, SBAnnotDB::mutparray* mutants) {
	if (mutants->empty()) return;
	sforeach(*mutants) array->add(E_->name);
}
inline void _annotate(SVariant* var, SBSeqList* ref, SBAnnotDB* db, svariant_param *par) {
	SBAnnotDB::geneparray genes;
	SBAnnotDB::mutparray mutants;
	if (-1 < var->pos[1].idx) {
		//Insertion
		if (var->type & INSERTION) {
			db->geneInfo(genes, var->pos[0], true, false);
			db->geneInfo(genes, var->pos[1], true, true);
			db->mutantInfo(mutants, var->pos[0]);
			if (!genes.empty()) _removeDuplicatedPtr(&genes);
			_annotComplexInsertion(var, &genes, ref);
		}
		//Local translocation / inverted translocation
		else if (var->type & TRANSLOCATION) {
			db->geneInfo(genes, var->pos[0], true, false);
			db->geneInfo(genes, var->pos[1], true, true);
			db->mutantInfo(mutants, var->pos[0], false);
			db->mutantInfo(mutants, var->pos[1], true);
			if (!genes.empty()) _removeDuplicatedPtr(&genes);
			if (!mutants.empty()) _removeDuplicatedPtr(&mutants);
			_annotTranslocation(var, &genes, ref);
		}
		//Local inversion
		else {
			sbpos left(var->pos[0].idx, var->pos[0].begin, var->pos[1].begin),
				right(var->pos[0].idx, var->pos[0].end, var->pos[1].end);
			db->geneInfo(genes, left, true, false);
			db->geneInfo(genes, right, true, true);
			db->mutantInfo(mutants, left, false);
			db->mutantInfo(mutants, right, true);
			if (!genes.empty()) _removeDuplicatedPtr(&genes);
			if (!mutants.empty()) _removeDuplicatedPtr(&mutants);
			_annotInversion(var, &left, &right, &genes, ref);
		}
	}
	else {
		String ori, alt;
		db->geneInfo(genes, var->pos[0], true, false);
		db->mutantInfo(mutants, var->pos[0], false);
		sforeach(genes) {
			gene_site gs(E_, &var->pos[0]);
			var->genes.add(gs);
		}
		if ((var->type & SNV) || (var->type & MNV)) {
			sforeach_(gene, var->genes) _annotSmallVar(var, &(*gene), ref, &ori, &alt);
		}
		else if (var->type & DELETION) {
			sforeach_(gene, var->genes) {
				if (var->pos[0].include(*(gene->info))) gene->mutation = NULL_MUT;
				else if (var->flag & SMALL_VARIANT || gene->info->include(var->pos[0])) _annotSmallDEL(var, &(*gene), ref, &ori, &alt);
				else if (var->pos[0].include(gene->info->begin)) gene->mutation = gene->info->dir ? TAIL_LESION : HEAD_LESION;
				else if (var->pos[0].include(gene->info->end)) gene->mutation = gene->info->dir ? HEAD_LESION : TAIL_LESION;
			}
		}
		else if (var->type & INSERTION) {
			sforeach_(gene, var->genes) {
				if (var->flag & SMALL_VARIANT) _annotSmallINS(var, &(*gene), ref, &ori, &alt);
			}
		}
		else if ((var->type & DUPLICATION) || (var->type & MULTIPLICATION)) {
			sforeach_(gene, var->genes) {
				if (var->pos[0].include(*(gene->info))) gene->mutation = MULTI_COPY;
				else if (var->flag & SMALL_VARIANT || gene->info->include(var->pos[0])) _annotSmallMUL(var, &(*gene), ref, &ori, &alt);
				else gene->mutation = SPLIT_MUT;
			}
		}
	}
	_makeMutList(&var->mutants, &mutants);
	auto asite = var->annotatedSite();
	bool avail = false;
	if ((asite & CDS) && (par->annot & CDS)) avail = true;
	else if ((asite & EXON) && (par->annot & EXON)) avail = true;
	else if ((asite & UTR) && (par->annot & UTR)) avail = true;
	else if ((asite & INTRON) && (par->annot & INTRON)) avail = true;
	else if (par->annot & INTER_GENE) avail = true;
	if (!avail) var->flag |= UNAVAILABLE_FLAG;



	/*
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
	*/
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
			if (E_.pos[0].begin + _par->max_dist < it_->pos[0].begin) break;
			if (it_->equal(E_, _par->max_dist)) {
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
void SVarFilter::setRefName(SVarList* vl) {
	if (_ref) {
		sforeach(*vl) {
			if (-1 < E_->pos[0].idx) E_->ref[0] = _ref->at(E_->pos[0].idx)->name;
			if (-1 < E_->pos[1].idx) E_->ref[1] = _ref->at(E_->pos[1].idx)->name;
		}
	}
}
void SVarFilter::annotate(SVariant* var) { _annotate(var, _ref, _db, _par); }
void SVarFilter::annotate(SVarList* vl, SWork* threads) {
	sforeach(*vl) {
		if (threads) threads->addTask(_annotate, E_, _ref, _db, _par);
		else _annotate(E_, _ref, _db, _par);
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
		if ((E_->flag & UNAVAILABLE_FLAG) || (E_->flag & NOT_USE_FLAG)) --s;
	}
	list->tidy(s);
}
void SVarFilter::linkGene(SVariant* var) {
	if (!_db || var->genes.empty()) return;
	SBAnnotDB::geneparray genes;
	sforeach(var->genes) {
		_db->geneInfo(genes, E_.gid);
		if (!genes.empty()) E_.info = genes[0];
	}
}
void SVarFilter::linkGenes(SVarList* list) { sforeach(*list) SVarFilter::linkGene(E_); }
void SVarFilter::check(SVariant* var) {
	if (_par) {
		/*
		*/
	}
	if (_target && !_target->empty() && _target->at(var->pos[0].idx).include(var->pos[0])) var->flag |= UNAVAILABLE_FLAG;
}
void SVarFilter::setReference(SBSeqList* ref) { _ref = ref; }
void SVarFilter::setDB(SBAnnotDB* db) { _db = db; }
void SVarFilter::setParam(svariant_param* p) { _par = p; }
void SVarFilter::setTarget(Array<sregion>* reg) { _target = reg; }