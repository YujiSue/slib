#include "sutil/scode.h"
#include "sio/sfile.h"

using namespace slib;
using namespace slib::sio;

suint SHash::crc32(ubytearray& data) {
	suint val = 0;
	val = crc32_z(val, static_cast<const Bytef*>(data.ptr()), data.size());
	return val;
}
bool SHash::crc32check(ubytearray& data, suint &ref) {
	return SHash::crc32(data) == ref;
}
void SHash::md5(String& data, ubytearray& digest) {
	digest.resize(16);
	MD5_CTX context;
	MD5Init(&context);
	MD5Update(&context, (unsigned char *)data.cstr(), (unsigned int)data.size());
	MD5Final(&digest[0], &context);
}
void SHash::md5(ubytearray& data, ubytearray& digest) {
	digest.resize(16);
	MD5_CTX context;
	MD5Init(&context);
	MD5Update(&context, data.ptr(), (unsigned int)data.size());
	MD5Final(&digest[0], &context);
}
void SHash::md5Str(String& data, String& str) {
	MD5_CTX context;
	unsigned char digest[16];
	MD5Init(&context);
	MD5Update(&context, (unsigned char*)data.cstr(), (unsigned int)data.size());
	MD5Final(&digest[0], &context);
	sforin(i, 0, 16) { str += SNumber::toHex(digest[i]); }
}
void SHash::md5Str(ubytearray& data, String& str) {
	MD5_CTX context;
	unsigned char digest[16];
	MD5Init(&context);
	MD5Update(&context, data.ptr(), (unsigned int)data.size());
	MD5Final(&digest[0], &context);
	sforin(i, 0, 16) { str += SNumber::toHex(digest[i]); }
}
bool SHash::md5check(ubytearray& data, const char* str) {
	String s;
	SHash::md5Str(data, s);
	return !strcmp(s.cstr(), str);
}
void SHash::sha256(String& data, ubytearray& digest) {
	digest.resize(256);
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, data.cstr(), data.size());
	SHA256_Final(digest.ptr(), &ctx);
}
void SHash::sha256(ubytearray& data, ubytearray& digest) {
	digest.resize(256);
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, data.ptr(), data.size());
	SHA256_Final(digest.ptr(), &ctx);
}
void SHash::sha256Str(String& data, String& str) {
	ubytearray digest;
	SHash::sha256(data, digest);
	str.clear();
	sforin(i, 0, 32) { str += SNumber::toHex(digest[i]); }
}
void SHash::sha256Str(ubytearray& data, String& str) {
	ubytearray digest;
	SHash::sha256(data, digest);
	str.clear();
	sforin(i, 0, 32) { str += SNumber::toHex(digest[i]); }
}
bool SHash::sha256check(ubytearray& data, const char* str) {
	String s;
	SHash::sha256Str(data, s);
	return !strcmp(s.cstr(), str);
}
void SHash::sha512(String& data, ubytearray& digest) {
	digest.resize(512);
	SHA512_CTX ctx;
	SHA512_Init(&ctx);
	SHA512_Update(&ctx, data.cstr(), data.size());
	SHA512_Final(digest.ptr(), &ctx);
}
void SHash::sha512(ubytearray& data, ubytearray& digest) {
	digest.resize(512);
	SHA512_CTX ctx;
	SHA512_Init(&ctx);
	SHA512_Update(&ctx, data.ptr(), data.size());
	SHA512_Final(digest.ptr(), &ctx);
}
void SHash::sha512Str(String& data, String& str) {
	ubytearray digest;
	SHash::sha512(data, digest);
	str.clear();
	sforin(i, 0, 64) { str += SNumber::toHex(digest[i]); }
}
void SHash::sha512Str(ubytearray& data, String& str) {
	ubytearray digest;
	SHash::sha512(data, digest);
	str.clear();
	sforin(i, 0, 64) { str += SNumber::toHex(digest[i]); }
}
bool SHash::sha512check(ubytearray& data, const char* str) {
	String s;
	SHash::sha512Str(data, s);
	return !strcmp(s.cstr(), str);
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
suint SZip::centralDir::size() { return 46 + flen + elen + clen; }
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
		dat.reserve((size_t)cd.rsize + 1);
		dat.resize((size_t)cd.csize + 10);
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
		ie.print();
	}
	catch (SException ex) {
		ex.print();
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
        lfh.rsize = (suint)dat.size();
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
        ie.print();
    } catch (SException ex) {
        ex.print();
    }
}
String SCode::urlEncode(const String& str) {
	String encoded;
	encoded.reserve(str.size() * 3);
	sforeach(str) {
		switch (E_) {
		case '%':
			encoded << "%25"; break;
		case ' ':
			encoded << "%20"; break;
		case '!':
			encoded << "%21"; break;
		case '\"':
			encoded << "%22"; break;
		case '#':
			encoded << "%23"; break;
		case '$':
			encoded << "%24"; break;
		case '&':
			encoded << "%26"; break;
		case '\'':
			encoded << "%27"; break;
		case '(':
			encoded << "%28"; break;
		case ')':
			encoded << "%29"; break;
		case '*':
			encoded << "%2A"; break;
		case '+':
			encoded << "%2B"; break;
		case ',':
			encoded << "%2C"; break;
		case '/':
			encoded << "%2F"; break;
		case ':':
			encoded << "%3A"; break;
		case ';':
			encoded << "%3B"; break;
		case '<':
			encoded << "%3C"; break;
		case '=':
			encoded << "%3D"; break;
		case '>':
			encoded << "%3E"; break;
		case '?':
			encoded << "%3F"; break;
		case '@':
			encoded << "%40"; break;
		case '[':
			encoded << "%5B"; break;
		case ']':
			encoded << "%5D"; break;
		case '^':
			encoded << "%5E"; break;
		case '`':
			encoded << "%60"; break;
		case '{':
			encoded << "%7B"; break;
		case '|':
			encoded << "%7C"; break;
		case '}':
			encoded << "%7D"; break;
		case '~':
			encoded << "%7E"; break;
		default:
			encoded << E_;
			break;
		}
	}
	return encoded;
}
String SCode::urlDecode(const String& str) {
	String decoded, code(3, '\0');
	decoded.reserve(str.size());
	sforeach(str) {
		if (E_ == '%' && it < str.end() - 2) {
			memcpy(code.ptr(), it.ptr(), 3);
			if (code == "%20") decoded << " ";
			else if (code == "%21") decoded << "!";
			else if (code == "%22") decoded << "\"";
			else if (code == "%23") decoded << "#";
			else if (code == "%24") decoded << "$";
			else if (code == "%25") decoded << "%";
			else if (code == "%26") decoded << "&";
			else if (code == "%27") decoded << "'";
			else if (code == "%28") decoded << "(";
			else if (code == "%29") decoded << ")";
			else if (code == "%2A") decoded << "*";
			else if (code == "%2B") decoded << "+";
			else if (code == "%2C") decoded << ",";
			else if (code == "%2F") decoded << "/";
			else if (code == "%3A") decoded << ":";
			else if (code == "%3B") decoded << ";";
			else if (code == "%3C") decoded << "<";
			else if (code == "%3D") decoded << "=";
			else if (code == "%3E") decoded << ">";
			else if (code == "%3F") decoded << "?";
			else if (code == "%40") decoded << "@";
			else if (code == "%5B") decoded << "[";
			else if (code == "%5D") decoded << "]";
			else if (code == "%5E") decoded << "^";
			else if (code == "%60") decoded << "`";
			else if (code == "%7B") decoded << "{";
			else if (code == "%7C") decoded << "|";
			else if (code == "%7D") decoded << "}";
			else if (code == "%7E") decoded << "~";
			else { decoded << code; it += 2; }
		}
		else decoded << E_;
	}
	return decoded;
}
const char *SCode::B64_STR = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
subyte SCode::b64i(const char &c) {
    subyte b = *((subyte *)&c);
    if (0x60 < b) return 26+b-0x61;
    else if (0x40 < b) return b-0x41;
    else if (0x2f < b) return 52+b-0x30;
    else if (b == 0x2b) return 62;
    else if (b == 0x2f) return 63;
    return 0;
}
void SCode::encodeB64Char(const char* ori, char *encoded, size_t size) {
    char dat[3];
    memset(dat, 0, 3);
    memcpy(dat, ori, size);
    encoded[0] = SCode::B64_STR[(dat[0]>>2)&0x3F];
    encoded[1] = SCode::B64_STR[((dat[0]&0x03)<<4)+((dat[1]>>4)&0x0F)];
    encoded[2] = size<2?'=':SCode::B64_STR[((dat[1]&0x0F)<<2)+((dat[2]>>6)&0x03)];
    encoded[3] = size<3?'=':SCode::B64_STR[dat[2]&0x3F];
}
void SCode::decodeB64Char(const char *data, char *decoded) {
    memset(decoded, 0, 3);
    subyte dec[4];
	sforin(i, 0, 4) dec[i] = (data[i] == '=' ? 0 : b64i(data[i]));
	decoded[0] = (dec[0] << 2) + ((dec[1] >> 4) & 0x03);
	decoded[1] = ((dec[1] & 0x0F) << 4) + ((dec[2] >> 2) & 0x0F);
	decoded[2] = ((dec[2] & 0x03) << 6) + (dec[3] & 0x3F);
}
void SCode::encodeBASE64(const String& ori, String& base) {
	base.resize((((ori.size() * 4 - 1) / 3) / 4 + 1) * 4);
	suinteger length = ori.size() / 3;
	auto op = ori.cstr();
	auto bp = &base[0];
	sforin(i, 0, length) {
		SCode::encodeB64Char(op, bp, 3);
		op += 3; bp += 4;
	}
	if (ori.size() % 3) encodeB64Char(op, bp, ori.size() % 3);
}
size_t SCode::encodeBASE64(const ubytearray& ori, String& base, bool size) {
	if (ori.empty()) base = "";
	else {
		sint length = (ori.size() - 1) / 3;
		auto optr = ori.ptr(); char* ptr = nullptr;
		if (size) {
			base.resize((length + 1) * 4 + 12);
			char encode[9];
			memset(encode, 0, 9);
			memcpy(encode, &size, 8);
			sforin(i, 0, 3) SCode::encodeB64Char(&encode[i * 3], &base[i * 4], 3);
			ptr = base.ptr(12);
		}
		else {
			base.resize((length + 1) * 4);
			ptr = base.ptr();
		}
		sforin(i ,0, length) SCode::encodeB64Char((const char*)optr, ptr, 3); optr += 3; ptr += 4;
		SCode::encodeB64Char((const char*)optr, ptr, ori.size() - 3 * length);
	}
	return ori.size();
}
void SCode::decodeBASE64(const String& base, String& ori) {
	sint length = base.size() / 4;
	ori.resize(length * 3);
	auto bp = base.cstr();
	auto op = ori.ptr();
	sforin(i, 0, length) {
		SCode::decodeB64Char(bp, op); bp += 4; op += 3;
	}
	ori.resize(strlen(ori.cstr()));
}
void SCode::decodeBASE64(const String& base, ubytearray& ori, size_t s) {
	auto ptr = base.ptr();
	if (s == -1) {
		char decode[9];
		sforin(i, 0, 3) { SCode::decodeB64Char(ptr, &decode[3 * i]); ptr += 4; }
		memcpy(&s, decode, 8);
	}
	ori.resize(((s - 1) / 3 + 1) * 3);
	auto optr = ori.ptr();
	sint length = (ori.size() - 1) / 3 + 1;
	sforin(i, 0, length) { SCode::decodeB64Char(ptr, (char*)optr); ptr += 4; optr += 3; }
	ori.resize(s);
}
void SCode::expandTo(ubytearray& ori, ubytearray& dest, size_t cap, sint bits, sint flush) {
	if (ori.empty()) return;
	sint capacity = (cap == -1 ? (sint)((double)ori.size() * 1.5) : cap);
	dest.resize(capacity);
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.next_in = (Bytef*)ori.ptr();
	strm.avail_in = (unsigned int)ori.size();
	strm.next_out = (Bytef*)dest.ptr();
	strm.avail_out = (suint)capacity;
	int res = inflateInit2(&strm, bits);
	if (res) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "inflateInit2", EXEC_TEXT(std::to_string(res)));
	res = inflate(&strm, flush);
	if (res != Z_STREAM_END || strm.total_in < ori.size()) {
		if (res == Z_BUF_ERROR || strm.total_in < ori.size()) {
			while (res == Z_BUF_ERROR || strm.total_in < ori.size()) {
				int capacity_ = (int)((double)capacity * 1.5);
				dest.resize(capacity_);
				strm.next_out = dest.ptr(strm.total_out);
				strm.avail_out = capacity_ - capacity;
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
	dest.resize(strm.total_out);
}
void SCode::expand(ubytearray &bytes, size_t cap, sint bits, sint flush) {
    if (bytes.empty()) return;
	int capacity = cap == -1 ? (bytes.size() * 1.5) : cap;
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
    int res = inflateInit2(&strm, bits);
    if (res) throw SException(ERR_INFO, SLIB_EXEC_ERROR, "inflateInit2", EXEC_TEXT(std::to_string(res)));
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
	auto capacity = (suint)((double)bytes.size() * 1.5);
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
				auto capacity_ = (suint)((double)capacity * 1.5);
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
