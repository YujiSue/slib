#ifndef SBIO_FASTA_H
#define SBIO_FASTA_H
#include "sio/sfile.h"
#include "sbioinfo/biutil.h"
namespace slib {
	namespace sbio {
		class SLIB_DLL Sequence;
		class SLIB_DLL SeqList;

		constexpr size_t FASTA_ROW_COUNT = 60;

		class SLIB_DLL Fasta {
			SFile _file;
			sushort _type;
			String _ln, _buffer;
			size_t _idx;

		public:
			stringarray titles, attributes;
			sizearray offsets;

		public:
			Fasta();
			Fasta(const char *path, sushort type = 0xFFFF);
			~Fasta();
			void operator>>(Sequence& seq);
			void operator>>(SeqList& list);
			void open(const char *path, sushort type = 0xFFFF);
			void makeIndex();
			size_t count();
			const String path() const;
			void setIndex(const size_t idx);
			void init();
		};
	}
}

#endif
