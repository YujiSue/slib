#include "sio/sfile.h"
#include "sutil/scode.h"
#include "smedia/simage.h"

using namespace slib;
using namespace slib::sio;

inline void _decode(String &str) {
	if (str.isQuoted()) str.transform(slib::sstyle::DELETE_QUOTE);
#if defined(WIN32_OS) || defined(WIN64_OS)
	if (str.empty()) str = "C:" + PATH_SEPARATOR;
	if (str.match(R(/^[A-Z][:]/))) return;
#else
	if (str.empty()) str = PATH_SEPARATOR;
	if (str.beginWith(PATH_SEPARATOR)) return;
#endif
	if (str == "~" || str.beginWith("~" + PATH_SEPARATOR)) str.replace(0, 1, HOME_PATH);
	else if (str.beginWith(".." + PATH_SEPARATOR)) {
		auto list = str.split(PATH_SEPARATOR);
		sio::SFile cur = sio::SFile::current();
		sforeach(list) {
			if (E_ == "..") cur.updir();
			else cur.child(E_);
		}
		str = cur.path();
	}
	else if (str.beginWith("." + PATH_SEPARATOR)) str.replace(0, 1, sio::CURRENT_PATH);
}
inline void _encode(String &str) {
	str.replace("\"", "\\\"");
}

void SFile::_setPath(const char *path) {
    _size = 0;
    _path = path;
    _decode(_path);
    if (_mode&sio::DIRECTORY) return;
	if (!fileExist(path)) return;
#if defined(WIN64_OS)
	struct _stat64 buf;
	int res = _stat64(_path.localize(), &buf);
#elif defined(WIN32_OS)
	struct _stat32 buf;
	int res = _stat32(_path.localize(), &buf);
#else
	struct stat buf;
	int res = stat(_path, &buf);
#endif
    if (!res) {
        if (S_ISDIR(buf.st_mode)) _mode |= sio::DIRECTORY;
        else _size = buf.st_size;
    }
}
void SFile::_open() {
    if (isDir()) return;
	if (_mode & sio::READ || _mode & sio::APPEND) {
		if (exist()) {
			std::ios_base::openmode om = std::ios::binary | std::ios::in;
			if (_mode & sio::WRITE || _mode & sio::APPEND) om |= std::ios::out;
#ifdef WIN_OS 
			_stream.open(_path.localize(), om);
#else
			_stream.open(_path, om);
#endif
			if (_stream.fail() || !_stream.is_open())
				throw SIOException(ERR_INFO, FILE_OPEN_ERROR, _path);
			auto current = _stream.tellg();
			_stream.seekg(0, std::ios::beg);
			auto beg = _stream.tellg();
			_stream.seekg(0, std::ios::end);
			auto end = _stream.tellg();
			_size = end - beg;
			_stream.clear();
			if (!(_mode & sio::APPEND)) _stream.seekg(current, std::ios::beg);
		}
		else throw SIOException(ERR_INFO, FILE_NOT_EXIST_ERROR, _path);
	}
	else  if (_mode & sio::WRITE) {
		std::ios_base::openmode om = std::ios::binary | std::ios::out;
		if (exist()) {
#ifdef WIN_OS 
			_stream.open(_path.localize(), om);
#else
			_stream.open(_path, om);
#endif
			if (_stream.fail() || !_stream.is_open())
				throw SIOException(ERR_INFO, FILE_OPEN_ERROR, _path);
		}
		else {
			om |= std::ios_base::trunc;
#ifdef WIN_OS 
			_stream.open(_path.localize(), om);
#else
			_stream.open(_path, om);
#endif
			if (_stream.fail() || !_stream.is_open())
				throw SIOException(ERR_INFO, FILE_OPEN_ERROR, _path);
		}
	}	
}
void SFile::_make() {
	if (isDir()) {
#if defined(MAC_OS)||defined(UNIX_OS)||defined(LINUX_OS)
		mkdir(_path, S_IRWXU | S_IRWXG | S_IRWXO);
#elif defined(WIN_OS)
		mkdir(_path.localize());
#endif
	}
	else {
		std::ios_base::openmode om = std::ios::binary | std::ios::out | std::ios::trunc;
		if (_mode & sio::READ) om |= std::ios::in;
#ifdef WIN_OS
		_stream.open(_path.localize(), om);
#else
		_stream.open(_path, om);
#endif
	}
}

SFile::SFile() : SObject(), _stream(), _size(0), _mode(0) {}
SFile::SFile(const char *path, int m) : SFile() {
    _mode = m; _setPath(path);
    if (!_mode || _mode == sio::DIRECTORY) return;
    else if (_mode & sio::CREATE) _make();
    else _open();
}
SFile::SFile(const SFile &file) : SFile(file._path, file._mode) {}
SFile::~SFile() { close(); }
SFile &SFile::operator=(const char *path) {
    close(); _size = 0; _mode = 0; _setPath(path); return *this;
}
SFile &SFile::operator=(const SFile &file) {
    close(); _path = file._path; _size = file._size; _mode = file._mode; return *this;
}
SFile &SFile::operator+=(const char *s) { _setPath(_path + s); return *this; }
SFile SFile::operator+(const char *s) const { SFile file(*this); return file += s; }
SFile SFile::current() { return SDirectory(CURRENT_PATH); }
SFile SFile::home() { return SDirectory(HOME_PATH); }
SFile SFile::createFile(const char *path) { return SFile(path, sio::CREATE); }
SFile SFile::makeDir(const char *path) { return SFile(path, sio::CREATE|sio::DIRECTORY); }
String SFile::path() const { return _path; }
suinteger SFile::size() const { return _size; }
int SFile::mode() const { return _mode; }
suinteger SFile::offset() { return _stream.tellg(); }
void SFile::setOffset(suinteger off, bool rev) {
	_stream.clear();
    if (rev) _stream.seekg(_size-off, std::ios::beg);
    else _stream.seekg(off, std::ios::beg);
}
void SFile::moveOffset(sinteger off) { _stream.seekg(off, std::ios::cur); }
void SFile::seek(sinteger off, SEEK_ORI ori) {
	std::ios_base::seekdir dir;
	switch(ori) {
		case BEGIN:
		dir = std::ios_base::beg;
		break;
		case CURRENT:
		dir = std::ios_base::cur;
		break;
		case END:
		dir = std::ios_base::end;
		break;
		default:
		dir = std::ios_base::beg;
		break;
	}
	_stream.seekg(off, dir);
}
void SFile::flush() { _stream.flush(); }
void SFile::clear() { _stream.clear(); }
stringarray SFile::components(const char *sep) const {
    size_t off = 0, len = _path.length(), s = strlen(sep);
    if (_path.beginWith(sep)) { off+=s; len-=s; }
    if (_path.endWith(sep)) len-=s;
    return _path.substring(off, len).split(sep);
}
String SFile::filename(bool ext, const char *sep) const {
    auto list = components(sep);
    if (ext) return list.last();
    else {
        auto pos_ = list.last().rfind(".");
        if (pos_ == NOT_FOUND) return list.last();
        else return list.last().substring(0, pos_);
    }
}
String SFile::extension(bool low) const {
    if (isDir() || _path.endWith(".")) return "";
    auto pos = _path.rfind(".");
    if (pos == NOT_FOUND) return "";
    return low?String::lower(_path.substring(pos+1)):_path.substring(pos+1);
}
bool SFile::isOpened() const { return _stream.is_open(); }
bool SFile::exist() const {
    int res;
#if defined(WIN64_OS)
    struct _stat64 buf;
    res = _stat64(_path.localize(), &buf);
    return !res;
#elif defined(WIN32_OS)
    struct _stat32 buf;
    res = _stat32(_path.localize(), &buf);
    return !res;
#else
    struct stat buf;
    res = stat(_path, &buf);
#endif
    return !res;
}
bool SFile::isDir() const { return _mode&sio::DIRECTORY; }
bool SFile::isRoot() const { return _path == ROOT_PATH; }
#if defined(WIN32_OS) || defined(WIN64_OS)
bool SFile::isDriveRoot(const char *s) const { return _path == ROOT_DRIVE("c"); }
#endif
bool SFile::empty() const { return _path.empty(); }
bool SFile::eof() { return _stream.eof(); }
void SFile::open(const char *path, int m) { _mode |= m; if(path) _setPath(path); _open(); }
void SFile::make(const char *path, int m) { _mode |= m; if(path) _setPath(path); _make(); }
void SFile::edit(const char *path, int m) { _mode |= m; if(path) _setPath(path); _open(); }
void SFile::append(const char* path, int m) {

}
void SFile::close() { if (isOpened()) _stream.close(); }
void SFile::updir() {
    close();
    if (isRoot()) return;
    if (_path.endWith(PATH_SEPARATOR)) _path.resize(_path.size()-PATH_SEPARATOR.length());
    _setPath(_path.substring(0, _path.rfind(PATH_SEPARATOR)));
    _mode |= sio::DIRECTORY;
}
void SFile::setdir(const char *name) {
	if (!isDir()) updir();
	String str(name);
	if (str.beginWith(".." + PATH_SEPARATOR)) {
		while (str.beginWith(".." + PATH_SEPARATOR)) {
			updir();
			str.clip(0, PATH_SEPARATOR.length() + 2);
		}
		_path += PATH_SEPARATOR + str;
	}
	else if (str.beginWith("." + PATH_SEPARATOR)) _path += str.substring(1);
	else _path += PATH_SEPARATOR + str;
	_mode |= sio::DIRECTORY;
}
void SFile::select(const char *name) {
	if (!isDir()) updir();
	String str(name);
	if (str.beginWith(".." + PATH_SEPARATOR)) {
		while (str.beginWith(".." + PATH_SEPARATOR)) {
			updir();
			str.clip(0, PATH_SEPARATOR.length() + 2);
		}
		_path += PATH_SEPARATOR + str;
	}
	else if (str.beginWith("." + PATH_SEPARATOR)) _path += str.substring(1);
	else _path += PATH_SEPARATOR + str;
	_mode = 0;
}
inline int filecopy(SFile &src, SFile dest) {
    try {
        if (src.isOpened()) src.setOffset(0);
        else src.open(nullptr, sio::READ);
        dest.open(nullptr, sio::CREATE|sio::WRITE);
        subyte buf[256];
        size_t count = src.size()/256, rest = src.size()%256;
        sforin(i, 0, count) {
            src.readBytes(buf, 256);
            dest.writeBytes(buf, 256);
        }
        src.readBytes(buf, rest);
        dest.writeBytes(buf, rest);
        src.close();
        dest.close();
    } catch (SIOException ie) {
        ie.print(); return 1;
    }
    return 0;
}
void SFile::copyTo(const char *path, int op) {
    String novel(path);
    _decode(novel);
    if (op&sio::CANCEL_IFEXIST && sio::fileExist(novel)) return;
    if (op&sio::BACKUP_ORI && sio::fileExist(novel)) {
        int postfix = 2;
        String alt = novel+postfix;
        while (sio::fileExist(alt)) {
            ++postfix;
            alt = novel+postfix;
        }
        novel = alt;
    }
    int res = 0;
    if (isDir()) {
        auto dir = SFile(novel, sio::CREATE|sio::DIRECTORY);
        auto list = fileList();
        sforeach(list) {
            if (E_.filename() == "." || E_.filename() == "..") continue;
            res += filecopy(E_, dir.child(E_.filename()));
        }
    }
    else res = filecopy(*this, SFile(novel));
    if(res) throw SIOException(ERR_INFO, SLIB_EXEC_ERROR, "filecopy", std::to_string(res).c_str());
}
inline int filerename(const char *src, const char *to) { return rename(src, to); }
void SFile::moveTo(const char *path, int op) {
    close();
    SFile novel = path;
    if (novel.isDir()) novel.select(filename());
    if (op&sio::CANCEL_IFEXIST && novel.exist()) return;
    if (op&sio::BACKUP_ORI && novel.exist()) {
        int postfix = 2;
        String alt = novel._path+postfix;
        while (sio::fileExist(alt)) {
            ++postfix;
            alt = novel._path+postfix;
        }
        novel._path = alt;
    }
#ifdef  WIN_OS
	auto res = filerename(_path.localize(), novel._path.localize());
#else 
	auto res = filerename(_path, novel._path);
#endif 	
    if(res) throw SIOException(ERR_INFO, SLIB_EXEC_ERROR, "filerename", std::to_string(res).c_str());
    *this = novel;
}
void SFile::rename(const char *name, int op) {
    close();
    SFile novel = parent().child(name);
    if (op&sio::CANCEL_IFEXIST && novel.exist()) return;
    if (op&sio::BACKUP_ORI && novel.exist()) {
        int postfix = 2;
        String alt = novel._path+postfix;
        while (sio::fileExist(alt)) {
            ++postfix;
            alt = novel._path+postfix;
        }
        novel._path = alt;
    }
#ifdef  WIN_OS
	auto res = filerename(_path.localize(), novel._path.localize());
#else 
	auto res = filerename(_path, novel._path);
#endif 	
	if (res) throw SIOException(ERR_INFO, SLIB_EXEC_ERROR, "filerename", std::to_string(res).c_str());
	*this = novel;
}
inline int fileremove(const char *s) { return remove(s); }
void SFile::remove() {
    close();
    String option;
	sint res;
#ifdef  WIN_OS
	if (isDir()) res = rmdir(_path.localize().cstr());
	else res = fileremove(_path.localize());
#else 
	if (isDir()) res = rmdir(_path.cstr());
	res = fileremove(_path);
#endif 	
	if (res) throw SIOException(ERR_INFO, SLIB_EXEC_ERROR, "fileremove", std::to_string(res).c_str());
	_path = ""; _size = 0; _mode = 0;
}
SFile SFile::parent(const char *sep) const {
    if (isRoot()) return *this;
    slib::String p = _path;
    if (_path.endWith(PATH_SEPARATOR)) p.resize(_path.size()-1);
    auto pos = p.rfind(sep);
    if (pos == NOT_FOUND || pos == 0) return SFile(PATH_SEPARATOR, sio::DIRECTORY);
    else p.clip(0, pos);
    return SFile(p, sio::DIRECTORY);
}
SFile SFile::child(const char *name, int m) const {
    if (isDir()) return SFile(_path+PATH_SEPARATOR+name, m);
    else return SFile(parent()._path+PATH_SEPARATOR+name, m);
}
inline bool endWithExt(const stringarray &ext, String &path) {
    sforeach(ext) { if(path.endWith(E_)) return true; }
    return false;
}
filearray SFile::fileList(const stringarray &ext, bool recur, bool hidden) {
    filearray files;
	if (!exist()) throw SIOException(ERR_INFO, FILE_NOT_EXIST_ERROR, _path);
    if(isDir()) {
#if defined(MAC_OS) || defined(UNIX_OS) || defined(LINUX_OS)
        struct dirent *dir;
        DIR *dp = opendir(_path);
        if (!dp)  throw SIOException(ERR_INFO, DIRECTORY_LOAD_ERROR, _path);
        while ((dir = readdir(dp))) {
            String str = String::lower(dir->d_name);
            if (str == "." || str == "..") continue;
            if (!hidden && str.beginWith(".")) continue;
            if (!ext.empty()) {
                sforeach(ext) {
					auto tmp = child(dir->d_name);
                    if (str.endWith("."+E_)) {
						files.add(tmp);
                        break;
                    }
					else if (recur && tmp.isDir()) files.append(tmp.fileList(ext, recur, hidden));
                }
            }
			else {
				auto tmp = child(dir->d_name);
				if (recur && tmp.isDir()) files.append(tmp.fileList(ext, recur, hidden));
				else files.add(tmp);
			}
        }
        closedir(dp);
#else
		WIN32_FIND_DATA fd;
		HANDLE  h;
#ifdef UNICODE
		h = FindFirstFile((_path+PATH_SEPARATOR+u8"*.*").unicode().c_str(), &fd);
#else
		h = FindFirstFile((_path + PATH_SEPARATOR + u8"*.*").localize().cstr(), &fd);
#endif
		if (h == INVALID_HANDLE_VALUE) throw SIOException(ERR_INFO, DIRECTORY_LOAD_ERROR, _path);
		do {
			String str = String::toUTF8(fd.cFileName);
			if (str == "." || str == "..") continue;
			if (!hidden && str.beginWith(".")) continue;
			if (!ext.empty()) {
				sforeach(ext) {
					auto tmp = child(str);
					if (str.endWith(u8"." + E_)) {
						files.add(tmp);
						break;
					}
					else if (recur && tmp.isDir()) files.append(tmp.fileList(ext, recur, hidden));
				}
			}
			else {
				auto tmp = child(str);
				if (recur && tmp.isDir()) files.append(tmp.fileList(ext, recur, hidden));
				else files.add(tmp);
			}
		} while (FindNextFile(h, &fd));
		FindClose(h);
#endif
    }
    return files;
}
filearray SFile::find(const char *que, bool recur) {
    filearray files;
	if (!exist()) throw SIOException(ERR_INFO, FILE_NOT_EXIST_ERROR, _path);
    if(isDir()) {
#ifndef WIN_OS
        struct dirent *dir;
        DIR *dp = opendir(_path);
        if (!dp) throw SIOException(ERR_INFO, FILE_NOT_EXIST_ERROR, _path);
        while ((dir = readdir(dp))) {
            if (dir->d_type&DT_DIR) {
                if (recur) {
                    SFile sub = child(dir->d_name);
                    files.append(sub.find(que, recur));
                }
                else continue;
            }
            String str(dir->d_name);
            if (str.match(Regex(String("/")+que+"/i"))) {
                files.add(child(dir->d_name));
            }
            else continue;
        }
        closedir(dp);
#else
		WIN32_FIND_DATA fd;
		HANDLE  h;
		String temp;
#ifdef UNICODE
		wchar_t wtemp[256];
		auto upath = _path.unicode();
		memcpy(wtemp, upath.c_str(), upath.length());
		wtemp[upath.length()] = '\0';
		h = FindFirstFile(wtemp, &fd);
#else
		char ctemp[256];
		auto upath = _path.localize();
		memcpy(ctemp, upath.cstr(), upath.length());
		ctemp[upath.length()] = '\0';
		h = FindFirstFile(ctemp, &fd);
#endif
		if (h == INVALID_HANDLE_VALUE) throw SIOException(ERR_INFO, DIRECTORY_LOAD_ERROR, _path);
		do {
#ifdef UNICODE
			temp = String::toUTF8(wtemp);
#else
			temp = String::toUTF8(ctemp);
#endif
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
				if (recur) {
					SFile sub = child(temp);
					files.append(sub.find(que, recur));
				}
				else continue;
			}
			String str(temp);
			if (str.match(Regex(String("/") + que + "/i"))) {
				files.add(child(temp));
			}
			else continue;
		} while (FindNextFile(h, &fd));
		FindClose(h);
#endif
    }
    return files;
}
void SFile::archive(const char *path) { SZip::archive(*this, path); }
void SFile::expand(const char *path) { SZip::expand(*this, path); }
String SFile::getClass() const { return "file"; }
String SFile::toString() const { return "file://"+_path; }
SObject *SFile::clone() const { return new SFile(*this); }
void SFile::readChars(char *str, size_t size) {
	_stream.read(str, size);
    str[size] = '\0';
}
void SFile::readString(String &str, size_t size) {
    str.resize(size);
    _stream.read(&str[0], size);
}
void SFile::readLine(String &str) {
    str = "";
	suinteger buffer_size = SIO_BUFFER_SIZE, pos, current;
    String buf(buffer_size, 0);
    while (!eof()) {
        current = _stream.tellg();
        if (_size < current + buffer_size) {
            buffer_size = _size - current;
            buf.resize(buffer_size);
        }
        if (!buffer_size) {
            _stream.setstate(_stream.eofbit);
            break;
        }
        _stream.read(&buf[0], buffer_size);
        bool crlf = true;
        pos = buf.find("\r\n");
        if (pos == NOT_FOUND) { pos = buf.find("\n"); crlf = false; }
        if (pos == NOT_FOUND) { pos = buf.find("\r"); crlf = false; }
        if (pos == NOT_FOUND) str += buf;
        else {
            str += buf.substring(0, pos);
            if (eof()) _stream.clear();
			_stream.seekg(current + pos + (crlf ? 2 : 1));
            break;
        }
    }
}
void SFile::readTo(String &str, const char *end) {
    str = "";
	suinteger buffer_size, pos, current;
    String buf;
    while(!eof()) {
        current = _stream.tellg();
        if(_size < current+SIO_BUFFER_SIZE) buffer_size = _size-current;
        else buffer_size = SIO_BUFFER_SIZE;
        if(!buffer_size) {
			_stream.setstate(_stream.eofbit);
            break;
        }
        buf.resize(buffer_size);
		_stream.read(&buf[0], buffer_size);
        pos = buf.find(end);
        if(pos == NOT_FOUND) str += buf;
        else {
            str += buf.substring(0, pos);
            if (eof()) _stream.clear();
			_stream.seekg(current+pos+strlen(end));
            break;
        }
    }
}
void SFile::readBytes(void *dat, size_t size) { _stream.read((char *)dat, size); }
void SFile::readChar(char &c) { _stream.read(&c, 1); }
void SFile::readUChar(unsigned char &c) { _stream.read((char *)&c, 1); };
void SFile::readByte(sbyte &val) { _stream.read((char *)&val, 1); }
void SFile::readUByte(subyte &val) { _stream.read((char *)&val, 1); }
void SFile::readShort(sshort &val) { _stream.read((char *)&val, 2); }
void SFile::readUShort(sushort &val) { _stream.read((char *)&val, 2); }
void SFile::readInt(sint &val) { _stream.read((char *)&val, 4); }
void SFile::readUInt(suint &val) { _stream.read((char *)&val, 4); }
void SFile::readInteger(sinteger &val) { _stream.read((char *)&val, 8); }
void SFile::readUInteger(suinteger &val) { _stream.read((char *)&val, 8); }
void SFile::readFloat(float &val) { _stream.read((char *)&val, 4); }
void SFile::readDouble(double &val) { _stream.read((char *)&val, 8); }
void SFile::readReal(sreal &val) { _stream.read((char *)&val, 8); }
void SFile::readData(SData &dat, size_t size) {
    dat.resize(size);
    _stream.read((char *)dat.ptr(), size);
}
inline void objRead(sio::SFile* file, sobj& obj);
inline void numRead(sio::SFile* file, sobj& num) {
	subyte tmp;
	file->readUByte(tmp);
	switch (tmp)
	{
	case SNumber::INTEGER:
	{
		sinteger i;
		file->readInteger(i);
		num = i;
		break;
	}
	case SNumber::UINTEGER:
	{
		suinteger ui;
		file->readUInteger(ui);
		num = ui;
		break;
	}
	case SNumber::BOOL:
	{
		sinteger i;
		file->readInteger(i);
		num = i != 0;
		break;
	}
	case SNumber::REAL:
	{
		double d;
		file->readDouble(d);
		num = d;
		break;
	}
	case SNumber::FRAC:
	{
		sint f[2];
		file->readInt(f[0]);
		file->readInt(f[1]);
		num = sfrac(f[0], f[1]);
		break;
	}
	case SNumber::COMPLEX:
	{
		float c[2];
		file->readFloat(c[0]);
		file->readFloat(c[1]);
		num = scomp(c[0], c[1]);
		break;
	}
	default:
		break;
	}
}
inline void strRead(sio::SFile* file, sobj& str) {
	suint len;
	file->readUInt(len);
	str = String(len, '\0');
	file->readChars(str.string().ptr(), len);
}
inline void dateRead(sio::SFile* file, sobj& date) {
	suint len;
	String form, dt;
	file->readUInt(len);
	file->readString(form, len);
	file->readUInt(len);
	file->readString(dt, len);
	date = SDate(dt, form);
}
inline void datRead(sio::SFile* file, sobj& dat) {
	suint len;
	file->readUInt(len);
	dat = SData(len);
	file->readBytes(dat.data().ptr(), len);
}
inline void arrayRead(sio::SFile* file, sobj& array) {
	suint tmp;
	file->readUInt(tmp);
	array = SArray((size_t)tmp);
	if (tmp == 0) return;
	sforeach(array) objRead(file, E_);
}
inline void dictRead(sio::SFile* file, sobj& dict) {
	suint tmp, len;
	file->readUInt(tmp);
	dict = SDictionary();
	if (tmp == 0) return;
	sforin(i, (suint)0, tmp) {
		file->readUInt(len);
		String key;
		sobj val;
		file->readString(key, len);
		objRead(file, val);
		dict.set(key.cstr(), val);
	}
}
inline void fileRead(sio::SFile* file, sobj& f) {
	suint len;
	String path;
	file->readUInt(len);
	file->readString(path, len);
	f = sio::SFile(path);
}
inline void tblRead(sio::SFile* file, sobj& tbl) {
	/*
	 */
}
inline void imgRead(sio::SFile* file, sobj& img) {
	subyte byte;
	file->readUByte(byte);
	if (byte == 0) {
		sushort type;
		suint width, height;
		file->readUShort(type);
		file->readUInt(width);
		file->readUInt(height);
		img = smedia::SImage(type, width, height);
		file->readBytes(img.image().ptr(), img.size());
	}
	else {
		suint len;
		String path;
		file->readUInt(len);
		file->readString(path, len);
		img = smedia::SImage(path, true);
	}
}
inline void objRead(sio::SFile* file, sobj& obj) {
	sint tmp;
	file->readInt(tmp);
	switch (tmp)
	{
	case (sint)NULL_OBJ:
		obj = snull;
		break;
	case (sint)NUMBER_OBJ:
		numRead(file, obj);
		break;
	case (sint)STRING_OBJ:
		strRead(file, obj);
		break;
	case (sint)DATE_OBJ:
		dateRead(file, obj);
		break;
	case (sint)DATA_OBJ:
		datRead(file, obj);
		break;
	case (sint)ARRAY_OBJ:
		arrayRead(file, obj);
		break;
	case (sint)DICT_OBJ:
		dictRead(file, obj);
		break;
	case (sint)FILE_OBJ:
		fileRead(file, obj);
		break;
	case (sint)TABLE_OBJ:
		tblRead(file, obj);
		break;
	case (sint)IMAGE_OBJ:
		imgRead(file, obj);
		break;
	default:
		break;
	}
}
void SFile::readSObject(sobj& obj) { objRead(this, obj); }
void SFile::operator>>(String &s) { readString(s, _size); }
void SFile::operator>>(SString &s) { readString(s, _size); }
void SFile::operator>>(SData &d) { readData(d, _size); }
void SFile::writeChars(const char *str) { _stream.write(str, strlen(str)); }
void SFile::writeString(const String &str) { _stream.write(str.cstr(), str.length()); }
void SFile::writeString(const SString &str) { _stream.write(str.cstr(), str.length()); }
void SFile::writeBytes(const void *dat, size_t size) { _stream.write((char *)dat, size); }
void SFile::writeChar(const char &c) { _stream.write(&c, 1); }
void SFile::writeUChar(const unsigned char &c) { _stream.write((char *)&c, 1); }
void SFile::writeByte(const sbyte &val) { _stream.write((char *)&val, 1); }
void SFile::writeUByte(const subyte &val) { _stream.write((char *)&val, 1); }
void SFile::writeShort(const sshort &val) { _stream.write((char *)&val, 2); }
void SFile::writeUShort(const sushort &val) { _stream.write((char *)&val, 2); }
void SFile::writeInt(const sint &val) { _stream.write((char *)&val, 4); }
void SFile::writeUInt(const suint &val) { _stream.write((char *)&val, 4); }
void SFile::writeInteger(const sinteger &val) { _stream.write((char *)&val, 8); }
void SFile::writeUInteger(const suinteger &val) { _stream.write((char *)&val, 8); }
void SFile::writeFloat(const float &val) { _stream.write((char *)&val, 4); }
void SFile::writeDouble(const double &val) { _stream.write((char *)&val, 8); }
void SFile::writeReal(const sreal &val) { _stream.write((char *)&val, 8); }
inline void objWrite(sio::SFile* file, const sobj& obj);
inline void numWrite(sio::SFile* file, const SNumber& num) {
	file->writeInt((sint)NUMBER_OBJ);
	file->writeUByte(num.type());
	file->writeBytes(num.ptr(), 8);
}
inline void strWrite(sio::SFile* file, const SString& str) {
	file->writeInt((sint)STRING_OBJ);
	file->writeUInt((suint)str.length());
	file->writeBytes(str.cstr(), str.size());
}
inline void dateWrite(sio::SFile* file, const SDate& date) {
	file->writeInt((sint)DATE_OBJ);
	file->writeUInt((suint)strlen(slib::sstyle::ISO8601));
	file->writeChars(slib::sstyle::ISO8601);
	file->writeUInt((suint)date.toString(slib::sstyle::ISO8601).length());
	file->writeString(date.toString());
}
inline void datWrite(sio::SFile* file, const SData& dat) {
	file->writeInt((sint)DATA_OBJ);
	file->writeUInt((suint)dat.size());
	file->writeBytes(dat.ptr(), dat.size());
}
inline void arrayWrite(sio::SFile* file, const SArray& array) {
	file->writeInt((sint)ARRAY_OBJ);
	file->writeUInt((suint)array.size());
	if (array.empty()) return;
	else { sforeach(array) objWrite(file, E_); }
}
inline void dictWrite(sio::SFile* file, const SDictionary& dict) {
	file->writeInt((sint)DICT_OBJ);
	file->writeUInt((suint)dict.size());
	if (dict.empty()) return;
	else { 
		sforeach(dict) {
			file->writeUInt((suint)E_.key.length());
			file->writeString(E_.key);
			objWrite(file, E_.value);
		}
	}
}
inline void fileWrite(sio::SFile* file, const sio::SFile& f) {
	file->writeInt((sint)FILE_OBJ);
	file->writeUInt((suint)f.path().length());
	file->writeBytes(f.path().cstr(), f.path().size());
}
inline void tblWrite(sio::SFile* file, const STable& tbl) {
	/*
	 */
}
inline void imgWrite(sio::SFile* file, const smedia::SImage& img) {
	file->writeInt((sint)IMAGE_OBJ);
	file->writeUByte(img.isLink() ? 1 : 0);
	if (img.isLink()) {
		file->writeUInt((suint)img.file().path().length());
		file->writeString(img.file().path());
	}
	else {
		file->writeUShort(img.type());
		file->writeUInt((suint)img.width());
		file->writeUInt((suint)img.height());
		file->writeBytes(img.ptr(), img.size());
	}
}
inline void objWrite(sio::SFile* file, const sobj& obj) {
	if (obj.isNull()) file->writeInt((sint)NULL_OBJ);
	else if (obj.isNum()) numWrite(file, obj.number());
	else if (obj.isStr()) strWrite(file, obj.string());
	else if (obj.isDate()) dateWrite(file, obj.date());
	else if (obj.isDat()) datWrite(file, obj.data());
	else if (obj.isArray()) arrayWrite(file, obj.array());
	else if (obj.isDict()) dictWrite(file, obj.dict());
	else if (obj.isFile()) fileWrite(file, obj.file());
	else if (obj.isTable()) tblWrite(file, obj.table());
	else if (obj.isImg()) imgWrite(file, obj.image());
}
void SFile::writeSObject(const sobj& obj) { objWrite(this, obj); }
SFile &SFile::operator<<(const int &val) { writeString(String(val)); return *this; }
SFile &SFile::operator<<(const unsigned int &val) { writeString(String(val)); return *this; }
SFile &SFile::operator<<(const size_t &val) { writeString(String(val)); return *this; }
SFile &SFile::operator<<(const sinteger &val) { writeString(String(val)); return *this; }
SFile &SFile::operator<<(const float &val) { writeString(String(val)); return *this; }
SFile &SFile::operator<<(const double &val) { writeString(String(val)); return *this; }
SFile &SFile::operator<<(const char *s) { writeChars(s); return *this; }
SFile &SFile::operator<<(const std::string &s) { writeChars(s.c_str()); return *this; }
SFile &SFile::operator<<(const String &s) { writeString(s); return *this; }
SFile &SFile::operator<<(const SString &s) { writeString(s); return *this; }
SFile &SFile::operator<<(const SDate &d) { writeString(d.toString()); return *this; }
SFile &SFile::operator<<(const sobj &obj) { writeString(obj.toString()); return *this; }
SFile::operator const char *() const { return _path.cstr(); }
bool SFile::operator<(const SFile &file) const { return _path < file._path; }
bool SFile::operator==(const SFile &file) const { return _path == file._path; }


slib::sio::siostream::siostream(int i) :_mode(i) {}
slib::sio::siostream::siostream(const char* s) : _mode(1) { _file = sio::SFile(s, sio::READ | sio::CREATE); }
slib::sio::siostream::~siostream() {}
void slib::sio::siostream::setStdMode() { _mode = 0; }
void slib::sio::siostream::setPath(const char* s) { _mode = 1; _file = sio::SFile(s, sio::READ | sio::CREATE); }
void slib::sio::siostream::read(slib::String& s) {
	if (_mode) _file >> s;
	else std::cin >> s;
}
void slib::sio::siostream::_output1() {
	if (_mode) { _file; _file.flush(); }
	else std::cout << std::flush;
}
void slib::sio::siostream::_output2() {
	if (_mode) { _file << NEW_LINE; _file.flush(); }
	else std::cout << std::endl;
}
slib::sio::siostream slib::SIO_ = slib::sio::siostream(0);