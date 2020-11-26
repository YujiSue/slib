#ifndef SLIB_STABLE_H
#define SLIB_STABLE_H

#include "sobj/sobject.h"
#include "sobj/sstring.h"
#include "sobj/sarray.h"
#include "smath.h"

namespace slib {
	using namespace smath;

	constexpr sushort OBJECT_COLUMN = 0x0000;
	constexpr sushort NUMBER_COLUMN = 0x0010;
	constexpr sushort INTEGER_COLUMN = 0x0011;
	constexpr sushort REAL_COLUMN = 0x0012;
	constexpr sushort BOOL_COLUMN = 0x0014;
	constexpr sushort STRING_COLUMN = 0x0020;
	constexpr sushort TEXT_COLUMN = 0x0021;
	constexpr sushort DATE_COLUMN = 0x0040;
	constexpr sushort DATA_COLUMN = 0x0080;
	constexpr sushort ARRAY_COLUMN = 0x0100;
	constexpr sushort DICT_COLUMN = 0x0200;
	constexpr sushort FUNC_COLUMN = 0x0400;
	constexpr sushort IMG_COLUMN = 0x0800;

	class SLIB_DLL STable;
	class SLIB_DLL SColumn : public SArray {
		friend STable;
	private:
		sushort _type;
		String _name;
		STable* _table;

	public:
		SColumn(int type = 0, const char* name = nullptr, const SArray& array = {});
		SColumn(SArray&& array);
		SColumn(const SColumn& column);
		~SColumn();

		static sushort colType(const sobj& obj);
		static String colTypeStr(int t);
		static sushort colTypeIndex(const char* t);

		sushort type() const;
		const String& name() const;

		void convert(sushort t);
		void setName(const char* n);
		void setTable(STable* tbl);

		String getClass() const;
		String toString() const;
		SObject* clone() const;
	};

	class SLIB_DLL STable : public SObject {
		friend SColumn;

	protected:
		String _name;
		SArray _columns, _rows;

	public:
		STable();
		STable(int row, int col);
		STable(const Array<scolumn>& cols, const SArray& rows);
		STable(const smat<sobj>& mat);
		STable(const sobj& obj);
		STable(const STable& table);
		~STable();

		STable& operator=(const STable& table);

		//IO
		void load(sobj obj);
		void load(const char* path);
		void loadTxt(const char* path, const char* sep, bool header);

		void save(sobj obj);
		void save(const char* path);
		void saveTxt(const char* path, const char* sep);

		String& name();
		const String& name() const;
		void setName(const char* name);

		size_t columnCount() const;
		bool hasColumn(const char* name) const;
		size_t columnIndex(const char* name) const;
		SColumn& column(int idx);
		const SColumn& column(int idx) const;
		SColumn& column(const char* name);
		const SColumn& column(const char* name) const;
		SArray& columns();
		const SArray& columns() const;
		void addColumn(const SColumn& col);
		void addColumnDict(const SDictionary& dict);
		void addColumn(const sobj& obj = snull);
		void insertColumn(size_t idx, const SColumn& col);
		void insertColumnDicr(size_t idx, const SDictionary& dict);
		void insertColumn(size_t idx, const sobj& obj);
		void setColumn(size_t idx, const SColumn& col);
		void setColumnDict(size_t idx, const SDictionary& dict);
		void setColumn(size_t idx, const sobj& col);
		void removeColumn(size_t idx);
		void removeColumns(size_t off, size_t len);
		void removeColumns(const srange& range);
		void clearColumns();
		void resizeColumn(size_t s);
		void sortBy(size_t idx, slib::ORDER order = ASC);

		size_t rowCount() const;
		sobj& operator[](int idx);
		const sobj& operator[](int idx) const;
		sobj& at(int idx);
		const sobj& at(int idx) const;
		SArray& row(int idx);
		const SArray& row(int idx) const;
		SArray& rows();
		const SArray& rows() const;
		sobj namedRow(int idx) const;
		void addRowArray(const SArray& array);
		void addRowDict(const SDictionary& dict);
		void addRow(const sobj& obj = snull);
		void insertRowArray(size_t idx, const SArray& array);
		void insertRowDict(size_t idx, const SDictionary& dict);
		void insertRow(size_t idx, const sobj& obj = snull);
		void setRowArray(size_t idx, const SArray& array);
		void setRowDict(size_t idx, const SDictionary& dict);
		void setRow(size_t idx, const sobj& obj = snull);
		void removeRow(int idx);
		void removeRows(size_t off, size_t len);
		void removeRows(const srange& range);
		void clearRows();
		void swapRow(size_t i1, size_t i2);
		void resizeRow(size_t s);

		void resize(size_t r, size_t c);
		sobj& value(int row, int col);
		const sobj& value(int row, int col) const;
		void setValue(int row, int col, const sobj& obj);
		void clear();
		svec2d<size_t> find(const sobj& obj, smath::DIRECTION dir = HORIZONTAL, size_t offset = 0);
		void toMatrix(smat<sobj>& mat);

		/*
		sobj& at(sint r, sint c);
		sobj getValue(sint row, sint col);

		STable &where();
		STable &and();
		STable &or();


		SArray getValues(sint roff, sint coff, sint row, sint col);
		SArray getValues(const char *rque, const char *cque);

		void setValue(sint row, sint col, sobj val);
		void setValues(sint row, sint col, sobj val);

		sobj search();
		*/

		String getClass() const;
		String toString() const;
		SObject* clone() const;
	};
}

#endif