#ifndef SBIO_SBSEQIO_H
#define SBIO_SBSEQIO_H

#include "sobj.h"

namespace slib {
	namespace sbio {

		constexpr size_t FASTA_ROW_CHAR = 60;

		class SLIB_DLL SBioSeq;
		class SLIB_DLL SBSeqList;

		struct SLIB_DLL abidir {
			sint number;
			sshort element_type;
			sshort element_size;
			sint num_elements;
			sint data_size;
			sint data_offset;
			sint data_handle;

			abidir();
			abidir(const abidir& dir);
			~abidir();

			abidir& operator=(const abidir& dir);
			bool operator<(const abidir& dir) const;
			bool operator==(const abidir& dir) const;
		};

		class SLIB_DLL SBSeqIO {
			friend SBioSeq;
		public:
			SBSeqIO();
			~SBSeqIO();

			static sushort fileType(sio::SFile& file);
			static void loadTXT(sio::SFile& file, SBioSeq* seq);
			static void loadABI(sio::SFile& file, SBioSeq* seq);
			static void loadGBK(sio::SFile& file, SBioSeq* seq);
			static void loadFASTA(sushort type, sio::SFile& file, SBioSeq* seq);
			static void loadFASTA(sushort type, sio::SFile& file, SBSeqList* list);
			static void saveTXT(sio::SFile& file, SBioSeq* seq);
			static void saveGBK(sio::SFile& file, SBioSeq* seq);
			static void saveFASTA(sio::SFile& file, SBioSeq* seq);
			static void saveFASTA(sio::SFile& file, SBSeqList* list);
			static void makeIndex(sushort type, sio::SFile& file, SBSeqList* list);
		};
	}
}

#endif