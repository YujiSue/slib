#include "sbioinfo/sngs.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::sio;
using namespace slib::sbio;

sngs_param::sngs_param() : splitread(true), paired(false), pcrdup(false), bin(1), parallele(false), ref(nullptr) {}
sngs_param::sngs_param(bool sr, bool p, bool dp, sint b) : splitread(sr), paired(p), pcrdup(dp), bin(b), parallele(false), ref(nullptr) {}
sngs_param::~sngs_param() {}
sngs_param& sngs_param::operator=(const sngs_param& par) {
	splitread = par.splitread;
	paired = par.paired;
	pcrdup = par.pcrdup;
	parallele = par.parallele;
	bin = par.bin;
	return *this;
}
void sngs_param::setRef(SBSeqList* r) { ref = r; }
void sngs_param::loadTarget(const char* s) {
	if (!ref) throw SBioInfoException(ERR_INFO, SLIB_NULL_ERROR);
	target.resize(ref->size());
	sio::SFile file(s, sio::READ);
	String row;
	while (!file.eof()) {
		file.readLine(row);
		auto values = row.split(TAB);
		if (row.empty() || row[0] == '#' || values.size() < 3) continue;
		target[ref->index[values[0]]].add(srange(values[1], values[2]));
	}
}
void sngs_param::decodeTarget(sobj obj) {
	target.clear();
	if (obj.isNull()) return;
	auto s = obj.size();
	target.resize(s);
	sforin(i, 0, s) {
		auto& array = obj[i].array();
		auto& region = target[i];
		sforeach(array) {
			auto ui = E_.uinteger();
			region.add(srange(ui & 0xFFFFFFFF, (ui >> 32) & 0xFFFFFFFF));
		}
	}
}
sobj sngs_param::encodeTarget() {
	if (target.empty()) return snull;
	sarray array(target.size());
	sforeachi(target) {
		array[i] = sarray();
		sforeach(target[i]) {
			suinteger ui = (suinteger)E_.begin | ((suinteger)E_.end << 32);
			array[i].add(snum(ui));
		}
	}
	return array;
}
void sngs_param::set(const sobj& obj) {
	if (obj.hasKey("sr")) splitread = obj["sr"];
	if (obj.hasKey("pair")) paired = obj["pair"];
	if (obj.hasKey("dp")) pcrdup = obj["dp"];
	if (obj.hasKey("parallele")) parallele = obj["parallele"];
	if (obj.hasKey("bin")) bin = obj["bin"];
	if (obj.hasKey("target")) decodeTarget(obj["target"]);
}
sobj sngs_param::toObj() {
	return {
		kv("sr", splitread),
		kv("pair", paired),
		kv("dp", pcrdup),
		kv("parallele", parallele),
		kv("bin", bin),
		kv("target", encodeTarget())
	};
}

summary_data::summary_data() {
	refnum = 0;
	bin = 1;
	reads = 0;
	length = 0.0;
	depth = 0.0;
	cover = 0.0;
}
summary_data::~summary_data() {}
suinteger summary_data::total() {
	if (target.empty()) return sstat::sum(reflen);
	else {
		suinteger sum = 0;
		sforeach(target) sum += E_.length(true);
		return sum;
	}
}
void summary_data::init() {
	reads = 0;
	length = 0.0;
	depth = 0.0;
	cover = 0.0;
	count.reset(0);
	bases.reset(0.0);
}
bool summary_data::comparable(summary_data& dat) {
	return refnum == dat.refnum && bin == dat.bin && reflen == dat.reflen;
}
depth_data::depth_data() : current(0.0f) {}
depth_data::~depth_data() {}
suinteger depth_data::total() {
	suinteger ui = 0;
	sforeach(count) ui += E_.size();
	return ui;
}
void depth_data::init() {
	sforeach(count) E_.reset(0);
	offset.reset(0);
}
srvar_data::srvar_data() {}
srvar_data::~srvar_data() {}
void srvar_data::init() {
	sforeach(variants) E_.clear();
	offset.reset(0);
}

SNGSData::SNGSData() { init(); }
SNGSData::SNGSData(sngs_param* p) : SNGSData() { setParam(p); }
SNGSData::SNGSData(sint bin, SBSeqList *ref) : SNGSData() { reset(bin, ref); }
SNGSData::SNGSData(const char *path) : SNGSData() { load(path); }
SNGSData::~SNGSData() {}

void SNGSData::setNum(size_t num) {
    summary.refnum = num;
	summary.reflen.resize(num, 0);
	summary.count.resize(num, 0);
	summary.bases.resize(num, 0.0);
	depth.count.resize(num);
	depth.offset.resize(num, 0);
	srvs.variants.resize(5 * num); //del, dup, inv, trs, trinv
	srvs.offset.resize(5 * num, 0);
}
void SNGSData::setLength(int idx, size_t len) {
	summary.reflen[idx] = len;
	depth.count[idx].resize((len - 1) / summary.bin + 1, 0);
}
void SNGSData::setBin(sint bin) {
	summary.bin = bin;
	sforin(r, 0, summary.refnum) {
		if (summary.reflen[r]) depth.count[r].resize((summary.reflen[r] - 1) / summary.bin + 1, 0.0f);
	}
}
void SNGSData::setParam(sngs_param* p) {
	if (p && p->ref) {
		summary.bin = p->bin;
		setNum(p->ref->size());
		_lock = UArray<SLock>(p->ref->size() * 5);
		sforin(r, 0, p->ref->size()) {
			setLength(r, p->ref->at(r)->length());
		}
	}
}
void SNGSData::addVariant(sint idx, svar_data& var) { srvs.variants[idx].add(var); }
void SNGSData::addVariantAsync(sint idx, svar_data& var) {
	_lock[idx].lock();
	srvs.variants[idx].add(var);
	_lock[idx].unlock();
}
inline void readVar(svar_data& var, sio::SFile& file) {
	subyte byte;
	sint tmp; 
	file.readUByte(byte);
	var.type = byte;
	file.readInt(var.pos[0].idx);
	file.readInt(tmp);
	var.pos[0].dir = tmp & 0x80000000 ? true : false;
	var.pos[0].begin = tmp & 0x7FFFFFFF;
	file.readInt(tmp);
	var.pos[0].end = var.pos[0].begin + tmp - 1;
	file.readInt(var.pos[1].idx);
	file.readInt(tmp);
	var.pos[1].dir = tmp & 0x80000000 ? true : false;
	var.pos[1].begin = tmp & 0x7FFFFFFF;
	file.readInt(tmp);
	var.pos[1].end = var.pos[1].begin + tmp - 1;
	file.readInt(tmp);
	var.alt.resize(tmp);
	if (tmp) file.readBytes(&var.alt[0], tmp);
	file.readInt(tmp);
	var.read[1] = tmp & 0xFFFF;
	var.read[0] = (tmp >> 16) & 0xFFFF;
	file.readReal(var.qual);
}
inline void loadVariant(vararray *variants, sio::SFile &file) {
    subyte byte;
    sint tmp;
    file.readInt(tmp);
    variants->resize(tmp);
    sforeach(*variants) {
        file.readUByte(byte);
        E_.type = byte;
        file.readInt(E_.pos[0].idx);
        file.readInt(tmp);
        E_.pos[0].dir = tmp&0x80000000?true:false;
        E_.pos[0].begin = tmp&0x7FFFFFFF;
        file.readInt(tmp);
        E_.pos[0].end = E_.pos[0].begin+tmp-1;
        file.readInt(E_.pos[1].idx);
        file.readInt(tmp);
        E_.pos[1].dir = tmp&0x80000000?true:false;
        E_.pos[1].begin = tmp&0x7FFFFFFF;
        file.readInt(tmp);
        E_.pos[1].end = E_.pos[1].begin+tmp-1;
        file.readInt(tmp);
        E_.alt.resize(tmp);
        if (tmp) file.readBytes(&E_.alt[0], tmp);
        file.readInt(tmp);
        E_.read[1] = tmp&0xFFFF;
        E_.read[0] = (tmp>>16)&0xFFFF;
        file.readReal(E_.qual);
    }
}
void SNGSData::load(const char *path) {
	try {
        _file.open(path);
        char magic[4];
		_file.readBytes(magic, 4);
        if(memcmp(BSM_MAGIC, magic, 3) != 0)
            throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR , magic, u8"BSM");
        sint tmp;
		_file.readInt(tmp);
		setNum(tmp);
		_file.readUInteger(summary.reads);
		_file.readReal(summary.length);
		_file.readReal(summary.depth);
		_file.readReal(summary.cover);
		_file.readInt(summary.bin);
        sforin(r, 0, summary.refnum) {
			_file.readInt(tmp);
            if(tmp != r) throw SBioInfoException(ERR_INFO, SLIB_CONFLICT_ERROR, "reference index", "index value");
			_file.readInt(tmp);
			setLength(r, tmp);
			_file.readInt(tmp);
			summary.count[r] = tmp;
			sforin(v, 0, 5) {
				srvs.offset[5 * r + v] = _file.offset();
				loadVariant(&srvs.variants[5 * r + v], _file);
			}
        }
		sforin(d, 0, summary.refnum) {
			depth.offset[d] = _file.offset();
			_file.readBytes(depth.count[d].ptr(), depth.count[d].size() * sizeof(float));
		}
		_loaded = true;
		_file.close();
    }
	catch (SBioInfoException be) {
		be.print();
	}
	catch (SIOException ie) {
		ie.print();
	}
	catch (SException ex) {
		if (_file.isOpened()) _file.close();
	}
}
inline void throughVariant(sio::SFile& file) {
	subyte byte;
	sint size, tmp;
	double tmpd;
	file.readInt(size);
	sforin(i, 0, size) {
		file.readUByte(byte);
		file.readInt(tmp);
		file.readInt(tmp);
		file.readInt(tmp);
		file.readInt(tmp);
		file.readInt(tmp);
		file.readInt(tmp);
		file.readInt(tmp);
		file.setOffset(file.offset() + tmp);
		file.readInt(tmp);
		file.readReal(tmpd);
	}
}
void SNGSData::open(const char* path) {
	try {
		_file.open(path);
		char magic[4];
		_file.readBytes(magic, 4);
		if (memcmp(BSM_MAGIC, magic, 3) != 0)
			throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, magic, u8"BSM");
		sint tmp;
		_file.readInt(tmp);
		setNum(tmp);
		_file.readUInteger(summary.reads);
		_file.readReal(summary.length);
		_file.readReal(summary.depth);
		_file.readReal(summary.cover);
		_file.readInt(summary.bin);
		sforin(r, 0, summary.refnum) {
			_file.readInt(tmp);
			if (tmp != r) throw SBioInfoException(ERR_INFO, SLIB_CONFLICT_ERROR, "reference index", "index value");
			_file.readInt(tmp);
			summary.reflen[r] = tmp;
			_file.readInt(tmp);
			summary.count[r] = tmp;
			sforin(v, 0, 5) {
				srvs.offset[5 * r + v] = _file.offset();
				throughVariant(_file);
			}
		}
		depth.offset[0] = _file.offset();
		sforin(d, 1, summary.refnum) {
			depth.offset[d] = depth.offset[d - 1] + ((summary.reflen[d - 1] - 1) / summary.bin + 1) * sizeof(float);
		}
		if (_file.eof()) _file.clear();
	}
	catch (SBioInfoException be) {
		be.print();
	}
	catch (SIOException ie) {
		ie.print();
	}
}
inline void saveVariant(Array<svar_data> *variants, sio::SFile &file) {
    sint tmp = (sint)variants->size();
    file.writeInt(tmp);
    sforeach(*variants) {
        file.writeUByte(E_.type&0xFF);
        file.writeInt(E_.pos[0].idx);
        tmp = (E_.pos[0].dir?0x80000000:0x00000000)|E_.pos[0].begin;
        file.writeInt(tmp);
        file.writeInt(E_.pos[0].length()+1);
        file.writeInt(E_.pos[1].idx);
        tmp = (E_.pos[1].dir?0x80000000:0x00000000)|E_.pos[1].begin;
        file.writeInt(tmp);
        file.writeInt(E_.pos[1].length()+1);
        file.writeInt((sint)E_.alt.size());
        if (E_.alt.size()) file.writeBytes(&E_.alt[0], E_.alt.size());
        tmp = (E_.read[0]<<16)|E_.read[1];
        file.writeInt(tmp);
        file.writeReal(E_.qual);
    }
    file.flush();
}
void SNGSData::save(const char *path) {
    try {
        sio::SFile file(path, sio::WRITE|sio::CREATE);
        file.writeBytes((void *)BSM_MAGIC, 3);
        file.writeByte(2);
        file.writeInt(summary.refnum);
        file.writeUInteger(summary.reads);
        file.writeReal(summary.length);
        file.writeReal(summary.depth);
        file.writeReal(summary.cover);
        file.writeInt(summary.bin);
        file.flush();
        sforin(r, 0, summary.refnum) {
            file.writeInt(r);
            file.writeInt(summary.reflen[r]);
			file.writeInt(summary.count[r]);
			sforin(v, 0, 5) saveVariant(&srvs.variants[5 * r + v], file);
        }
        sforin(d, 0, summary.refnum) {
			file.writeBytes(depth.count[d].ptr(), depth.count[d].size() * sizeof(float));
            file.flush();
        }
    } 
	catch (SBioInfoException be) {
		be.print();
	}
	catch (SIOException ie) {
		ie.print();
	}
}
void SNGSData::readVariant(sint r, sint v) {
	if (!_file.isOpened()) throw SIOException(ERR_INFO);
	if (_file.eof()) _file.clear();
	_file.setOffset(srvs.offset[5 * r + v]);
}
void SNGSData::nextVar() {
	readVar(srvs.current, _file);
}
void SNGSData::readDepth(sint r, sint p) {
	if (!_file.isOpened()) throw SIOException(ERR_INFO);
	if (_file.eof()) _file.clear();
	_file.setOffset(depth.offset[r] + p * sizeof(float));
}
void SNGSData::nextDp() {
	_file.readFloat(depth.current);
}
/*
inline void varIdx1(Array<svar_data> *variants, varparray *index) {
    if (variants->empty()) return;
	auto it = variants->begin();
	auto idx = index->ptr();
	auto ptr = &E_;
	auto pos = (E_.pos[0].begin >> 14);
	sforin(i, 0, pos + 1) { (*idx) = ptr; ++idx; }
	NEXT_;
    while(it < variants->end()) {
		auto p = (E_.pos[0].begin >> 14);
		if (pos < p) {
			sforin(i, pos, p) { (*idx) = ptr; ++idx; }
			pos = p; ptr = &E_;
		}
    }
	sforin(i, pos, index->size()) { (*idx) = ptr; ++idx; }
}
*/
inline void varIdx2(Array<svar_data>* variants, varparray* pindex, varparray* nindex) {
	/*
    idx1->reset(-1); idx2->reset(-1);
    if (vec->empty()) return;
    auto off = -1;
    auto idx = idx1;
    bool rev = false;
    sforeach(*vec) {
        if (rev != E_.pos[0].dir) {
            rev = true;
            idx = idx2;
            off = -1;
        }
        if (off < E_.pos[0].begin) {
            idx->at(E_.pos[0].begin>>14) = it-vec->begin();
            while (off < E_.pos[0].begin) off += BAM_INDEX_BIN;
        }
    }
	*/
}
inline void varIdx3(Array<svar_data> *vec, intarray2d *idxs) {
    sforeach(*idxs) E_.reset(-1);
    if (vec->empty()) return;
    int off = -1, current = 0;
    auto idx = idxs->ptr();
    sforeach(*vec) {
        if (E_.pos[1].idx != current) {
            current = E_.pos[1].idx;
            idx = &idxs->at(current);
            off = -1;
        }
        if (off < E_.pos[0].begin) {
            idx->at(E_.pos[0].begin>>14) = it-vec->begin();
            while (off < E_.pos[0].begin) off += BAM_INDEX_BIN;
        }
    }
}

inline void varIdx4(Array<svar_data> *vec, intarray2d *idxs) {
    sforeach(*idxs) E_.reset(-1);
    if (vec->empty()) return;
    int off = -1, current = 0;
    auto idx = idxs->ptr();
    bool rev = false;
    sforeach(*vec) {
        if (E_.pos[1].idx != current) {
            current = E_.pos[1].idx;
            rev = E_.pos[0].dir;
            idx = &idxs->at(2*current+(rev?1:0));
            off = -1;
        }
        else if (rev != E_.pos[0].dir) {
            rev = E_.pos[0].dir;
            idx = &idxs->at(2*current+(rev?1:0));
            off = -1;
        }
        if (off < E_.pos[0].begin) {
            idx->at(E_.pos[0].begin>>14) = it-vec->begin();
            while (off < E_.pos[0].begin) off += BAM_INDEX_BIN;
        }
    }
}
/*
void SNGSData::makeVIndex(Array<varparray>& index, svariant_param* vp, SWork* threads) {
	auto size = 4 + 3 * summary.refnum - 1;
	index.resize(size * summary.refnum);
	sforin(r, 0, summary.refnum) {
		auto len = ((summary.reflen[r] - 1) >> 14) + 1;

		if (threads) {
			index[r * size].resize(len, nullptr);
			index[r * size + 1].resize(len, nullptr);
			index[r * size + 2].resize(len, nullptr);
			index[r * size + 3].resize(len, nullptr);

			threads->addTask(varIdx1, &srvs.variants[5 * r], &index[r * size]);
			threads->addTask(varIdx1, &srvs.variants[5 * r + 1], &index[r * size + 1]);
			threads->addTask(varIdx2, &srvs.variants[5 * r + 2], &index[r * size + 2], &index[r * size + 3]);

		}
		else {
			varIdx1(&srvs.variants[5 * r], &index[r * size]);
			varIdx1(&srvs.variants[5 * r + 1], &index[r * size + 1]);
			varIdx2(&srvs.variants[5 * r + 2], &index[r * size + 2], &index[r * size + 3]);

		}
		//if (threads) threads->addTask(varIdx3, &srvs.variants[5 * r + 3], &trsidx[i]);
		//if (threads) threads->addTask(varIdx4, &srvs.variants[5 * r + 4], &trinvidx[i]);
	}
	if (threads) threads->complete();
}
*/
/*
void SNGSData::varindex(svariant_param *vp) {
    sforin(r, 0, summary.refnum) {
		_threads.addTask(varIdx1, &srvs.variants[5 * r], &delidx[i]);
		_threads.addTask(varIdx1, &srvs.variants[5 * r + 1], &insidx[i]);
		_threads.addTask(varIdx2, &srvs.variants[5 * r + 2], &invidx[2 * i], &invidx[2 * i + 1]);
		_threads.addTask(varIdx3, &srvs.variants[5 * r + 3], &trsidx[i]);
		_threads.addTask(varIdx4, &srvs.variants[5 * r + 4], &trinvidx[i]);
    }
    _threads.complete();
}

inline void _shrink(Array<svar_data> *vec, size_t count) {
    std::sort(vec->begin(), vec->end(),
              [](const svar_data &v1, const svar_data &v2) {
                  if (!(v1.type)) return false;
                  if (!(v2.type)) return true;
                  return v1 < v2;
              });
    vec->resize(count);
}
*/
inline void _varSubtract(vararray *v1, vararray *v2, svariant_param *vp) {
	SVarFilter filter(nullptr, nullptr, vp);
	filter.subtract(*v1, *v2);
}
void SNGSData::subtract(SNGSData & dat, svariant_param *vp, SWork* threads) {
	if (summary.comparable(dat.summary)) {
		sforin(r, 0, summary.refnum) {
			sforin(v, 0, 5) {
				if (threads) threads->addTask(_varSubtract, &srvs.variants[5 * r + v], &dat.srvs.variants[5 * r + v], vp);
				_varSubtract(&srvs.variants[5 * r + v], &dat.srvs.variants[5 * r + v], vp);
			}
		}
		if (threads) threads->complete();
	}
	else SBioInfoException(ERR_INFO, SLIB_CONFLICT_ERROR, "reference count", "another summary");
}
/*
void _uncovered(float *dp, sint size, sint length, sint bin, sint *count) {
	sforin(i, 0, size - 1) {
		if ((*dp) < 1.0) (*count) += bin - (int)((*dp) * bin);
		++dp;
	}
	sint rest = length - (size - 1) * bin;
	if ((*dp) < 1.0) (*count) += rest - (int)((*dp) * rest);
}
void SNGSData::tidy(svariant_param *vp) {
    suint count = 0;
    total_length = 0;
    sforin(i, 0, ref_num) {
        //if (target_seq) total_length += target[i].length()+1;
        total_length += ref_length[i];
        total_reads += read_count[i];
        count += read_count[i];
        depth[i][depth_size[i]-1] *= depth_bin/(ref_length[i]-(depth_size[i]-1)*depth_bin);
        average_length += read_length[i];
        read_length[i] /= (double)read_count[i];
        _threads.addTask(_uncovered, depth[i].ptr(), depth_size[i], ref_length[i], depth_bin, &uncovered[i]);
        sforin (j, 0, 5) _threads.addTask(_var_integrate, &variants[5*i+j], vp);
    }
    average_depth = average_length/(double)total_length;
    average_length /= (double)count;
    _threads.complete();
    covered_region = 1.0-(double)sstat::sum(uncovered.ptr(), uncovered.size())/total_length;
}
*/

inline void _varMerge(vararray *variants, svariant_param* vp) {
	SVarFilter filter(nullptr, nullptr, vp);
	filter.consolidate(*variants);
}
inline void _addDepth(int r, SNGSData *dat1, SNGSData* dat2, sint *count) {
	auto len = dat1->depth.count[r].size() - 1;
	auto bin = dat1->summary.bin;
	auto dp1 = dat1->depth.count[r].ptr(), dp2 = dat2->depth.count[r].ptr();
    sforin(i, 0, len) {
        (*dp1) += (*dp2);
		if ((*dp1) < 1.0) (*count) += bin - (int)((*dp1) * bin);
        ++dp1; ++dp2;
    }
    (*dp1) += (*dp2);
	auto rest = dat1->summary.reflen[r] - len * bin;
	if ((*dp1) < 1.0) (*count) += rest - (int)((*dp1) * rest);
}
void SNGSData::integrate(SNGSData& dat, svariant_param* vp, SWork* threads) {
	if (summary.comparable(dat.summary)) {
		summary.length *= sstat::sum(summary.count);
		summary.length += dat.summary.length * sstat::sum(dat.summary.count);
		summary.reads += dat.summary.reads;
		sveci uncover(summary.refnum, 0);
		sforin(r, 0, summary.refnum) {
			summary.bases[r] *= summary.count[r];
			summary.bases[r] += dat.summary.bases[r] * dat.summary.count[r];
			summary.count[r] += dat.summary.count[r];
			summary.bases[r] /= summary.count[r];
			if (threads) threads->addTask(_addDepth, r, this, &dat, &uncover[r]);
			else _addDepth(r, this, &dat, &uncover[r]);
			sforin(v, 0, 5) {
				if (threads) threads->addTask(_varMerge, &srvs.variants[5 * r + v], vp);
				else _varMerge(&srvs.variants[5 * r + v], vp);
			}
		}
		summary.depth = summary.length / summary.total();
		summary.length /= sstat::sum(summary.count);
		if (threads) threads->complete();
		summary.cover = 1.0 - (double)sstat::sum(uncover) / summary.total();
	}
	else throw SBioInfoException(ERR_INFO, SLIB_CONFLICT_ERROR, "reference count", CONFLICT_TEXT("this", "dat"));
}
/*
String SNGSData::brief() const {
	String str;
	str << String("=") * 60 << NEW_LINE;
	str << SPACE * 5 << "Reference:" << NEW_LINE;
	str << SPACE * 5 << String("-") * 50 << NEW_LINE;
	str << SPACE * 10 << String("Name:").filled(24) << "|" << String("Length").filled(15, ' ') << NEW_LINE;
	str << SPACE * 5 << String("-") * 50 << NEW_LINE;
	sforin(r, 0, summary.refnum) {
		str << SPACE * 10 << summary.ref.name[r].filled(24) << "|" << String("Length").filled(15, ' ') << NEW_LINE;
	}
	str << SPACE * 5 << String("-") * 50 << NEW_LINE;
	str << SPACE * 5 << String("Reference count:").filled(25) << String(summary.refnum).filled(15, ' ', true) << NEW_LINE;
	str << SPACE * 5 << String("Total length:").filled(25) << String(total()).filled(15, ' ', true) << NEW_LINE;
	str << SPACE * 5 << String("Total read:").filled(25) << String(total()).filled(15, ' ', true) << NEW_LINE;
	str << NEW_LINE * 2;
	str << 

	str << String("=") * 60 << NEW_LINE;
}
*/
bool SNGSData::isLoaded() { return _loaded; }
void SNGSData::reset(sint bin, SBSeqList* ref) {
	setBin(bin);
	setNum(ref->size());
	sforin(r, 0, ref->size()) setLength(r, ref->at(r)->length());
}
void SNGSData::init() {
	if (_file.isOpened()) _file.close();
	_loaded = false;
	summary.init();
	depth.init();
	srvs.init();
}