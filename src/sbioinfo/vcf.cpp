#include "sbioinfo/variant.h"
#include "sbioinfo/vcf.h"

slib::String slib::sbio::vcfColErrorText(const char* s) { return "'" + S(s) + "'" + " does not have required column(s)."; }
slib::String slib::sbio::vcfColSizeErrorText(const char* s, int i) { return "'" + S(s) + "'" + " has only " + S(i) + " columns."; }

const stringarray slib::sbio::svcf::HEADER_KEY = {
	"INFO", "FILTER", "FORMAT", "SAMPLE", "PEDIGREE"
};

inline void toSNV(slib::sbio::Variant& var, slib::String& a, stringarray& data) {
	var.type = slib::sbio::SNV;
	var.pos[0].end = var.pos[0].begin;
	var.alt = a;
	var.attribute["_ref_"] = data[3];
}
inline void toMNV(slib::sbio::Variant& var, slib::String& a, stringarray& data) {
	int off = 0, end;
	while (off < data[3].size() && data[3][off] == a[off]) ++off;
	end = (int)data[3].size();
	while (0 < end && data[3][end - 1] == a[end - 1]) --end;
	var.pos[0].begin += off;
	var.pos[0].end = var.pos[0].begin + (end - off) - 1;
	var.type = ((end - off) == 1 ? slib::sbio::SNV : slib::sbio::MNV);
	var.alt = a.substring(off, end - off);
	var.attribute["_ref_"] = data[3].substring(off, end - off);
}
inline void toINS(slib::sbio::Variant& var, slib::String& a, stringarray& data) {
	var.type = slib::sbio::INSERTION;
	int off = 1, len = (int)(a.length() - data[3].length());
	while (off < data[3].length() &&
		data[3][(int)data[3].length() - off] == a[(int)a.length() - off]) ++off;
	var.pos[0].begin += (int)data[3].length() - off + 1;
	var.pos[0].end = var.pos[0].begin;
	var.alt = a.substring(data[3].length() - off + 1, len).cstr();
	var.attribute["_ref_"] = "";
}
inline void toDEL(slib::sbio::Variant& var, slib::String& a, stringarray& data) {
	var.type = slib::sbio::DELETION;
	int off = 1, len = (int)data[3].length() - (int)a.length();
	while (off < a.length() &&
		data[3][off] == a[off + 1]) ++off;
	var.pos[0].begin += off;
	var.pos[0].end = var.pos[0].begin + len - 1;
	var.attribute["_ref_"] = data[3].substring(off, len);
}
inline int slib::sbio::svcf::refindex(const char* name, const SDictionary& dict) {
	sfor(dict["contig"]) {
		if ($_["ID"] == name) return (int)$INDEX(dict["contig"]);
	}
	return -1;
}
inline void slib::sbio::svcf::vcf2Vars(stringarray& data, Array<Variant>& variants, const SDictionary& dict) {
	//if (var.flag & slib::sbio::NOT_USE_FLAG || var.flag & slib::sbio::UNAVAILABLE_FLAG) return;
	//var.ref[0] = data[0];
	auto alts = data[4].split(",");
	auto vcount = alts.size();
	variants.resize(vcount);
	sforin(i, 0, vcount) {
		auto& var = variants[i];
		var.pos[0].begin = data[1].intValue();
		var.varid = data[2];
		auto& alt = alts[i];
		if (sstr::isEnclosed(alt, "<>")) {
			alt.clip(1, alt.size() - 2);
			auto svs = alt.split(":");
			sforeach(sv, svs) {
				if (sv == "CNV") var.flag |= CN_VARIANT;
				else {
					var.flag |= SR_VARIANT;



				}
			}
		}
		else if (alt[0] == '[' || alt[0] == ']' || alt[-1] == '[' || alt[-1] == ']') {
			var.flag |= SR_VARIANT;



		}
		else {
			var.flag |= SMALL_VARIANT;
			var.pos[0].idx = svcf::refindex(data[0], dict);
			if (data[3].size() == alt.size()) {
				if (data[3].size() == 1) toSNV(var, alt, data);
				else toMNV(var, alt, data);
			}
			else if (data[3].size() < alt.size()) toINS(var, alt, data);
			else toDEL(var, alt, data);
		}
		var.qual = data[5].floatValue();
		var.attribute["filter"] = data[6];
	}
}
inline void slib::sbio::svcf::readInfo(slib::String& info, Array<Variant>& variants, const SDictionary& dict) {
	if (info.empty() || info == ".") {
		sforeach(var, variants) var.attribute["info"] = snull;
		return;
	}
	if (info.beginWith(".;")) info.clip(2);
	if (info.endWith(";.")) info.resize(info.size() - 2);
	if (info.match(";.;")) info.replace(";.;", ";");
	sattribute attributes = info.parse(";", "=");
	sforeach(attr, attributes) {
		if (dict[attr.key()]["Number"] == "A") {
			auto vals = attr.value().split(",");
			sfor2(variants, vals) {
				if ($_1.flag & NOT_USE_FLAG || $_1.flag & UNAVAILABLE_FLAG) return;
				if (!$_1.attribute.hasKey("info")) $_1.attribute["info"] = SDictionary();
				$_1.attribute["info"].set(attr.key(), $_2);
			}
		}
		else {
			sforeach(var, variants) {
				if (var.flag & NOT_USE_FLAG || var.flag & UNAVAILABLE_FLAG) continue;
				if (!var.attribute.hasKey("info")) var.attribute["info"] = SDictionary();
				var.attribute["info"].set(attr.key(), attr.value());
			}
		}
	}
}
inline void slib::sbio::svcf::readFormat(slib::String& fkey, slib::String& format, Array<Variant>& variants, const SDictionary& dict) {
	if (fkey == ".") { sfor(variants) $_.attribute["format"] = snull; return; }
	else { sfor(variants) $_.attribute["format"] = {D_("_key_", fkey)}; }
	auto keys = fkey.split(":"), attributes = format.split(":");
	sfor2(keys, attributes) {
		if ($_1 == "GT") {
			stringarray genotype = $_2.match("/") ? $_2.split("/") : $_2.split("|");
			sveci gtvec;
			sforeach(gen, genotype) {
				if (sstr::isNumeric(gen)) {
					auto gidx = gen.intValue();
					if (gtvec.size() <= gidx) gtvec.resize(gidx + 1, 0);
					++gtvec[gidx];
				}
			}
			if (sstat::sum(gtvec) == 2) {
				// Not variant
				if (gtvec[0] == 2) {
					sforeach(var, variants) var.flag |= NOT_USE_FLAG;
				}
				// Hetero
				else if (gtvec[0] == 1) {
					sforin(g, 1, gtvec.size()) {
						if (gtvec[g] == 1) variants[g - 1].genotype |= HETERO_VAR;
						else variants[g - 1].flag |= NOT_USE_FLAG;
					}
				}
				// Homo or trans hetero
				else {
					sforin(g, 1, gtvec.size()) {
						if (gtvec[g] == 1) variants[g - 1].genotype |= TRANS_HETERO_VAR;
						else if (gtvec[g] == 2) variants[g - 1].genotype |= HOMO_VAR;
						else variants[g - 1].flag |= NOT_USE_FLAG;
					}
				}
			}
			else if (sstat::sum(gtvec) == 0) {
				sforeach(var, variants) var.flag |= NOT_USE_FLAG;
			}
		}
		else {
			if (dict[$_1]["Number"] == "A") {
				stringarray vals = $_2.split(",");
				sforeach(var, variants) {
					if (!var.attribute.hasKey("format")) var.attribute["format"] = SDictionary();
					var.attribute["format"].set($_1, vals[&var - variants.data()]);
				}
			}
			else { 
				sforeach(var, variants) {
					if (!var.attribute.hasKey("format")) var.attribute["format"] = SDictionary();
					var.attribute["format"].set($_1, $_2);
				}
			}
		}
	}
}
inline void slib::sbio::svcf::setUniqueValue(Array<Variant>& variants, const SDictionary& attribute) {
	sforeach(var, variants) {
		sforeach(prog, attribute["_prog_"]) {
			if (prog == "tvc") {
				var.depth[0][0] = var.attribute["info"]["FDP"];
				var.frequency = var.attribute["info"]["AF"];
				var.read[0] = var.attribute["info"]["FSAF"];
				var.read[1] = var.attribute["info"]["FSAR"];
			}
			else if (prog == "gatk") {
				var.depth[0][0] = var.attribute["format"]["DP"];
				var.read[0] = (var.genotype & HETERO_VAR) ? var.attribute["format"]["AD"].intValue() : (var.depth[0][0] - var.attribute["format"]["AD"].intValue());
				var.frequency = (float)var.read[0] / var.depth[0][0];
			}
			else if (prog == "gdv") {}
			else if (prog == "evep") {



			}
			else if (prog == "sift") {
				if (var.attribute["info"].hasKey("SIFTINFO")) {
					//Allele|Transcript|GeneId|GeneName|Region|VariantType|Ref_Amino_Acid/Alt_AminoAcid|Amino_position|SIFT_score|SIFT_median|SIFT_num_seqs|Allele_Type|SIFT_prediction
					auto annots = var.attribute["info"]["SIFTINFO"].split(",");
					Map<String, int> gidx;
					sforeach(annot, annots) {
						auto cols = annot.split("|");
						if (!gidx.hasKey(cols[2])) {
							gidx[cols[2]] = (int)var.annotation.size();
							var.annotation.add();
							auto& gdat = var.annotation[-1];
							gdat.gid = cols[2];
							gdat.name = cols[3];
						}
						auto& gdat = var.annotation[gidx[cols[2]]];
						sbio::TranscriptAnnotData tdat;
						tdat.name = cols[1];
						if (cols[4] == "CDS") {
							if (gdat.type != GENE_TYPE::PROTEIN_CODING) gdat.type = GENE_TYPE::PROTEIN_CODING;
							tdat.site = sbio::CDS; 
							tdat.type = TRANSCRIPT_TYPE::M_RNA;
						}
						else if (cols[4] == "UTR_3") {
							if (gdat.type != GENE_TYPE::PROTEIN_CODING) gdat.type = GENE_TYPE::PROTEIN_CODING;
							tdat.site = sbio::UTR3; 
							tdat.type = TRANSCRIPT_TYPE::M_RNA;
						}
						else if (cols[4] == "UTR_5") {
							if (gdat.type != GENE_TYPE::PROTEIN_CODING) gdat.type = GENE_TYPE::PROTEIN_CODING;
							tdat.site = sbio::UTR5;
							tdat.type = TRANSCRIPT_TYPE::M_RNA;
						}
						else if (cols[4] == "tRNA") {
							if (gdat.type == GENE_TYPE::MISC_GENE) gdat.type = GENE_TYPE::NON_CODING;
							tdat.site = sbio::EXON; 
							tdat.type = TRANSCRIPT_TYPE::T_RNA;
						}
						else if (cols[4] == "rRNA") {
							if (gdat.type == GENE_TYPE::MISC_GENE) gdat.type = GENE_TYPE::NON_CODING;
							tdat.site = sbio::EXON; 
							tdat.type = TRANSCRIPT_TYPE::R_RNA;
						}
						else if (cols[4] == "ncRNA") {
							if (gdat.type == GENE_TYPE::MISC_GENE) gdat.type = GENE_TYPE::NON_CODING;
							tdat.site = sbio::EXON;
							tdat.type = TRANSCRIPT_TYPE::NC_RNA;
						}
						else if (cols[4] == "lincRNA") {
							if (gdat.type == GENE_TYPE::MISC_GENE) gdat.type = GENE_TYPE::NON_CODING;
							tdat.site = sbio::EXON;
							tdat.type = TRANSCRIPT_TYPE::LINC_RNA;
						}
						else if (cols[4] == "snoRNA") {
							if (gdat.type == GENE_TYPE::MISC_GENE) gdat.type = GENE_TYPE::NON_CODING;
							tdat.site = sbio::EXON;
							tdat.type = TRANSCRIPT_TYPE::SNO_RNA;
						}
						else if (cols[4] == "miRNA") {
							if (gdat.type == GENE_TYPE::MISC_GENE) gdat.type = GENE_TYPE::NON_CODING;
							tdat.site = sbio::EXON; 
							tdat.type = TRANSCRIPT_TYPE::MI_RNA;
						}
						else if (cols[4] == "piRNA") {
							if (gdat.type == GENE_TYPE::MISC_GENE) gdat.type = GENE_TYPE::NON_CODING;
							tdat.site = sbio::EXON; 
							tdat.type = TRANSCRIPT_TYPE::PI_RNA;
						}
						if (cols[5] == "SYNONYMOUS") tdat.mutation = sbio::SYNONYMOUS;
						else if (cols[5] == "NONSYNONYMOUS") tdat.mutation = sbio::MISSENSE;
						else if (cols[5] == "START-LOST") tdat.mutation = sbio::FMET_LOSS;
						else if (cols[5] == "STOP-GAIN") tdat.mutation = sbio::NONSENSE;
						else if (cols[5].beginWith("FRAMESHIFT")) {
							if (tdat.site & CDS) tdat.mutation = sbio::FRAME_SHIFT;
							else tdat.mutation = sbio::INDEL;
						}
						else if (cols[5].beginWith("NONFRAMESHIFT")) {
							if (tdat.site & CDS) tdat.mutation = sbio::FRAME_SHIFT;
							else tdat.mutation = sbio::INDEL;
						}
						if (cols[6] != "NA/NA") {
							tdat.substitution.prefix = 'p';
							auto strs = cols[6].split("/");
							tdat.substitution.ori = strs[0];
							tdat.substitution.alt = strs[1];
						}
						if (cols[7] != "NA") {
							tdat.substitution.pos.begin = cols[7].intValue();
							tdat.substitution.pos.end = tdat.substitution.pos.begin;
						}
						if (cols[8] != "NA") {
							tdat.score = cols[8].floatValue();
						}
						if (cols[12] == "TOLERATED") tdat.effect = VARIANT_EFFECT::TOLERATED;
						else if (cols[12].match("DELETERIOUS")) tdat.effect = VARIANT_EFFECT::DELETERIOUS;
						gdat.transcripts.add(tdat);
					}
				}
			}
			else if (prog == "slib") {


			
			}
		}
	}
}
inline void slib::sbio::svcf::readVariant(slib::String& row, Array<slib::sbio::Variant>& variants, const SDictionary &attribute) {
	auto columns = row.split(TAB);
	if (columns.size() < 8) throw VcfFormatException(vcfColSizeErrorText(row, columns.size()));
	vcf2Vars(columns, variants, attribute);
	readInfo(columns[7], variants, attribute["INFO"]);
	if (9 < columns.size()) {
		readFormat(columns[8], columns[9], variants, attribute["FORMAT"]);
	}
	setUniqueValue(variants, attribute);
}
inline void slib::sbio::svcf::getHeader(slib::String& str, SDictionary& attribute) {
	size_t pos = str.find("=");
	if (pos == NOT_FOUND) return;
	slib::String key = str.substring(2, pos - 2), val = str.substring(pos + 1);
	if (key == "contig" || key == "SAMPLE") {
		if (!attribute[key]) attribute[key] = SArray();
		if (sstr::isEnclosed(val, "<>")) val.clip(1, val.size() - 2);
		attribute[key].add(val.parse(",", "="));
	}
	else if (sstr::isEnclosed(val, "<>")) {
		val.clip(1, val.size() - 2);
		auto attr = val.parse(",", "=");
		if (attr.hasKey("ID")) attribute[key][attr["ID"]] = attr;
		else attribute[key] = attr;
	}
	else attribute[key] = val;
}
void slib::sbio::svcf::getSamples(slib::String& row, SDictionary& attribute) {
	auto columns = row.split(TAB);
	SArray samples;
	auto p = columns.indexOf("FORMAT");
	if (p == NOT_FOUND) p = columns.indexOf("INFO");
	if (p == NOT_FOUND) p = columns.indexOf("FILTER");
	if (p == NOT_FOUND) p = columns.indexOf("QUAL");
	if (p == NOT_FOUND) throw VcfFormatException(vcfColErrorText(row));
	auto it = columns.begin() + p + 1;
	sforin(i, p + 1, columns.size()) {
		samples.add({ D_("ID", $_) });
		$NEXT;
	}
	if (!attribute.hasKey("SAMPLE")) attribute["SAMPLE"] = samples;
}

void slib::sbio::svcf::defineSamples(const slib::sbio::VarList& list, IOStream& stream) {
	if (list.attribute.hasKey("SAMPLE")) {
		sfor(list.attribute["SAMPLE"]) stream << "##SAMPLE" << "=<ID=" << $_["ID"] << ">" << LF;
	}
	else if (list.attribute.hasKey("sample")) stream << "##SAMPLE" << "=<ID=" << list.attribute["sample"] << ">" << LF;
}
void slib::sbio::svcf::defineContigs(const slib::sbio::VarList& list, IOStream& stream) {
	if (list.attribute.hasKey("contig")) {
		sfor(list.attribute["contig"]) {
			stream << "##contig=<ID=" << $_["ID"] << ",length=" << $_["length"] << ">" << LF; stream.flush();
		}
	}
	else {
		auto& ref = list.reference();
		sfor(ref) {
			stream << "##contig=<ID=" << $_.name << ",length=" << $_.length();
			if (ref.attribute.hasKey("ver")) stream << ",assembly=" << ref.attribute["ver"];
			stream << ">" << LF; stream.flush();
		}
	}
}
inline void exoprtVCFHeaderInfo(const char *tag, const slib::String &key, const slib::SDictionary &dict, slib::IOStream& stream) {
	stream << "##" << tag << "=<ID=" << key;
	if (dict.hasKey("Type")) stream << ",Type=" << dict["Type"];
	if (dict.hasKey("Number")) stream << ",Number=" << dict["Number"];
	sforeach(data, dict) {
		if (data.key() != "Type" && data.key() != "Number" && data.key() != "Description") {
			stream << "," << data.key() << "=" << data.value();
		}
	}
	if (dict.hasKey("Description")) stream << ",Description=" << slib::sstr::dquote(dict["Description"]);
	stream << ">" << slib::LF;
}
void slib::sbio::svcf::defineAlt(const slib::sbio::VarList& list, slib::IOStream& stream) {
	sindex defined;
	if (list.attribute.hasKey("ALT")) {
		sfor(list.attribute["ALT"]) {
			defined[$_.key()] = 1;
			exoprtVCFHeaderInfo("ALT", $_.key(), $_.value(), stream);
		}
	}
	if (list.attribute.hasKey("detect-type")) {
		if (list.attribute["detect-type"].match("SV")) {
			if (!defined.hasKey("DEL")) stream << "##ALT=<ID=DEL,Description=\"Deletion\">" << LF;
				if (!defined["INS"]) stream << "##ALT=<ID=INS,Description=\"Insertion\">" << LF;
				if (!defined["DUP"]) stream << "##ALT=<ID=DUP,Description=\"Duplication\">" << LF;
				if (!defined["DUP:TANDEM"]) stream << "##ALT=<ID=DUP:TANDEM,Description=\"Tandem duplication\">" << LF;
				if (!defined["INV"]) stream << "##ALT=<ID=INV,Description=\"Inversion\">" << LF;
		}
		if (list.attribute["detect-type"].match("CNV")) {
			if (!defined["CNV"]) stream << "##ALT=<ID=CNV,Description=\"Copy number variant\">" << LF;
		}
	}
	stream.flush();
}
void slib::sbio::svcf::defineFilter(const slib::sbio::VarList& list, slib::IOStream& stream) {
	sindex defined;
	if (list.attribute.hasKey("FILTER")) {
		sfor(list.attribute["FILTER"]) {
			defined[$_.key()] = 1;
			exoprtVCFHeaderInfo("FILTER", $_.key(), $_.value(), stream);
		}
	}
	if (list.attribute.hasKey("filter")) {
		sfor(list.attribute["filter"]) {
			if (!defined.hasKey($_.key())) stream << "##FILTER=<ID=" << $_.key() << ",Description=\"" << $_.value() << "\">" << LF;
		}
	}
	stream.flush();
}
void slib::sbio::svcf::defineInfo(const slib::sbio::VarList& list, slib::IOStream& stream) {
	sindex defined;
	if (list.attribute.hasKey("INFO")) {
		sfor(list.attribute["INFO"]) {
			defined[$_.key()] = 1;
			exoprtVCFHeaderInfo("INFO", $_.key(), $_.value(), stream);
		}
	}
	if (!defined.hasKey("AF")) stream << "##INFO=<ID=AF,Number=1,Type=Float,Description=\"Variant frequency\">" << LF;
	if (!defined.hasKey("DP")) stream << "##INFO=<ID=DP,Number=1,Type=Integer,Description=\"Total depth\">" << LF;
	if (list.attribute["detect-type"].match("SV")) {
		if (!defined.hasKey("SVTYPE")) stream << "##INFO=<ID=SVTYPE,Number=1,Type=String,Description=\"SV type\">" << LF;
		if (!defined.hasKey("SVLEN")) stream << "##INFO=<ID=SVLEN,Number=1,Type=Integer,Description=\"SV size\">" << LF;
	}
	if (list.attribute["_prog_"].include("slib")) {
		if (!defined.hasKey("ASCF")) stream << "##INFO=<ID=ASCF,Number=1,Type=Integer,Description=\"Split/Chimera read count (forward)\">" << LF;
		if (!defined.hasKey("ASCR")) stream << "##INFO=<ID=ASCR,Number=1,Type=Integer,Description=\"Split/Chimera read count (reverse)\">" << LF;

		if (!defined.hasKey("INS")) stream << "##INFO=<ID=INS,Number=1,Type=String,Description=\"Insertion sequence\">" << LF;
		if (!defined.hasKey("INS:E")) stream << "##INFO=<ID=INS:E,Number=1,Type=String,Description=\"Aligned position of the inserted element\">" << LF;

		if (!defined.hasKey("INV:EXT")) stream << "##INFO=<ID=INV:EXT,Number=1,Type=String,Description=\"External break points of the inversion\">" << LF;
		if (!defined.hasKey("INV:INT")) stream << "##INFO=<ID=INV:INT,Number=1,Type=String,Description=\"Internal break points of the inversion\">" << LF;

		if (!defined.hasKey("TRS:FST")) stream << "##INFO=<ID=TRS:FST,Number=1,Type=String,Description=\"Break points on 1st linkage group of the translocation\">" << LF;
		if (!defined.hasKey("TRS:SND")) stream << "##INFO=<ID=TRS:SND,Number=1,Type=String,Description=\"Break points on 2nd linkage group of the translocation\">" << LF;

		if (!defined.hasKey("VPOS")) stream << "##INFO=<ID=VPOS,Number=1,Type=String,Description=\"Genomic coordinates of the variant\">" << LF;

		if (!defined.hasKey("ADP")) stream << "##INFO=<ID=ADP,Number=1,Type=Float,Description=\"Normalized depth of the variant site (sample)\">" << LF;
		if (!defined.hasKey("ADP2")) stream << "##INFO=<ID=ADP2,Number=1,Type=Float,Description=\"Normalized depth of the second variant site (sample)\">" << LF;
		if (!defined.hasKey("BDP")) stream << "##INFO=<ID=BDP,Number=1,Type=Float,Description=\"Normalized depth of the variant site (control)\">" << LF;
		if (!defined.hasKey("BDP2")) stream << "##INFO=<ID=BDP2,Number=1,Type=Float,Description=\"Normalized depth of the second variant site (control)\">" << LF;
		if (!defined.hasKey("CP")) stream << "##INFO=<ID=CP,Number=1,Type=Float,Description=\"Copy number\">" << LF;
		if (!defined.hasKey("CP2")) stream << "##INFO=<ID=CP2,Number=1,Type=Float,Description=\"Copy number of the second variant site\">" << LF;

		if (!defined.hasKey("VTAG")) stream << "##INFO=<ID=VTAG,Number=1,Type=String,Description=\"Variant category tag\">" << LF;
		if (!defined.hasKey("VDESC")) stream << "##INFO=<ID=VDESC,Number=1,Type=String,Description=\"Variant type description\">" << LF;
		if (!defined.hasKey("ANT")) stream << "##INFO=<ID=ANT,Number=1,Type=String,Description=\"Annotation information\">" << LF;
		if (!defined.hasKey("MUT")) stream << "##INFO=<ID=MUT,Number=1,Type=String,Description=\"Other mutations related to the annotated gene(s)\">" << LF;
	}
	stream.flush();
}
void slib::sbio::svcf::defineFormat(const slib::sbio::VarList& list, slib::IOStream& stream) {
	sindex defined;
	if (list.attribute.hasKey("FORMAT")) {
		sfor(list.attribute["FORMAT"]) {
			defined[$_.key()] = 1;
			exoprtVCFHeaderInfo("FORMAT", $_.key(), $_.value(), stream);
		}
	}
	if (!defined.hasKey("GT")) stream << "##INFO=<ID=GT,Number=A,Type=String,Description=\"Genotype\">" << LF;
	if (!defined.hasKey("GQ")) stream << "##INFO=<ID=GQ,Number=A,Type=Float,Description=\"Genotype quality\">" << LF;
	stream.flush();
}
void slib::sbio::svcf::writeHeader(const slib::sbio::VarList& list, IOStream& stream) {
	defineSamples(list, stream);
	defineContigs(list, stream);
	defineAlt(list, stream);
	defineFilter(list, stream);
	defineInfo(list, stream);
	if (list.attribute.hasKey("PEDIGREE")) {
		sfor(list.attribute["PEDIGREE"]) {
			exoprtVCFHeaderInfo("PEDIGREE", $_.key(), $_.value(), stream);
		}
	}
}
void slib::sbio::svcf::writeFilter(const Variant& var, IOStream& stream) {
	if (var.attribute.hasKey("filter")) stream << var.attribute["filter"] << "\t";
	else if (var.flag == NOT_USE_FLAG || var.flag == UNAVAILABLE_FLAG) stream << "NG\t";
	else stream << "PASS\t";
}
void slib::sbio::svcf::writeInfo(const Variant& var, const slib::sbio::SeqList& ref, IOStream& stream) {
	String infostr = "";
	if (var.attribute.hasKey("info") && var.attribute["info"]) {
		sfor(var.attribute["info"]) infostr << $_.key() << "=" << $_.value() << ";";
	}
	else if (var.flag & SR_VARIANT) {
		// SV type
		infostr << "VTAG=" << sbio::sutil::varTypeStr(var.type, ":") << ";";
		infostr << "VDESC=" << sbio::sutil::varTypeDesc(var.type) << ";";
		// Split/Chimera read count
		infostr << "ASCF=" << (int)var.read[0] << ";" << "ASCR=" << (int)var.read[1] << ";";
		// Allele frequency 
		infostr << "AF=" << slib::numToString(var.frequency, "%.2f") << ";";
		//
		if (var.type & INSERTION && var.pos[1].idx != -1) {
			infostr << "VPOS=" << ref[var.pos[0].idx].name << ":" << var.pos[0].begin << "-" << var.pos[0].end << ";";
			if (var.alt.size() && var.alt != "/") infostr << "INS=" << var.alt << ";";
			infostr << "INS:E=" << ref[var.pos[1].idx].name << ":" << var.pos[1].begin << "-" << var.pos[1].end << "(" << (var.pos[1].dir ? "-" : "+") << ");";
		}
		else {
			//
			if (var.type & TRANSLOCATION) {
				infostr << "TRS:FST" << ref[var.pos[0].idx].name << ":" << var.pos[0].begin << "-" << var.pos[0].end << ";";
				infostr << "TRS:SND" << ref[var.pos[1].idx].name << ":" << var.pos[1].begin << "-" << var.pos[1].end << "(" << (var.pos[1].dir ? "-" : "+") << ")";
				infostr << "ADP=" << slib::numToString(var.depth[0][0], "%.2f") << ";";
				infostr << "ADP2=" << slib::numToString(var.depth[1][0], "%.2f") << ";";
				infostr << "BDP=" << slib::numToString(var.depth[0][1], "%.2f") << ";";
				infostr << "BDP2=" << slib::numToString(var.depth[1][1], "%.2f") << ";";
				infostr << "CP=" << slib::numToString(var.copy[0], "%.2f") << ";";
				infostr << "CP2=" << slib::numToString(var.copy[1], "%.2f") << ";";
				if (var.alt.size() && var.alt != "/") infostr << "INS=" << var.alt << ";";
			}
			else if (var.type & INVERSION) {
				infostr << "INV:EXT=" << ref[var.pos[0].idx].name << ":" << var.pos[0].begin << "-" << var.pos[0].end << ";";
				infostr << "INV:INT=" << ref[var.pos[1].idx].name << ":" << var.pos[1].begin << "-" << var.pos[1].end << ";";
				infostr << "ADP=" << slib::numToString(var.depth[0][0], "%.2f") << ";";
				infostr << "ADP2=" << slib::numToString(var.depth[1][0], "%.2f") << ";";
				infostr << "BDP=" << slib::numToString(var.depth[0][1], "%.2f") << ";";
				infostr << "BDP2=" << slib::numToString(var.depth[1][1], "%.2f") << ";";
				infostr << "CP=" << slib::numToString(var.copy[0], "%.2f") << ";";
				infostr << "CP2=" << slib::numToString(var.copy[1], "%.2f") << ";";
				if (var.alt.size() && var.alt != "/") infostr << "INS=" << var.alt << ";";
			}
			else {
				infostr << "VPOS=" << ref[var.pos[0].idx].name << ":" << var.pos[0].begin << "-" << var.pos[0].end << ";";
				infostr << "SVLEN=" << var.pos[0].length(true) << ";";
				infostr << "END=" << var.pos[0].end << ";";
				infostr << "ADP=" << slib::numToString(var.depth[0][0], "%.2f") << ";";
				infostr << "BDP=" << slib::numToString(var.depth[0][1], "%.2f") << ";";
				infostr << "CP=" << slib::numToString(var.copy[0], "%.2f") << ";";
				if (var.alt.size() && var.alt != "/") infostr << "INS=" << var.alt << ";";
			}
		}
		if (var.annotation.size()) {
			String annotinfo;
			sforeach(gene, var.annotation) {
				annotinfo << "[" << 
					gene.gid << "|" << 
					gene.name << "|" << 
					sbio::sutil::geneType((int)gene.type) << "|" <<
					gene.varSite() << "|";
				sforeach(rna, gene.transcripts) {
					annotinfo << rna.name << "::" << sbio::sutil::transcriptType((int)rna.type) << "::" <<
						rna.pos.begin << "-" << rna.pos.end << "::" << rna.mutType() << "::" << rna.mutFormat() << "|";
				}
				annotinfo[-1] = ']';
			}
			infostr << "ANT=" << annotinfo << ";";
		}
		if (var.attribute.hasKey("mutant")) {
			infostr << "MUT=" << var.attribute["mutant"].toString("csv") << ";";
		}
	}
	if (infostr.empty()) infostr << "." << '\t';
	else infostr[-1] = '\t';
	stream << infostr;
}

void slib::sbio::svcf::writeFormat(const Variant& var, IOStream& stream) {
	String formstr = "", valstr = "";
	if (var.attribute.hasKey("format")) {
		auto keys = var.attribute["format"]["_key_"].split(":");
		sfor(keys) {
			formstr << $_ << ":";
			if ($_ == "GT") {
				if (var.genotype & HETERO_VAR) valstr << "0/1";
				else if (var.genotype & HOMO_VAR) valstr << "1/1";
				else if (var.genotype & TRANS_HETERO_VAR) valstr << "1/2";
				valstr << ":";
			}
			else {
				valstr << var.attribute["format"][$_] << ":";
			}
		}
	}
	else if (var.flag & SR_VARIANT) {
		formstr << "GT:GQ:";
		if (var.genotype & HETERO_VAR) valstr << "0/1:";
		else if (var.genotype & HOMO_VAR) valstr << "1/1:";
		else if (var.genotype & TRANS_HETERO_VAR) valstr << "1/2:";
		else valstr << "0/0:";
		
		//valstr << var.gqual;
		
		valstr << ".";

	}
	if (formstr.empty()) formstr << "." << '\t';
	else formstr[-1] = '\t';
	if (valstr.empty()) valstr << "." << '\n';
	else valstr[-1] = '\n';
	stream << formstr << valstr;
}

inline void writeCNV(const slib::sbio::Variant& var, const slib::sbio::SeqList& ref, slib::IOStream& stream) {
	stream << ref[var.pos[0].idx].name << slib::TAB <<
		var.pos[0].begin << slib::TAB <<
		var.varid << slib::TAB <<
		ref[var.pos[0].idx].raw(var.pos[0].begin - 1, 1) << slib::TAB <<
		"<cnv>" << slib::TAB <<
		slib::numToString(var.qual, "%.2f") << slib::TAB;
	slib::sbio::svcf::writeFilter(var, stream);
	slib::sbio::svcf::writeInfo(var, ref, stream);
	slib::sbio::svcf::writeFormat(var, stream);
}
inline void writeSV(const slib::sbio::Variant& var, const slib::sbio::SeqList& ref, slib::IOStream& stream) {
	if (var.pos[1].idx == -1) {
		slib::String altstr;
		//
		if (var.type & slib::sbio::DELETION) altstr = "<DEL>";
		if (var.type & slib::sbio::INSERTION) altstr = "<INS>";
		if (var.type & slib::sbio::DUPLICATION || var.type & slib::sbio::MULTIPLICATION) altstr = "<DUP>";
		//
		stream << ref[var.pos[0].idx].name << slib::TAB <<
			var.pos[0].begin << slib::TAB <<
			var.varid << slib::TAB <<
			ref[var.pos[0].idx].raw(var.pos[0].begin - 1, 1) << slib::TAB <<
			altstr << slib::TAB <<
			slib::numToString(var.qual, "%.2f") << slib::TAB;
		slib::sbio::svcf::writeFilter(var, stream);
		slib::sbio::svcf::writeInfo(var, ref, stream);
		slib::sbio::svcf::writeFormat(var, stream);
	}
	else {
		if ((var.type & slib::sbio::TRANSLOCATION) && !(var.type & slib::sbio::INSERTION)) {
			// (DEL+)TRS
			//		pos[0].begin >> pos[1].end | pos[1].begin >> pos[0].end
			//
			// (DEL+)INV+TRS
			//		pos[0].begin >< pos[1].begin | pos[1].end <> pos[0].end
			//
			auto vidx = 1;
			auto refstr = ref[var.pos[0].idx].raw(var.pos[0].begin - 1, 1);
			auto alts = var.alt.split("/");
			// pos[0].begin
			{
				stream << ref[var.pos[0].idx].name << slib::TAB << var.pos[0].begin << slib::TAB <<
					var.varid << "_bnd" << vidx << slib::TAB << refstr << slib::TAB;
				if (var.pos[1].dir) {
					stream << refstr << alts[0] << "]" << ref[var.pos[1].idx].name << ":" << var.pos[1].begin << "]" << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx + 2 << ";";
				}
				else {
					stream << refstr << alts[0] << "[" << ref[var.pos[1].idx].name << ":" << var.pos[1].end << "[" << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx + 3 << ";";
				}
				slib::sbio::svcf::writeInfo(var, ref, stream);
				slib::sbio::svcf::writeFormat(var, stream);
			}

			// pos[0].end
			{
				++vidx;
				refstr = ref[var.pos[0].idx].raw(var.pos[0].end - 1, 1);
				stream << ref[var.pos[0].idx].name << slib::TAB << var.pos[0].end << slib::TAB <<
					var.varid << "_bnd" << vidx << slib::TAB << refstr << slib::TAB;
				if (var.pos[1].dir) {
					stream << "[" << ref[var.pos[1].idx].name << ":" << var.pos[1].end << "[" << alts[1] << refstr << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx + 2 << ";";
				}
				else {
					stream << "]" << ref[var.pos[1].idx].name << ":" << var.pos[1].begin << "]" << alts[1] << refstr << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx + 1 << ";";
				}
				stream << slib::TAB << "." << slib::TAB << "." << slib::NL;
			}
			// pos[1].begin
			{
				++vidx;
				refstr = ref[var.pos[1].idx].raw(var.pos[1].begin - 1, 1);
				stream << ref[var.pos[1].idx].name << slib::TAB << var.pos[1].begin << slib::TAB <<
					var.varid << "_bnd" << vidx << slib::TAB << refstr << slib::TAB;
				if (var.pos[1].dir) {
					/*
					* caution alt direction
					*/
					stream << alts[1] << refstr  << "]" << ref[var.pos[0].idx].name << ":" << var.pos[0].begin << "]" << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx - 1 << ";";
				}
				else {
					stream << alts[1] << refstr << "[" << ref[var.pos[0].idx].name << ":" << var.pos[0].end << "[" << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx - 1 << ";";
				}
				stream << slib::TAB << "." << slib::TAB << "." << slib::NL;
			}
			// pos[1].end
			{
				++vidx;
				refstr = ref[var.pos[1].idx].raw(var.pos[1].end - 1, 1);
				stream << ref[var.pos[1].idx].name << slib::TAB << var.pos[1].end << slib::TAB <<
					var.varid << "_bnd" << vidx << slib::TAB << refstr << slib::TAB;
				if (var.pos[1].dir) {
					/*
					* caution alt direction
					*/
					stream << "[" << ref[var.pos[0].idx].name << ":" << var.pos[0].end << "[" << refstr << alts[1] << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx - 3 << ";";
				}
				else {
					stream << "]" << ref[var.pos[0].idx].name << ":" << var.pos[0].begin << "]" << alts[0] << refstr << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx - 3 << ";";
				}
				stream << slib::TAB << "." << slib::TAB << "." << slib::NL;
			}
		}
		else {
			// (DEL+)INS
			//		pos[0].begin >> pos[1].begin ... pos[1].end >> pos[0].end
			//
			// (DEL+)INV+INS
			//		pos[0].begin >< pos[1].end ... pos[1].begin <> pos[0].end
			//
			// (DEL+)TRS+INS
			//		pos[0].begin >> pos[1].begin ... pos[1].end >> pos[0].end
			//
			// (DEL+)TRS+INV+INS
			//		pos[0].begin >< pos[1].end ... pos[1].begin <> pos[0].end
			//
			// (DEL+)INV
			//		pos[0].begin >< pos[1].end ... pos[1].begin <> pos[0].end
			// 
			auto vidx = 1;
			auto refstr = ref[var.pos[0].idx].raw(var.pos[0].begin - 1, 1);
			auto alts = var.alt.split("/");
			// pos[0].begin
			{
				stream << ref[var.pos[0].idx].name << slib::TAB << var.pos[0].begin << slib::TAB <<
					var.varid << "_bnd" << vidx << slib::TAB << refstr << slib::TAB;
				if (var.pos[1].dir) {
					stream << refstr << alts[0] << "]" << ref[var.pos[1].idx].name << ":" << var.pos[1].end << "]" << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx + 3 << ";";
				}
				else {
					stream << refstr << alts[0] << "[" << ref[var.pos[1].idx].name << ":" << var.pos[1].begin << "[" << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx + 2 << ";";
				}
				slib::sbio::svcf::writeInfo(var, ref, stream);
				slib::sbio::svcf::writeFormat(var, stream);
			}
			// pos[0].end
			{
				++vidx;
				refstr = ref[var.pos[0].idx].raw(var.pos[0].end - 1, 1);
				stream << ref[var.pos[0].idx].name << slib::TAB << var.pos[0].end << slib::TAB <<
					var.varid << "_bnd" << vidx << slib::TAB << refstr << slib::TAB;
				if (var.pos[1].dir) {
					stream << "[" << ref[var.pos[1].idx].name << ":" << var.pos[1].begin << "[" << alts[1] << refstr << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx + 1 << ";";
				}
				else {
					stream << "]" << ref[var.pos[1].idx].name << ":" << var.pos[1].end << "]" << alts[1] << refstr << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx + 2 << ";";
				}
				stream << slib::TAB << "." << slib::TAB << "." << slib::NL;
			}
			// pos[1].begin
			{
				++vidx;
				refstr = ref[var.pos[1].idx].raw(var.pos[1].begin - 1, 1);
				stream << ref[var.pos[1].idx].name << slib::TAB << var.pos[1].begin << slib::TAB <<
					var.varid << "_bnd" << vidx << slib::TAB << refstr << slib::TAB;
				if (var.pos[1].dir) {
					/*
					* caution alt direction
					*/					
					stream << "[" << ref[var.pos[0].idx].name << ":" << var.pos[0].end << "[" << alts[1] << refstr << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx - 1 << ";";
				}
				else {
					stream << "]" << ref[var.pos[0].idx].name << ":" << var.pos[0].begin << "]" << alts[0] << refstr << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx - 2 << ";";
				}
				stream << slib::TAB << "." << slib::TAB << "." << slib::NL;
			}
			// pos[1].end
			{
				++vidx;
				refstr = ref[var.pos[1].idx].raw(var.pos[1].end - 1, 1);
				stream << ref[var.pos[1].idx].name << slib::TAB << var.pos[1].end << slib::TAB <<
					var.varid << "_bnd" << vidx << slib::TAB << refstr << slib::TAB;
				if (var.pos[1].dir) {
					/*
					* caution alt direction
					*/
					stream << refstr << alts[0] << "]" << ref[var.pos[0].idx].name << ":" << var.pos[0].begin << "]" << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx - 3 << ";";
				}
				else {
					stream << refstr << alts[1] << "[" << ref[var.pos[0].idx].name << ":" << var.pos[0].end << "[" << slib::TAB;
					stream << slib::numToString(var.qual, "%.2f") << slib::TAB;
					slib::sbio::svcf::writeFilter(var, stream);
					stream << "SVTYPE=BND;MATEID=" << var.varid << "_bnd" << vidx - 2 << ";";
				}
				stream << slib::TAB << "." << slib::TAB << "." << slib::NL;
			}
		}
	}
}

void slib::sbio::svcf::writeVariant(const Variant& var, const SeqList& ref, IOStream& stream) {
	if (var.flag & sbio::CN_VARIANT) writeCNV(var, ref, stream);
	else if (var.flag & sbio::SR_VARIANT) writeSV(var, ref, stream);
	else {
		if (var.type == DELETION) {
			auto refstr = ref[var.pos[0].idx].raw(var.pos[0].begin - 2, var.pos[0].end + 2 - var.pos[0].begin),
				altstr = refstr.substring(0, 1);
			stream << ref[var.pos[0].idx].name << TAB << (var.pos[0].begin - 1) << TAB <<
				var.varid << TAB << refstr << TAB << altstr << TAB;
		}
		else if (var.type == DUPLICATION || var.type == INSERTION) {
			auto refstr = ref[var.pos[0].idx].raw(var.pos[0].begin - 2, 1),
				altstr = refstr.substring(0, 1) + var.alt;
			stream << ref[var.pos[0].idx].name << TAB << (var.pos[0].begin - 1) << TAB <<
				var.varid << TAB << refstr << TAB << altstr << TAB;
		}
		else {
			auto refstr = ref[var.pos[0].idx].raw(var.pos[0].begin - 1, var.pos[0].length(true));
			stream << ref[var.pos[0].idx].name << TAB << var.pos[0].begin << TAB <<
				var.varid << TAB << refstr << TAB << var.alt << TAB;
		}
		stream << slib::numToString(var.qual, "%.2f") << TAB;
		svcf::writeFilter(var, stream);
		svcf::writeInfo(var, ref, stream);
		svcf::writeFormat(var, stream);
	}	
}

slib::sbio::VcfFile::VcfFile() : _vlist(nullptr) {}
slib::sbio::VcfFile::VcfFile(VarList* vl) { _vlist = vl; header = _vlist->attribute; }
slib::sbio::VcfFile::VcfFile(const char* s) { open(s); }
slib::sbio::VcfFile::~VcfFile(){}
slib::sbio::VcfFile& slib::sbio::VcfFile::operator=(const slib::sbio::VarList& vl) { 
	_vlist = const_cast<VarList*>(&vl); return *this;
}
void slib::sbio::VcfFile::operator >> (VarList& vl) {
	Variant* v;
	vl.clearAll();
	vl.attribute["_origin_"] = "vcf";
	bool format = false;
	if (header.empty()) readHeader();
	vl.attribute.update(header);
	while (v = next()) { 
		if (v->flag & NOT_USE_FLAG || v->flag & UNAVAILABLE_FLAG) continue;
		vl.add(sptr<Variant>(*v)); 
	}
}
void slib::sbio::VcfFile::open(const char* path) {
	SFile::open(path, slib::sio::READ);
	readHeader();
}
void slib::sbio::VcfFile::save(const char* path) {
	if (!_vlist) throw NullException(nullErrorText("VarList object"));
	SFile out(path, slib::sio::MAKE);
	IOStream stream(out, slib::sio::FILEIO|slib::sio::OSTREAM);
	stream << "##fileformat=VCF" << svcf::VERSION << LF;
	stream << "##date=" << SDate().toString() << LF;
	if (_vlist->attribute.hasKey("_prog_")) {
		sforeach(prog, _vlist->attribute["_prog_"]) stream << "##source=" << prog << LF;
	}
	// 
	svcf::writeHeader(*_vlist, stream);
	//
	stream << "#CHROM\tPOS\tID\tREF\tALT\tQUAL\tFILTER\tINFO\tFORMAT";
	if (_vlist->attribute.hasKey("SAMPLE")) {
		sforeach(smpl, _vlist->attribute["SAMPLE"]) stream << "\t" << smpl["ID"];
	}
	else if (_vlist->attribute.hasKey("sample")) stream << "\t" << _vlist->attribute["sample"];
	else stream << "\t" << "list";
	stream << "\n";
	stream.flush();
	_buffer.clear();
	sfor(*_vlist) { svcf::writeVariant(*$_, _vlist->reference(), stream); }
}
void checkProg(slib::SDictionary &dict) {
	dict["_prog_"] = slib::SArray();
	// Check  caller
	if (dict.hasKey("GATKCommandLine")) dict["_prog_"].add("gatk");
	if (dict.hasKey("DeepVariant_version")) dict["_prog_"].add("gdv");
	if (dict.hasKey("source")) {
		slib::SString& src = dict["source"];
		if (src.match("Torrent Variant Caller")) dict["_prog_"].add("tvc");
		else if (src.match("(slib)")) dict["_prog_"] = "slib";
	}
	// Check annotator
	if (dict["INFO"].hasKey("CSQ") && dict["INFO"]["CSQ"]["Description"].match("Ensembl VEP")) dict["_prog_"].add("vep");
	if (dict["INFO"].hasKey("SIFTINFO")) dict["_prog_"].add("sift");
}
void slib::sbio::VcfFile::readHeader() {
	header.clear();
	while (*this) {
		SFile::readLine(_ln);
		if (_ln.empty()) continue;
		if (_ln.beginWith("##")) svcf::getHeader(_ln, header);
		else if (_ln[0] == '#') svcf::getSamples(_ln, header);
		else break;
	}
	checkProg(header);
}
slib::sbio::Variant* slib::sbio::VcfFile::next() {
	if (SFile::eof()) return nullptr;
	if (_buffer.size()) _buffer.removeAt(0);
	while (_buffer.empty()) {
		svcf::readVariant(_ln, _buffer, header);
		SFile::readLine(_ln);
	}
	return &_buffer[0];
}