#ifndef SBIO_SEQIO_H
#define SBIO_SEQIO_H
#include "sobj/stext.h"
#include "sio/sfile.h"
#include "sio/stream.h"
#include "sbioinfo/biutil.h"
#include "sbioinfo/fasta.h"
namespace slib {
	namespace sbio {
		class SLIB_DLL Sequence;
		class SLIB_DLL SeqList;

		class AbiDir {
		public:
			sint number;
			sshort element_type;
			sshort element_size;
			sint num_elements;
			sint data_size;
			sint data_offset;
			sint data_handle;
		public:
			AbiDir();
			AbiDir(const AbiDir& dir);
			~AbiDir();
			AbiDir& operator=(const AbiDir& dir);
			bool operator<(const AbiDir& dir) const;
			bool operator==(const AbiDir& dir) const;

		};
		constexpr char ABI_MAGIC[5] = "ABIF";
		namespace sio {
			extern SLIB_DLL String fileType(SFile& file);
			extern SLIB_DLL void writeSeq(IOStream& stream, const Sequence& seq, const char* format = nullptr);
			//extern SLIB_DLL void writeSeqs(IOStream& stream, const SeqList& seqs, const char* format = nullptr);
			extern SLIB_DLL void writeTxt(IOStream& stream, const Sequence& seq);
			extern SLIB_DLL void writeFa(IOStream& stream, const Sequence& seq);
			extern SLIB_DLL void writeFa(IOStream& stream, const  SeqList& seqs);
			extern SLIB_DLL void writeGbk(IOStream& stream, const Sequence& seq);
			extern SLIB_DLL void readTxt(SFile& file, Sequence& seq);
			extern SLIB_DLL void readFa(SFile& file, Sequence& seq);
			extern SLIB_DLL void readGbk(SFile& file, Sequence& seq);
			extern SLIB_DLL void readAbi(SFile& file, Sequence& seq);
		}
	}
}

#endif