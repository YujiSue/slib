#ifndef SMATH_HASH_H
#define SMATH_HASH_H
extern "C" {
#include "zlib/zlib.h"
#include "smath/md5.h"
#include "smath/sha.h"
}
#include "sbasic/array.h"
#include "sbasic/string.h"
namespace slib {
	extern inline void digest(const char* m, const subyte* data, const size_t s, ubytearray& hash);
	extern inline void digest(const char* m, String& data, ubytearray& hash);
	extern inline void digest(const char* m, ubytearray& data, ubytearray& hash);
	extern inline void digestString(const char* m, const subyte* data, const size_t s, String& hash);
	extern inline void digestString(const char* m, String& data, String& hash);
	extern inline void digestString(const char* m, ubytearray& data, String& hash);
	extern inline bool check(const char* m, const subyte* data, const size_t s, const ubytearray &ref);
	extern inline bool check(const char* m, const subyte* data, const size_t s, const slib::String& ref);

	extern inline suint crc32(const subyte* data, const size_t s);
	extern inline bool crc32check(const subyte* data, const size_t s, const suint& ref);

	extern inline void md5(const subyte* data, const size_t s, ubytearray& hash);
	extern inline void md5Str(const subyte* data, const size_t s, String& str);
	extern inline bool md5check(const subyte* data, const size_t s, const ubytearray& ref);
	extern inline bool md5check(const subyte* data, const size_t s, const String &ref);

	extern inline void sha256(const subyte* data, const size_t s, ubytearray& hash);
	extern inline void sha256Str(const subyte* data, const size_t s, String& str);
	extern inline bool sha256check(const subyte* data, const size_t s, const ubytearray& ref);
	extern inline bool sha256check(const subyte* data, const size_t s, const String& ref);

	extern inline void sha512(const subyte* data, const size_t s, ubytearray& hash);
	extern inline void sha512Str(const subyte* data, const size_t s, String& str);
	extern inline bool sha512check(const subyte* data, const size_t s, const ubytearray& ref);
	extern inline bool sha512check(const subyte* data, const size_t s, const String& ref);
	
}
#endif