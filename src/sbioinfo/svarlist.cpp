#include "sbioinfo/svariant.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::sbio;

SVarList::SVarList() : Array<svar>() {}
SVarList::SVarList(const char* s) : SVarList() { load(s); }
SVarList::SVarList(const SVarList &vl) : Array<svar>(vl) {
    //par = vl.par;
	list_type = vl.list_type;
    list_name = vl.list_name;
    list_attribute = vl.list_attribute;
}
SVarList::~SVarList() {}

void SVarList::load(const char *path, SBSeqList *ref, sattribute* converter) {
	sio::SFile file(path);
	auto ext = file.extension();
	if (ext == "vcf") SVarIO::loadVCF(file, this, ref, converter);
	else if (ext == "txt") SVarIO::loadTxt(file, this, ref);
	else if (ext == "json") SVarIO::loadJSON(file, this);

}

void SVarList::save(const char *path, SBSeqList *ref) {
	sio::SFile file(path);
	auto ext = file.extension();
	if (ext == "vcf") SVarIO::saveVCF(file, this, ref);
	else if (ext == "txt") SVarIO::saveTxt(file, this);
	else if (ext == "json") SVarIO::saveJSON(file, this);
}

void SVarList::tidy(size_t s) {
	if (s == -1) {
		s = size();
		sforeach(*this) {
			if (E_->flag & UNAVAILABLE_FLAG || (E_->flag & NOT_USE_FLAG)) --s;
		}
	}
	this->sort([](const svar& v1, const svar& v2) {
		if ((v1->flag & UNAVAILABLE_FLAG) || (v1->flag & NOT_USE_FLAG)) return false;
		if ((v2->flag & UNAVAILABLE_FLAG) || (v2->flag & NOT_USE_FLAG)) return true;
		return *v1 < *v2;		
	});
	this->resize(s);
}
void SVarList::clearAll() {
    clear();
	refnum = 0;
    refname.clear();
	reflength.clear();
	ref_ver = "";
	list_type = 0;
    list_name = "";
    list_attribute.clear();
}