#include "sbioinfo/variant.h"
/*
inline void _splice(String* ori, String* alt, bool dir, sregion reg, int *pos, int *pos2 = nullptr) {
	String otmp, atmp;
	otmp.reserve(ori->size() + 1);
	atmp.reserve(alt->size() + 1);
	sfor(reg) {
		if ($_.include(*pos)) (*pos) = otmp.length() + (*pos) - $_.begin;
		if (pos2 && $_.include(*pos2)) (*pos2) = otmp.length() + (*pos2) - $_.begin;
		otmp += ori->substring($_.begin, $_.length(true));
		atmp += alt->substring($_.begin, $_.length(true));
	}
	ori->swap(otmp); alt->swap(atmp);
	if (dir) {
		(*pos) = ori->size() - (*pos) - 1;
		if(pos2) (*pos2) = ori->size() - (*pos2) - 1;
		sna::toComplement(*ori);
		sna::toComplement(*alt);
	}
}
inline void _removeDeleted(String* seq) {
	ArrayIterator<char> beg, end;
	sfor(*seq) {
		if ($_ == '-') {
			beg = it; end = beg;
			while ($_ == '-' && it < seq->end()) { ++end; $NEXT;($); }
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
		trs->pos = off + 1; trs->ori << ori->at(off-1); trs->alt << "-"; return;
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
inline void _annotComplexInsertion(Variant* var, AnnotDB::geneparray* genes, SeqList* ref) {
	if (genes->empty()) return;
	AnnotDB::geneparray outs, ints, com;
	sfor(*genes) {
		if ($_->idx == var->pos[0].idx && $_->overlap(var->pos[0])) {
			if ($_->idx == var->pos[1].idx && $_->overlap(var->pos[1])) com.add(E_);
			else outs.add(E_);
		}
		else ints.add(E_);
	}
	//Rearrangement of genes
	if (!com.empty()) {
		sfor(com) {
			sbio::gene_site gs($_, &var->pos[0]);
			gs.check(&var->pos[1]);
			gs.mutation = SELF_REARRANGEMENT;
			var->genes.add(gs);
		}
	}
	//Genes in the insertion sites
	if (!outs.empty()) {
		sfor(outs) {
			sbio::gene_site gs($_, &var->pos[0]);
			if (var->type & DELETION && var->pos[0].include(*E_)) gs.mutation = NULL_MUT;
			else if ($_->include(var->pos[0])) gs.mutation = SPLIT_MUT;
			else if (var->pos[0].include($_->begin)) gs.mutation = $_->dir ? TAIL_LESION : HEAD_LESION;
			else if (var->pos[0].include($_->end)) gs.mutation = $_->dir ? HEAD_LESION : TAIL_LESION;
			var->genes.add(gs);
		}
	}
	//Inserted genes
	if (!ints.empty()) {
		sfor(ints) {
			sbio::gene_site gs($_, &var->pos[1]);
			if (var->pos[1].include(*E_)) {
				if (var->type & DUPLICATION || var->type & MULTIPLICATION) gs.mutation = MULTI_COPY;
				else gs.mutation = ECTOPIC_MUT;
			}
			else if (!(var->pos[1].include($_->begin))) gs.mutation =  $_->dir ? TAIL_LESION : HEAD_LESION;
			else if (!(var->pos[1].include($_->end))) gs.mutation = $_->dir ? TAIL_LESION : HEAD_LESION;
			var->genes.add(gs);
		}
	}
}
inline void _annotTranslocation(Variant* var, AnnotDB::geneparray* genes, SeqList* ref) {
	if (genes->empty()) return;
	AnnotDB::geneparray lows, highs;
	sfor(*genes) {
		if ($_->idx == var->pos[0].idx) lows.add(E_);
		else highs.add(E_);
	}
	if (!lows.empty()) {
		sfor(lows) {
			sbio::gene_site gs($_, &var->pos[0]);
			if (var->type & DELETION && var->pos[0].include(*E_)) gs.mutation = NULL_MUT;
			else if ($_->include(var->pos[0])) gs.mutation = SPLIT_MUT;
			else if (var->pos[0].include($_->begin)) gs.mutation = $_->dir ? TAIL_LESION : HEAD_LESION;
			else if (var->pos[0].include($_->end)) gs.mutation = $_->dir ? HEAD_LESION : TAIL_LESION;
			var->genes.add(gs);
		}
	}
	if (!highs.empty()) {
		sfor(highs) {
			sbio::gene_site gs($_, &var->pos[1]);
			if (var->type & DELETION && var->pos[1].include(*E_)) gs.mutation = NULL_MUT;
			else if ($_->include(var->pos[1])) gs.mutation = SPLIT_MUT;
			else if (var->pos[1].include($_->begin)) gs.mutation = $_->dir ? TAIL_LESION : HEAD_LESION;
			else if (var->pos[1].include($_->end)) gs.mutation = $_->dir ? HEAD_LESION : TAIL_LESION;
			var->genes.add(gs);
		}
	}
}
inline void _annotInversion(Variant* var, sbpos *left, sbpos *right, AnnotDB::geneparray* genes, SeqList* ref) {
	if (genes->empty()) return;
	AnnotDB::geneparray lefts, rights, com;
	sfor(*genes) {
		if ($_->overlap(*left)) {
			if ($_->overlap(*left)) com.add(E_);
			else lefts.add(E_);
		}
		else rights.add(E_);
	}
	//Rearrangement of genes
	if (!com.empty()) {
		sfor(com) {
			sbio::gene_site gs($_, left);
			gs.check(right);
			gs.mutation = SELF_REARRANGEMENT;
			var->genes.add(gs);
		}
	}
	if (!lefts.empty()) {
		sfor(lefts) {
			sbio::gene_site gs($_, left);
			if (var->type & DELETION && left->include(*E_)) gs.mutation = NULL_MUT;
			else if ($_->include(*left)) gs.mutation = SPLIT_MUT;
			else if (left->include($_->begin)) gs.mutation = $_->dir ? TAIL_LESION : HEAD_LESION;
			else if (left->include($_->end)) gs.mutation = $_->dir ? HEAD_LESION : TAIL_LESION;
			var->genes.add(gs);
		}
	}
	if (!rights.empty()) {
		sfor(rights) {
			sbio::gene_site gs($_, right);
			if (var->type & DELETION && right->include(*E_)) gs.mutation = NULL_MUT;
			else if ($_->include(*right)) gs.mutation = SPLIT_MUT;
			else if (right->include($_->begin)) gs.mutation = $_->dir ? TAIL_LESION : HEAD_LESION;
			else if (right->include($_->end)) gs.mutation = $_->dir ? HEAD_LESION : TAIL_LESION;
			var->genes.add(gs);
		}
	}
}
inline void _annotSmallVar(Variant* var, gene_site *gene, SeqList* ref, String *ori, String* alt) {
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
inline void _annotSmallDEL(Variant* var, gene_site* gene, SeqList* ref, String* ori, String* alt) {
	sfor_(tit, gene->transcripts) {
		if (tit->site &sfor_(tit, gene->transcripts) {
		if (tit->site & CDS) {
			auto region = tit->info->codingRegion();
		 CDS) {
			auto region = tit->info->codingRegion();
			*ori = ref->at(var->pos[0].idx)->raw((size_t)tit->info->begin - 1, tit->info->length(true));
			*alt = *ori;
			sint off = var->pos[0].begin - tit->info->begin;
			sforin(i, 0, var->pos[0].length(true)) {
				if ((off + i) < 0 || (sint)alt->length() <= (off + i)) continue;
				alt->at(off + i) = '-';
			}
			region.shift(-tit->info->begin);
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
inline void _annotSmallINS(Variant* var, gene_site* gene, SeqList* ref, String* ori, String* alt) {
	sfor_(tit, gene->transcripts) {
		if (tit->site & CDS) {
			tit->mutation = INDEL_MUT;
			auto region = tit->info->codingRegion();
			region.shift(-tit->info->begin);
			*ori = ref->at(var->pos[0].idx)->raw((size_t)tit->info->begin - 1, tit->info->length(true));
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
inline void _annotSmallMUL(Variant* var, gene_site* gene, SeqList* ref, String* ori, String* alt) {
	sfor_(tit, gene->transcripts) {
		if (tit->site & CDS) {
			tit->mutation = INDEL_MUT;
			auto region = tit->info->codingRegion();
			region.shift(-tit->info->begin);
			*ori = ref->at(var->pos[0].idx)->raw((size_t)tit->info->begin - 1, tit->info->length(true));
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
inline void _removeDuplicatedPtr(Array<T*>* array) {
	Array<T*> newarray;
	auto size = array->size();
	array->sort();
	auto it = array->begin();
	while (it < array->end()) {
		newarray.add(E_);
		if (it < (array->end() - 1) && (*(it + 1)) == (*it)) $NEXT;;
		$NEXT;;
	}
	newarray.sort([](const T* t1, const T* t2) {
		if (t1->idx != t2->idx) return t1->idx < t2->idx;
		else return t1->begin < t2->begin;
		}
	);
	array->swap(newarray);
}
inline void _makeMutList(stringarray* array, AnnotDB::mutparray* mutants) {
	if (mutants->empty()) return;
	sfor(*mutants) array->add($_->name);
}
inline void _annotate(Variant* var, SeqList* ref, AnnotDB* db, svariantparamam *par) {
	AnnotDB::geneparray genes;
	AnnotDB::mutparray mutants;
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
		sfor(genes) {
			gene_site gs($_, &var->pos[0]);
			var->genes.add(gs);
		}
		if ((var->type & SNV) || (var->type & MNV)) {
			sfor_(gene, var->genes) _annotSmallVar(var, &(*gene), ref, &ori, &alt);
		}
		else if (var->type & DELETION) {
			sfor_(gene, var->genes) {
				if (var->pos[0].include(*(gene->info))) gene->mutation = NULL_MUT;
				else if (var->flag & SMALL_VARIANT || gene->info->include(var->pos[0])) _annotSmallDEL(var, &(*gene), ref, &ori, &alt);
				else if (var->pos[0].include(gene->info->begin)) gene->mutation = gene->info->dir ? TAIL_LESION : HEAD_LESION;
				else if (var->pos[0].include(gene->info->end)) gene->mutation = gene->info->dir ? HEAD_LESION : TAIL_LESION;
			}
		}
		else if (var->type & INSERTION) {
			sfor_(gene, var->genes) {
				if (var->flag & SMALL_VARIANT) _annotSmallINS(var, &(*gene), ref, &ori, &alt);
			}
		}
		else if ((var->type & DUPLICATION) || (var->type & MULTIPLICATION)) {
			sfor_(gene, var->genes) {
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
}
*/
slib::sbio::VarFilter::VarFilter(SeqList* r, AnnotDB* d, VarParam* p, Array<sregion>* t) : reference(r), annotdb(d), param(p), target(t) {}
slib::sbio::VarFilter::~VarFilter() {}
slib::sbio::VarFilter& slib::sbio::VarFilter::operator=(const slib::sbio::VarFilter& filter) {
	reference = filter.reference; 
	annotdb = filter.annotdb;
	param = filter.param;
	target = filter.target;
	return *this;
}

/*
size_t _checkDel(Array<SVar>* variants, SVParam *par) {
	variants->sort(slib::sbio::sutil::delsorter);
	auto sz = variants->size();
	sfor(*variants) {
		if ($_.type == 0) continue;
		auto it_ = $ + 1;
		while (it_ < variants->end()) {
			if ($_.lt(*it_, par)) break;
			if ($_.equal(*it_, par)) { $_.merge(*it_, par); --sz; }
			++it_;
		}
	}
	return sz;
}
size_t _checkDup(Array<SVar>* variants, SVParam* par) {
	variants->sort(slib::sbio::sutil::dupsorter);
	auto sz = variants->size();
	sfor(*variants) {
		if ($_.type == 0) continue;
		auto it_ = $ + 1;
		while (it_ < variants->end()) {
			if ($_.lt(*it_, par)) break;
			if ($_.equal(*it_, par)) { $_.merge(*it_, par); --sz; }
			++it_;
		}
	}
	return sz;
}
size_t _checkIns(Array<SVar>* variants, SVParam* par) {
	variants->sort(slib::sbio::sutil::inssorter);
	auto sz = variants->size();
	sfor(*variants) {
		if ($_.type == 0) continue;
		auto it_ = $ + 1;
		while (it_ < variants->end()) {
			if ($_.lt(*it_, par)) break;
			if ($_.equal(*it_, par)) { $_.merge(*it_, par); --sz; }
			++it_;
		}
	}
	return sz;
}
size_t _checkInv(Array<SVar>* variants, SVParam* par) {
	variants->sort(slib::sbio::sutil::invsorter);
	auto sz = variants->size();
	auto current = variants->begin();
	sforin(it, current, variants->end()) {
		if ($_.type == 0) continue;
		else if ($_.pos[0].dir) { current = $; break; }
		auto it_ = $ + 1;
		while (it_ < variants->end() && it_->pos[1].dir) {
			if ($_.lt(*it_, par)) break;
			if ($_.equal(*it_, par)) { $_.merge(*it_, par); --sz; }
			++it_;
		}
	}
	sforin(it, current, variants->end()) {
		if ($_.type == 0) continue;
		auto it_ = $ + 1;
		while (it_ < variants->end()) {
			if ($_.lt(*it_, par)) break;
			if ($_.equal(*it_, par)) { $_.merge(*it_, par); --sz; }
			++it_;
		}
	}
	return sz;
}
size_t _checkTrs(VarFilter* filter, Array<SVar>* variants) {
	variants->sort(slib::sbio::sutil::trssorter);

	auto sz = variants->size();
	//
	auto current = variants->begin();
	sfori(*(filter->reference)) {
		if (i < current->pos[1].idx) continue;
		sforin(it, current, variants->end()) {
			if ($_.type == 0) continue;
			if ($_.pos[1].idx != i) {
				current = $; break;
			}
			auto it_ = $ + 1;
			while (it_ < variants->end() && it_->pos[1].idx == i) {
				if ($_.pos[0].end + filter->param->svp.max_dist < it_->pos[0].end) break;
				if (filter->equal($_, *it_)) {
					filter->merge($_, *it_); --sz;
				}
				++it_;
			}
		}
	}
	return sz;
}
size_t _checkTrInv(VarFilter* filter, Array<SVar>* variants) {
	//
	variants->sort([](const SVar& sv1, const SVar& sv2) {
		
		});
	auto sz = variants->size();
	//
	auto current = variants->begin();
	sfori(*(filter->reference)) {
		if (i < current->pos[1].idx) continue;
		sforin(it, current, variants->end()) {
			if ($_.type == 0) continue;
			if ($_.pos[0].dir || $_.pos[1].idx != i) {
				current = $; break;
			}
			auto it_ = $ + 1;
			while (it_ < variants->end() && it_->pos[1].idx == i) {
				if ($_.pos[0].end + filter->param->svp.max_dist < it_->pos[0].end) break;
				if (filter->equal($_, *it_)) {
					filter->merge($_, *it_); --sz;
				}
				++it_;
			}
		}

		sforin(it, current, variants->end()) {
			if ($_.type == 0) continue;
			if ($_.pos[1].idx != i) {
				current = $; break;
			}
			auto it_ = $ + 1;
			while (it_ < variants->end() && it_->pos[1].idx == i) {
				if ($_.pos[0].begin + filter->param->svp.max_dist < it_->pos[0].begin) break;
				if (filter->equal($_, *it_)) {
					filter->merge($_, *it_); --sz;
				}
				++it_;
			}
		}
	}
	return sz;

}
*/
/*
void slib::sbio::VarFilter::subtract(Array<SVar>& variants1, Array<SVar>& variants2) {
	if (variants1.empty() || variants2.empty()) return;
	size_t sz = variants1.size();
	auto sorter = slib::sbio::sutil::svsorter(variants1[0].type & 0xFF);
	variants1.sort(sorter);
	auto sorter2 = slib::sbio::sutil::svsorter(variants2[0].type & 0xFF, true);
	variants2.sort(sorter2);
	auto compare = variants2.begin();
	//
	sfor(variants1) {
		if (compare->lt($_, &param->svp)) ++compare;
		auto vit = compare;
		if (!$_.comparable(*compare) || $_.lt(*vit, &param->svp)) break;
		if ($_.equal(*compare, reference, &param->svp)) { $_.type = 0; --sz; }
	}
	variants1.sort(sorter);
	variants1.resize(sz);
}
*/
void slib::sbio::VarFilter::merge(VarList& vl1, VarList& vl2) {
	if (vl2.empty()) return;
	/*
	if (!(vl1.linkageGroups() == vl2.linkageGroups())) {

		throw RefMismatchException(refMisErrorText(
			vl1.path(), vl1.linkageGroups().size(), ,
			vl2.path(), vl2.linkageGroups().size(), ));

	}
	*/
	auto dist = param ? param->svp.max_dist : 0;
	if (!vl2.attribute.empty()) {
		sfor(vl2.attribute) {
			if (!vl1.attribute.hasKey($_.key())) vl1.attribute.insert($_);
		}
	}
	vl1.append(vl2);
	auto size = vl1.size();
	//vl1.sort();
	sfor(vl1) {
		if ($_->flag & UNAVAILABLE_FLAG) continue;
		sforin(vit, it + 1, vl1.end()) {
			if ((*vit)->flag & UNAVAILABLE_FLAG) continue;
			if ($_->comparable(*vit) && (*vit)->lt(*$_, param)) {
				if ($_->equal(**vit, reference, param)) { (*vit)->flag |= UNAVAILABLE_FLAG; --size; }
			}
			else break;
		}
	}
	//vl1.tidyUp(size);
}
void slib::sbio::VarFilter::subtract(VarList& vl1, VarList& vl2) {
	if (vl1.empty() || vl2.empty()) return;
	auto size = vl1.size();
	vl2.addFlag(NOT_USE_FLAG);
	vl1.append(vl2);
	vl1.sort();
	sfor(vl1) {
		if ($_->flag & UNAVAILABLE_FLAG) continue;
		sforin(vit, it + 1, vl1.end()) {			
			if ((*vit)->flag & UNAVAILABLE_FLAG) continue;
			if ($_->lt(**vit, param)) break;
			if ($_->equal(**vit, reference, param)) {
				if (!($_->flag & NOT_USE_FLAG)) $_->flag |= UNAVAILABLE_FLAG;
				else if (!((*vit)->flag & NOT_USE_FLAG)) (*vit)->flag |= UNAVAILABLE_FLAG;
			}
		}
	}
	vl1.tidyUp();
	vl2.removeFlag(NOT_USE_FLAG);
}
void slib::sbio::VarFilter::common(VarList& vl1, VarList& vl2) {
	if (vl1.empty() || vl2.empty()) return;
	auto size = vl1.size();
	vl1.addFlag(UNAVAILABLE_FLAG);
	vl2.addFlag(NOT_USE_FLAG);
	vl1.append(vl2);
	vl1.sort();
	sint count = 0;
	sfor(vl1) {
		sforin(vit, it + 1, vl1.end()) {
			if (!((*vit)->lt(*$_, param))) break;
			if ($_->equal(**vit, reference, param)) {
				if ($_->flag & UNAVAILABLE_FLAG) { $_->flag -= UNAVAILABLE_FLAG; ++count; }
				else if ((*vit)->flag & UNAVAILABLE_FLAG) { (*vit)->flag -= UNAVAILABLE_FLAG; ++count; }
			}
		}
	}
	vl1.tidyUp(count);
	vl2.removeFlag(NOT_USE_FLAG);
}

void _translate(int i, slib::sbio::Variant* var, slib::sbio::VarFilter* filter) {
	auto &codon = filter->annotdb->chromosomes[var->pos[i].idx].type == 
		(slib::suint)slib::sbio::LG_TYPE::MT_GENOME ? 
		filter->param->mtcodon : 
		filter->param->codon;
	//auto seq = filter->reference->
	//filter->buffer[i] = seq->transcribe()->translate(codon).raw();
}

void _delGeneCheck(slib::sbio::Variant* var, slib::sbio::TranscriptInfo* rna, slib::sbio::TranscriptAnnotData* ti) {
	if (var->pos[0].include(*rna)) ti->mutation = slib::sbio::NULL_MUT;
	else if (ti->site & slib::sbio::CDS) {
		auto tpos = slib::sbio::sutil::codingSite(var->pos[0], rna, rna->gene->dir);
		if (tpos.begin == 0 && 0 < tpos.end) ti->mutation = slib::sbio::HEAD_LESION;

		//if (tpos.begin < ori.size() && tpos.end == ori.size()) ti->mutation = slib::sbio::TAIL_LESION;

		/*
		seq1 = reference->at(var->pos[0].idx).subsequence(rna->coding(), gene->dir);
		ori = seq1.transcribe().translate().raw();
		if (tpos.begin == 0 && 0 < tpos.end) ti.mutation = HEAD_LESION;
		else if (tpos.begin < ori.size() && tpos.end == ori.size()) ti.mutation = TAIL_LESION;
		else {
			if (tpos.begin + 1 < tpos.end) seq1.remove(srange(tpos.begin + 1, tpos.end));
			if (var->alt.match("|")) seq2 = Sequence(var->alt.substring(0, var->alt.find("|")), DNA_SEQ);
			else seq2 = Sequence(var->alt, DNA_SEQ);
			if (seq2.length()) seq1.insert(tpos.begin + 1, seq2);
			mut = seq1.transcribe().translate().raw();
			if (mut.match("*")) mut.clip(0, mut.find("*"));
			if (mut[0] != 'M') ti.mutation = FMET_LOSS;
			if (((tpos.length(true) - seq2.length()) % 3 == 0) &&
				((ori.size() - mut.size()) == (tpos.length(true) - seq2.length()) / 3)) ti.mutation = IN_FRAME;
			else ti.mutation = FRAME_SHIFT;
		}
		*/
	}
	else ti->mutation = slib::sbio::INDEL;
}
void slib::sbio::VarFilter::annotate(Variant* var) {
	Array<GeneInfo*> genes1, genes2;
	Sequence seq1, seq2;
	String ori, mut;
	// pos[0] annotation
	annotdb->selectGenes(var->pos[0], &genes1);
	// pos[1] annotation
	if (var->pos[1].idx != -1) annotdb->selectGenes(var->pos[1], &genes2);
	var->annotation.reserve(genes1.size() + 1);
	// 
	sforeach(gene, genes1) {
		//
		var->annotation.add(gene);
		if (gene->transcripts.empty()) continue;
		//
		auto& gi = var->annotation[-1];
		gi.transcripts.reserve(gene->transcripts.size());
		//
		sforeach(rna, gene->transcripts) {
			if (!rna->overlap(var->pos[0])) continue;
			// Mutation site check
			gi.transcripts.add(rna);
			auto& ti = gi.transcripts[-1];
			sforeach(structure, rna->structures) {
				if (structure.overlap(var->pos[0])) ti.site |= structure.type;
			}
			// Intron check
			auto introns = (*rna) ^ (rna->exons());
			sforeach(intron, introns) {
				if (intron.overlap(var->pos[0])) {
					if (var->pos[0].overlap(srange(intron.begin + 1, intron.begin + param->splice_reg)) ||
						var->pos[0].overlap(srange(intron.end - param->splice_reg, intron.end - 1))) {
						ti.site |= SPLICE_SITE;
					}
					else ti.site |= INTRON;
				}
			}
			if (var->type & INSERTION) {
				// Simple insertion
				if (var->pos[1].idx == -1) {
					if (ti.site & CDS) {
						seq1 = reference->at(var->pos[0].idx).subsequence(rna->coding());
						if (gene->dir) seq1.complement();
						ori = seq1.transcribe()
							.translate(annotdb->chromosomes[var->pos[0].idx].type == (suint)LG_TYPE::MT_GENOME ? param->mtcodon : param->codon)
							.raw();
						auto tpos = sbio::sutil::codingSite(var->pos[0], rna, gene->dir);
						if (tpos.begin + 1 < tpos.end) seq1.remove(srange(tpos.begin + 1, tpos.end));
						
						
						if (var->alt.match("|")) seq2 = Sequence(var->alt.substring(0, var->alt.find("|")), DNA_SEQ);
						else seq2 = Sequence(var->alt, DNA_SEQ);
						
						
						//seq1.insert(tpos.begin + 1, seq2);


						mut = seq1.transcribe().translate().raw();
						if (mut.match("*")) mut.clip(0, mut.find("*"));
						if (mut[0] != 'M') ti.mutation = FMET_LOSS;
						if ((seq2.length() % 3) == 0 && ((mut.size() - ori.size()) == (seq2.length() / 3))) ti.mutation = IN_FRAME;
						else ti.mutation = FRAME_SHIFT;
					}
					else ti.mutation = INDEL;
				}
				// Complex insertion
				else {



				}
			}
			else {
				// Simple translocation
				if (var->type & TRANSLOCATION) {
					if (var->pos[0].include(*rna)) ti.mutation = NULL_MUT;



				}
				// Simple inversion
				else if (var->type & INVERSION) {
					ti.mutation = REARRANGE_MUT;
					//if (rna->include(var->pos[0]) ||
					//	)

					//if (srange()) ti.mutation = NULL_MUT;



					if (ti.site & CDS) {
						
						seq1 = reference->at(var->pos[0].idx).subsequence(rna->coding());
						if (gene->dir) seq1.complement();
						ori = seq1.transcribe()
							.translate(annotdb->chromosomes[var->pos[0].idx].type == (suint)LG_TYPE::MT_GENOME ? param->mtcodon : param->codon)
							.raw();

						auto tpos = sbio::sutil::codingSite(var->pos[0], rna, gene->dir);


						if (tpos.begin == 0 && 0 < tpos.end) ti.mutation = HEAD_LESION;
						else if (tpos.begin < ori.size() && tpos.end == ori.size()) ti.mutation = TAIL_LESION;
						else {
							if (tpos.begin + 1 < tpos.end) seq1.remove(srange(tpos.begin + 1, tpos.end));
							if (var->alt.match("|")) seq2 = Sequence(var->alt.substring(0, var->alt.find("|")), DNA_SEQ);
							else seq2 = Sequence(var->alt, DNA_SEQ);

							//if (seq2.length()) seq1.insert(tpos.begin + 1, seq2);

							mut = seq1.transcribe().translate().raw();
							if (mut.match("*")) mut.clip(0, mut.find("*"));
							if (mut[0] != 'M') ti.mutation = FMET_LOSS;
							if (((tpos.length(true) - seq2.length()) % 3 == 0) &&
								((ori.size() - mut.size()) == (tpos.length(true) - seq2.length()) / 3)) ti.mutation = IN_FRAME;
							else ti.mutation = FRAME_SHIFT;
						}
					}
					else ti.mutation = REARRANGE_MUT;
				}
				else {
					// Simple deletion
					if (var->type & DELETION) {
						if (var->pos[0].include(*rna)) ti.mutation = NULL_MUT;
						else if (ti.site & CDS) {
							seq1 = reference->at(var->pos[0].idx).subsequence(rna->coding());
							if (gene->dir) seq1.complement();
							ori = seq1.transcribe().translate().raw();
							auto tpos = sbio::sutil::codingSite(var->pos[0], rna, gene->dir);
							if (tpos.begin == 0 && 0 < tpos.end) ti.mutation = HEAD_LESION;
							else if (tpos.begin < ori.size() && tpos.end == ori.size()) ti.mutation = TAIL_LESION;
							else {
								if (tpos.begin + 1 < tpos.end) seq1.remove(srange(tpos.begin + 1, tpos.end));
								if (var->alt.match("|")) seq2 = Sequence(var->alt.substring(0, var->alt.find("|")), DNA_SEQ);
								else seq2 = Sequence(var->alt, DNA_SEQ);


								//if (seq2.length()) seq1.insert(tpos.begin + 1, seq2);
								
								
								mut = seq1.transcribe().translate().raw();
								if (mut.match("*")) mut.clip(0, mut.find("*"));
								if (mut[0] != 'M') ti.mutation = FMET_LOSS;
								if (((tpos.length(true) - seq2.length()) % 3 == 0) &&
									((ori.size() - mut.size()) == (tpos.length(true) - seq2.length()) / 3)) ti.mutation = IN_FRAME;
								else ti.mutation = FRAME_SHIFT;
							}
						}
						else ti.mutation = INDEL;
					}
					// Simple du/multi-plication
					else if (var->type & DUPLICATION || var->type & MULTIPLICATION) {
						if (ti.site & CDS) {
							seq1 = reference->at(var->pos[0].idx).subsequence(rna->coding());
							if (gene->dir) seq1.complement();
							ori = seq1.transcribe().translate().raw();
							auto tpos = sbio::sutil::codingSite(var->pos[0], rna, gene->dir);
							if (tpos.begin + 1 < tpos.end) seq1.remove(srange(tpos.begin + 1, tpos.end));
							if (var->alt.match("|")) seq2 = Sequence(var->alt.substring(0, var->alt.find("|")), DNA_SEQ);
							else seq2 = Sequence(var->alt, DNA_SEQ);


							//seq1.insert(tpos.begin + 1, seq2);
							
							
							mut = seq2.transcribe().translate().raw();
							if (mut.match("*")) mut.clip(0, mut.find("*"));
							if (mut[0] != 'M') ti.mutation = FMET_LOSS;
							if ((seq2.length() % 3) == 0 && ((mut.size() - ori.size()) == (seq2.length() / 3))) ti.mutation = IN_FRAME;
							else ti.mutation = FRAME_SHIFT;
						}
						else ti.mutation = MULTI_COPY;
					}
				}
			}

			if (ti.site & CDS) {
				//src = 


			}
			else if (ti.site & EXON) ti.mutation = INDEL;
		}
	}
}
inline void _annotate(slib::sbio::VarFilter* filter, slib::sbio::Variant* var) { filter->annotate(var); }
void slib::sbio::VarFilter::annotate(Array<SPointer<Variant>>& variants, SWork* threads) {
	sfor(variants) {
		if (threads) threads->addTask(_annotate, this, $_);
		else annotate($_);
	}
	if (threads) threads->complete();
}



/*
void VarFilter::checkRepeat(VarList* list, SeqList* ref) {
	sforin(i, 0, list->size()) {
		auto& vi = list->at(i);
		if (!(vi->flag & SMALL_VARIANT)) continue;
		String prev, post, mut, mut_;
		if (vi->pos[0].length(true) == 1) {
			if (vi->type == INSERTION) {
				prev = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 4, 3);
				post = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 1, 3);
				mut = String(vi->alt) * 3;
				if (prev == mut || post == mut) vi->attribute["repeat"] = true;
			}
			else if (vi->type == DELETION) {
				prev = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 3, 3);
				post = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 1, 3);
				mut = vi->attribute["Ref"].toString() * 3;
				if (prev == mut || post == mut) vi->attribute["repeat"] = true;
			}
			else if (vi->type == SNV) {
				prev = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 4, 3);
				post = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin, 3);
				mut = String(vi->alt) * 3;
				if (prev == mut || post == mut) vi->attribute["repeat"] = true;
			}
		}
		else if (vi->pos[0].length(true) == 2) {
			if (vi->type == INSERTION) {
				if (vi->alt[0] == vi->alt[1]) {
					prev = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 6, 5);
					post = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 1, 5);
					mut = String(vi->alt[0]) * 5;
					if (prev == mut || post == mut) vi->attribute["repeat"] = true;
				}
				else {
					prev = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 4, 3);
					post = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 1, 3);
					mut = String(vi->alt[0]) * 3, mut_ = String(vi->alt[1]) * 3;
					if (prev == mut && post == mut_) vi->attribute["repeat"] = true;
				}
			}
			else if (vi->type == DELETION) {
				String seq = vi->attribute["Ref"];
				if (seq[0] == seq[1]) {
					prev = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 5, 5);
					post = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 1, 5);
					mut = String(seq[0]) * 5;
					if (prev == mut || post == mut) vi->attribute["repeat"] = true;
				}
				else {
					prev = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin - 3, 3);
					post = ref->at(vi->pos[0].idx).raw(vi->pos[0].begin, 3);
					mut = String(seq[0]) * 3, mut_ = String(seq[1]) * 3;
					if (prev == mut && post == mut_) vi->attribute["repeat"] = true;
				}
			}
		}
	}
}

void VarFilter::filter(VarList* list) {
	size_t s = list->size();
	sfor(*list) {
		if (!check($_)) $_->flag |= UNAVAILABLE_FLAG;
		if (($_->flag & UNAVAILABLE_FLAG) || ($_->flag & NOT_USE_FLAG)) --s;
	}
	list->tidy(s);
}
void VarFilter::linkGene(Variant* var) {
	if (!_db || var->genes.empty()) return;
	AnnotDB::geneparray genes;
	sfor(var->genes) {
		_db->geneInfo(genes, $_.gid);
		if (!genes.empty()) $_.info = genes[0];
	}
}
void VarFilter::linkGenes(VarList* list) { sfor(*list) VarFilter::linkGene($_); }
*/
/*
inline String breakSeq(SVar* var, SeqList* ref, int len) {
	if (var->pos[0].dir == var->pos[1].dir) {
		return ref->raw(sbpos(var->pos[0].idx, var->pos[0].end < (len - 1) ? 0 : var->pos[0].end - len + 1, var->pos[0].end)) <<
			(var->alt.size() < len? var->alt : var->alt.substring(0, len/2) + var->alt.substring(var->alt.size() - len / 2)) <<
			ref->raw(sbpos(var->pos[1].idx, var->pos[1].begin, var->pos[1].begin + len - 1));
	}
	else {
		if (var->pos[0].dir) {
			return ref->raw(sbpos(var->pos[0].idx, var->pos[0].begin, var->pos[0].begin + 19, var->pos[0].dir)) <<
				(var->alt.size() < len ? var->alt : var->alt.substring(0, len / 2) + var->alt.substring(var->alt.size() - len / 2)) <<
				ref->raw(sbpos(var->pos[1].idx, var->pos[1].begin, var->pos[1].begin + 19, var->pos[1].dir));
		}
		else {
			return ref->raw(sbpos(var->pos[0].idx, var->pos[0].end < 19 ? 0 : var->pos[0].end - 19, var->pos[0].end, var->pos[0].dir)) <<
				(var->alt.size() < len ? var->alt : var->alt.substring(0, len / 2) + var->alt.substring(var->alt.size() - len / 2)) <<
				ref->raw(sbpos(var->pos[1].idx, var->pos[0].end < 19 ? 0 : var->pos[0].end - 19, var->pos[1].end, var->pos[1].dir));
		}
	}
}
*/
void slib::sbio::VarFilter::check(slib::sbio::Variant* var) {
	if ((var->flag & NOT_USE_FLAG) || (var->flag & UNAVAILABLE_FLAG)) return;
	bool passed = true;
	if (var->flag & SMALL_VARIANT) {
		if (var->type == SNV || var->type == MNV) {
			if (var->depth[0][0] < param->vcp.min_depth[0] ||
				(var->read[0] + var->read[1]) < param->vcp.min_vdepth[0] ||
				var->qual < param->vcp.min_qual[0] ||
				var->frequency < param->vcp.min_freq[0]) passed = false;
		}
		else if (var->type == DELETION) {
			if (var->depth[0][0] < param->vcp.min_depth[1] ||
				(var->read[0] + var->read[1]) < param->vcp.min_vdepth[1] ||
				var->qual < param->vcp.min_qual[1] ||
				var->frequency < param->vcp.min_freq[1]) passed = false;
		}
		else if (var->type == INSERTION || var->type == DUPLICATION) {
			if (var->depth[0][0] < param->vcp.min_depth[2] ||
				(var->read[0] + var->read[1]) < param->vcp.min_vdepth[2] ||
				var->qual < param->vcp.min_qual[2] ||
				var->frequency < param->vcp.min_freq[2]) passed = false;
		}
	}
	else if (var->flag & CN_VARIANT) {
		if (var->pos[0].length(true) < param->cnvp.min_length) {
			if (var->pos[1].idx == -1) passed = false;
			else if (var->pos[1].length(true) < param->cnvp.min_length) passed = false;
		}
	}
	else if (var->flag & SR_VARIANT) {
		//
		if ((var->read[0] + var->read[1]) < param->svp.min_read ||
			var->frequency < param->svp.min_freq ||
			var->qual < param->svp.min_qual ||
			param->svp.read_bias < sbio::sutil::readBias(var->read)) passed = false;
		//
	}
	// Genotype
	if (param->homo_select) {
		if (!(var->genotype & HOMO_VAR)) passed = false;
	}
	// Target
	if (passed && target && target->size()) {
		if (!target->at(var->pos[0].idx).overlap(var->pos[0])) {
			if (var->pos[1].idx == -1) passed = false;
			else if (!target->at(var->pos[1].idx).overlap(var->pos[1])) passed = false;
		}
	}
	// Annotation
	if (passed && param->annot < 0xFF) {
		subyte site = 0;
		sfor(var->annotation) {
			sforeach(rna, $_.transcripts) site |= rna.site;
		}
		if (!(param->annot & site)) passed = false;
	}
	// Final
	if (!passed) {
		var->flag |= UNAVAILABLE_FLAG;
		var->attribute["filter"] = "NG";
	}
}
void slib::sbio::VarFilter::filter(slib::sbio::VarList& list) { sfor(list) check($_); }
void slib::sbio::VarFilter::setReference(SeqList* ref) { reference = ref; }
void slib::sbio::VarFilter::setDB(AnnotDB* db) { annotdb = db; }
void slib::sbio::VarFilter::setParam(VarParam* p) { param = p; }
void slib::sbio::VarFilter::setTarget(Array<sregion>* reg) { target = reg; }