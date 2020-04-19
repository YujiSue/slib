#include "sbioinfo/sngs.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::sio;
using namespace slib::sbio;

sngs_param::sngs_param() {
    thread_count = DEFAULT_THREAD_COUNT;
    depth_bin = DEFAULT_DEPTH_BIN;
    parallel = false;
	ignore_dup = true;
}
sngs_param::~sngs_param() {}

sngs_param &sngs_param::operator=(const sngs_param &par) {
    thread_count = par.thread_count;
    depth_bin = par.depth_bin;
    parallel = par.parallel;
	ignore_dup = par.ignore_dup;
    return *this;
}

void sngs_param::set(const sobj &obj) {
    if (obj["thread"]) thread_count = obj["thread"];
    if (obj["bin"]) depth_bin = obj["bin"];
    if (obj["parallel"]) parallel = obj["parallel"];
	if (obj["duplication"]) ignore_dup = obj["duplication"];
}
sobj sngs_param::toObj() {
    return { kv("thread", thread_count), kv("bin", depth_bin), kv("parallel", parallel), kv("duplication", ignore_dup) };
}


SNGSData::SNGSData() {
    target_seq = false;
    ref_num = 0;
    total_length = 0;
    total_reads = 0;
    average_length = 0.0;
    average_depth = 0.0;
    covered_region = 0.0;
    depth_bin = 1;
}
SNGSData::SNGSData(sngs_param *p) : SNGSData() {
    setParam(p);
}
SNGSData::SNGSData(sngs_param *p, SBSeqList *list) : SNGSData(p) {
    setNum(list->size());
    sforin(i, 0, list->size()) setLength(i, list->at(i)->length());
}
SNGSData::SNGSData(sngs_param *p, SBamFile *bam) : SNGSData(p) {
    setNum(bam->info.ref_num);
    sforin(i, 0, bam->info.ref_num) setLength(i, bam->info.ref_length[i]);
}
SNGSData::SNGSData(const char *path) : SNGSData() { load(path); }
SNGSData::~SNGSData() {}

void SNGSData::setNum(sint n) {
    ref_num = n;
    ref_length.resize(ref_num, 0);
    depth_size.resize(ref_num, 0);
    uncovered.resize(ref_num, 0);
    read_count.resize(ref_num, 0);
    read_length.resize(ref_num, 0.0);
	variants.resize(5 * ref_num); //del, dup, inv, trs, trinv
    depth.resize(ref_num);
    delidx.resize(ref_num); 
    insidx.resize(ref_num);
	invidx.resize(2 * ref_num);
    trsidx.resize(ref_num);
    trinvidx.resize(ref_num);
    sforin(i, 0, ref_num) {
        trsidx[i].resize(ref_num);
		trinvidx[i].resize(2 * ref_num);
    }
    //_mtxs = _array<std::mutex>(5*ref_num);
}
void SNGSData::setLength(int i, sint l) {
    ref_length[i] = l;
	depth_size[i] = (ref_length[i] - 1) / depth_bin + 1;
    depth[i].resize(depth_size[i]);
	int len = ((ref_length[i] - 1) >> 14) + 1;
    delidx[i].resize(len);
    insidx[i].resize(len);
	invidx[2 * i].resize(len);
	invidx[2 * i + 1].resize(len);
    sforin(j, 0, ref_num) {
        if (i == j) continue;
        trsidx[i][j].resize(len);
		trinvidx[i][2 * j].resize(len);
		trinvidx[i][2 * j + 1].resize(len);
    }
}

inline void loadVariant(Array<svar_data> *variants, sio::SFile &file) {
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
        sio::SFile file;
        file.open(path);
        char magic[4];
        file.readBytes(magic, 4);
        if(memcmp(BSM_MAGIC, magic, 3) != 0)
            throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR , magic, u8"BSM");
        sint tmp;
        file.readInt(tmp);
        if (tmp != ref_num) setNum(tmp);
        file.readUInteger(total_reads);
        file.readReal(average_length);
        file.readReal(average_depth);
        file.readReal(covered_region);
        file.readInt(depth_bin);
        sforin(i, 0, ref_num) {
            file.readInt(tmp);
            if(tmp != i) throw SBioInfoException(ERR_INFO, SLIB_CONFLICT_ERROR, "reference index", "index value");
            file.readInt(tmp);
            if (tmp != ref_length[i]) setLength(i, tmp);
            file.readInt(tmp);
            read_count[i] = tmp;
			sforin(k, 0, 5) loadVariant(&variants[5 * i + k], file);
        }
		sforin(l, 0, ref_num)
			file.readBytes(depth[l].ptr(), depth_size[l] * sizeof(float));
    } catch (SIOException ie) {
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
        file.writeInt(E_.alt.size());
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
        file.writeInt(ref_num);
        file.writeUInteger(total_reads);
        file.writeReal(average_length);
        file.writeReal(average_depth);
        file.writeReal(covered_region);
        file.writeInt(depth_bin);
        file.flush();
        sforin(i, 0, ref_num) {
            file.writeInt(i);
            file.writeInt(ref_length[i]);
            file.writeInt(read_count[i]);
			sforin(k, 0, 5) saveVariant(&variants[5 * i + k], file);
        }
        sforin(l, 0, ref_num) {
			file.writeBytes(depth[l].ptr(), depth_size[l] * sizeof(float));
            file.flush();
        }
    } catch (SIOException ie) {
		ie.print();
    }
}

void SNGSData::setParam(sngs_param *p) {
    depth_bin = p->depth_bin;
    _threads.setSize(p->thread_count);
}
void SNGSData::lock(int r, int v) { _mtxs[5 * r + v].lock(); }
void SNGSData::unlock(int r, int v) { _mtxs[5 * r + v].unlock(); }

double SNGSData::depthAt(const sbpos &pos) {
    int beg = pos.begin/depth_bin, end = pos.end/depth_bin;
    float *dp = &depth[pos.idx][beg];
    if (beg == end) return *dp;
    double cp = 0.0;
    cp += (*dp)*(depth_bin-pos.begin%depth_bin); ++dp;
    sforin(i, beg+1, end) { cp += (*dp)*depth_bin; ++dp; }
    cp += (*dp)*(pos.end-end*depth_bin+1);
    return cp/(pos.length(true));
}
/*
double SNGSData::totalDepthIn(sint ref, sint pos, sint length) {
    if(!length) return 0.f;
    float *dp = &depth[ref][0];
    int beg = pos/bin, end = (pos+length-1)/bin;
    if (beg == end) return dp[beg];
    double cp = 0.0;
    cp += dp[beg]*(bin-pos%bin);
    sforin(i, beg+1, end-1) cp += dp[i]*bin;
    cp += dp[end]*(pos+length-(end-1)*bin);
    return cp;
}
float SNGSData::depthIn(sint ref, sint pos, sint length) {
    return totalDepthIn(ref, pos, length)/length;
}
*/
inline void varIdx1(Array<svar_data> *vec, intarray *idx) {
    idx->reset(-1);
    if (vec->empty()) return;
    auto off = -1;
    sforeach(*vec) {
        if (off < E_.pos[0].begin) {
            idx->at(E_.pos[0].begin>>14) = it-vec->begin();
            while (off < E_.pos[0].begin) off += BAM_INDEX_BIN;
        }
    }
}
inline void varIdx2(Array<svar_data> *vec, intarray *idx1, intarray *idx2) {
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

void SNGSData::varindex(svariant_param *vp) {
    sforin(i, 0, ref_num) {
        _threads.addTask(varIdx1, &variants[5*i], &delidx[i]);
        _threads.addTask(varIdx1, &variants[5*i+1], &insidx[i]);
        _threads.addTask(varIdx2, &variants[5*i+2], &invidx[2*i], &invidx[2*i+1]);
        _threads.addTask(varIdx3, &variants[5*i+3], &trsidx[i]);
        _threads.addTask(varIdx4, &variants[5*i+4], &trinvidx[i]);
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

inline void subtractVar(Array<svar_data> *v1, Array<svar_data> *v2, size_t dist) {
    Array<std::pair<int8_t, svar_data *>> vec;
    size_t size = v1->size();
    sforeach(*v1) vec.add(0, &E_);
    sforeach(*v2) vec.add(1, &E_);
    std::sort(vec.begin(), vec.end(),
              [](const std::pair<int8_t, svar_data *> &p1, const std::pair<int8_t, svar_data *> &p2) {
                  return *(p1.second) < *(p2.second);
              });
    sforeach(vec) {
        if (!it->second) continue;
        auto it_ = it+1;
        svar_data *vp = nullptr;
        while (it_ < vec.end()) {
            if (E_.second->equal(*it_->second, dist)) {
                if (!E_.first) vp = E_.second;
                else if (!it_->first) vp = it_->second;
                it_->second = nullptr;
            }
            else if (!(E_.second->lt(*it_->second, dist))) break;
            ++it_;
        }
        if (vp) { vp->type = 0; --size; }
    }
    _shrink(v1, size);
}

void SNGSData::subtract(SNGSData &sum, svariant_param *vp) {
    if (ref_num != sum.ref_num)
        throw SBioInfoException(ERR_INFO, SLIB_CONFLICT_ERROR, "reference count", "another summary");
    sforin(i, 0, ref_num) {
        sforin(j, 0, 5)
            _threads.addTask(subtractVar, &variants[5*i+j], &sum.variants[5*i+j], vp->max_dist);
    }
    _threads.complete();
}

inline void _var_integrate(Array<svar_data> *vec, svariant_param *vp) {
    if (vec->empty()) return;
    auto size = vec->size();
    vec->sort();
    sforeach(*vec) {
        if (!(E_.type)) continue;
        auto it_ = it+1;
        while (it_ < vec->end()) {
            if (!(E_.lt(*it_, vp->max_dist))) break;
            if (E_.equal(*it_, vp->max_dist)) {
               E_ += (*it_); it_->type = 0; --size;
            }
            ++it_;
        }
    }
    _shrink(vec, size);
}

void _uncovered(float *dp, sint size, sint length, sint bin, sint *count) {
    sforin(i, 0, size-1) {
        if ((*dp) < 1.0) (*count) += bin-(int)((*dp)*bin);
        ++dp;
    }
    sint rest = length-(size-1)*bin;
    if ((*dp) < 1.0) (*count) += rest-(int)((*dp)*rest);
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

inline void _merge_depth(float *dp1, float *dp2, sint size, sint length, sint bin, sint *count) {
    sforin(i, 0, size-1) {
        (*dp1) += (*dp2);
        if ((*dp1) < 1.0) (*count) += bin-(int)((*dp1)*bin);
        ++dp1; ++dp2;
    }
    (*dp1) += (*dp2);
    sint rest = length-(size-1)*bin;
    if ((*dp1) < 1.0) (*count) += rest-(int)((*dp1)*rest);
}

void SNGSData::integrate(SNGSData &sum, svariant_param *vp) {
    if (ref_num != sum.ref_num || !(ref_length == sum.ref_length) || depth_bin != sum.depth_bin)
        throw SBioInfoException(ERR_INFO, SLIB_CONFLICT_ERROR, "ref_num", CONFLICT_TEXT("this", "sum"));
    
    total_length = 0;
    average_length *= total_reads;
    average_length += sum.average_length*sum.total_reads;
    total_reads += sum.total_reads;
    
    sforin(i, 0, ref_num) {
        if (target_seq && sum.target_seq) {
            target[i].merge(sum.target[i]);
            total_length += target[i].length()+1;
        }
        else total_length += ref_length[i];
        read_count[i] += sum.read_count[i];
		_merge_depth(depth[i].ptr(), sum.depth[i].ptr(), depth_size[i], ref_length[i], depth_bin, &uncovered[i]);
        //_threads.addTask(_merge_depth, depth[i].ptr(), sum.depth[i].ptr(), depth_size[i], ref_length[i], depth_bin, &uncovered[i]);
        sforin(j, 0, 5) {
            variants[5*i+j].append(sum.variants[5*i+j]);
			_var_integrate(&variants[5 * i + j], vp);
            //_threads.addTask(_var_integrate, &variants[5*i+j], vp);
        }
    }
    average_depth = average_length/(double)total_length;
    average_length /= (double)total_reads;
    //_threads.complete();
    covered_region = 1.0-(double)smath::sstat::sum(uncovered.ptr(), uncovered.size())/total_length;
}

void SNGSData::init() {
    total_reads = 0;
    total_length = 0;
    average_length = 0.0;
    average_depth = 0.0;
    covered_region = 0.0;
    memset(&read_count[0], 0, ref_num*sizeof(int64_t));
    memset(&read_length[0], 0, ref_num*sizeof(double));
    sforin(i, 0, ref_num) {
        sforin(j, 0, 5) variants[5*i+j].clear();
        memset(depth[i].ptr(), 0, depth_size[i]*sizeof(float));
        delidx[i].reset(0);
        insidx[i].reset(0);
        invidx[2*i].reset(0);
        invidx[2*i+1].reset(0);
        sforin(k, 0, ref_num) {
            trsidx[i][k].reset(0);
            trinvidx[i][2*k].reset(0);
            trinvidx[i][2*k+1].reset(0);
        }
    }
}