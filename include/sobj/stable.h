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
		sobj& at(sinteger i);
		const sobj& at(sinteger i) const;
		sobj& operator[](sinteger i);
		const sobj& operator[](sinteger i) const;
		sobj get(sinteger i) const;
		SArray get(sinteger r, sinteger h) const;
		void convert(sushort t);
		void setName(const char* n);
		void set(sinteger i, sobj v) const;
		void set(sinteger r, sinteger h, sobj v) const;
		void clear(sinteger i) const;
		void clear(sinteger r, sinteger h) const;
		
		String getClass() const;
		String toString() const;
		SObject* clone() const;
	};
	class SLIB_DLL SRow : public SObject, public Array<SObjPtr> {
		friend STable;
	private:
		STable* _table;

	private:
		void setTable(STable* tbl);
	public:
		SRow();
		SRow(size_t size);
		SRow(std::initializer_list<SObjPtr> li);
		SRow(const stringarray& strarray);
		SRow(const sobj& obj);
		SRow(SRow&& row) noexcept;
		SRow(const SRow& row);
		~SRow();

		SObjPtr& operator[](const char* s);
		const SObjPtr& operator[](const char* s) const;
		sobj get(sinteger i) const;
		SArray get(sinteger c, sinteger w) const;
		void set(sinteger i, sobj v);
		void set(sinteger c, sinteger w, sobj v);
		void clear(sinteger i);
		void clear(sinteger c, sinteger w);

		String getClass() const;
		String toString() const;
		SObject* clone() const;
	};
	class SLIB_DLL STable : public SObject {
		friend SColumn;
		friend SRow;
	private:
		sint _lastcol;

	protected:
		SArray _columns, _rows;

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
		void loadJson(const char* path);

		void save(sobj obj);
		void save(const char* path);
		void saveTxt(const char* path, const char* sep);
		void saveJson(const char* path);


		size_t columnCount() const;
		bool hasColumn(const char* name) const;
		size_t columnIndex(const char* name) const;
		SColumn& operator[](const char *name);
		const SColumn& operator[](const char *name) const;
		SColumn& columnAt(sinteger idx);
		const SColumn& columnAt(sinteger idx) const;
		SColumn& column(const char* name);
		const SColumn& column(const char* name) const;
		SArray& columns();
		const SArray& columns() const;
		void addColumn(const char *s = nullptr);
		void addColumn(const SColumn& col);
		void addColumns(const Array<SColumn>& cols);
		void insertColumn(sinteger idx, const SColumn& col);
		void setColumn(sinteger idx, const SColumn& col);
		void removeColumn(sinteger idx);
		void removeColumns(size_t off, size_t len);
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
