#ifndef SLIB_STABLE_H
#define SLIB_STABLE_H

#include "sobj/sobject.h"
#include "sobj/sstring.h"
#include "sobj/sarray.h"
#include "smath/smath.h"

namespace slib {
    using namespace smath;
    
    class SOBJ_DLL STable;
    class SOBJ_DLL SColumn : public SArray {
        friend STable;
    public:
        static const sushort NUMBER_COLUMN = 0x0010;
        static const sushort INTEGER_COLUMN = 0x0011;
        static const sushort REAL_COLUMN = 0x0012;
        static const sushort BOOL_COLUMN = 0x0014;
        static const sushort STRING_COLUMN = 0x0020;
        static const sushort TEXT_COLUMN = 0x0021;
        static const sushort DATE_COLUMN = 0x0040;
        static const sushort DATA_COLUMN = 0x0080;
        static const sushort ARRAY_COLUMN = 0x0100;
        static const sushort DICT_COLUMN = 0x0200;
        static const sushort FUNC_COLUMN = 0x0400;
        static const sushort IMG_COLUMN = 0x0800;
        
    private:
        sushort _type;
        String _name;
        STable *_table;
        
    public:
        SColumn();
        SColumn(int type, const char *name = nullptr);
        SColumn(int type, const char *name, const SArray &array);
        SColumn(int type, const char *name, SArray &&array);
        SColumn(SColumn *col, STable *table);
        SColumn(const SColumn &column);
        ~SColumn();
        
        static sushort colType(const sobj &obj);
        static String colTypeStr(int t);
        static sushort colTypeIndex(const char *t);
        
		sushort type() const;
        const String &name() const;
        
        void setType(sushort t);
        void setName(const char *n);
        void setTable(STable *tbl);
        
        String getClass() const;
        String toString() const;
        SObject *clone() const;
    };
    
    class SOBJ_DLL STable : public SObject {
        friend SColumn;

    protected:
        String _name;
        SArray _columns, _rows;
        
	private:
		void _addRow(const SArray& array);
		void _addRow(const stringarray& array);
		void _addRow(const SDictionary& dict);

    public:
        STable();
        STable(const char *name, const Array<scolumn> &cols = {}, const SArray &rows = {});
        STable(const smat<sobj> &mat);
        STable(const sobj &obj);
        STable(const STable &table);
        ~STable();
        
        STable &operator=(const STable &table);
        
        //IO
        void load(const SDictionary &dict);
        void load(const char *path);
        void loadTxt(const char *path, const char *sep, bool header);
        
        void save(const SDictionary &param);
        void save(const char *path);
        void saveTxt(const char *path, const char *sep);
        
		String& name();
        const String &name() const;
        void setName(const char *name);
        
        size_t columnCount() const;
        bool hasColumn(const char *name) const;
        size_t columnIndex(const char *name) const;
        SColumn &column(int idx);
		const SColumn& column(int idx) const;
		SColumn& column(const char *name);
		const SColumn& column(const char* name) const;
		SArray &columns();
        const SArray &columns() const;
        
        void addColumn(const scolumn&col);
        void insertColumn(size_t idx, const scolumn&col);
        void setColumn(size_t idx, const scolumn&col);
        void removeColumn(size_t idx);
        void removeColumns(size_t off, size_t len);
        void removeColumns(const srange &range);
        void clearColumns();
        void resizeColumn(size_t s);
        void sortBy(size_t idx, slib::ORDER order = ASC);
        
        size_t rowCount() const;
        sobj &operator[](int idx);
        const sobj &operator[](int idx) const;
        sobj &at(int idx);
        const sobj &at(int idx) const;
        SArray &row(int idx);
        const SArray &row(int idx) const;
		SArray& rows();
        const SArray &rows() const;
        sdict namedRow(int idx) const;
        
        //void addRow();
		void addRow(const sobj& obj = snull);
        void insertRow(size_t idx, const sobj &obj = snull);
        
        void setRow(size_t idx, const SArray &array);
        void setRow(size_t idx, const stringarray &array);
        void setRow(size_t idx, const SDictionary &dict);
        void setRow(size_t idx, const sobj &obj);
        
        void removeRow(int idx);
        void removeRows(size_t off, size_t len);
        void removeRows(const srange &range);
        void clearRows();
        void swapRow(size_t i1, size_t i2);
        
        void resizeRows(size_t s);
        sobj &value(int row, int col);
        const sobj &value(int row, int col) const;
        void setValue(int row, int col, const sobj &obj);
        
        void clear();
        
        std::pair<size_t, size_t> find(const sobj &obj, smath::DIRECTION dir = HORIZONTAL, size_t offset = 0);
        smat<sobj> toMatrix();
        
        String getClass() const;
        String toString() const;
        SObject *clone() const;
    };
}

#endif