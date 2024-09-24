#ifndef SBIO_VCF_H
#define SBIO_VCF_H
#include "smath/stat.h"
#include "sio/sfile.h"
#include "sio/stream.h"
#include "sbioinfo/seq.h"
namespace slib {
	namespace sbio {
#define VcfFormatException(X) slib::sbio::SBioInfoException(slib::FORMAT_ERROR, "VCF format error.", X)
		extern SLIB_DLL String vcfColErrorText(const char* s);
		extern SLIB_DLL String vcfColSizeErrorText(const char *s, int i);

		class SLIB_DLL Variant;
		class SLIB_DLL VarList;

		namespace svcf {
			constexpr char VERSION[5] = "v4.1";
			extern const sattribute ION_VAR_CALL;
			extern const stringarray HEADER_KEY;
			extern inline void vcf2Vars(stringarray& data, Array<Variant>& variants, const SDictionary& dict);
			extern inline int refindex(const char *name, const SDictionary& dict);
			extern inline void readInfo(slib::String& info, Array<Variant>& variants, const SDictionary& dict);
			extern inline void readFormat(slib::String& fkey, slib::String& format, Array<Variant>& variants, const SDictionary& dict);
			extern inline void setUniqueValue(Array<Variant>& variants, const SDictionary& attribute);
			extern inline void readVariant(slib::String& row, Array<Variant>& variants, const SDictionary& attribute);
			extern inline void getHeader(slib::String& row, SDictionary& attribute);
			extern inline void getSamples(slib::String& row, SDictionary& attribute);
			extern SLIB_DLL void defineSamples(const slib::sbio::VarList& list, IOStream& stream);
			extern SLIB_DLL void defineContigs(const slib::sbio::VarList& list, IOStream& stream);
			extern SLIB_DLL void defineAlt(const slib::sbio::VarList& list, IOStream& stream);
			extern SLIB_DLL void defineFilter(const slib::sbio::VarList& list, IOStream& stream);
			extern SLIB_DLL void defineInfo(const slib::sbio::VarList& list, IOStream& stream);
			extern SLIB_DLL void defineFormat(const slib::sbio::VarList& list, IOStream& stream);
			extern SLIB_DLL void writeHeader(const slib::sbio::VarList& list, IOStream& stream);
			extern SLIB_DLL void writeFilter(const slib::sbio::Variant& var, IOStream& stream);
			extern SLIB_DLL void writeInfo(const slib::sbio::Variant& var, const slib::sbio::SeqList& ref, IOStream& stream);
			extern SLIB_DLL void writeFormat(const slib::sbio::Variant& var, IOStream& stream);
			extern SLIB_DLL void writeVariant(const slib::sbio::Variant& var, const slib::sbio::SeqList& ref, IOStream& stream);
		}

		class SLIB_DLL VcfFile : public SFile {
			friend VarList;

			//SeqList* _reference;
			//int _refnum;
			//stringarray _refname;
			//intarray _reflength;

			VarList* _vlist;

			//SFile _file;
			String _ln, _title;
			Array<Variant> _buffer;
			//sattribute _valuemap;
		public:
			SDictionary header;

		public:
			VcfFile();
			VcfFile(VarList* vl);
			VcfFile(const char* path);
			~VcfFile();
			VcfFile& operator=(const VarList& vl);
			void operator >> (slib::sbio::VarList& vl);
			void open(const char* path);
			//void load(const char* s, SeqList* ref = nullptr);
			void save(const char* path);

			void readHeader();
			Variant* next();
		};
	}
}
#endif