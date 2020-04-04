#ifndef SLIB_SCODE_H
#define SLIB_SCODE_H

#include "sbasic/sbasic.h"
#include "sobj/sdata.h"
#include "sio/sfile.h"
extern "C" {
    #include "zlib/zlib.h"
}

namespace slib {

	class SLIB_DLL SChecker {
    public:
		static suint crc32(subyte* byte, size_t size);
        static bool crc32check(subyte *byte, size_t size, suint &ref);

    };
    
    class SLIB_DLL SZip {
    public:
        const static suint LOCAL_FILE_HEADER_SIG;
        const static suint DATA_DESCRIPTOR_SIG;
        const static suint CENTRAL_DIR_SIG;
        const static suint CENTRAL_DIR_END_SIG;
        
        struct localFileHeader {
            sushort ver, flag, method, time, date, flen, elen;
            suint sig, crc, csize, rsize;
            String name;
            ubytearray ext;
            localFileHeader();
            ~localFileHeader();
        };
        struct dataDescriptor {
            suint sig, crc, csize, rsize;
            dataDescriptor();
            ~dataDescriptor();
        };
        struct centralDir {
            sushort ver, mver, flag, method, time, date, flen, elen, clen, disk, iattr;
            suint sig, crc, csize, rsize, eattr, offset;
            String name, comment;
            ubytearray ext;
            centralDir();
            ~centralDir();
            suint size();
        };
        struct centralDirEnd {
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
		void extract(const char* path, SData &data);
		stringarray fileList();
        static void archive(sio::SFile &ori, const char *dest = nullptr, const char *encrypt = nullptr);
        static void expand(sio::SFile &ori, const char *dest = nullptr, const char *decrypt = nullptr);
    };
    
    class SLIB_DLL SCode {
    public:
        static void urlEncode(String &str);
        static void urlDecode(String &str);
        
        static const char *B64_STR;
        static subyte b64i(const char &c);
        static size_t base64CharCount(size_t size);
        static suinteger decodeCharCount(const char *base);
        static void encodeB64Char(const void *data, char *encoded, size_t size);
        static void decodeB64Char(const char *data, void *decoded);
        static void encodeBASE64(const void *ori, size_t size, char *base);
        static void decodeBASE64(const char *base, void *ori, const size_t &size);
        
        static void expand(ubytearray &bytes);
        static void compress(ubytearray &bytes);
        
    };
}
#endif