#include "sbioinfo/ngs.h"

slib::sbio::sngs::Summary::Summary() {
	seqtype = 0;
	refnum = 0;
	total = 0;
	avelen = 0.0;
	avedp = 0.0;
	cover = 0.0;
	bin = 1;
}
slib::sbio::sngs::Summary::~Summary() {}

void slib::sbio::sngs::Summary::reset(int n, sint* l) {
	seqid.clear();
	sample.clear();
	platform.clear();
	if (n != -1) {
		refnum = n;
		reflen.resize(refnum);
		count.resize(refnum);
		bases.resize(refnum);
	}
	if (l) {
		sforin(i, 0, n) { reflen[i] = *l; ++l; }
	}
	total = 0;
	avelen = 0.0;
	avedp = 0.0;
	cover = 0.0;
	count.reset(0);
	bases.reset(0);
}
bool slib::sbio::sngs::Summary::comparable(sngs::Summary& dat) {
	return refnum == dat.refnum && bin == dat.bin && reflen == dat.reflen;
}
inline int svindex(int i, slib::sbio::SVREAD_TYPE t) { return i * SV_TYPE_COUNT + (int)t; }
slib::sbio::NGSData::NGSData(slib::sbio::SeqList *ref, sngs::SEQ_TYPE type, int bin) : _loaded(false) {
	summary.seqtype = (subyte)type;
	summary.bin = bin; 
	if (ref) {
		setRefNum((int)ref->size());
		sforin(r, 0, ref->size()) { setRefLength(r, (int)ref->at(r).length()); }
	}
}
slib::sbio::NGSData::NGSData(const char *path) : slib::sbio::NGSData() { load(path); }
slib::sbio::NGSData::~NGSData() {}

inline void loadVariant(slib::Array<slib::sbio::SVar>* variants, slib::SFile& file) {
	slib::sint tmp;
	file.readInt(tmp);
	variants->resize(tmp);
	sfor(*variants) {
		file.readUShort($_.type);
		file.readInt($_.pos[0].idx);
		file.readInt(tmp);
		$_.pos[0].dir = tmp & 0x80000000 ? true : false;
		$_.pos[0].begin = tmp & 0x7FFFFFFF;
		file.readInt(tmp);
		$_.pos[0].end = $_.pos[0].begin + tmp - 1;
		file.readInt($_.pos[1].idx);
		file.readInt(tmp);
		$_.pos[1].dir = tmp & 0x80000000 ? true : false;
		$_.pos[1].begin = tmp & 0x7FFFFFFF;
		file.readInt(tmp);
		$_.pos[1].end = $_.pos[1].begin + tmp - 1;
		file.readInt(tmp);
		$_.alt.resize(tmp);
		if (tmp) file.readBytes(&$_.alt[0], tmp);
		file.readInt(tmp);
		$_.read[1] = tmp & 0xFFFF;
		$_.read[0] = (tmp >> 16) & 0xFFFF;
		file.readDouble($_.qual);
	}
}
void slib::sbio::NGSData::open(const char* path) {
	//
	_file.open(path);
	//
	sint tmp;
	subyte ver = 1;
	char magic[4];
	// Check
	_file.readBytes(magic, 4);
	if (memcmp(BSM_MAGIC, magic, 3) != 0)
		throw BioFileFormatException(BSM_FORM_ERR, bsmFormatErrorText(magic));
	//
	ver = (subyte)magic[3];
	//
	_file.readUByte(summary.seqtype);
	_file.readInt(tmp);
	setRefNum(tmp);
	_file.readUInteger(summary.total);
	_file.readDouble(summary.avelen);
	_file.readDouble(summary.avedp);
	_file.readDouble(summary.cover);
	_file.readInt(summary.bin);
	//
	if (1 < (int)ver) {
		_file.readInt(tmp);
		summary.seqid.resize(tmp);
		if (tmp) _file.readChars(&summary.seqid[0], tmp);
		_file.readInt(tmp);
		summary.sample.resize(tmp);
		if(tmp) _file.readChars(&summary.sample[0], tmp);
		_file.readInt(tmp);
		summary.platform.resize(tmp);
		if (tmp) _file.readChars(&summary.platform[0], tmp);
	}
	//
	sforin(r, 0, summary.refnum) {
		_file.readInt(tmp);
		if (tmp != r) throw SBioInfoException();// (ERR_INFO, SLIB_CONFLICT_ERROR, "reference index", "index value");
		//
		_file.readInt(tmp);
		setRefLength(r, tmp);
		_file.readInt(summary.count[r]);
		_file.readDouble(summary.bases[r]);
		//
		sforin(v, 0, SV_TYPE_COUNT) {
			loadVariant(&variants[SV_TYPE_COUNT * r + v], _file);
		}
	}
	// Make depth index	_index[0] = (suinteger)_file.offset();
	sforin(r, 1, summary.refnum) {
		_index[r] = _index[r - 1] + (((summary.reflen[r - 1] - 1) / summary.bin) + 1) * sizeof(float);
	}
}
void slib::sbio::NGSData::load(const char* path) {
	open(path);
	sforin(r, 0, summary.refnum) {
		depth[r].resize(((summary.reflen[r] - 1) / summary.bin) + 1);
		_file.readBytes(depth[r].data(), depth[r].size() * sizeof(float));
	}
	_file.close();
	_loaded = true;
}
inline void saveVariant(slib::Array<slib::sbio::SVar>* variants, slib::SFile& file) {
	slib::sint tmp = (int)variants->size();
	file.writeInt(tmp);
	sfor(*variants) {
		file.writeUShort($_.type);
		file.writeInt($_.pos[0].idx);
		tmp = ($_.pos[0].dir ? 0x80000000 : 0x00000000) | $_.pos[0].begin;
		file.writeInt(tmp);
		file.writeInt($_.pos[0].length() + 1);
		file.writeInt($_.pos[1].idx);
		tmp = ($_.pos[1].dir ? 0x80000000 : 0x00000000) | $_.pos[1].begin;
		file.writeInt(tmp);
		file.writeInt($_.pos[1].length() + 1);
		file.writeInt((slib::sint)$_.alt.size());
		if ($_.alt.size()) file.writeBytes(&$_.alt[0], $_.alt.size());
		tmp = ($_.read[0] << 16) | $_.read[1];
		file.writeInt(tmp);
		file.writeDouble($_.qual);
	}
	file.flush();
}
void slib::sbio::NGSData::save(const char* path) {
	float buf;
	slib::SFile file(path, slib::sio::MAKE);
	file.writeBytes((void*)BSM_MAGIC, 4);
	file.writeUByte(summary.seqtype);
	file.writeInt(summary.refnum);
	file.writeUInteger(summary.total);
	file.writeDouble(summary.avelen);
	file.writeDouble(summary.avedp);
	file.writeDouble(summary.cover);
	file.writeInt(summary.bin);
	file.flush();
	// attribute
	file.writeInt((int)summary.seqid.size());
	file.writeChars(summary.seqid);
	file.writeInt((int)summary.sample.size());
	file.writeChars(summary.sample);
	file.writeInt((int)summary.platform.size());
	file.writeChars(summary.platform);
	//
	sforin(r, 0, summary.refnum) {
		file.writeInt(r);
		file.writeInt(summary.reflen[r]);
		file.writeInt(summary.count[r]);
		file.writeDouble(summary.bases[r]);
		sforin(v, 0, SV_TYPE_COUNT) {
			saveVariant(&variants[SV_TYPE_COUNT * r + v], file);
		}
	}
	if (_loaded || (!depth.empty() && !depth[0].empty())) {
		sforin(r, 0, summary.refnum) {
			file.writeBytes(depth[r].data(), depth[r].size() * sizeof(float));
			file.flush();
		}
	}
	else {
		if (_file.eof()) _file.clear();
		sforin(r, 0, summary.refnum) {
			auto sz = ((summary.reflen[r] - 1) / summary.bin) + 1;
			_file.setOffset(_index[r]);
			sforin(d, 0, sz) {
				_file.readFloat(buf);
				file.writeFloat(buf);
			}
		}
	}
}
float* slib::sbio::NGSData::readDp(int idx, size_t off, size_t len, float* dp) {
	if (_loaded) return &depth[idx][(int)off / summary.bin];
	else {
		if (_file.eof()) _file.clear();
		Range<suinteger> range(_index[idx] + (off / summary.bin), ((len == -1 ? summary.reflen[idx] : (off + len)) - 1) / summary.bin + 1);
		_file.setOffset(range.begin);
		_file.readBytes(dp, range.length(true) * sizeof(float));
		return dp;
	}
}
void slib::sbio::NGSData::setSource(slib::sbio::BamFile& bam) {
	reset(bam.info.ref_num, bam.info.ref_length.data());
	auto lines = bam.info.text.splitLine();
	sforeach(ln, lines) {
		if (ln.beginWith("@RG")) {
			ln.replace("@RG", "").trim();
			auto rgdata = ln.parse("\t", ":");
			sfor(rgdata) {
				if ($_.key().match(REG("/id/i"))) summary.seqid = $_.value();
				else if ($_.key().match(REG("/sm/i"))) summary.sample = $_.value();
				else if ($_.key().match(REG("/pl/i"))) summary.platform = $_.value();
			}
			break;
		}
	}
}
void slib::sbio::NGSData::setRefNum(int n) {
	summary.refnum = n;
	summary.reflen.resize(n);
	summary.count.resize(n);
	summary.bases.resize(n);
	depth.resize(n);
	variants.resize(SV_TYPE_COUNT * n);
	_index.resize(n);
}
void slib::sbio::NGSData::setRefLength(int i, int l) {
	summary.reflen[i] = l;
	depth[i].resize(((summary.reflen[i] - 1) / summary.bin) + 1);
	depth[i].reset(0.f);
}
void slib::sbio::NGSData::setBin(int bin) {
	summary.bin = bin;
	sforin(r, 0, summary.refnum) {
		if (summary.reflen[r]) depth[r].resize((summary.reflen[r] - 1) / summary.bin + 1, 0.0f);
	}
}
void slib::sbio::NGSData::addVariant(SVar& var) {
	auto idx = svindex(var.pos[0].idx, slib::sbio::sutil::toSVIndex(var.type));
	variants[idx].add(var);
}
bool slib::sbio::NGSData::isLoaded() { return _loaded; }
slib::String slib::sbio::NGSData::path() const { return _file.path(); }
void slib::sbio::NGSData::print(IOStream& strm) {
	strm << S_(=) * 60 << NL << 
		SP * 5 << sstr::rfill("Reference count:", ' ', 25) << 
		sstr::rfill(String(summary.refnum), ' ', 15) << NL <<
		SP * 5 << sstr::rfill("Total read:", ' ', 25) <<
		sstr::rfill(String(summary.total), ' ', 15) << NL <<
		SP * 5 << sstr::rfill("Mean read length:", ' ', 25) <<
		sstr::rfill(String(summary.avelen), ' ', 15) << NL <<
		SP * 5 << sstr::rfill("Average depth:", ' ', 25) <<
		sstr::rfill(String(summary.avedp), ' ', 15) << NL <<
		SP * 5 << sstr::rfill("Cover ratio:", ' ', 25) <<
		sstr::rfill(String(summary.cover), ' ', 15) << NL <<
		S_(=) * 60 << NL;
	strm.flush();
}
void slib::sbio::NGSData::reset(int n, sint* l) {
	if (_file.isOpened()) _file.close();
	summary.reset(n, l);
	if (n != -1) {
		_index.resize(n);
		depth.resize(n);
		variants.resize(SV_TYPE_COUNT * n);
	}
	if (l) {
		sforin(r, 0, summary.refnum) {
			depth[r].resize(((summary.reflen[r] - 1) / summary.bin) + 1);
		}
	}
	sfor(depth) $_.reset(0.f);
	sfor(variants) $_.clear();
}