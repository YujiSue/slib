#ifndef SLIB_SCODE_H
#define SLIB_SCODE_H

//#include "sbasic.h"
#include "sobj/sstring.h"
#include "sobj/sdata.h"
#include "sio/sfile.h"
#include "sio/stream.h"
extern "C" {
#include "zlib/zlib.h"
#include "smath/md5.h"
#include "smath/sha.h"
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
	/*
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
		SFile _file;
		Array<centralDir> _contents;

	public:
		SZip();
		SZip(const char* path);
		~SZip();

		void load(const char* path);
		void save(const char* path);
		void extract(const char* path, SData& data);
		stringarray fileList();
		static void archive(SFile& ori, const char* dest = nullptr, const char* encrypt = nullptr);
		static void expand(SFile& ori, const char* dest = nullptr, const char* decrypt = nullptr);
	};
	*/
	namespace sutil {
		constexpr subyte DECODE = 0x01;
		constexpr subyte ENCODE = 0x02;


	}
	constexpr char BASE64_CHAR[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	constexpr char URL_BASE64_CHAR[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
	namespace sutil {
		extern SLIB_DLL String decode(const String& method, const char* s);
		extern SLIB_DLL String encode(const String& method, const char* s);
		extern SLIB_DLL String decodeURL(const char* s);
		extern SLIB_DLL String encodeURL(const char* s);
		extern SLIB_DLL void decodeBase64Char(const char* src, subyte* dest);
		extern SLIB_DLL void decodeBase64(const char* src, String& dest);
		extern SLIB_DLL void decodeBase64(const char* src, ubytearray& dest);
		extern SLIB_DLL void decodeBase64(const SObjPtr& src, SObjPtr& dest);
		extern SLIB_DLL void encodeBase64Char(const char* src, char* dest);
		extern SLIB_DLL void encodeBase64(const char* src, String &dest);
		extern SLIB_DLL void encodeBase64(const ubytearray& src, String& dest);
		extern SLIB_DLL void encodeBase64(const SObjPtr& src, SObjPtr& dest);

		extern void SLIB_DLL deflate(ubytearray& dest, const ubytearray& ori, size_t cap = -1, int bits = 31);
		extern void SLIB_DLL inflate(ubytearray& dest, const ubytearray& ori, size_t cap = -1, int bits = 31);


		extern sobj compress(const char* path);
		extern sobj compress(const SData& data);
		extern sobj decompress(const char* path);
		extern sobj decompress(const SData& data);

		extern sobj archive(const char* path);
		extern sobj archive(const SData& data);
		extern sobj expand(const char* path);
		extern sobj expand(const SData& data);



	}
	
	class SLIB_DLL SCode {
	public:
		static String urlEncode(const String& str);
		static String urlDecode(const String& str);
		
		inline static size_t predictSize(const char* method, subyte type, const subyte* ori, const size_t sz);
		inline static void encode(const char* method, const subyte* ori, const size_t sz, slib::IOStream &stream);
		inline static void encode(const char* method, const String& ori, slib::IOStream& stream);
		inline static void encode(const char* method, const SData& ori, slib::IOStream& stream);
		inline static void decode(const char* method, const subyte* ori, const size_t sz, slib::IOStream& stream);
		inline static void decode(const char* method, const String& ori, slib::IOStream& stream);
		inline static void decode(const char* method, const SData& ori, slib::IOStream& stream);

		inline static void hash(const char* method, const subyte* ori, const size_t sz, slib::IOStream& stream);
		//inline static void hash(const char* method, const subyte* ori, const size_t sz, slib::IOStream& stream);
		inline static void hash(const char* method, const String& ori, slib::IOStream& stream);
		inline static bool check(const subyte* ref, const subyte* que, const size_t sz);

		inline static void compress(const char* method, const subyte* ori, const size_t sz, slib::IOStream& stream);
		inline static void compress(const char* method, const String& ori, slib::IOStream& stream);
		inline static void compress(const char* method, const SData& ori, const size_t sz, slib::IOStream& stream);
		inline static void decompress(const char* method, const subyte* ori, const size_t sz, slib::IOStream& stream);
		inline static void decompress(const char* method, const String& ori, const size_t sz, slib::IOStream& stream);
		inline static void decompress(const char* method, const SData& ori, const size_t sz, slib::IOStream& stream);

		inline static void archive();
		inline static void expand();


		//static void expand(ubytearray& bytes, size_t cap = -1, sint bits = 31, sint flush = Z_NO_FLUSH);
		//static void compress(ubytearray& bytes);



		static const char* B64_STR;
		static subyte b64i(const char& c);
		static void encodeB64Char(const char* ori, char* encoded, size_t size);
		static void decodeB64Char(const char* data, char* decoded);
		static void encodeBASE64(const String& ori, String& base);
		static size_t encodeBASE64(const ubytearray& ori, String& base, bool size = false);
		static void decodeBASE64(const String& base, String& ori);
		static void decodeBASE64(const String& base, ubytearray& ori, size_t s = -1);

		static void expandTo(ubytearray& dest, ubytearray& ori, size_t cap = -1, sint bits = 31, sint flush = Z_NO_FLUSH);
		static void expand(ubytearray& bytes, size_t cap = -1, sint bits = 31, sint flush = Z_NO_FLUSH);
		static void compress(ubytearray& bytes);

	};


}
#endif