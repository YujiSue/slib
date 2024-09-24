#ifndef SLIB_STABLE_H
#define SLIB_STABLE_H
#include "smath/numeric.h"
#include "sobj/sobjptr.h"
#include "sobj/sdate.h"
#include "sobj/sdata.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"
namespace slib {
	class SLIB_DLL STable;
	class SLIB_DLL SColumn {
		friend STable;
	protected:
		STable* _table;
	public:
		String name;
		SDictionary attribute;
	protected:
		void setTable(STable* tbl);
	public:
		SColumn(const char* name = nullptr, const SDictionary& attr = {});
		SColumn(const SColumn& column);
		~SColumn();
		SColumn& operator=(const SColumn& col);
		
		size_t size() const;
		bool empty() const;
		SObjPtr& at(const int i);
		const SObjPtr& at(const int i) const;
		SObjPtr& operator[](const int i);
		const SObjPtr& operator[](const int i) const;
		void clear(const size_t off = 0, const size_t sz = -1);
		SObjPtr toObj() const;
		//String toString(const char *f = nullptr) const;
	};
	class SLIB_DLL RowIterator;
	class SLIB_DLL RowCIterator;
	class SLIB_DLL SRow {
		friend RowIterator;
		friend RowCIterator;
		friend STable;
	protected:
		STable* _table;
		SObjPtr* _values;
	protected:
		void setTable(STable* tbl);
	public:
		SRow();
		SRow(SObjPtr *r, STable *t);
		SRow(const SRow& row);
		~SRow();
		SRow& operator=(const SRow& col);
		const SArray& values() const;
		size_t size() const;
		bool empty() const;
		SObjPtr& at(const int i);
		const SObjPtr& at(const int i) const;
		SObjPtr& operator[](const int i);
		const SObjPtr& operator[](const int i) const;
		SObjPtr& operator[](const String& s);
		const SObjPtr& operator[](const String& s) const;
		bool hasColumn(const String& s) const;
		SIterator begin();
		SCIterator begin() const;
		SIterator end();
		SCIterator end() const;
		SObjPtr toObj() const;
	};
	class SLIB_DLL RowIterator {
		typedef std::random_access_iterator_tag iterator_category;
		typedef SRow value_type;
		typedef std::ptrdiff_t difference_type;
		typedef SRow* pointer;
		typedef SRow& reference;
	private:
		SRow _row;
	public:
		RowIterator();
		RowIterator(ArrayIterator<SObjPtr> it, STable *tbl);
		RowIterator(const RowIterator& it);
		~RowIterator();
		RowIterator& operator=(const RowIterator& it);
		reference operator*();
		pointer operator->();
		reference operator[](std::ptrdiff_t diff);
		RowIterator& operator++();
		RowIterator operator++(int);
		RowIterator& operator--();
		RowIterator operator--(int);
		RowIterator& operator+=(std::ptrdiff_t diff);
		RowIterator& operator-=(std::ptrdiff_t diff);
		RowIterator operator+(std::ptrdiff_t diff);
		RowIterator operator-(std::ptrdiff_t diff);
		difference_type operator-(const RowIterator it) const;
		void swap(RowIterator it1, RowIterator it2);
		bool operator<(const RowIterator& it) const;
		bool operator<=(const RowIterator& it) const;
		bool operator>(const RowIterator& it) const;
		bool operator>=(const RowIterator& it) const;
		bool operator==(const RowIterator& it) const;
		bool operator!=(const RowIterator& it) const;
	};
	class SLIB_DLL RowCIterator {
		typedef std::random_access_iterator_tag iterator_category;
		typedef const SRow value_type;
		typedef std::ptrdiff_t difference_type;
		typedef const SRow* pointer;
		typedef const SRow& reference;
	private:
		const SRow _row;
	public:
		RowCIterator();
		RowCIterator(const SObjPtr* obj, const STable* tbl);
		RowCIterator(const RowCIterator& it);
		~RowCIterator() {}
		RowCIterator& operator=(const RowCIterator& it);
		reference operator*();
		pointer operator->();
		reference operator[](std::ptrdiff_t diff);
		RowCIterator& operator++();
		RowCIterator operator++(int);
		RowCIterator& operator--();
		RowCIterator operator--(int);
		RowCIterator& operator+=(std::ptrdiff_t diff);
		RowCIterator& operator-=(std::ptrdiff_t diff);
		RowCIterator operator+(std::ptrdiff_t diff);
		RowCIterator operator-(std::ptrdiff_t diff);
		difference_type operator-(const RowCIterator it) const;
		void swap(RowCIterator it1, RowCIterator it2);
		bool operator<(const RowCIterator& it) const;
		bool operator<=(const RowCIterator& it) const;
		bool operator>(const RowCIterator& it) const;
		bool operator>=(const RowCIterator& it) const;
		bool operator==(const RowCIterator& it) const;
		bool operator!=(const RowCIterator& it) const;
	};

	class SLIB_DLL STable : public SObject {
		friend SRow;
		friend SColumn;

	protected:
		Array<SColumn> _columns;
		SArray _rows;
	public:
		STable();
		STable(const size_t row, const size_t col);
		STable(const Array<SColumn>& cols, const SArray& rows = {});
		STable(const sobj& obj);
		STable(const STable& table);
		~STable();

		STable& operator=(const STable& table);
		//IO
		/*
		void load(sobj obj);
		void load(const char* path);
		void loadTxt(const char* path, const char* sep, bool header);
		void loadJson(const char* path);

		void save(sobj obj);
		void save(const char* path);
		void saveTxt(const char* path, const char* sep);
		void saveJson(const char* path);
		*/

		size_t ncol() const;
		size_t nrow() const;
		const Array<SColumn>& columns() const;
		const SArray& rows() const;

		void resize(const size_t rn, const size_t cn);
		size_t colIndex(const char* name) const;
		SRow operator[](const int idx);
		SColumn& operator[](const char* name);
		SColumn& operator[](const String &name);
		SRow row(const int idx);
		const SRow row(const int idx) const;
		SColumn& column(const int idx);
		const SColumn& column(const int idx) const;

		RowIterator begin();
		RowCIterator begin() const;
		RowIterator end(); 
		RowCIterator end() const;

		void resizeColumn(const size_t sz);
		STable& addColumn(const SColumn& col);
		STable& appendColumn(const Array<SColumn>& cols);
		void insertColumn(const sinteger i, const SColumn& col);
		void removeColumn(const sinteger i);
		void resizeRow(const size_t sz);
		STable &addRow(SObjPtr obj = snull);
		//STable& addRow(const SArray& row);
		//STable& addRow(const SDictionary& row);
		void insertRow(const sinteger i, const SArray& row);
		void removeRow(const sinteger i);
		void append(const Array<SColumn>& cols);
		void append(const SArray& rows);
		STable& set(const sinteger row, const sinteger col, const SObjPtr& val);

		SObjPtr toObj() const;

		void clear();

		/*


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
		//void resize(size_t r, size_t c);
		Range<size_t> find(const sobj& obj, smath::DIRECTION dir = HORIZONTAL, srange offset = srange(0, 0));
		void sortBy(const char *name, slib::ORDER order = ASC);
		*/
		void clearAll();

		String getClass() const;
		String toString(const char *format = nullptr) const;
		SObject* clone() const;
	};
	extern SLIB_DLL String toString(const SRow& row, const char *format = nullptr);
	extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const SRow& row);
	extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const STable& tbl);
}

#endif
