#ifndef SBIO_BIUTIL_H
#define SBIO_BIUTIL_H
#include "sbasic/region.h"
#include "smath/matrix.h"
#include "sobj/sobjptr.h"
#include "sbioinfo/biconst.h"
#include "sbioinfo/biexception.h"
namespace slib {
	namespace sbio {
		class SLIB_DLL SeqList;
		class SLIB_DLL AnnotInfo;
		namespace sutil {
			extern SLIB_DLL void reverse(srange& range, const size_t sz);

			extern SLIB_DLL double rawVal(double v, double lim = snum::D_EPS);
			extern SLIB_DLL double phredVal(double v, double lim = snum::D_EPS);

			extern SLIB_DLL suint countBin(Map<sint, suint>& order, srange range);
			extern SLIB_DLL sushort getBin(srange range);
			extern SLIB_DLL void getBins(ushortarray& bins, srange range);
			extern SLIB_DLL void getBins(ushortarray& bins, const sregion& region);

			extern SLIB_DLL String geneType(sushort i);
			extern SLIB_DLL sushort geneTypeIdx(const char* s);
			extern SLIB_DLL sobj decodeGeneType(sushort i);
			extern SLIB_DLL sushort encodeGeneType(sobj obj);
			extern SLIB_DLL String transcriptType(sushort i);
			extern SLIB_DLL sushort transcriptTypeIdx(const char* s);
			extern SLIB_DLL sobj decodeTranscriptType(sushort i);
			extern SLIB_DLL sushort encodeTranscriptType(sobj obj);
			extern SLIB_DLL stringarray geneSite(sushort i);
			extern SLIB_DLL sushort geneSiteIdx(const char* s);
			extern SLIB_DLL sushort geneSiteIdx(const stringarray& s);
			extern SLIB_DLL sobj decodeGeneSite(sushort i);
			extern SLIB_DLL sushort encodeGeneSite(sobj obj);
			/*
			extern SLIB_DLL String varType(sushort i);
			extern SLIB_DLL stringarray varTypes(sushort i);
			extern SLIB_DLL sushort varTypeIdx(const char* s);
			extern SLIB_DLL sushort varTypeIdx(const stringarray& s);
			extern SLIB_DLL sobj decodeVarType(sushort i);
			extern SLIB_DLL sushort encodeVarType(sobj obj);
			*/
			extern SLIB_DLL stringarray mutType(sushort i);
			extern SLIB_DLL sushort mutTypeIdx(const char* s);
			extern SLIB_DLL sushort mutTypeIdx(const stringarray& s);
			extern SLIB_DLL sobj decodeMutType(sushort i);
			extern SLIB_DLL sushort encodeMutType(sobj obj);

			//extern SLIB_DLL String toString(const RefPos& p, SBioAnnotDB& db, const char* f = "ucsc");

		}
		class SLIB_DLL DirRange : public Range<sint> {
		public:
			bool dir;
		public:
			DirRange();
			DirRange(sint b, sint e, bool d = false);
			DirRange(const DirRange& r);
			~DirRange();

			DirRange& operator = (const DirRange& r);
			void init();
			bool operator < (const DirRange& r) const;
			bool operator == (const DirRange& r) const;
			bool operator != (const DirRange& r) const;
		};
		class SLIB_DLL RefPos : public DirRange {
		public:
			sint idx;
		public:
			RefPos();
			RefPos(sint i, sint b, sint e, bool d = false);
			RefPos(sint i, const DirRange& r);
			RefPos(const RefPos& p);
			~RefPos();
			RefPos& operator = (const RefPos& p);
			void init();
			String toString(const SeqList& ref) const;
			String toString(const Array<AnnotInfo>& chrs) const;
			bool operator < (const RefPos& p) const;
			bool operator == (const RefPos& p) const;
			bool operator != (const RefPos& p) const;
		public:
			static RefPos toPos(const char* s, const sindex& refindex, bool zerobase = false);
		};

		class SLIB_DLL DirRegion : public Array<DirRange> {
		public:
			DirRegion();
			DirRegion(const DirRegion& reg);
			~DirRegion();
			DirRegion& operator = (const DirRegion& reg);
			void init();
		};
		class SLIB_DLL RefRegion : public Array<DirRegion> {
		public:
			RefRegion();
			RefRegion(const RefRegion& reg);
			~RefRegion();

			RefRegion& operator = (const RefRegion& reg);
			bool operator < (const RefRegion& reg) const;
			bool operator == (const RefRegion& reg) const;
			bool operator != (const RefRegion& reg) const;
		};
	
		//
		using CODON_TABLE = smath::Matrix4D<smath::Vector4D<subyte>>;
		extern SLIB_DLL CODON_TABLE DEFAULT_CODON;
		extern SLIB_DLL CODON_TABLE DEFAULT_MT_CODON;
	}
	
	using sbpos = sbio::RefPos;
}

#endif 