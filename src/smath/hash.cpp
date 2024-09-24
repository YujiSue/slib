#include "smath/numeric.h"
#include "smath/hash.h"
inline void slib::digest(const char* m, const slib::subyte* data, const size_t s, ubytearray& hash) {
	slib::String method(m);
	if (method == "md5") slib::md5(data, s, hash);
	else if (method == "sha256") slib::sha256(data, s, hash);
	else if (method == "sha512") slib::sha512(data, s, hash);
}
inline void slib::digest(const char* m, slib::String& data, ubytearray& hash) {
	slib::String method(m);
	if (method == "md5") slib::md5((const slib::subyte *)data.cstr(), data.size(), hash);
	else if (method == "sha256") slib::sha256((const slib::subyte*)data.cstr(), data.size(), hash);
	else if (method == "sha512") slib::sha512((const slib::subyte*)data.cstr(), data.size(), hash);
}
inline void slib::digest(const char* m, ubytearray& dat, ubytearray& hash) {
	slib::String method(m);
	if (method == "md5") slib::md5(dat.data(), dat.size(), hash);
	else if (method == "sha256") slib::sha256(dat.data(), dat.size(), hash);
	else if (method == "sha512") slib::sha512(dat.data(), dat.size(), hash);
}
inline void slib::digestString(const char* m, const slib::subyte* data, const size_t s, slib::String &hash) {
	slib::String method(m);
	if (method == "md5") slib::md5Str(data, s, hash);
	else if (method == "sha256") slib::sha256Str(data, s, hash);
	else if (method == "sha512") slib::sha512Str(data, s, hash);
}
inline void slib::digestString(const char* m, slib::String& data, slib::String& hash) {
	slib::String method(m);
	if (method == "md5") slib::md5Str((const slib::subyte*)data.cstr(), data.size(), hash);
	else if (method == "sha256") slib::sha256Str((const slib::subyte*)data.cstr(), data.size(), hash);
	else if (method == "sha512") slib::sha512Str((const slib::subyte*)data.cstr(), data.size(), hash);
}
inline void slib::digestString(const char* m, ubytearray& dat, slib::String& hash) {
	slib::String method(m);
	if (method == "md5") slib::md5Str(dat.data(), dat.size(), hash);
	else if (method == "sha256") slib::sha256Str(dat.data(), dat.size(), hash);
	else if (method == "sha512") slib::sha512Str(dat.data(), dat.size(), hash);
}
inline bool slib::check(const char* m, const slib::subyte* data, const size_t s, const ubytearray& ref) {
	ubytearray hash;
	slib::digest(m, data, s, hash);
	return hash == ref;
}
inline bool check(const char* m, const slib::subyte* data, const size_t s, const slib::String& ref) {
	slib::String hash;
	slib::digestString(m, data, s, hash);
	return hash == ref;
}
inline slib::suint slib::crc32(const slib::subyte* data, const size_t s) {
	suint val = 0;
	val = crc32_z(val, static_cast<const Bytef*>(data), s);
	return val;
}
inline bool slib::crc32check(const slib::subyte* data, const size_t s, const suint& ref) { return ref == slib::crc32(data, s); }
inline void slib::md5(const slib::subyte* data, const size_t s, ubytearray& hash) {
	hash.resize(16);
	MD5_CTX context;
	MD5Init(&context);
	MD5Update(&context, const_cast<unsigned char *>(data), (unsigned)s);
	MD5Final(&hash[0], &context);
}
inline void slib::md5Str(const slib::subyte* data, const size_t s, String& str) {
	MD5_CTX context;
	unsigned char digest[16];
	auto p = &digest[0];
	MD5Init(&context);
	MD5Update(&context, const_cast<unsigned char*>(data), (unsigned)s);
	MD5Final(&digest[0], &context);
	str.clear();
	str.reserve(33);
	sforin(i, 0, 16) { str += slib::toHex(*p); ++p; }
}
inline bool slib::md5check(const slib::subyte* data, const size_t s, const ubytearray& ref) {
	ubytearray hash;
	slib::md5(data, s, hash);
	return hash == ref;
}
inline bool slib::md5check(const slib::subyte* data, const size_t s, const slib::String& ref) {
	slib::String hash;
	slib::md5Str(data, s, hash);
	return hash == ref;
}
inline void slib::sha256(const slib::subyte* data, const size_t s, ubytearray& hash) {
	hash.resize(256);
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, data, s);
	SHA256_Final(hash.data(), &ctx);
}
inline void slib::sha256Str(const slib::subyte* data, const size_t s, slib::String& str) {
	ubytearray digest;
	slib::sha256(data, s, digest);
	str.clear(); 
	str.reserve(65);
	sfor(digest) { str += slib::toHex($_); }
}
inline bool slib::sha256check(const slib::subyte* data, const size_t s, const ubytearray& ref) {
	ubytearray digest;
	slib::sha256(data, s, digest);
	return digest == ref;
}
inline bool slib::sha256check(const slib::subyte* data, const size_t s, const slib::String& ref) {
	String digest;
	slib::sha256Str(data, s, digest);
	return digest == ref;
}
inline void slib::sha512(const slib::subyte* data, const size_t s, ubytearray& hash) {
	hash.resize(512);
	SHA512_CTX ctx;
	SHA512_Init(&ctx);
	SHA512_Update(&ctx, data, s);
	SHA512_Final(hash.data(), &ctx);
}
inline void slib::sha512Str(const slib::subyte* data, const size_t s, slib::String& str) {
	ubytearray digest;
	slib::sha512(data, s, digest);
	str.clear();
	str.reserve(129);
	sfor(digest) { str += slib::toHex($_); }
}
inline bool slib::sha512check(const slib::subyte* data, const size_t s, const ubytearray& ref) {
	ubytearray digest;
	slib::sha512(data, s, digest);
	return digest == ref;
}
inline bool slib::sha512check(const slib::subyte* data, const size_t s, const slib::String& ref) {
	String digest;
	slib::sha512Str(data, s, digest);
	return digest == ref;
}
