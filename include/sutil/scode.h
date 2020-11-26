#ifndef SLIB_SCODE_H
#define SLIB_SCODE_H

#include "sbasic.h"
#include "sobj/sdata.h"
#include "sio/sfile.h"
extern "C" {
#include "zlib/zlib.h"
#include "crypto/md5.h"
#include "crypto/sha.h"
}

namespace slib {

	class SLIB_DLL SHash {
	public:
		static suint crc32(ubytearray& data);
		static bool crc32check(ubytearray& data, suint& ref);

		static void md5(String& data, ubytearray& digest);
		static void md5(ubytearray& data, ubytearray& digest);
		static void md5Str(String& data, String& str);
		static void md5Str(ubytearray& data, String& str);
		static bool md5check(ubytearray& data, const char *str);

		static void sha256(String& data, ubytearray& digest);
		static void sha256(ubytearray& data, ubytearray& digest);
		static void sha256Str(String& data, String& str);
		static void sha256Str(ubytearray& data, String& str);
		static bool sha256check(ubytearray& data, const char* str);

		static void sha512(String& data, ubytearray& digest);
		static void sha512(ubytearray& data, ubytearray& digest);
		static void sha512Str(String& data, String& str);
		static void sha512Str(ubytearray& data, String& str);
		static bool sha512check(ubytearray& data, const char* str);

	};
	/*
	class SLIB_DLL SEncryption {
		static void encrypt(ubytearray& data, ubytearray& enc, const char* method);
		static void decrypt(const char* method, ubytearray& data, ubytearray& dec);
	};
	*/
	class SLIB_DLL SZip {
	public:
		const static suint LOCAL_FILE_HEADER_SIG;
		const static suint DATA_DESCRIPTOR_SIG;
		const static suint CENTRAL_DIR_SIG;
		const static suint CENTRAL_DIR_END_SIG;

		struct SLIB_DLL localFileHeader {
			sushort ver, flag, method, time, date, flen, elen;
			suint sig, crc, csize, rsize;
			String name;
			ubytearray ext;
			localFileHeader();
			~localFileHeader();
		};
		struct SLIB_DLL dataDescriptor {
			suint sig, crc, csize, rsize;
			dataDescriptor();
			~dataDescriptor();
		};
		struct SLIB_DLL centralDir {
			sushort ver, mver, flag, method, time, date, flen, elen, clen, disk, iattr;
			suint sig, crc, csize, rsize, eattr, offset;
			String name, comment;
			ubytearray ext;
			centralDir();
			~centralDir();
			suint size();
		};
		struct SLIB_DLL centralDirEnd {
			sushort num, disk, record, sum, clen;
			suint sig, size, offset;
			String comment;
			centralDirEnd();
			~centralDirEnd();
		};

	private:
		sio::SFile _file;
		Array<centralDir> _contents;

	public:
		SZip();
		SZip(const char* path);
		~SZip();

		void load(const char* path);
		void save(const char* path);
		void extract(const char* path, SData& data);
		stringarray fileList();
		static void archive(sio::SFile& ori, const char* dest = nullptr, const char* encrypt = nullptr);
		static void expand(sio::SFile& ori, const char* dest = nullptr, const char* decrypt = nullptr);
	};

	class SLIB_DLL SCode {
	public:
		static void urlEncode(String& str);
		static void urlDecode(String& str);

		static const char* B64_STR;
		static subyte b64i(const char& c);
		static size_t base64CharCount(size_t size);
		static suinteger decodeCharCount(const char* base);
		static void encodeB64Char(const void* data, char* encoded, size_t size);
		static void decodeB64Char(const char* data, void* decoded);
		static void encodeBASE64(const String& ori, String& base);
		static void encodeBASE64(const void* ori, size_t size, char* base);
		static void decodeBASE64(const String& base, String& ori);
		static void decodeBASE64(const char* base, void* ori, const size_t& size);

		static void expandTo(ubytearray& ori, ubytearray& dest, size_t cap = -1, sint bits = 31, sint flush = Z_NO_FLUSH);
		static void expand(ubytearray& bytes, size_t cap = -1, sint bits = 31, sint flush = Z_NO_FLUSH);
		static void compress(ubytearray& bytes);

	};
}
#endif