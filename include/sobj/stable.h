#ifndef SLIB_STABLE_H
#define SLIB_STABLE_H

#include "sobj/sobject.h"
#include "sobj/sstring.h"
#include "sobj/sarray.h"
#include "sutil/ssearch.h"
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
	class SLIB_DLL SColumn : public SObject {
		friend STable;
	private:
		sushort _type;
		String _name;
		STable* _table;

	private:
		void setTable(STable* tbl);

	public:
		SColumn(int type = 0, const char* name = nullptr);
		SColumn(const char* name);
		SColumn(const SDictionary &dict);
		SColumn(const SColumn& column);
		~SColumn();
		SColumn& operator=(const SColumn& col);
		static sushort colType(const sobj& obj);
		static String colTypeStr(int t);
		static sushort colTypeIndex(const char* t);

		sushort type() const;
		const String& name() const;
		size_t size() const;
		bool empty() const;
		sobj& at(sint i);
		const sobj& at(sint i) const;
		sobj& operator[](sint i);
		const sobj& operator[](sint i) const;
		sobj getValue(sint i) const;
		SArray getValues(sint r, sint h) const;
		void clearValue(sint i) const;
		void clearValues(sint r, sint h) const;
		void convert(sushort t);
		void setName(const char* n);
		void setValue();
		void setValues();
		
		String getClass() const;
		String toString() const;
		SObject* clone() const;
	};

	class SLIB_DLL STable : public SObject {
		friend SColumn;
	private:
		sint _lastcol;

	protected:
		Array<SColumn> _columns;
		SArray _rows;

	public:
		STable();
		STable(int row, int col);
		STable(const Array<SColumn>& cols, const SArray& rows);
		STable(const sobj& obj);
		STable(const STable& table);
		~STable();

		STable& operator=(const STable& table);
		void initWithArray(SArray& array, bool header = false);
		void initWithDict(SDictionary& dict);

		//IO
		void load(sobj obj);
		void load(const char* path);
		void loadTxt(const char* path, const char* sep, bool header);

		void save(sobj obj);
		void save(const char* path);
		void saveTxt(const char* path, const char* sep);


		size_t columnCount() const;
		bool hasColumn(const char* name) const;
		size_t columnIndex(const char* name) const;
		SColumn& operator[](const char *name);
		const SColumn& operator[](const char *name) const;
		SColumn& columnAt(int idx);
		const SColumn& columnAt(int idx) const;
		SColumn& column(const char* name);
		const SColumn& column(const char* name) const;
		Array<SColumn>& columns();
		const Array<SColumn>& columns() const;
		void addColumn(const char *s = nullptr);
		void addColumn(const SColumn& col);
		void addColumns(const Array<SColumn>& cols);
		void insertColumn(size_t idx, const SColumn& col);
		void setColumn(size_t idx, const SColumn& col);
		void removeColumn(size_t idx);
		void removeColumns(size_t off, size_t len);
		void removeColumns(const srange& range);
		void swapColumns(size_t i1, size_t i2);
		void resizeColumn(size_t s);
		
		size_t rowCount() const;
		SArray& operator[](int idx);
		const SArray& operator[](int idx) const;
		SArray& rowAt(int idx);
		const SArray& rowAt(int idx) const;
		SArray& rows();
		const SArray& rows() const;
		void addRow();
		void addRow(const SArray& array);
		void addRows(const SArray& array);
		void insertRow(size_t idx, const SArray& array);
		void setRow(size_t idx, const SArray& array);
		void updateRow(size_t idx, const SDictionary& dict);
		void removeRow(int idx);
		void removeRows(size_t off, size_t len);
		void removeRows(const srange& range);
		void clearRows();
		void swapRows(size_t i1, size_t i2);
		void resizeRow(size_t s);

		void add(sobj obj, smath::DIRECTION dir);
		void insert(sint idx, sobj obj, smath::DIRECTION dir);
		void set(sint idx, sobj obj, smath::DIRECTION dir);
		sobj& at(sint r, sint c);
		const sobj& at(sint r, sint c) const;
		sobj getValue(sint r, sint c) const;
		SArray getValues(sint r, sint c, sint h, sint w) const;
		void clearValue(sint r, sint c);
		void clearValues(sint r, sint c, sint h, sint w);
		void setValue(sint r, sint c, sobj val);
		void updateValues(sint r, const SDictionary &values);
		void setValues(sint r, sint c, const SArray &values);
		void resize(size_t r, size_t c);
		Range<size_t> find(const sobj& obj, smath::DIRECTION dir = HORIZONTAL, srange offset = srange(0, 0));
		void sortBy(const char *name, slib::ORDER order = ASC);
		void clearAll();
		
		String getClass() const;
		String toString() const;
		String toString(const char *f) const;
		SObject* clone() const;
	};
}

#endif