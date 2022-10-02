#include "sbioinfo/svariant.h"

using namespace slib;
using namespace slib::sbio;

svc_param::svc_param() {
	sforin(i, 0, 4) {
		min_depth[i] = DEFAULT_VC_DP;
		min_vdepth[i] = DEFAULT_VC_VDP;
		min_freq[i] = DEFAULT_VC_FREQ;
		homo_freq[i] = DEFAULT_VC_HOMO_FREQ;
		min_qual[i] = DEFAULT_VC_QUAL;
	}
}
svc_param::svc_param(const svc_param& par) {
	sforin(i, 0, 4) {
		min_depth[i] = par.min_depth[i];
		min_vdepth[i] = par.min_vdepth[i];
		min_freq[i] = par.min_freq[i];
		homo_freq[i] = par.homo_freq[i];
		min_qual[i] = par.min_qual[i];
	}
}
svc_param::~svc_param() {}
svc_param& svc_param::operator=(const svc_param& par) {
	sforin(i, 0, 4) {
		min_depth[i] = par.min_depth[i];
		min_vdepth[i] = par.min_vdepth[i];
		min_freq[i] = par.min_freq[i];
		homo_freq[i] = par.homo_freq[i];
		min_qual[i] = par.min_qual[i];
	}
	return *this;
}
void svc_param::set(const sobj& obj) {
	if (obj["snv"]) {
		auto snvdic = obj["snv"];
		if (!snvdic["dp"].isNull()) min_depth[0] = snvdic["dp"];
		if (!snvdic["vdp"].isNull()) min_vdepth[0] = snvdic["vdp"];
		if (!snvdic["freq"].isNull()) min_freq[0] = snvdic["freq"];
		if (!snvdic["border"].isNull()) homo_freq[0] = snvdic["border"];
		if (!snvdic["qual"].isNull()) min_qual[0] = snvdic["qual"];
	}
	if (obj["mnv"]) {
		auto mnvdic = obj["mnv"];
		if (!mnvdic["dp"].isNull()) min_depth[1] = mnvdic["dp"];
		if (!mnvdic["vdp"].isNull()) min_vdepth[1] = mnvdic["vdp"];
		if (!mnvdic["freq"].isNull()) min_freq[1] = mnvdic["freq"];
		if (!mnvdic["border"].isNull()) homo_freq[1] = mnvdic["border"];
		if (!mnvdic["qual"].isNull()) min_qual[1] = mnvdic["qual"];
	}
	if (obj["del"]) {
		auto deldic = obj["del"];
		if (!deldic["dp"].isNull()) min_depth[2] = deldic["dp"];
		if (!deldic["vdp"].isNull()) min_vdepth[2] = deldic["vdp"];
		if (!deldic["freq"].isNull()) min_freq[2] = deldic["freq"];
		if (!deldic["border"].isNull()) homo_freq[2] = deldic["border"];
		if (!deldic["qual"].isNull()) min_qual[2] = deldic["qual"];
	}
	if (obj["ins"]) {
		auto insdic = obj["ins"];
		if (!insdic["dp"].isNull()) min_depth[3] = insdic["dp"];
		if (!insdic["vdp"].isNull()) min_vdepth[3] = insdic["vdp"];
		if (!insdic["freq"].isNull()) min_freq[3] = insdic["freq"];
		if (!insdic["border"].isNull()) homo_freq[3] = insdic["border"];
		if (!insdic["qual"].isNull()) min_qual[3] = insdic["qual"];
	}
}
sobj svc_param::toObj() {
	sdict snvdic = { kv("dp", min_depth[0]), kv("vdp", min_vdepth[0]), kv("freq", min_freq[0]),
		kv("border", homo_freq[0]), kv("dp", min_qual[0]) };
	sdict mnvdic = { kv("dp", min_depth[1]), kv("vdp", min_vdepth[1]), kv("freq", min_freq[1]),
		kv("border", homo_freq[1]), kv("dp", min_qual[1]) };
	sdict deldic = { kv("dp", min_depth[2]), kv("vdp", min_vdepth[2]), kv("freq", min_freq[2]),
		kv("border", homo_freq[2]), kv("dp", min_qual[2]) };
	sdict insdic = { kv("dp", min_depth[3]), kv("vdp", min_vdepth[3]), kv("freq", min_freq[3]),
		kv("border", homo_freq[3]), kv("dp", min_qual[3]) };

	return {
		kv("snv", snvdic), kv("mnv", mnvdic), kv("del", deldic), kv("ins", insdic)
	};
}

scnv_param::scnv_param() {
	min_length = DEFAULT_MIN_CVLEN;
	min_bg = DEFAULT_BG_DEPTH;
	border[0] = DEFAULT_HOMO_DEL;
	border[1] = DEFAULT_DELCP;
	border[2] = DEFAULT_DUPCP;
	border[3] = DEFAULT_HOMO_DUPCP;
	border[4] = DEFAULT_MULCP;
	min_qual = DEFAULT_MIN_QUAL;
	emission = smatd(6, 6, 
		{
			1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 0.0, 0.0, 1.0
		});
	transition = smatd(6, 6,
		{
			0.5, 0.0, 0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.5, 0.0, 0.0, 0.0,
			1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0,
			0.0, 0.0, 0.5, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0, 0.5, 0.0,
			0.0, 0.0, 0.5, 0.0, 0.0, 0.5
		});
}
scnv_param::scnv_param(const scnv_param& par) {
	min_length = par.min_length;
	min_bg = par.min_bg;
	memcpy(border, par.border, sizeof(double) * 5);
	min_qual = par.min_qual;
	emission = par.emission;
	transition = par.transition;
}
scnv_param::~scnv_param() {}
scnv_param& scnv_param::operator=(const scnv_param& par) {
	min_length = par.min_length;
	min_bg = par.min_bg;
	memcpy(border, par.border, sizeof(double) * 5);
	min_qual = par.min_qual;
	emission = par.emission;
	transition = par.transition;
	return *this;
}
void scnv_param::set(const sobj& obj) {
	if (obj["len"]) min_length = obj["len"];
	if (obj["bg"]) min_bg = obj["bg"];
	if (obj["border"]) {
		auto& array = obj["border"].array();
		sforin(i, 0, 5) border[i] = array[i];
	}
	if (obj["qual"]) min_qual = obj["qual"];
	if (obj["emit"]) {
		auto pit = obj["emit"].array().begin();
		sforeach(emission) { E_ = *pit; ++pit; }
	}
	if (obj["trans"]) {
		auto pit = obj["trans"].array().begin();
		sforeach(transition) { E_ = *pit; ++pit; }
	}
}
sobj scnv_param::toObj() {
	sobj cnvborder, cnvprob, cnvtrans;
	sforin(i, 0, 5) cnvborder.add(border[i]);
	sforeach(emission) cnvprob.add(E_);
	sforeach(transition) cnvtrans.add(E_);

	return { 
		kv("len", min_length), kv("bg", min_bg), kv("border", cnvborder),
		kv("qual", min_qual), kv("emit", cnvprob), kv("trans", cnvtrans)
	};
}

ssrv_param::ssrv_param() {
	sforin(i, 0, 5) {
		detect_var[i] = true;
		min_sr[i] = DEFAULT_SR_READ;
	}
	sforin(i, 0, 4) {
		detect_comp_var[i] = true;
		min_comp_sr[i] = 2 * DEFAULT_SR_READ;
	}
	min_length[0] = DEFAULT_MIN_DEL_SIZE;
	min_length[1] = DEFAULT_MIN_INS_SIZE;
	min_length[2] = DEFAULT_MIN_INV_SIZE;
	freq_bin = DEFAULT_FREQ_BIN;
	min_freq = DEFAULT_MIN_FREQ;
	max_fr_bias = DEFAULT_FR_BIAS;
	max_comp_bias = DEFAULT_COMP_BIAS;
	homo_freq = DEFAULT_HOMO_FREQ;
	min_qual = 0.0;
}
ssrv_param::ssrv_param(const ssrv_param& par) {
	memcpy(detect_var, par.detect_var, sizeof(bool) * 5);
	memcpy(detect_comp_var, par.detect_comp_var, sizeof(bool) * 4);
	memcpy(min_sr, par.min_sr, sizeof(sint) * 5);
	memcpy(min_comp_sr, par.min_comp_sr, sizeof(sint) * 4);
	memcpy(min_length, par.min_length, sizeof(sint) * 3);
	freq_bin = par.freq_bin;
	min_freq = par.min_freq;
	max_fr_bias = par.max_fr_bias;
	max_comp_bias = par.max_comp_bias;
	homo_freq = par.homo_freq;
	min_qual = par.min_qual;
}
ssrv_param::~ssrv_param() {}
ssrv_param& ssrv_param::operator=(const ssrv_param& par) {
	memcpy(detect_var, par.detect_var, sizeof(bool) * 5);
	memcpy(detect_comp_var, par.detect_comp_var, sizeof(bool) * 4);
	memcpy(min_sr, par.min_sr, sizeof(sint) * 5);
	memcpy(min_comp_sr, par.min_comp_sr, sizeof(sint) * 4);
	memcpy(min_length, par.min_length, sizeof(sint) * 3);
	freq_bin = par.freq_bin;
	min_freq = par.min_freq;
	max_fr_bias = par.max_fr_bias;
	max_comp_bias = par.max_comp_bias;
	homo_freq = par.homo_freq;
	min_qual = par.min_qual;
	return *this;
}
void ssrv_param::set(const sobj& obj) {
	if (obj["del"]) {
		detect_var[0] = obj["del"]["detect"];
		min_sr[0] = obj["del"]["read"];
	}
	if (obj["dup"]) {
		detect_var[1] = obj["dup"]["detect"];
		min_sr[1] = obj["dup"]["read"];
	}
	if (obj["inv"]) {
		detect_var[2] = obj["inv"]["detect"];
		min_sr[2] = obj["inv"]["read"];
	}
	if (obj["trs"]) {
		detect_var[3] = obj["trs"]["detect"];
		min_sr[3] = obj["trs"]["read"];
	}
	if (obj["trinv"]) {
		detect_var[4] = obj["trinv"]["detect"];
		min_sr[4] = obj["trinv"]["read"];
	}
	if (obj["cins"]) {
		detect_comp_var[0] = obj["cins"]["detect"];
		min_comp_sr[0] = obj["cins"]["read"];
	}
	if (obj["cinv"]) {
		detect_comp_var[1] = obj["cinv"]["detect"];
		min_comp_sr[1] = obj["cinv"]["read"];
	}
	if (obj["ctrs"]) {
		detect_comp_var[2] = obj["ctrs"]["detect"];
		min_comp_sr[2] = obj["ctrs"]["read"];
	}
	if (obj["ctrinv"]) {
		detect_comp_var[3] = obj["ctrinv"]["detect"];
		min_comp_sr[3] = obj["ctrinv"]["read"];
	}
	if (obj["length"]) {
		auto& array = obj["length"].array();
		sforin(i, 0, 3) min_length[i] = array[i];
	}
	if (obj["bin"]) freq_bin = obj["bin"];
	if (obj["freq"]) min_freq = obj["freq"];
	if (obj["fbias"]) max_fr_bias = obj["fbias"];
	if (obj["cbias"]) max_comp_bias = obj["cbias"];
	if (obj["homo"]) homo_freq = obj["homo"];
	if (obj["qual"]) min_qual = obj["qual"];
}
sobj ssrv_param::toObj() {
	return {
		kv("del", V({ kv("detect", detect_var[0]), kv("read", min_sr[0]) })),
		kv("dup", V({ kv("detect", detect_var[1]), kv("read", min_sr[1]) })),
		kv("inv", V({ kv("detect", detect_var[2]), kv("read", min_sr[2]) })),
		kv("trs", V({ kv("detect", detect_var[3]), kv("read", min_sr[3]) })),
		kv("trinv", V({ kv("detect", detect_var[4]), kv("read", min_sr[4]) })),
		kv("cins",  V({ kv("detect", detect_comp_var[0]), kv("read", min_comp_sr[0]) })),
		kv("cinv", V({ kv("detect", detect_comp_var[1]), kv("read", min_comp_sr[1]) })),
		kv("ctrs", V({ kv("detect", detect_comp_var[2]), kv("read", min_comp_sr[2]) })),
		kv("ctrinv", V({ kv("detect", detect_comp_var[3]), kv("read", min_comp_sr[3]) })),
		kv("length", V({ min_length[0], min_length[1], min_length[2] })),
		kv("freq", min_freq), kv("fbias", max_fr_bias), kv("cbias", max_comp_bias),
		kv("bin", freq_bin), kv("homo", homo_freq), kv("qual", min_qual)
	};
}

svariant_param::svariant_param() {
	homo_select = false;
	annot = 0x01FF;
	max_dist = DEFAULT_DIST;
	max_diff = DEFAULT_DIFF;
}
svariant_param::svariant_param(const svariant_param& par) {
	homo_select = par.homo_select;
	annot = par.annot;
	max_dist = par.max_dist;
	max_diff = par.max_diff;
}
svariant_param::~svariant_param() {}
svariant_param& svariant_param::operator=(const svariant_param& par) {
	homo_select = par.homo_select;
	annot = par.annot;
	max_dist = par.max_dist;
	max_diff = par.max_diff;
	return *this;
}
void svariant_param::set(const sobj& obj) {
	if (obj["homo"]) homo_select = obj["homo"];
	if (obj["annot"]) annot = obj["annot"];
	if (obj["dist"]) max_dist = obj["dist"];
	if (obj["diff"]) max_diff = obj["diff"];
	if (obj["cnvpar"]) cnv_par.set(obj["cnvpar"]);
	if (obj["srvpar"]) srv_par.set(obj["srvpar"]);
	if (obj["smvpar"]) smv_par.set(obj["smvpar"]);
}
sobj svariant_param::toObj() {
	return {
		kv("homo", homo_select), kv("annot", annot), kv("dist", max_dist), kv("diff", max_diff), 
		kv("cnvpar", cnv_par.toObj()), kv("srvpar", srv_par.toObj()), kv("smvpar", smv_par.toObj())
	};
}
float SVarUtil::readBias(const sushort *r) {
	return abs((float)(r[0] - r[1]) / (r[0] + r[1]));
}
float SVarUtil::combBias(sushort *r1, sushort *r2) {
	return abs((float)((r1[0] + r1[1]) - (r2[0] + r2[1])) / (r1[0] + r1[1] + r2[0] + r2[1]));
}
const stringarray SVarIO::VCF_TABLE_COLUMNS = {
	"Chr", "Pos", "Ref", "Var","Type","Len", "Qual", "Freq", "Cov", "Allele cov",
	"Genotype", "Gene", "Region", "Repeat", "Mutation", "Substitution"
};
const stringarray SVarIO::DEFAULT_TABLE_COLUMNS = {
	"Sample", "Name", "Type", "Chr1", "Pos1", "Len1", "Chr2", "Pos2", "Len2", "Qual", "Freq", "Genotype",
	"Gene", "Site", "Mutation", "Mutant", "Copy1", "Copy2", "Split read"
};

const sattribute SVarIO::ION_VAR_CALL = { ks("FDP", "copy"), ks("AF", "freq"), ks("FSAF", "pread"), ks("FSAR", "nread") };

void SVarIO::loadTxt(sio::SFile& file, SVarList* list, SBSeqList* ref) {}
void SVarIO::loadTSV(sio::SFile& file, SVarList* list, SBSeqList* ref) {
	list->clearAll();
	list->filetype = "tsv";
	if (!file.isOpened()) file.open(nullptr, sio::READ);
	String row;
	file.readLine(row);
	auto col = row.split(TAB);
	while (!file.eof()) {
		file.readLine(row);
		auto dat = row.split(TAB);
		sforeachi(col) {
			SVariant *var = new SVariant();
			if (col[i] == "Chr" || col[i] == "Chr1") {
				var->ref[0] = dat[i]; var->pos[0].idx = (sint)ref->index[dat[i]];
			}
			else if (col[i] == "Chr2") {
				var->ref[1] = dat[i]; var->pos[1].idx = (sint)ref->index[dat[i]];
			}
			else if (col[i] == "Pos" || col[i] == "Pos1") var->pos[0].begin = dat[i];
			else if (col[i] == "Pos2") var->pos[1].begin = dat[i];
			else if (col[i] == "Len" || col[i] == "Len1") var->pos[0].end = var->pos[0].begin + dat[i].intValue() - 1;
			else if (col[i] == "Len2") var->pos[1].end = var->pos[1].begin + dat[i].intValue() - 1;
			else if (col[i] == "Cov" || col[i] == "Cov1") var->copy.depth[0][0] = dat[i];
			else if (col[i] == "Cov2") var->copy.depth[1][0] = dat[i];
			else if (col[i] == "Control Cov" || col[i] == "Control Cov1") var->copy.depth[0][1] = dat[i];
			else if (col[i] == "Control Cov2") var->copy.depth[1][1] = dat[i];

			//else if (col[i] == "Allele cov") var->read[0] = dat[i];
			else if (col[i] == "Normalized Cov") var->copy.ndepth[0][0] = dat[i];
			
			//else if (col[i] == "Normalized Allele cov") var->read[0] = dat[i];
			else if (col[i] == "Copy" || col[i] == "Copy1") var->copy.ratio[0] = dat[i];
			else if (col[i] == "Copy2") var->copy.ratio[1] = dat[i];
			//			else if (col[i] == "Gene") file << "" << TAB;
			//			else if (col[i] == "Region") file << "" << TAB;
			//			else if (col[i] == "Mutant") file << "" << TAB;
			//			else if (col[i] == "Repeat") file << "" << TAB;
			//			else if (col[i] == "Mutation") file << "" << TAB;
			//			else if (col[i] == "Substitution") file << "" << TAB;

			else if (col[i] == "Ref" && dat[i] != "-") var->attribute["Ref"] = dat[i];
			else if (col[i] == "Var" && dat[i] != "-") var->alt = dat[i];
			else if (col[i] == "Type") var->type = sbiutil::varTypeIdx(dat[i]);
			else if (col[i] == "Genotype" || col[i] == "Homo") var->homo = (dat[i] == "Homo");
			else if (col[i] == "Qual") var->qual = dat[i];
			else if (col[i] == "Freq") var->copy.frequency= dat[i];
			else if (col[i] == "List" && list->name != dat[i]) list->name = dat[i];
			else if (col[i] == "Name") var->name = dat[i];
			list->add(var);
		}
	}
	file.close();
}
inline void _readVCFHeader(String &row, SVarList *list) {
	size_t pos = row.find("=");
	if (pos == NOT_FOUND) return;
	String key = row.substring(2, pos - 2),
		val = row.substring(pos + 1);
	if (key == "INFO" || key == "FILTER" || key == "FORMAT" ||
		key == "ALT" || key == "PEDIGREE") {
		if (val.first() == '<' && val.last() == '>') val.transform(sstyle::DELETE_QUOTE);
		sattribute attr = val.parse(",", "=");
		sdict content;
		sforeach(attr) {
			if (E_.key == "ID") continue;
			content[E_.key] = E_.value.isQuoted() ? String::dequot(E_.value) : E_.value;
		}
		list->attribute[key][attr["ID"]] = content;
	}
	else if (key == "contig") {
		val.transform(sstyle::DELETE_QUOTE);
		sattribute attr = val.parse(",", "=");
		sdict content;
		sforeach(attr) {
			if (E_.key == "ID") list->refname.add(E_.value.isQuoted() ? String::dequot(E_.value) : E_.value);
			else if (E_.key == "length") list->reflength.add(E_.value.intValue());
			else if (E_.key == "assembly") list->refver = E_.value.isQuoted() ? String::dequot(E_.value) : E_.value;
		}
	}
	else {
		if (val.isQuoted()) val.transform(sstyle::DELETE_QUOTE);
		list->attribute[key] = val;
	}
}
inline void _readVCFTitle(String& row, SVarList *list, bool &format) {
	row.clip(1);
	stringarray header = row.split("\t");
	if (8 < header.size()) format = true;
	list->name = format ? header[9] : header[8];
}
inline void _VCFToSNV(SVariant &var, String& alt, stringarray& data) {
	var.type = SNV;
	var.attribute["Ref"] = data[3];
	var.alt = alt;
	var.pos[0].end = var.pos[0].begin;
}
inline void _VCFToMNV(SVariant &var, String& alt, stringarray& data) {
	int off = 0, end;
	while (off < data[3].length() && data[3][off] == alt[off]) ++off;
	end = data[3].length();
	while (0 < end && data[3][end - 1] == alt[end - 1]) --end;
	var.pos[0].begin += off;
	var.pos[0].end = var.pos[0].begin + (end - off) - 1;
	var.type = ((end - off) == 1 ? SNV : MNV);
	var.alt = alt.substring(off, end - off);
	var.attribute["Ref"] = data[3].substring(off, end - off);
}
inline void _VCFToDEL(SVariant &var, String& alt, stringarray& data) {
	var.type = DELETION;
	sint off = 1, len = (sint)data[3].length() - (sint)alt.length();
	while (off < alt.length() &&
		data[3][(sint)data[3].length() - off] == alt[(sint)alt.length() - off]) ++off;
	var.pos[0].begin += (sint)alt.length() - off + 1;
	var.pos[0].end = var.pos[0].begin + len - 1;
	var.attribute["Ref"] = data[3].substring(alt.length() - off + 1, len);
}
inline void _VCFToINS(SVariant &var, String& alt, stringarray& data) {
	var.type = INSERTION;
	int off = 1, len = (sint)(alt.length() - data[3].length());
	while (off < data[3].length() &&
		data[3][(sint)data[3].length() - off] == alt[(sint)alt.length() - off]) ++off;
	var.pos[0].begin += (sint)data[3].length() - off + 1;
	var.pos[0].end = var.pos[0].begin;
	var.alt = alt.substring(data[3].length() - off + 1, len).cstr();
	var.attribute["Ref"] = "";
}
inline void _VCFToVar(SVariant &var, String &alt, stringarray &data, stringarray &refname) {
	var.flag = SMALL_VARIANT;
	var.ref[0] = data[0];
	var.pos[0].idx = (sint)refname.find(data[0]);
	var.pos[0].begin = data[1].intValue();
	var.name = data[2];
	if (data[3].length() == alt.length()) {
		if (data[3].length() == 1) _VCFToSNV(var, alt, data);
		else _VCFToMNV(var, alt, data);
	}
	else if (data[3].length() < alt.length()) _VCFToINS(var, alt, data);
	else _VCFToDEL(var, alt, data);
	var.qual = data[5].real();
	var.attribute["Filter"] = data[6];
}
inline void _setVCFInfo(Array<SVariant>& variants, String& data, sobj& info_attr) {
	sattribute infos = data.parse(";", "=");
	sforeach(infos) {
		if (info_attr[E_.key]["Number"] == "A") {
			stringarray vals = E_.value.split(",");
			if (vals.size() != variants.size()) {
				std::cout << "err vcfinfo" << std::endl;
				throw SBioInfoException(ERR_INFO);
			}
			sforin(v, 0, variants.size()) variants[v].attribute[E_.key] = vals[v];
		}
		else { sforeach_(vit, variants) vit->attribute[E_.key] = E_.value; }
	}
}
inline void _setVCFFormat(Array<SVariant>& variants, String &keydat, String &formdat, sobj& form_attr) {
	stringarray fmkeys = keydat.split(":"), formats = formdat.split(":");
	sforin(i, 0, fmkeys.size()) {
		auto& key = fmkeys[i]; auto& format = formats[i];
		if (key == "GT") {
			stringarray genotype = format.contain("/") ?
				format.split("/") : format.split("|");
			if (variants.size() == 1) {
				if (genotype.size() != 2) {
					std::cout << "genotype error" << std::endl;
					throw SBioInfoException(ERR_INFO);
				}
				if (genotype[0] == genotype[1]) variants[0].homo = true;
				else variants[0].homo = false;
				variants[0].attribute[key] = format;
			}
			else {
				if (genotype.size() < 2) {
					std::cout << "genotype error" << std::endl;
					throw SBioInfoException(ERR_INFO);
				}
				sforin(v, 0, variants.size()) {
					size_t idx;
					if ((idx = genotype.find(String(v + 1))) == NOT_FOUND) variants[v].flag = UNAVAILABLE_FLAG;
					else {
						if (genotype.first() == "0") {
							variants[v].homo = false;
							variants[v].attribute[key] = "0/1";
						}
						else {
							if (genotype.find(String(v + 1), idx + 1) == NOT_FOUND) {
								variants[v].homo = false;
								variants[v].attribute[key] = "1/2";
							}
							else {
								variants[v].homo = true;
								variants[v].attribute[key] = "1/1";
							}
						}
					}
				}
			}
		}
		else {
		if (form_attr[key]["Number"] == "A") {
			stringarray vals = format.split(",");
			sforin(v, 0, variants.size()) variants[v].attribute[key] = vals[v];
		}
		else { sforeach_(vit, variants) vit->attribute[key] = format; }
		}
	}
}
inline void _readVCFData(String& row, SVarList* list, Array<SVariant>& variants, const sattribute* converter, bool format) {
	stringarray data = row.split("\t");
	if (format && data.size() < 10) throw SBioInfoException(ERR_INFO);
	else if (data.size() < 8) throw SBioInfoException(ERR_INFO);
	stringarray alts = data[4].split(",");
	auto vcount = alts.size();
	variants.resize(vcount);
	sforin(i, 0, vcount) _VCFToVar(variants[i], alts[i], data, list->refname);
	_setVCFInfo(variants, data[7], list->attribute["INFO"]);
	if (format) _setVCFFormat(variants, data[8], data[9], list->attribute["FORMAT"]);
	if (converter) {
		sforeach(*converter) {
			sforeach_(vit, variants) {
				if (vit->pos[0].idx < 0 || !(vit->attribute.hasKey(E_.key))) continue;
				auto& val = vit->attribute[E_.key];
				if (E_.value == "name") vit->name = (const char*)val;
				if (E_.value == "copy") vit->copy.depth[0][0] = val;
				if (E_.value == "pread") vit->read[0] = val;
				if (E_.value == "nread") vit->read[1] = val;
				if (E_.value == "freq") vit->copy.frequency = val;
				if (E_.value == "alt") vit->alt = (const char*)val;
				if (E_.value == "qual") vit->qual = val;
			}
		}
	}
	sforeach(variants) {
		if (E_.flag != UNAVAILABLE_FLAG) list->add(E_);
	}
}
void SVarIO::loadVCF(sio::SFile& file, SVarList* list, SBSeqList* ref, const sattribute* converter) {
	list->clearAll();
	list->filetype = "vcf";
	bool format = false;
	if (!file.isOpened()) file.open(nullptr, sio::READ);
	list->name = file.filename(false);
	Array<SVariant> variants;
	String row;
	while (!file.eof()) {
		file.readLine(row);
		if (row.empty()) continue;
		if (row.beginWith("##")) _readVCFHeader(row, list);
		else if (row[0] == '#') _readVCFTitle(row, list, format);
		else {
			_readVCFData(row, list, variants, converter, format);
			variants.clear();
		}
	}
	list->refnum = (sint)list->refname.size();
	list->sort();
	file.close();
}
void SVarIO::loadJSON(sio::SFile& file, SVarList* list) {
	SJson json(file);
	sforeach(json.array()) {
		std::cout << E_ << std::endl;
		list->add(E_);
		std::cout << list->last()->toString() << std::endl;
	}
}

void SVarIO::saveTxt(sio::SFile& file, SVarList* list) {}
void SVarIO::saveTSV(sio::SFile& file, SVarList* list, const stringarray& col) {
	if (!file.isOpened()) file.make();
	if (col.empty()) return;
	file << toString(col, TAB) << LF; file.flush();
	if (list->empty()) return;
	sforeach(*list) {
		sforeach_(cit, col) {
			if (*cit == "Chr" || *cit == "Chr1") file << E_->ref[0] << TAB;
			else if (*cit == "Chr2") file << (E_->pos[1].idx < 0 ? "-" : E_->ref[1]) << TAB;
			else if (*cit == "Pos" || *cit == "Pos1") file << E_->pos[0].begin << TAB;
			else if (*cit == "Pos2") file << (E_->pos[1].idx < 0 ? "-" : String(E_->pos[1].begin)) << TAB;
			else if (*cit == "Len" || *cit == "Len1") {
				if (E_->type & DELETION || E_->type == INVERSION || E_->type == DUPLICATION || E_->type == MULTIPLICATION) file << E_->pos[0].length(true) << TAB;
				else if (E_->type == INSERTION && E_->pos[1].idx < 0) file << E_->alt.length() << TAB;
				else if (E_->type == SNV || E_->type == MNV) file << E_->alt.length() << TAB;
				else file << "0" << TAB;
			}
			else if (*cit == "Len2") {
				if (E_->pos[1].idx < 0) {
					if (E_->type == DELETION) file << E_->alt.length() << TAB;
					else file << "-" << TAB;
				}
				else {
					if (E_->type & INSERTION) file << E_->pos[1].length() << TAB;
					else if ((E_->type & TRANSLOCATION) && (E_->type & DELETION)) file << E_->pos[1].length(false) - 1 << TAB;
					else if (E_->type & INVERSION) file << E_->pos[1].length(true) << TAB;
					else if (E_->type == SNV || E_->type == MNV) file << E_->alt.length() << TAB;
					else file << E_->pos[1].length() << TAB;
				}
			}
			else if (*cit == "Cov" || *cit == "Cov1") file << SNumber(E_->copy.depth[0][0]).precised(2) << TAB;
			else if (*cit == "Cov2") file << SNumber(E_->copy.depth[1][0]).precised(2) << TAB;
			/*
			*/
			else if (*cit == "Allele Cov" || *cit == "Split read") file << E_->total() << TAB;
			else if (*cit == "Allele Cov(+)" || *cit == "Split read(Fwd)") file << E_->read[0] << TAB;
			else if (*cit == "Allele Cov(-)" || *cit == "Split read(Rev)") file << E_->read[1] << TAB;
			else if (*cit == "Control Cov") file << SNumber(E_->copy.depth[0][1]).precised(2) << TAB;
			else if (*cit == "Copy" || *cit == "Copy1") file << SNumber(E_->copy.ratio[0]).precised(2) << TAB;
			else if (*cit == "Copy2") file << SNumber(E_->copy.ratio[1]).precised(2) << TAB;
			else if (*cit == "Read bias") file << SNumber(SVarUtil::readBias(E_->read)).precised(2) << TAB;
			else if (*cit == "Gene") {
				if (!E_->genes.empty()) {
					stringarray gnames;
					auto count = 0;
					sforeach_(git, E_->genes) {
						if (count == 5) break;
						gnames.add(git->name); ++count;
					}
					file << slib::toString(gnames) << (E_->genes.size() > 5 ? " etc...":"") << TAB;
				}
				else file << "-" << TAB;
			}
			else if (*cit == "Transcript") {
				if (!E_->genes.empty()) {
					stringarray ttypes;
					auto count = 0;
					sforeach_(git, E_->genes) {
						if (count == 5) break;
						ttypes.add(sbiutil::geneType(git->type)); ++count;
					}
					file << slib::toString(ttypes) << (E_->genes.size() > 5 ? " etc..." : "") << TAB;
				}
				else file << "-" << TAB;
			}
			else if (*cit == "Region" || *cit == "Site") {
				if (!E_->genes.empty()) {
					stringarray regions;
					auto count = 0;
					sforeach_(git, E_->genes) {
						if (count == 5) break;
						auto sites = sbiutil::geneSite(git->annotatedSite());
						regions.add(sites.empty()?"-":sites[0]);
						++count;
					}
					file << slib::toString(regions) << (E_->genes.size() > 5 ? " etc,,," : "") << TAB;
				}
				else file << "-" << TAB;
			}
			else if (*cit == "Mutant") {
				if (!E_->mutants.empty()) {
					file << (E_->mutants.size() > 5 ? slib::toString(E_->mutants.subarray(0, 5)) : slib::toString(E_->mutants)) << TAB;
				}
				else file << "-" << TAB;
			}
			else if (*cit == "Repeat") file << (E_->attribute["repeat"] ? "true" : "false") << TAB;
			else if (*cit == "Mutation") {
				if (!E_->genes.empty()) {
					stringarray mnames;
					auto count = 0;
					sforeach_(git, E_->genes) {
						if (count == 5) break;
						auto muts = sbiutil::mutType(git->mutation);
						mnames.add(muts.empty() ? "-" : muts[0]);
						++count;
					}
					file << slib::toString(mnames) << (E_->genes.size() > 5 ? " etc,,," : "") << TAB;
				}
				else file << "-" << TAB;
			}
			else if (*cit == "Substitution") {
				if (!E_->genes.empty()) {
					stringarray subs;
					auto count = 0;
					sforeach_(git, E_->genes) {
						if (count == 5) break;
						if (git->transcripts.empty()) subs.add("No transcript");
						else {
							String sub;
							sforeach_(tit, git->transcripts) {
								if (tit->site & sbio::CDS && 0 < tit->pos) sub << tit->name << ":" << tit->ori << tit->pos << tit->alt << ";";
							}
							subs.add(sub);
						}
						++count;
					}
					file << slib::toString(subs) << (E_->genes.size() > 5 ? " etc,,," : "") << TAB;
				}
				else file << "-" << TAB;
			}
			else if (*cit == "Ref") file << (E_->attribute["Ref"] ? E_->attribute["Ref"] : "-") << TAB;
			else if (*cit == "Var") file << (E_->alt.size() ? E_->alt : "-") << TAB;
			else if (*cit == "Type") file << slib::toString(sbiutil::varTypes(E_->type), "+") << TAB;
			else if (*cit == "Genotype" || *cit == "Homo") file << (E_->homo ? "Homo" : "Hetero") << TAB;
			else if (*cit == "Qual") file << SNumber(E_->qual).precised(2) << TAB;
			else if (*cit == "Freq") file << SNumber(E_->copy.frequency).precised(2) << TAB;
			else if (*cit == "Name") file << E_->name << TAB;
			else if (*cit == "Sample") file << list->name << TAB;
		}
		file << LF; file.flush();
	}
}
inline void _writeVCFContig(SVarList* list, SFile& file) {
	sforin(i, 0, list->refnum) {
		file << "##contig=<ID=" << list->refname[i];
		if (i < list->reflength.size()) file << ",length=" << list->reflength[i];
		if (list->refver.length()) file << ",assembly=" << list->refver;
		file << ">" << LF; file.flush();
	}
}
inline void _writeVCFHeader(SVarList* list, SFile& file, const char* tag) {
	auto data = list->attribute[tag];
	auto keys = data.keyset();
	sforeach(keys) {
		file << "##" << tag << "=<ID=" << E_;
		if (!data[E_].isNull()) {
			auto& dat = data[E_].dict();
			sforeach_(it_, dat) file << "," << it_->key << "=" << it_->value;
		}
		file << ">" << LF; file.flush();
	}
}
inline void _makeVCFAttribute(SVarList* list) {}

void SVarIO::saveVCF(sio::SFile& file, SVarList* list, SBSeqList* ref) {
	if (!file.isOpened()) file.make();
	file << "##fileformat=VCFv4.1" << LF;
	file << "##source=libsbioinfo" << LF;
	_writeVCFContig(list, file);
	bool format = false;
	stringarray info_key, format_key;
	if (list->filetype != "vcf") _makeVCFAttribute(list);
	format = list->attribute.hasKey("FORMAT");
	if (list->attribute["INFO"]) {
		info_key = list->attribute["INFO"].keyset();
		_writeVCFHeader(list, file, "INFO");
	}
	if (list->attribute["FILTER"]) _writeVCFHeader(list, file, "FILTER");
	if (list->attribute["FORMAT"]) {
		format_key = list->attribute["FORMAT"].keyset();
		_writeVCFHeader(list, file, "FORMAT");
	}
	if (list->attribute["ALT"]) _writeVCFHeader(list, file, "ALT");
	if (list->attribute["PEDIGREE"]) _writeVCFHeader(list, file, "PEDIGREE");
	file << "#CHROM\tPOS\tID\tREF\tALT\tQUAL\tFILTER\tINFO\t" << (format ? "FORMAT\t" : "") << list->name << LF; file.flush();
	sforeach(*list) {
		if (E_->type == DELETION) {
			auto refstr = ref->at(E_->pos[0].idx)->raw(E_->pos[0].begin - 2, E_->pos[0].end + 2 - E_->pos[0].begin),
				altstr = refstr.substring(0, 1);
			file << E_->ref[0] << "\t" << (E_->pos[0].begin - 1) << "\t" << E_->name << "\t" <<
				refstr << "\t" << altstr << "\t" << E_->qual << "\t" <<
				E_->attribute["Filter"] << "\t";
		}
		else if (E_->type == INSERTION) {
			auto refstr = ref->at(E_->pos[0].idx)->raw(E_->pos[0].begin - 2, 1),
				altstr = refstr.substring(0, 1) + E_->alt;
			file << E_->ref[0] << "\t" << (E_->pos[0].begin - 1) << "\t" << E_->name << "\t" <<
				refstr << "\t" << altstr << "\t" << E_->qual << "\t" <<
				E_->attribute["Filter"] << "\t";
		}
		else {
			file << E_->ref[0] << "\t" << E_->pos[0].begin << "\t" << E_->name << "\t" <<
				(E_->attribute["Ref"] ? E_->attribute["Ref"] : "N") << "\t" <<
				E_->alt << "\t" << E_->qual << "\t" <<
				E_->attribute["Filter"] << "\t";
		}
		String infostr = "";
		for (auto iit = info_key.begin(); iit < info_key.end(); ++iit) {
			if (E_->attribute[*iit]) infostr << (*iit) << "=" << E_->attribute[*iit] << ";";
		}
		if (infostr == "") infostr = "\t";
		else infostr.last() = '\t';
		file << infostr;
		if (format) {
			String format_key_str = "", format_str = "";
			for (auto fit = format_key.begin(); fit < format_key.end(); ++fit) {
				if (E_->attribute[*fit]) {
					format_key_str << (*fit) << ":";
					format_str << E_->attribute[*fit] << ":";
				}
			}
			if (format_key_str == "") format_key_str = "\t";
			else format_key_str.last() = '\t';
			if (format_str == "") format_str = LF;
			else format_str.last() = '\n';
			file << format_key_str << format_str;
		}
		else file << LF;
		file.flush();
	}
}
void SVarIO::saveJSON(sio::SFile& file, SVarList* list) {
	sarray array;
	sforeach(*list) array.add(E_->toObj());
	SJson json(array);
	json.save(file.path());
}
