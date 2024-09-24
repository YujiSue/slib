#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sdate.h"
#include "sobj/sdata.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"

#include "sio/sfile.h"


std::ostream& operator<<(std::ostream& os, const slib::SFile& file) { return os << "file://" << file.path(); }
void slib::SFile::operator>>(slib::String& str) {
	if (!_stream.is_open()) throw;
	str.resize(_size);
	readChars(&str[0], _size);
	_stream.clear();
	_stream.seekg(0);
}


//#include "sutil/scode.h"
//#include "smedia/simage.h"
/*
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
		SFile cur = SFile::current();
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
*/
inline void slib::SFile::_fopen() {
	if (!_mode) return;
	std::ios_base::openmode mode = (std::ios_base::openmode)0;
	if (_mode & slib::sio::DIRECTORY) {
		if (_mode & slib::sio::MAKE) slib::sfs::makeDir(_path);
		return;
	}
	if (_mode & slib::sio::WRITE) {
		mode |= std::ios_base::out | std::ios_base::binary;
		if (_mode == slib::sio::MAKE) mode |= std::ios_base::trunc;
		else if (_mode == slib::sio::APPEND) {
			if(slib::sfs::exist(_path)) mode |= std::ios_base::app;
			else mode |= std::ios_base::trunc;
		}
		else if (_mode == slib::sio::EDIT) {
			if (slib::sfs::exist(_path)) mode |= std::ios_base::ate;
			else mode |= std::ios_base::trunc;
		}
	}
	else if (_mode & slib::sio::READ) {
		mode = std::ios_base::in | std::ios_base::binary;
		if (!slib::sfs::exist(_path)) throw FileNotFoundException(_path);
	}
	else mode = (std::ios_base::openmode)0;
#ifdef WIN_OS
	_stream.open(_path.localize(), mode);
#else
	_stream.open(_path, mode);
#endif
	if (isOpened() && (_mode & slib::sio::READ)) {
		_stream.seekg(0, std::ios_base::end);
		_size = _stream.tellg();
		_stream.clear();
		_stream.seekg(0, std::ios_base::beg);
	}
}

slib::SFile::SFile() : SObject(), _stream(), _size(0), _mode(0) {}
slib::SFile::SFile(const char *path, int m) : SFile() {
	setPath(path); _mode = m; _fopen();
}
slib::SFile::SFile(const slib::SFile &file) : SFile(file._path, file._mode) {}
slib::SFile::~SFile() { close(); }
slib::SFile & slib::SFile::operator=(const SFile &file) {
    close(); 
	_path = file._path; 
	_mode = file._mode; 
	_fopen();
	return *this;
}
slib::SFile slib::SFile::root() { return SDirectory(slib::ssys::root()); }
slib::SFile slib::SFile::home() { return SDirectory(slib::ssys::home()); }
slib::SFile slib::SFile::current() { return SDirectory(slib::ssys::current()); }
slib::SFile slib::SFile::parent() const {
	auto p = _path.rfind(sio::PATH_SEPARATOR);
	if (p == 0) return root();
	else if (p == NOT_FOUND) return SDirectory(_path);
	else return SDirectory(_path.substring(0, p));
}
slib::SFile slib::SFile::child(const char* name) const {
	if (isDir()) return SFile(sfs::joinPath(_path, name));
	else return SFile(sfs::joinPath(parent().path(), name));
}
void slib::SFile::open(const char* path, const subyte m) {
	close(); setPath(path); _mode = m; _fopen();
}

bool slib::SFile::eof() const { return _stream.eof(); }
bool slib::SFile::isOpened() const { return _stream.is_open(); }
slib::String slib::SFile::path() const { return _path; }
slib::String slib::SFile::dirName() const {
	return sfs::splitPath(_path).first;
}
slib::String slib::SFile::fileName(bool ext) const {
	return sfs::fileName(_path, ext);

	/*
	auto list = _path.split(sep);
	if (ext) return list[-1];
	else {
		auto pos = list[-1].rfind(".");
		if (pos == NOT_FOUND) return list[-1];
		else return list[-1].substring(0, pos);
	}
	*/
}
slib::String slib::SFile::extension(bool lower) const {
	if (isDir() || _path.endWith(".")) return "";
	auto ext = sfs::extension(_path);
	if (lower) return sstr::toLower(ext);
	else return ext;
	/*
	auto pos = _path.rfind(".");
	if (pos == NOT_FOUND) return "";
	return lower ? sstr::toLower(_path.substring(pos + 1)) : _path.substring(pos + 1);
	*/
}
void slib::SFile::setPath(const char* s) { 
	if (isOpened()) throw SIOException();
	_path = sfs::absolutePath(s);
}
bool slib::SFile::exist() const { return sfs::exist(_path); }
size_t slib::SFile::size() const { return _size; }
slib::subyte slib::SFile::mode() const { return _mode; }
bool slib::SFile::isDir() const { return _mode & sio::DIRECTORY; }
std::streampos slib::SFile::offset() { return _stream.tellg(); }
void slib::SFile::setOffset(std::streampos off, bool rev) {
	_stream.clear();
	if (rev) _stream.seekg(_size - off, std::ios::beg);
    else _stream.seekg(off, std::ios::beg);
}
void slib::SFile::seek(std::streampos pos, slib::SFile::SEEK from) {
	switch(from) {
		case BEGIN:
			_stream.seekg(pos, std::ios_base::beg);
			break;
		case CURRENT:
			_stream.seekg(pos, std::ios_base::cur);
			break;
		case END:
			_stream.seekg(pos, std::ios_base::end);
			break;
		default:
		break;
	}
}
void slib::SFile::flush() { _stream.flush(); }
void slib::SFile::clear() { _stream.clear(); }

void slib::SFile::close() { if (isOpened()) _stream.close(); }


/*

void SFile::open(const char *path, int m) { _mode |= m; if(path) _setPath(path); _open(); }
void SFile::make(const char *path, int m) { _mode |= m; if(path) _setPath(path); _make(); }
void SFile::edit(const char *path, int m) { _mode |= m; if(path) _setPath(path); _open(); }
void SFile::append(const char* path, int m) {

}
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

inline bool endWithExt(const stringarray &ext, String &path) {
    sforeach(ext) { if(path.endWith(E_)) return true; }
    return false;
}
*/
stringarray slib::SFile::dirList(bool recur, bool hidden) {
	stringarray dirs;
	//if (!exist()) throw SIOException(ERR_INFO, FILE_NOT_EXIST_ERROR, _path);
	if (isDir()) {
#if defined(MAC_OS) || defined(UNIX_OS) || defined(LINUX_OS)
		struct dirent* dir;
		DIR* dp = opendir(_path);
		//if (!dp)  throw SIOException(ERR_INFO, DIRECTORY_LOAD_ERROR, _path);
		while ((dir = readdir(dp))) {
			String str = sstr::toLower(dir->d_name);
			if (str == "." || str == "..") continue;
			auto tmp = sfs::joinPath(path(), dir->d_name);
			if (sfs::isDir(tmp)) {
				dirs.add(tmp);
				if (recur) dirs.append(SDirectory(tmp).dirList(recur, hidden));
			}
		}
		closedir(dp);
#else
		WIN32_FIND_DATA fd;
		HANDLE  h;
#ifdef UNICODE
		h = FindFirstFile(sfs::joinPath(_path, u8"*.*").unicode().c_str(), &fd);
#else
		h = FindFirstFile(sfs::joinPath(_path, u8"*.*").localize().cstr(), &fd);
#endif
		if (h == INVALID_HANDLE_VALUE) throw SIOException();// (ERR_INFO, DIRECTORY_LOAD_ERROR, _path);
		do {
			String str = String::toUTF8(fd.cFileName);
			if (str == "." || str == "..") continue;
			auto tmp = sfs::joinPath(path(), str);
			if (sfs::isDir(tmp)) {
				dirs.add(tmp);
				if (recur) dirs.append(SDirectory(tmp).dirList(recur, hidden));
			}
		} while (FindNextFile(h, &fd));
		FindClose(h);
#endif
	}
	return dirs;
}
stringarray slib::SFile::fileList(const stringarray &ext, bool recur, bool hidden) {
	stringarray files;
	//if (!exist()) throw SIOException(ERR_INFO, FILE_NOT_EXIST_ERROR, _path);
    if(isDir()) {
#if defined(MAC_OS) || defined(UNIX_OS) || defined(LINUX_OS)
        struct dirent *dir;
        DIR *dp = opendir(_path);
        //if (!dp)  throw SIOException(ERR_INFO, DIRECTORY_LOAD_ERROR, _path);
        while ((dir = readdir(dp))) {
            String str = sstr::toLower(dir->d_name);
            if (str == "." || str == "..") continue;
            if (!hidden && str.beginWith(".")) continue;
            if (!ext.empty()) {
                sfor(ext) {
					auto tmp = sfs::joinPath(path(), dir->d_name);
                    if (str.endWith("."+$_)) {
						files.add(tmp);
                        break;
                    }
					else if (recur && sfs::isDir(tmp)) {
						auto subdir = SDirectory(tmp);
						files.append(subdir.fileList(ext, recur, hidden));
					}
                }
            }
			else {
				auto tmp = sfs::joinPath(path(), dir->d_name);
				if (recur && sfs::isDir(tmp)) {
					auto subdir = SDirectory(tmp);
					files.append(subdir.fileList(ext, recur, hidden));
				}
				else files.add(tmp);
			}
        }
        closedir(dp);
#else
		WIN32_FIND_DATA fd;
		HANDLE  h;
#ifdef UNICODE
		h = FindFirstFile(sfs::joinPath(_path, u8"*.*").unicode().c_str(), &fd);
#else
		h = FindFirstFile(sfs::joinPath(_path, u8"*.*").localize().cstr(), &fd);
#endif
		if (h == INVALID_HANDLE_VALUE) throw SIOException();// (ERR_INFO, DIRECTORY_LOAD_ERROR, _path);
		do {
			String str = String::toUTF8(fd.cFileName);
			if (str == "." || str == "..") continue;
			if (!hidden && str.beginWith(".")) continue;
			if (!ext.empty()) {
				sfor(ext) {
					auto tmp = sfs::joinPath(path(), str);
					if (str.endWith(u8"." + $_)) {
						files.add(tmp);
						break;
					}
					else if (recur && sfs::isDir(tmp)) {
						auto subdir = SDirectory(tmp);
						files.append(subdir.fileList(ext, recur, hidden));
					}
				}
			}
			else {
				auto tmp = sfs::joinPath(path(), str);
				if (recur && sfs::isDir(tmp)) {
					auto subdir = SDirectory(tmp);
					files.append(subdir.fileList(ext, recur, hidden));
				}
				else files.add(tmp);
			}
		} while (FindNextFile(h, &fd));
		FindClose(h);
#endif
    }
    return files;
}
/*
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
*/
void slib::SFile::readLine(slib::String& str, const bool ignore_quot) {
	str.clear();
	char ch;
	bool quot[2] = { false, false };
	while (true) {
		ch = (char)_stream.get();
		if (ch == (char)0xFF) _stream.setstate(_stream.rdstate() | std::ios_base::eofbit);
		if (_stream.eof()) break;
		if ((ch == '\n' || ch == '\r') &&
			(ignore_quot || (!ignore_quot && !quot[0] && !quot[1]))) {
			if (ch == '\r' && !_stream.eof()) {
				ch = _stream.get();
				if (ch != '\n') _stream.unget();
			}
			break;
		}
		else {
			if (ch == '\'' && !quot[1]) quot[0] = !quot[0];
			else if (ch == '"' && !quot[0]) quot[1] = !quot[1];
		}
		if((int)ch) str << ch;
	}
}
void slib::SFile::readTo(const char* que) {
	if (!que) return;
	char q = que[0], ch;
	auto len = strlen(que);
	chararray target(len);
	while (!_stream.eof()) {
		ch = (char)_stream.get();
		if (ch == (char)0xFF || _stream.eof()) break;
		if (ch == q) {
			if (len == 1) break;
			_stream.unget();
			_stream.read(&target[0], len);
			if (!memcmp(target.data(), que, len)) break;
		}
	}
}
void slib::SFile::readChars(char* str, size_t size) {
	readBytes(str, size);
	str[size] = '\0';
}
void slib::SFile::readString(String& str, size_t size) {
	str.resize(size);
	readChars(&str[0], size);
}
void slib::SFile::readBytes(void* dat, size_t size) { _stream.read((char*)dat, size); }
void slib::SFile::readChar(char& c) { readBytes(&c, 1); }
void slib::SFile::readUChar(unsigned char& c) { readBytes(&c, 1); };
void slib::SFile::readByte(sbyte& val) { readBytes(&val, 1); }
void slib::SFile::readUByte(subyte& val) { readBytes(&val, 1); }
void slib::SFile::readShort(sshort& val) { readBytes(&val, 2); }
void slib::SFile::readUShort(sushort& val) { readBytes(&val, 2); }
void slib::SFile::readInt(sint& val) { readBytes(&val, 4); }
void slib::SFile::readUInt(suint& val) { readBytes(&val, 4); }
void slib::SFile::readInteger(sinteger& val) { readBytes(&val, 8); }
void slib::SFile::readUInteger(suinteger& val) { readBytes(&val, 8); }
void slib::SFile::readFloat(float& val) { readBytes(&val, 4); }
void slib::SFile::readDouble(double& val) { readBytes(&val, 8); }
void slib::SFile::readReal(sreal& val) { readBytes(&val, 8); }
inline void objRead(slib::SFile* file, sobj& obj) {
	char cls[16];
	
	file->readChars(cls, 16);
	if (!memcmp(cls, "null", 4)) obj = snull;
	else if (!memcmp(cls, "integer", 7)) {
		slib::sinteger i;
		file->readInteger(i);
		obj = i;
	}
	else if (!memcmp(cls, "bool", 4)) {

	}
	else if (!memcmp(cls, "string", 6)) {
		auto str = new slib::SString();
		file->readSString(*str);
		obj = str;
	}

	else if (!memcmp(cls, "array", 5)) {
		auto arr = new slib::SArray();
		file->readSArray(*arr);
		obj = arr;
	}
	else if (!memcmp(cls, "dict", 4)) {
		auto dict = new slib::SDictionary();
		file->readSDict(*dict);
		obj = dict;
	}
	else throw slib::Exception();
}
void slib::SFile::readSString(SString& str) {
	suint tmp;
	readUInt(tmp);
	str.resize(tmp);
	if (tmp == 0) return;
	readChars(&str[0], tmp);
}
void slib::SFile::readSArray(SArray& array) {
	suint tmp;
	readUInt(tmp);
	array.resize(tmp);
	if (tmp == 0) return;
	sfor(array) readSObject($_);
}
void slib::SFile::readSDict(SDictionary& dict) {
	suint tmp, len;
	readUInt(tmp);
	dict = SDictionary();
	if (tmp == 0) return;
	sforin(i, (suint)0, tmp) {
		readUInt(len);
		String key;
		sobj val;
		readString(key, len);
		readSObject(val);
		dict.set(key, val);
	}
}
void slib::SFile::readSObject(SObjPtr& obj) {
	char magic[8];
	readChars(magic, 8);
	if (!strcmp(magic, "integer")) {
		sinteger tmp;
		readInteger(tmp);
		obj = SNumber(tmp);
	}
	else if (!strcmp(magic, "bool")) {
		sbyte tmp;
		readByte(tmp);
		obj = SNumber(tmp == 1);
	}
	else if (!strcmp(magic, "real")) {
		double tmp;
		readDouble(tmp);
		obj = SNumber(tmp);
	}
	else if (!strcmp(magic, "string")) {
		SString tmp;
		readSString(tmp);
		obj = tmp;
	}
	else if (!strcmp(magic, "date")) {
//		suinteger tmp;
//		readUInteger(tmp);
//		obj = SDate(tmp);
	}
	else if (!strcmp(magic, "data")) {}
	else if (!strcmp(magic, "array")) {
		SArray tmp;
		readSArray(tmp);
		obj = tmp;
	}
	else if (!strcmp(magic, "dict")) {
		SDictionary tmp;
		readSDict(tmp);
		obj = tmp;
	}

}

void slib::SFile::readData(SData& dat, size_t size) {
	dat.resize(size);
	readBytes(dat.data(), size);
}
//void slib::SFile::operator>>(String& s) { readString(s, _size); }
//void slib::SFile::operator>>(SString& s) { readString(s, _size); }
void slib::SFile::operator>>(SData& d) { readData(d, _size); }

slib::SFile& slib::SFile::operator<<(const int& val) { writeString(String(val)); return *this; }
//slib::SFile& slib::SFile::operator<<(const unsigned int& val) { writeString(String(val)); return *this; }
slib::SFile& slib::SFile::operator<<(const size_t& val) { writeString(String(val)); return *this; }
slib::SFile& slib::SFile::operator<<(const int64_t& val) { writeString(String(val)); return *this; }
slib::SFile& slib::SFile::operator<<(const float& val) { writeString(String(val)); return *this; }
slib::SFile& slib::SFile::operator<<(const double& val) { writeString(String(val)); return *this; }
slib::SFile& slib::SFile::operator<<(const char* s) { writeChars(s); return *this; }
slib::SFile& slib::SFile::operator<<(const std::string& s) { writeChars(s.c_str()); return *this; }
slib::SFile& slib::SFile::operator<<(const Char& c) { writeString(c.toString()); return *this; }
slib::SFile& slib::SFile::operator<<(const String& s) { writeString(s); return *this; }
slib::SFile& slib::SFile::operator<<(const SString& s) { writeString(s); return *this; }
slib::SFile& slib::SFile::operator<<(const SDate& d) { writeString(d.toString()); return *this; }
slib::SFile& slib::SFile::operator<<(const SData& d) { writeBytes(d.data(), d.size()); return *this; }
slib::SFile& slib::SFile::operator<<(const SElement& e) { writeString(e.toString()); return *this; }
slib::SFile& slib::SFile::operator<<(const SObjPtr& o) { writeString(o.toString()); return *this; }
//slib::SFile& slib::SFile::operator<<(const sobj& obj) { writeString(obj.toString()); return *this; }
void slib::SFile::writeChars(const char* str) { _stream.write(str, strlen(str)); }
void slib::SFile::writeCharsAs(const char* str, const size_t size) {
	auto len = str ? strlen(str) : 0;
	if (len <= size) {
		if (len) _stream.write(str, len);
		len = size - len;
		sforin(i, 0_u, len) { writeByte(0); }
	}
	else _stream.write(str, size);
}
void slib::SFile::writeString(const String& str) { _stream.write(str.cstr(), str.size()); }
void slib::SFile::writeString(const SString& str) { _stream.write(str.cstr(), str.size()); }
void slib::SFile::writeBytes(const void* dat, size_t size) { _stream.write((char*)dat, size); }
void slib::SFile::writeChar(const char& c) { _stream.write(&c, 1); }
void slib::SFile::writeUChar(const unsigned char& c) { _stream.write((char*)&c, 1); }
void slib::SFile::writeByte(const sbyte& val) { _stream.write((char*)&val, 1); }
void slib::SFile::writeUByte(const subyte& val) { _stream.write((char*)&val, 1); }
void slib::SFile::writeShort(const sshort& val) { _stream.write((char*)&val, 2); }
void slib::SFile::writeUShort(const sushort& val) { _stream.write((char*)&val, 2); }
void slib::SFile::writeInt(const sint& val) { _stream.write((char*)&val, 4); }
void slib::SFile::writeUInt(const suint& val) { _stream.write((char*)&val, 4); }
void slib::SFile::writeInteger(const sinteger& val) { _stream.write((char*)&val, 8); }
void slib::SFile::writeUInteger(const suinteger& val) { _stream.write((char*)&val, 8); }
void slib::SFile::writeFloat(const float& val) { _stream.write((char*)&val, 4); }
void slib::SFile::writeDouble(const double& val) { _stream.write((char*)&val, 8); }
//void slib::SFile::writeReal(const sreal& val) { _stream.write((char*)&val, 8); }
void slib::SFile::writeSString(const SString& str) {
	writeUInt((suint)str.size());
	writeString(str);
}
void slib::SFile::writeSArray(const SArray& array) {
	writeUInt((suint)array.size());
	sfor(array) writeSObject($_);
}
void slib::SFile::writeSDict(const SDictionary& dict) {
	writeUInt((suint)dict.size());
	sfor(dict) {
		writeUInt((suint)$_.key().size());
		writeString($_.key());
		writeSObject($_.value());
	}
}
void slib::SFile::writeSObject(const sobj& obj) {
	auto cls = obj.getClass();
	writeCharsAs(cls.cstr(), 8);
	if (cls == "integer") writeInteger(obj.integer());
	else if (cls == "real") writeDouble(obj.doubleValue());
	else if (cls == "bool") writeByte(obj ? 1 : 0);
	else if (cls == "string") writeSString(obj.string());
	else if (cls == "date") {}
	else if (cls == "data") {}
	else if (cls == "array") writeSArray(obj.array());
	else if (cls == "dict") writeSDict(obj.dict());

}

/*

inline void objRead(SFile* file, sobj& obj);
inline void numRead(SFile* file, sobj& num) {
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
inline void strRead(SFile* file, sobj& str) {
	suint len;
	file->readUInt(len);
	str = String(len, '\0');
	file->readChars(str.string().ptr(), len);
}
inline void dateRead(SFile* file, sobj& date) {
	suint len;
	String form, dt;
	file->readUInt(len);
	file->readString(form, len);
	file->readUInt(len);
	file->readString(dt, len);
	date = SDate(dt, form);
}
inline void datRead(SFile* file, sobj& dat) {
	suint len;
	file->readUInt(len);
	dat = SData(len);
	file->readBytes(dat.data().ptr(), len);
}
inline void arrayRead(SFile* file, sobj& array) {
	suint tmp;
	file->readUInt(tmp);
	array = SArray((size_t)tmp);
	if (tmp == 0) return;
	sforeach(array) objRead(file, E_);
}
inline void dictRead(SFile* file, sobj& dict) {
	
}
inline void fileRead(SFile* file, sobj& f) {
	suint len;
	String path;
	file->readUInt(len);
	file->readString(path, len);
	f = SFile(path);
}
inline void tblRead(SFile* file, sobj& tbl) {
}
inline void imgRead(SFile* file, sobj& img) {
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

void SFile::readSObject(sobj& obj) { objRead(this, obj); }

inline void objWrite(SFile* file, const sobj& obj);
inline void numWrite(SFile* file, const SNumber& num) {
	file->writeInt((sint)NUMBER_OBJ);
	file->writeUByte(num.type());
	file->writeBytes(num.ptr(), 8);
}
inline void strWrite(SFile* file, const SString& str) {
	file->writeInt((sint)STRING_OBJ);
	file->writeUInt((suint)str.length());
	file->writeBytes(str.cstr(), str.size());
}
inline void dateWrite(SFile* file, const SDate& date) {
	file->writeInt((sint)DATE_OBJ);
	file->writeUInt((suint)strlen(slib::sstyle::ISO8601));
	file->writeChars(slib::sstyle::ISO8601);
	file->writeUInt((suint)date.toString(slib::sstyle::ISO8601).length());
	file->writeString(date.toString());
}
inline void datWrite(SFile* file, const SData& dat) {
	file->writeInt((sint)DATA_OBJ);
	file->writeUInt((suint)dat.size());
	file->writeBytes(dat.ptr(), dat.size());
}
inline void arrayWrite(SFile* file, const SArray& array) {
	file->writeInt((sint)ARRAY_OBJ);
	file->writeUInt((suint)array.size());
	if (array.empty()) return;
	else { sforeach(array) objWrite(file, E_); }
}
inline void dictWrite(SFile* file, const SDictionary& dict) {
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
inline void fileWrite(SFile* file, const SFile& f) {
	file->writeInt((sint)FILE_OBJ);
	file->writeUInt((suint)f.path().length());
	file->writeBytes(f.path().cstr(), f.path().size());
}
inline void tblWrite(SFile* file, const STable& tbl) {
}
inline void imgWrite(SFile* file, const smedia::SImage& img) {
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
void SFile::writeSObject(const sobj& obj) { objWrite(this, obj); }
SFile::operator const char *() const { return _path.cstr(); }
bool SFile::operator<(const SFile &file) const { return _path < file._path; }
bool SFile::operator==(const SFile &file) const { return _path == file._path; }


slib::sio::siostream::siostream(int i) :_mode(i) {}
slib::sio::siostream::siostream(const char* s) : _mode(1) { _file.open(s, sio::READ | sio::WRITE | sio::CREATE); }
slib::sio::siostream::~siostream() {}
void slib::sio::siostream::setStdMode() { 
	_mode = 0; 
	if(_file.isOpened()) _file.close(); 
}
void slib::sio::siostream::setPath(const char* s) { 
	_mode = 1; 
	if (_file.isOpened()) _file.close();
	_file.open(s, sio::READ | sio::WRITE | sio::CREATE);
}
void slib::sio::siostream::read(slib::String& s) {
	if (_mode) _file >> s;
	else std::cin >> s;
}
void slib::sio::siostream::_output1() {
	if (_mode) { _file.flush(); }
	else std::cout << std::flush;
}
void slib::sio::siostream::_output2() {
	if (_mode) { _file << NEW_LINE; _file.flush(); }
	else std::cout << std::endl;
}
slib::sio::siostream slib::SIO_ = slib::sio::siostream(0);
*/
slib::SFile::operator bool() const {
	return isOpened() && (_mode & sio::READ) && !eof();
}

slib::String slib::SFile::getClass() const { return "file"; }
slib::String slib::SFile::toString(const char *format) const { return "file://" + _path; }
slib::SObject* slib::SFile::clone() const { return new SFile(*this); }
