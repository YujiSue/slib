#include "smath/numeric.h"
#include "sutil/scode.h"
#include "sio/sfile.h"
slib::String slib::sutil::decodeURL(const char* s) {
	slib::String decoded, code(3, '\0'), str(s);
	sfor(str) {
		if ($_ == '%' && it < str.end() - 2) {
			memcpy(&code[0], it.ptr(), 3);
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
		else decoded << $_;
	}
	return decoded;
}
slib::String slib::sutil::encodeURL(const char* s) {
	slib::String encoded, str(s);
	sfor(str) {
		switch ($_) {
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
			encoded << $_;
			break;
		}
	}
	return encoded;
}


slib::String slib::SCode::urlEncode(const slib::String& str) {
	slib::String encoded;
	encoded.reserve(str.size() * 3);
	sfor(str) {
		switch ($_) {
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
			encoded << $_;
			break;
		}
	}
	return encoded;
}
slib::String slib::SCode::urlDecode(const slib::String& str) {
	slib::String decoded, code(3, '\0');
	decoded.reserve(str.size());
	sfor(str) {
		if ($_ == '%' && it < str.end() - 2) {
			memcpy(&code[0], it.ptr(), 3);
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
		else decoded << $_;
	}
	return decoded;
}

void slib::sutil::inflate(ubytearray& dest, const ubytearray& ori, size_t cap, int bits) {
	dest.clear();
	if (ori.empty()) return;
	cap = (cap == (size_t)-1 ? (uInt)(ori.size() * 3 / 2) : cap);
	dest.resize(cap);
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.next_in = (Bytef*)ori.data();
	strm.avail_in = (unsigned int)ori.size();
	strm.next_out = (Bytef*)dest.data();
	strm.avail_out = (unsigned)cap;
	auto res = inflateInit2(&strm, bits);
	//if (res) throw Exception();
	res = inflate(&strm, Z_NO_FLUSH);
	if (res != Z_STREAM_END || strm.total_in < ori.size()) {
		if (res == Z_BUF_ERROR || strm.total_in < ori.size()) {
			while (res == Z_BUF_ERROR || strm.total_in < ori.size()) {
				auto cap_ = cap * 3 / 2;
				dest.resize(cap_);
				strm.next_out = (Bytef*)dest.data(strm.total_out);
				strm.avail_out = (uInt)(cap_ - cap);
				res = inflate(&strm, Z_NO_FLUSH);
				cap = cap_;
			}
		}
		if (res == Z_STREAM_ERROR || res == Z_DATA_ERROR || res == Z_MEM_ERROR)
			throw Exception();
	}
	res = inflateEnd(&strm);
	if (res == Z_DATA_ERROR)
		throw Exception();
	dest.resize(strm.total_out);
}
void slib::sutil::deflate(ubytearray& dest, const ubytearray& ori, size_t cap, int bits) {
	dest.clear();
	if (ori.empty()) return;
	cap = (cap == (size_t)-1 ? ori.size() * 3/ 2 : cap);
	dest.resize(cap);
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.next_in = (Bytef*)ori.data();
	strm.avail_in = (unsigned)ori.size();
	strm.next_out = (Bytef*)dest.data();
	strm.avail_out = (uInt)cap;
	int res = deflateInit2(&strm, Z_DEFAULT_COMPRESSION,
		Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
	if (res) throw Exception();
	res = Z_OK;
	res = deflate(&strm, Z_FINISH);
	if (res != Z_STREAM_END || strm.total_in < ori.size()) {
		if (res == Z_BUF_ERROR || strm.total_in < ori.size()) {
			while (res == Z_BUF_ERROR || strm.total_in < ori.size()) {
				auto cap_ = cap * 3 / 2;
				dest.resize(cap_);
				strm.next_out = (Bytef*)dest.data(strm.total_out);
				strm.avail_out = (uInt)(cap_ - cap);
				res = deflate(&strm, Z_FINISH);
				cap = cap_;
			}
		}
		if (res == Z_STREAM_ERROR)
			throw Exception();
	}
	res = deflateEnd(&strm);
	if (res == Z_DATA_ERROR)
		throw Exception();
	dest.resize(strm.total_out);
}

inline slib::subyte b64(const char& c) {
	auto b = (slib::subyte)c;
	if (b == 0x2f || b == 0x5f) return 63;
	else if (b == 0x2b || b == 0x2d) return 62;
	else if (0x60 < b) return 26 + b - 0x61;
	else if (0x40 < b) return b - 0x41;
	else if (0x2f < b) return 52 + b - 0x30;
	return 0;
}
void slib::sutil::decodeBase64Char(const char* src, subyte* dest) {
	subyte dec[4];
	sforin(i, 0, 4) dec[i] = (src[i] == '=' ? 0 : b64(src[i]));
	dest[0] = (dec[0] << 2) | ((dec[1] >> 4) & 0x03);
	dest[1] = ((dec[1] & 0x0F) << 4) | ((dec[2] >> 2) & 0x0F);
	dest[2] = ((dec[2] & 0x03) << 6) | (dec[3] & 0x3F);
}
void slib::sutil::decodeBase64(const char* s, String& dest) {
	auto length = strlen(s) / 4;
	dest.resize(length * 3);
	auto it = &dest[0];
	sforin(i, 0_u, length) {
		decodeBase64Char(s, (subyte *)$); s += 4; $ += 3;
	}
	dest.resize(strlen(dest.cstr()));
}
void slib::sutil::decodeBase64(const char* s, ubytearray& dest) {
	auto length = strlen(s) / 4;
	dest.resize(length * 3);
	auto it = dest.data();
	sforin(i, 0_u, length) {
		decodeBase64Char(s, $); s += 4; $ += 3;
	}
}
void slib::sutil::decodeBase64(const SObjPtr& src, SObjPtr& dest) {
	if (dest.isData()) decodeBase64(src.string().cstr(), dest.data());
	else {
		dest = SString();
		decodeBase64(src.string().cstr(), dest.string());
	}
}
/*
void slib::sutil::encodeBase64Char(const subyte* src, char* dest, const size_t sz) {
	char dat[3];
	memset(dat, 0, 3);
	memcpy(dat, ori, size);
	encoded[0] = BASE64_CHAR[(dat[0] >> 2) & 0x3F];
	encoded[1] = BASE64_CHAR[((dat[0] & 0x03) << 4) + ((dat[1] >> 4) & 0x0F)];
	encoded[2] = size < 2 ? '=' : SCode::B64_STR[((dat[1] & 0x0F) << 2) + ((dat[2] >> 6) & 0x03)];
	encoded[3] = size < 3 ? '=' : SCode::B64_STR[dat[2] & 0x3F];
}
*/
void slib::sutil::encodeBase64(const char* src, String& dest) {

}
void slib::sutil::encodeBase64(const ubytearray& src, String& dest) {}
void slib::sutil::encodeBase64(const SObjPtr& src, SObjPtr& dest) {}

/*

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

void readZippCentEnd(sio::SFile &ori, SZip::centralDirEnd &cde) {
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
void readZippCent(sio::SFile &ori, SZip::centralDir &cd) {
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
void readLocalHeader(sio::SFile &ori, SZip::centralDir &cd, SZip::localFileHeader &lfh, SData &dat) {
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
void writeLocalHeader(sio::SFile &file, SZip::localFileHeader &lfh, SData &dat) {
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

*/