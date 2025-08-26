#include "sio/sio.h"
#include "sutil/sjson.h"
#include "sbioinfo/variant.h"
#include "sbioinfo/gff.h"
#include "sbioinfo/vcf.h"

slib::sbio::VarList::VarList::VarList() : Array<sptr<Variant>>(), _ref(nullptr), _ftype((VL_FILE_TYPE)0), _load(false) {}
slib::sbio::VarList::VarList::VarList(slib::sbio::SeqList* ref) : slib::sbio::VarList() { setReference(_ref); }
slib::sbio::VarList::VarList::VarList(const char* s) : VarList() { load(s); }
slib::sbio::VarList::VarList::VarList(const VarList &vl) : Array<sptr<Variant>>(vl) {
	_ref = vl._ref;
	_ftype = vl._ftype;
	_load = vl._load;
	_lg = vl._lg;
    attribute = vl.attribute;
}
slib::sbio::VarList::VarList::~VarList() {}
void slib::sbio::VarList::open(const char* path, SeqList* ref) {
	auto ext = sfs::extension(path);
	_ref = ref;
	if (ext == "txt" || ext == "tsv") {
		auto txt = new SFile(path, slib::sio::READ);
		String ln;
		while (!txt) {
			txt->readLine(ln);
			if (ln[0] == '#') {
				auto vals = ln.split(TAB);
				attribute[vals[0]] = sjson::parse(vals[1]);
			}
			else {
				attribute["_header_"] = ln.split(TAB);
				break;
			}
		}
		_file = txt;
		_ftype = VL_FILE_TYPE::TXT;
	}
	else if (ext == "vcf") {
		auto vcf = new VcfFile(this);
		vcf->open(path);
		attribute = vcf->header;
		if (attribute.hasKey("contig")) {
			sfor(attribute["contig"]) _lg.add(Pair<String, int>($_["ID"], $_["length"]));
		}
		_file = (SFile*)vcf;
		_ftype = VL_FILE_TYPE::VCF;
	}
	else if (ext == "json") {
		_file = sjson::load(path);
		_lg.resize(_file["reference"].size());
		sfor2(_lg, _file["reference"]) {
			$_1.first = $_2["name"];
			$_1.second = $_2["length"];
		}
		attribute = _file["header"];
		_ftype = VL_FILE_TYPE::JSON;
	}
}
void slib::sbio::VarList::load(const char *path, SeqList *ref) {
	open(path, ref);
	switch (_ftype) {
	case VL_FILE_TYPE::TXT:
	{
		Variant var;
		auto txt = _file.file();
		auto colheader = attribute["_header_"];
		auto ridx = _ref->nameIndex();
		String ln;
		while (txt) {
			txt.readLine(ln);
			auto vals = ln.split(TAB);
			var.clear();
			sfor2(colheader, vals) {
				if ($_1 == "VarID")  var.varid = $_2;
				else if ($_1 == "Sample")  var.attribute["sample"] = $_2;
				/*
				else if ($_1 == "Tag")  var.type = ;
				else if ($_ == "Type")  strm << sbio::sutil::varTypeDesc(var.type) << (endl ? slib::LF : slib::TAB);
				*/
				else if ($_1 == "Category") {
					if ($_2 == "VC") var.flag = slib::sbio::SMALL_VARIANT;
					else  if ($_2 == "CNV") var.flag = slib::sbio::CN_VARIANT;
					else if ($_2 == "SV") var.flag = slib::sbio::SR_VARIANT;
				}
				
				if ($_1 == "Chr" || $_1 == "Chr1") var.pos[0].idx = ridx[$_2];
				else if ($_1 == "Chr2") var.pos[1].idx = ridx[$_2];
				else if ($_1 == "Pos" || $_1 == "Pos1") var.pos[0].begin = $_2.intValue();
				else if ($_1 == "Pos2") var.pos[1].begin = $_2.intValue();
				else if ($_1 == "Len" || $_1 == "Len1") var.pos[0].end = var.pos[0].begin + $_2.intValue() - 1;
				else if ($_1 == "Len2") var.pos[1].end = var.pos[1].begin + $_2.intValue() - 1;
				/*
				else if (col[i] == "Cov" || col[i] == "Cov1") var->depth[0][0] = dat[i].floatValue();
				else if (col[i] == "Allele Cov") var->depth[0][0] = dat[i].floatValue();
				else if (col[i] == "Cov2") var->depth[1][0] = dat[i].floatValue();
				else if (col[i] == "Allele Cov2") var->depth[1][0] = dat[i].floatValue();
				else if (col[i] == "Control Cov" || col[i] == "Control Cov1") var->depth[0][1] = dat[i].floatValue();
				else if (col[i] == "Control Cov2") var->depth[1][1] = dat[i].floatValue();
				else if (col[i] == "Copy" || col[i] == "Copy1") var->copy[0] = dat[i].floatValue();
				else if (col[i] == "Copy2") var->copy[1] = dat[i].floatValue();
				//			else if (col[i] == "Gene") file << "" << TAB;
				//			else if (col[i] == "Region") file << "" << TAB;
				//			else if (col[i] == "Mutant") file << "" << TAB;
				//			else if (col[i] == "Repeat") file << "" << TAB;
				//			else if (col[i] == "Mutation") file << "" << TAB;
				//			else if (col[i] == "Substitution") file << "" << TAB;

				else if (col[i] == "Ref" && dat[i] != "-") var->attribute["Ref"] = dat[i];
				else if (col[i] == "Var" && dat[i] != "-") var->alt = dat[i];
				else if (col[i] == "Type") var->type = sbio::sutil::varType(dat[i]);
				else if (col[i] == "Genotype" || col[i] == "Homo") var->genotype = (dat[i] == "Homo" ? sbio::HOMO_VAR : sbio::HETERO_VAR);
				else if (col[i] == "Qual") var->qual = dat[i].doubleValue();
				else if (col[i] == "Freq") var->frequency = dat[i].floatValue();
				else if (col[i] == "List" && list->name != dat[i]) list->name = dat[i];
				else if ($_1 == "Name") var->varid = $_2;
				*/
			}
			add(var);
		}
		break;
	}
	case VL_FILE_TYPE::VCF:
	{
		Variant* var;
		auto vcf = dynamic_cast<VcfFile *>(&_file.file());
		while (var = vcf->next()) { add(*var); }
		break;
	}
	case VL_FILE_TYPE::JSON:
	{
		Array<SPointer<Variant>>::resize(_file["variants"].size());
		sfor2(_file["variants"], *this) { $_2 = new Variant($_1.dict()); }
		break;
	}
	default:
		break;
	}
	_load = true;
}
void slib::sbio::VarList::save(const char *path, const char *opts) {
	sattribute attr;
	if (opts) attr = S(opts).parse(";", "=");
	auto ext = sfs::extension(path);
	if (ext == "txt") {
		SFile tf(path, slib::sio::MAKE);
		IOStream fs(tf, slib::sio::OSTREAM | slib::sio::FILEIO);
		auto header = attr.hasKey("cols") ?
			attr["cols"].split(",") :
			stringarray({
				"Sample", "Name", "Type", "Chr1", "Pos1", "Len1", "Chr2", "Pos2", "Len2", "Alt/Ins",
				"Qual", "Copy1", "Copy2", "Allele cov", "Freq", "Genotype", "Gene", "Site", "Mutation", "Mutant"
				});
		sbio::sio::writeHead(header, fs);
		sfor(*this) sbio::sio::writeVariant($_, header, _lg, fs);
	}
	else if (ext == "tsv") {
		SFile tf(path, slib::sio::MAKE);
		IOStream fs(tf, slib::sio::OSTREAM | slib::sio::FILEIO);
		auto header = attr.hasKey("cols") ? 
			attr["cols"].split(",") : 
			stringarray({
				"VarID", "Category", "Tag", "Type", 
				"Chr1", "Pos1", "Len1", "Ref", "Chr2", "Pos2", "Len2", "Alt/Ins",
				"Qual", "Freq", "Genotype", "Dp1", "Dp2", "VarRead", "Copy1", "Copy2",
				"Gene", "Site", "Mutation", "Substitution", "Filter"
				});
		sbio::sio::writeHead(header, fs);
		sfor(*this) sbio::sio::writeVariant($_, header, _lg, fs);
	}
	else if (ext == "vcf") {
		if (!_file || _ftype != VL_FILE_TYPE::VCF) {}
		VcfFile vf(this);
		vf.save(path);
	}
	else if (ext == "json") {
		sobj obj = {
			D_("reference", SArray(_lg.size())),
			D_("header", attribute),
			D_("variants", SArray(size()))
		};
		sfor2(_lg, obj["reference"]) { $_2 = { D_("name", $_1.first), D_("length", $_1.second) }; }
		sfor2(*this, obj["variants"]) { $_2 = $_1->toObj(); }
		sjson::save(obj, path);
	}
}
void slib::sbio::VarList::setReference(SeqList* r) { 
	_ref = r;
	_lg.resize(_ref->size());
	sfor2(*_ref, _lg) { $_2 = Pair<String, int>($_1.name, (int)$_1.length()); }
}
const slib::sbio::SeqList& slib::sbio::VarList::reference() const {
	if (!_ref) throw NullException(nullErrorText("Reference sequence"));
	return *_ref; 
}
const slib::Array<slib::Pair<slib::String, int>>& slib::sbio::VarList::linkageGroups() const { return _lg; }

slib::sbio::Variant* slib::sbio::VarList::next() {
	if (_file) {
		switch(_ftype) {
		case VL_FILE_TYPE::TXT:
			break;
		case VL_FILE_TYPE::GTF:
		{
			GffData *dat = dynamic_cast<GffFile*>(&_file.file())->next();
			if (dat) { _buffer = Variant(*dat, _ref); return &_buffer; }
			else return nullptr;
		}
		case VL_FILE_TYPE::VCF:
			return dynamic_cast<VcfFile*>(&_file.file())->next();
		default:
			break;
		}
	}
	return nullptr;
}

void slib::sbio::VarList::VarList::tidyUp(size_t s) {
	if (s == -1) {
		s = size();
		sfor(*this) {
			if ($_->flag & UNAVAILABLE_FLAG || ($_->flag & NOT_USE_FLAG)) --s;
		}
	}
	this->sort([](const sptr<Variant>& v1, const sptr<Variant>& v2) {
		if ((v1->flag & UNAVAILABLE_FLAG) || (v1->flag & NOT_USE_FLAG)) return false;
		if ((v2->flag & UNAVAILABLE_FLAG) || (v2->flag & NOT_USE_FLAG)) return true;
		return *v1 < *v2;		
	});
	this->resize(s);
}
void slib::sbio::VarList::VarList::addFlag(sushort f) { 
	sfor(*this) $_->flag |= f; 
}
void slib::sbio::VarList::VarList::removeFlag(sushort f) {
	sfor(*this) { if ($_->flag & f) $_->flag -= f; }
}
void slib::sbio::VarList::clearAll() {
    clear();
	_file = snull;
	_ftype = (VL_FILE_TYPE)0;
	_load = false;
	_lg.clear();
    attribute.clear();
}
slib::String slib::sbio::VarList::path() const {
	if (!_file.isNull()) return _file.file().path();
	return "";
}
