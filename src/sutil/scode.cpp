#include "sutil/scode.h"
#include "sio/sfile.h"

using namespace slib;
using namespace slib::sio;

suint SChecker::crc32(subyte* byte, size_t size) {
	suint val = 0;
	val = crc32_z(val, static_cast<const Bytef*>(byte), size);
	return val;
}
bool SChecker::crc32check(subyte* byte, size_t size, suint &ref) {
	return SChecker::crc32(byte, size) == ref;
}

const suint SZip::LOCAL_FILE_HEADER_SIG = 0x04034B50;
const suint SZip::DATA_DESCRIPTOR_SIG = 0x08074B50;
const suint SZip::CENTRAL_DIR_SIG = 0x02014B50;
const suint SZip::CENTRAL_DIR_END_SIG = 0x06054B50;

SZip::localFileHeader::localFileHeader() {
    ver = 0; flag = 0; method = 0; time = 0; date = 0; flen = 0; elen = 0;
    sig = LOCAL_FILE_HEADER_SIG; crc = 0; csize = 0; rsize = 0;
}
SZip::localFileHeader::~localFileHeader() {}
SZip::dataDescriptor::dataDescriptor() {
    sig = DATA_DESCRIPTOR_SIG; crc = 0; csize = 0; rsize = 0;
}
SZip::dataDescriptor::~dataDescriptor() {}
SZip::centralDir::centralDir() {
    ver = 0; mver = 0; flag = 0; method = 0; time = 0; date = 0;
    flen = 0; elen = 0; clen = 0; disk = 0; iattr = 0;
    sig = 0; crc = 0; csize = 0; rsize = 0; eattr = 0; offset = 0;
}
SZip::centralDir::~centralDir() {}
suint SZip::centralDir::size() { return 46+flen+elen+clen; }
SZip::centralDirEnd::centralDirEnd() {
    num = 0; disk = 0; record = 0; sum = 0; clen = 0;
    sig = CENTRAL_DIR_END_SIG; size = 0; offset = 0;
}
SZip::centralDirEnd::~centralDirEnd() {}

inline void readZippCentEnd(sio::SFile &ori, SZip::centralDirEnd &cde) {
	ori.readBytes(&cde.sig, 4);
	if (cde.sig != SZip::CENTRAL_DIR_END_SIG) throw SException(ERR_INFO);
	ori.readBytes(&cde.num, 2);
	ori.readBytes(&cde.disk, 2);
	ori.readBytes(&cde.record, 2);
	ori.readBytes(&cde.sum, 2);
	ori.readBytes(&cde.size, 4);
	ori.readBytes(&cde.offset, 4);
	ori.readBytes(&cde.clen, 2);
	if (cde.clen) ori.readString(cde.comment, cde.clen);
	ori.clear();
	ori.setOffset(cde.offset);
}
inline void readZippCent(sio::SFile &ori, SZip::centralDir &cd) {
	ori.readBytes(&cd.sig, 4);
	if (cd.sig != SZip::CENTRAL_DIR_SIG) throw SException(ERR_INFO);
	ori.readBytes(&cd.ver, 2);
	ori.readBytes(&cd.mver, 2);
	ori.readBytes(&cd.flag, 2);
	ori.readBytes(&cd.method, 2);
	ori.readBytes(&cd.time, 2);
	ori.readBytes(&cd.date, 2);
	ori.readBytes(&cd.crc, 4);
	ori.readBytes(&cd.csize, 4);
	ori.readBytes(&cd.rsize, 4);
	ori.readBytes(&cd.flen, 2);
	ori.readBytes(&cd.elen, 2);
	ori.readBytes(&cd.clen, 2);
	ori.readBytes(&cd.disk, 2);
	ori.readBytes(&cd.iattr, 2);
	ori.readBytes(&cd.eattr, 4);
	ori.readBytes(&cd.offset, 4);
	if (cd.flen) ori.readString(cd.name, cd.flen);
	if (cd.elen) { cd.ext.resize(cd.elen); ori.readBytes(cd.ext.ptr(), cd.elen); }
	if (cd.clen) ori.readString(cd.comment, cd.clen);
}
inline void readLocalHeader(sio::SFile &ori, SZip::centralDir &cd, SZip::localFileHeader &lfh, SData &dat) {
	ori.setOffset(cd.offset);
	ori.readBytes(&lfh.sig, 4);
	if (lfh.sig != SZip::LOCAL_FILE_HEADER_SIG) throw SException(ERR_INFO);
	ori.readBytes(&lfh.ver, 2);
	ori.readBytes(&lfh.flag, 2);
	ori.readBytes(&lfh.method, 2);
	ori.readBytes(&lfh.time, 2);
	ori.readBytes(&lfh.date, 2);
	ori.readBytes(&lfh.crc, 4);
	ori.readBytes(&lfh.csize, 4);
	ori.readBytes(&lfh.rsize, 4);
	ori.readBytes(&lfh.flen, 2);
	ori.readBytes(&lfh.elen, 2);
	if (lfh.flen) ori.readString(lfh.name, lfh.flen);
	if (lfh.elen) { lfh.ext.resize(lfh.elen); ori.readBytes(lfh.ext.ptr(), lfh.elen); }
	if (cd.csize) {
		dat.clear();
		dat.reserve(cd.rsize + 1);
		dat.resize(cd.csize + 10);
		dat[0] = 0x1F; dat[1] = 0x8B; dat[2] = 0x08; dat[9] = 0x13;
		ori.readBytes(&dat[10], cd.csize);
	}
	else dat.clear();
}
inline void writeLocalHeader(sio::SFile &file, SZip::localFileHeader &lfh, SData &dat) {
    file.writeUInt(lfh.sig);
    file.writeUShort(lfh.ver);
    file.writeUShort(lfh.flag);
    file.writeUShort(lfh.method);
    file.writeUShort(lfh.time);
    file.writeUShort(lfh.date);
    file.writeUInt(lfh.crc);
    file.writeUInt(lfh.csize);
    file.writeUInt(lfh.rsize);
    
    file.writeUInt(lfh.csize);
    file.writeUInt(lfh.csize);
    
}
SZip::SZip() {

}
SZip::SZip(const char* path) : SZip() { load(path); }
SZip::~SZip() {

	if (_file.isOpened()) _file.close();
}
void SZip::load(const char* path) {
	try {
		//decrypt is not supported yet
		_file.open(path, sio::READ);
		centralDirEnd cde;
		_file.seek(-22, sio::SFile::END);
		/*
		 *
		 */
		readZippCentEnd(_file, cde);
		_contents.resize(cde.sum);
		sforeach(_contents) readZippCent(_file, E_);
	}
    catch (sio::SIOException ie) {
		ie.toString();
	}
	catch (SException ex) {
		ex.toString();
	}
}
void SZip::save(const char* path) {

}
void SZip::extract(const char* path, SData& data) {
	sforeach(_contents) {
		if (E_.name == path) {
			localFileHeader lfh;
			readLocalHeader(_file, E_, lfh, data);
		}
	}
}
stringarray SZip::fileList() {
	stringarray array;
	sforeach(_contents) array.add(E_.name);
	return array;
}
void SZip::archive(sio::SFile &ori, const char *dest, const char *encrypt) {
    //encrypt is not supported yet
    sio::SFile file;
    filearray list;
    if (!dest) file = sio::SFile::createFile(ori.parent().path()+PATH_SEPARATOR+ori.filename()+".zip");
    else file = dest;
    centralDirEnd cde;
    if (ori.isDir()) {
        
    }
    else {
        if (ori.isOpened()) ori.close();
        localFileHeader lfh;
        centralDir cd;
        SData dat;
        dat.load(ori);
        lfh.rsize = dat.size();
        cd.rsize = lfh.rsize;
        dat.compress();
        cde.record = 1; cde.sum = 1; cde.size = cd.size();
        
        
    }
    
    
    
    
    
}

void SZip::expand(SFile &ori, const char *dest, const char *decrypt) {
    try {
        //decrypt is not supported yet
        SFile file;
		if (dest) file = SDirectory(dest);
		else {
			file = ori.parent(); 
			file.setdir(ori.filename(false)); 
		}
		if (!file.exist()) file.make();
        centralDirEnd cde;
        if (!ori.isOpened()) ori.open();
        ori.seek(-22, sio::SFile::END);
        readZippCentEnd(ori, cde);
        Array<centralDir> cds(cde.sum);
        sforeach(cds) readZippCent(ori, E_);
        SData dat;
        localFileHeader lfh;
        sforeach(cds) {
            if (E_.name.beginWith("__MACOSX")) {
                /*
                 *
                 */
                continue;
            }
			if (!E_.rsize || E_.name.contain("/")) {
				auto layers = E_.name.split("/");
				size_t layer = layers.size()-(E_.name.endWith("/")?0:1);
				String path = file.path();
				sforin(l, 0, layer) {
					sio::SFile::makeDir(path + PATH_SEPARATOR + layers[l]);
					path += PATH_SEPARATOR + layers[l];
				}
				if (!E_.name.endWith("/")) {
					readLocalHeader(ori, E_, lfh, dat);
					if (!dat.empty()) {
						dat.expand();
						dat.save(path + PATH_SEPARATOR + layers.last());
					}
				}
			}
            else {
                readLocalHeader(ori, E_, lfh, dat);
                if (!dat.empty()) {
                    dat.expand();
                    dat.save(file.path()+PATH_SEPARATOR+E_.name);
                }
            }
        }
    } catch (sio::SIOException ie) {
        ie.toString();
    } catch (SException ex) {
        ex.toString();
    }
}

void SCode::urlEncode(String &str) {
    str.replace("%", "%25");
    str.replace(" ", "%20");
    str.replace("!", "%21");
    str.replace("\"", "%22");
    str.replace("#", "%23");
    str.replace("$", "%24");
    str.replace("&", "%26");
    str.replace("'", "%27");
    str.replace("(", "%28");
    str.replace(")", "%29");
    str.replace("*", "%2A");
    str.replace("+", "%2B");
    str.replace(",", "%2C");
    str.replace("/", "%2F");
    str.replace(":", "%3A");
    str.replace(";", "%3B");
    str.replace("<", "%3C");
    str.replace("=", "%3D");
    str.replace(">", "%3E");
    str.replace("?", "%3F");
    str.replace("@", "%40");
    str.replace("[", "%5B");
    str.replace("]", "%5D");
    str.replace("^", "%5E");
    str.replace("`", "%60");
    str.replace("{", "%7B");
    str.replace("|", "%7C");
    str.replace("}", "%7D");
    str.replace("~", "%7E");
}
void SCode::urlDecode(String &str) {
    str.replace("%20", " ");
    str.replace("%21", "!");
    str.replace("%22", "\"");
    str.replace("%23", "#");
    str.replace("%24", "$");
    str.replace("%26", "&");
    str.replace("%27", "'");
    str.replace("%28", "(");
    str.replace("%29", ")");
    str.replace("%2A", "*");
    str.replace("%2B", "+");
    str.replace("%2C", ",");
    str.replace("%2F", "/");
    str.replace("%3A", ":");
    str.replace("%3B", ";");
    str.replace("%3C", "<");
    str.replace("%3D", "=");
    str.replace("%3E", ">");
    str.replace("%3F", "?");
    str.replace("%40", "@");
    str.replace("%5B", "[");
    str.replace("%5D", "]");
    str.replace("%5E", "^");
    str.replace("%60", "`");
    str.replace("%7B", "{");
    str.replace("%7C", "|");
    str.replace("%7D", "}");
    str.replace("%7E", "~");
    str.replace("%25", "%");
}

const char *SCode::B64_STR = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
subyte SCode::b64i(const char &c) {
    uint8_t b = *((uint8_t *)&c);
    if (0x60 < b) return 26+b-0x61;
    else if (0x40 < b) return b-0x41;
    else if (0x2f < b) return 52+b-0x30;
    else if (b == 0x2b) return 62;
    else if (b == 0x2f) return 63;
    return 0;
}
void SCode::encodeB64Char(const void *data, char *encoded, size_t size) {
    char dat[3];
    memset(dat, 0, 3);
    memcpy(dat, data, size);
    encoded[0] = SCode::B64_STR[(dat[0]>>2)&0x3F];
    encoded[1] = SCode::B64_STR[((dat[0]&0x03)<<4)+((dat[1]>>4)&0x0F)];
    encoded[2] = size<2?'=':SCode::B64_STR[((dat[1]&0x0F)<<2)+((dat[2]>>6)&0x03)];
    encoded[3] = size<3?'=':SCode::B64_STR[dat[2]&0x3F];
}

size_t SCode::base64CharCount(size_t size) { return 12+((size-1)/3+1)*4; }

void SCode::decodeB64Char(const char *data, void *decoded) {
    memset(decoded, 0, 3);
    uint8_t dec[4];
    sforin(i, 0, 4) dec[i] = data[i] == '='?0:b64i(data[i]);
    ((char *)decoded)[0] = (dec[0]<<2)+((dec[1]>>4)&0x03);
    ((char *)decoded)[1] = ((dec[1]&0x0F)<<4)+((dec[2]>>2)&0x0F);
    ((char *)decoded)[2] = ((dec[2]&0x03)<<6)+(dec[3]&0x3F);
}

uint64_t SCode::decodeCharCount(const char *base) {
    char decode[9];
    sforin(i, 0, 3) SCode::decodeB64Char(&base[4*i], &decode[3*i]);
    uint64_t size = 0;
    memcpy(&size, decode, 8);
    return size;
}
void SCode::encodeBASE64(const void *ori, size_t size, char *base) {
    char encode[9];
    memset(encode, 0, 9);
    memcpy(encode, &size, 8);
    sforin(i, 0, 3) SCode::encodeB64Char((const void *)&encode[i*3], &base[i*4], 3);
    uint64_t length = (size-1)/3+1;
    sforin(i, 0, length-1) SCode::encodeB64Char(&((char *)ori)[i*3], &base[12+i*4], 3);
    encodeB64Char(&((char *)ori)[(length-1)*3], &base[12+(length-1)*4], size-3*(length-1));
    base[12+length*4] = '\0';
}
void SCode::decodeBASE64(const char *base, void *ori, const size_t &size) {
    uint64_t length = (size-1)/3+1;
    sforin(i, 0, length) SCode::decodeB64Char(&base[12+i*4], &((char *)ori)[i*3]);
}

void SCode::expand(ubytearray &bytes) {
    if (bytes.empty()) return;
    int capacity = bytes.size()*1.5;
    ubytearray tmp;
    tmp.resize(capacity);
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef *)bytes.ptr();
    strm.avail_in = (unsigned int)bytes.size();
    strm.next_out = (Bytef *)tmp.ptr();
    strm.avail_out = capacity;
    int res = inflateInit2(&strm, 31);
    if (res) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "inflateInit2", EXEC_TEXT(std::to_string(res)));
    int flush = Z_NO_FLUSH;
    res = inflate(&strm, flush);
    if (res != Z_STREAM_END || strm.total_in < bytes.size()) {
        if (res == Z_BUF_ERROR || strm.total_in < bytes.size()) {
            while (res == Z_BUF_ERROR || strm.total_in < bytes.size()) {
                int capacity_ = capacity*1.5;
                tmp.resize(capacity_);
                strm.next_out = tmp.ptr(strm.total_out);
                strm.avail_out = capacity_-capacity;
                res = inflate(&strm, flush);
                capacity = capacity_;
            }
        }
        if (res == Z_STREAM_ERROR || res == Z_DATA_ERROR || res == Z_MEM_ERROR)
            throw SException(ERR_INFO, SLIB_EXEC_ERROR, "inflate", EXEC_TEXT(std::to_string(res)));
    }
    res = inflateEnd(&strm);
    if (res == Z_DATA_ERROR)
        throw SException(ERR_INFO, SLIB_EXEC_ERROR, "inflateEnd", EXEC_TEXT(std::to_string(res)));
    bytes.swap(tmp); bytes.resize(strm.total_out);
}
void SCode::compress(ubytearray &bytes) {
    if (bytes.empty()) return;
    ubytearray tmp;
    int capacity = bytes.size()*1.5;
    tmp.resize(capacity);
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef *)bytes.ptr();
    strm.avail_in = (unsigned int)bytes.size();
    strm.next_out = (Bytef *)tmp.ptr();
    strm.avail_out = capacity;
    int res = deflateInit2(&strm, Z_DEFAULT_COMPRESSION,
                           Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
    if (res) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "deflateInit2", EXEC_TEXT(std::to_string(res)));
    int flush = Z_FINISH;
    res = Z_OK;
    res = deflate(&strm, flush);
    if (res != Z_STREAM_END || strm.total_in < bytes.size()) {
        if (res == Z_BUF_ERROR || strm.total_in < bytes.size()) {
            while (res == Z_BUF_ERROR || strm.total_in < bytes.size()) {
                auto capacity_ = capacity*1.5;
                tmp.resize(capacity_);
                strm.next_out = &((Bytef *)tmp.ptr())[strm.total_out];
                strm.avail_out = capacity_-capacity;
                res = deflate(&strm, flush);
                capacity = capacity_;
            }
        }
        if (res == Z_STREAM_ERROR)
            throw SException(ERR_INFO, SLIB_EXEC_ERROR, "deflate", EXEC_TEXT(std::to_string(res)));
    }
    res = deflateEnd(&strm);
    if (res == Z_DATA_ERROR)
        throw SException(ERR_INFO, SLIB_EXEC_ERROR, "deflateEnd", EXEC_TEXT(std::to_string(res)));
    bytes.swap(tmp);
    bytes.resize(strm.total_out);
}
