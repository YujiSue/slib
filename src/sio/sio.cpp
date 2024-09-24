#include "sio/sio.h"
#include "sutil/ssys.h"

slib::SIOException::SIOException() : Exception() { prefix = "IO"; }
slib::SIOException::SIOException(int i, const char* msg, const char* desc) : Exception(i, msg, desc) { prefix = "IO"; }
slib::SIOException::~SIOException() {}

slib::String slib::sio::nofileErrorText(const char* path) { return "'" + S(path) + "' does not exist."; }
slib::String slib::sio::fileopenErrorText(const char* path) { return "Cannot open '" + S(path) + "'."; }
slib::String slib::sio::filereadErrorText(const char* path) { return "Cannot read '" + S(path) + "'."; }
slib::String slib::sio::filewriteErrorText(const char* path) { return "Cannot write to '" + S(path) + "'."; }

slib::String slib::ssys::root() {
#ifdef WIN_OS
	return "C:\\";
#else
	return "/";
#endif
}
slib::String slib::ssys::home() {
#ifdef WIN_OS
	return String(getenv("USERPROFILE"));
#else
	return String(getenv("HOME"));
#endif
}
slib::String slib::ssys::current() {
	char tmp[256];
	auto res = getcwd(tmp, 256);
	if (res) return String(const_cast<const char*>(tmp));
	throw ProcException(procErrorText("getcwd"));
}
void slib::ssys::setCurrent(const char* path) {
	auto res = chdir(path);
	if (res != 0) throw ProcException(procErrorText("chdir"));
}
stringarray slib::sfs::pathComponents(const char* path) { 
	if (!path || path[0] == '\0') return {};
	auto count = 0, len = (int)strlen(sio::PATH_SEPARATOR);
	while (!memcmp(path, sio::PATH_SEPARATOR, len)) { ++count;  path += len; }
	if (count) {
		auto array = S(path).split(sio::PATH_SEPARATOR);
#ifdef WIN_OS
		if (count == 1) array[0] = S(sio::PATH_SEPARATOR) + array[0];
		else if (count == 2) {
			array[1] = S(sio::PATH_SEPARATOR) * 2 + array[0] + S(sio::PATH_SEPARATOR) + array[1];
			array.removeAt(0);
		}
#else
		array[0] = S(sio::PATH_SEPARATOR) + array[0];
#endif
		return array;
	}
	else return S(path).split(sio::PATH_SEPARATOR);
}
slib::Pair<slib::String, slib::String> slib::sfs::splitPath(const char* path) {
	String str(path);
	size_t p = 0;
#if defined(__GNUC__) && (__GNUC__ < 5)
	if (str.match("/")) p = str.rfind("/");
	else if (str.match("\\")) p = str.rfind("\\");
	else p = NOT_FOUND;
#else
	p = str.rfind(REG("/[\\/\\\\]+/"));
#endif
	if (p == 0) return Pair<slib::String, slib::String>(ssys::root(), "");
	else if (p == NOT_FOUND) return Pair<slib::String, slib::String>(str, "");
	else return Pair<slib::String, slib::String>(str.substring(0, p), str.substring(p + 1, -1));
}
slib::String slib::sfs::fileName(const char* path, bool ext) { 
	auto fn = sfs::splitPath(path).second;
	return (ext || fn.rfind(".") == NOT_FOUND) ? fn : fn.substring(0, fn.rfind("."));
}
slib::String slib::sfs::extension(const char* path) { return slib::String(path).split(".")[-1]; }
slib::String slib::sfs::absolutePath(const char* path) {
	String apath(path);
	if (sstr::isQuoted(apath)) apath.clip(srange(1, -1));
#ifdef WIN_OS
	if (apath.empty()) apath = ssys::root();
	if (apath.match(REG("/^[A-Z][:]/"))) return apath;
	if (apath.beginWith("\\\\")) return apath;
#else
	if (apath.empty()) apath = ssys::root();
	if (apath.beginWith(sio::PATH_SEPARATOR)) return apath;
#endif
	if (apath[0] == '~') {
		if (apath == "~") return ssys::home();
		else if (apath[1] == '\\' || apath[1] == '/') {
			apath.replace(0, 1, ssys::home());
		}
	}
	else if (apath[0] == '.') {
		if (apath == ".") return ssys::current();
		else if (apath[1] == '\\' || apath[1] == '/') {
			apath.replace(0, 1, ssys::current());
		}
		else if (apath[1] == '.') {
			auto cpath = sfs::pathComponents(ssys::current());
			if (apath == "..") {
				cpath.resize(cpath.size() - 1);
				return toString(cpath, sio::PATH_SEPARATOR);
			}
			else {
				auto layers = sfs::pathComponents(apath);
				auto count = 0;
				sfor(layers) {
					if ($_ == "..") ++count;
					else break;
				}
				if (count) {
					cpath.resize(cpath.size() - count);
					layers.remove(0, count);
					if (layers.empty()) apath = toString(cpath, sio::PATH_SEPARATOR);
					else apath = sfs::joinPath(toString(cpath, sio::PATH_SEPARATOR), toString(layers, sio::PATH_SEPARATOR));
				}
				else apath = sfs::joinPath(ssys::current(), apath);
			}
		}
	}
	else apath = sfs::joinPath(ssys::current(), apath);
	if (1 < apath.size() && 
		(apath[-1] == '\\' || apath[-1] == '/')) apath.resize(apath.size() - 1);
	return  apath;
}
bool slib::sfs::isDir(const char* path) {
#ifdef WIN_OS
	auto path_ = S(path).localize();
	struct _stat64 buf;
	auto res = _stat64(path_, &buf);
	return !res && S_ISDIR(buf.st_mode);
#else
	struct stat buf;
	auto res = stat(path, &buf);
	return !res && S_ISDIR(buf.st_mode);
#endif
}
slib::sushort slib::sfs::permission(const char* path) {
	/*
	* 
	*/
	return 0;
}
bool slib::sfs::exist(const char* s) {
	String path(s);
#ifdef WIN_OS
	path = path.localize();
	struct _stat64 buf;
	auto res = _stat64(path, &buf);
	return !res;
#else
	struct stat buf;
	auto res = stat(path, &buf);
	return !res;
#endif
}
int slib::sfs::make(const char* s) {
	String path(s);
#ifdef WIN_OS
	path = path.localize();
#endif
	FILE* fp = fopen(path, "w");
	if (!fp) return 1;
	else fclose(fp);
	return 0;
}
inline int _makeDir(const char* s) {
	slib::String path(s);
#ifdef WIN_OS
	path = path.localize();
#endif
	if (slib::sfs::exist(path)) { if (!slib::sfs::isDir(path)) throw slib::Exception(); }
#ifdef WIN_OS
	return mkdir(path);
#else
	return mkdir(path, 0777);
#endif
}
int slib::sfs::makeDir(const char *s, bool recurrent, sushort permit) {
	if (recurrent) {
		int res = 0;
		auto components = pathComponents(s);
		String recurpath(components[0]);
		if (!exist(recurpath))res = _makeDir(recurpath);
		sforin(it, components.begin() + 1, components.end()) {
			recurpath << sio::PATH_SEPARATOR << $_;
			if (exist(recurpath)) continue;
			res = _makeDir(recurpath);
		}
		return res;
	}
	else return _makeDir(s);
}
inline int mv(const char* from, const char* to) { return rename(from, to); }
int slib::sfs::move(const char* f, const char* t, subyte op) { 
	String from(f), to(t);
#ifdef WIN_OS
	from = from.localize();
	to = to.localize();
#endif
	return mv(from, to);
}
inline int _copy(slib::String from, slib::String to) {
	size_t packet = 1 << 12, fsize;
	ubytearray buffer(packet);
#ifdef WIN_OS
	from = from.localize();
	to = to.localize();
#endif
	std::ifstream ifs(from, std::ios::in | std::ios::binary);
	std::ofstream ofs(to, std::ios::out | std::ios::trunc | std::ios::binary);
	ifs.seekg(0, std::ios::end);
	fsize = (size_t)ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	ifs.clear();
	while ((size_t)ifs.tellg() + packet <= fsize) {
		ifs.read((char*)buffer.data(), packet);
		ofs.write((char*)buffer.data(), packet);
		ofs.flush();
	}
	packet = fsize - (size_t)ifs.tellg();
	if (packet) {
		ifs.read((char*)buffer.data(), packet);
		ofs.write((char*)buffer.data(), packet);
		ofs.flush();
	}
	return 0;
}
int slib::sfs::copy(const char* f, const char* t, subyte op) { 
	if (sfs::isDir(f)) {
		if (sfs::exist(t)) {
			if (sfs::isDir(t)) {

			}
			else throw slib::Exception();
		}
		else {

		}
		return 0;
	}
	else {
		if (sfs::exist(t) && sfs::isDir(t))
			return _copy(f, sfs::joinPath(t, sfs::fileName(f)));
		return _copy(f, t);
	}
}
inline int rm(const char* s) { return remove(s); }
int slib::sfs::remove(const char* s) {
	String path(s);
#ifdef WIN_OS
	path = path.localize();
#endif
	return rm(path.cstr());
}

