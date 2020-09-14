#ifndef SLIB_SARRAY_H
#define SLIB_SARRAY_H

#include "sobj/sobject.h"

namespace slib {    
    class SLIB_DLL SArray : public SObject, public Array<SObjPtr> {
    public:
        SArray();
        SArray(int size);
        SArray(size_t size);
        SArray(std::initializer_list<SObjPtr> li);
        SArray(const char *str, const char *sep = ",");
        SArray(const intarray &iarray);
        SArray(const stringarray &strarray);
        SArray(const sobj &obj);
		SArray(SArray&& array) noexcept;
        SArray(const SArray &array);
        ~SArray();
        SArray &operator = (const SArray &array);
        void load(const char *path);
        void save(const char *path);
        SArray subarray(size_t off, size_t len = -1) const;
        SArray subarray(sarr_citer<SObjPtr> beg, sarr_citer<SObjPtr> end) const;
        SArray subarray(srange range) const;
        size_t search(const char *que, size_t offset = 0) const;
        void sortby(const char *key, slib::ORDER order = ASC);
        String getClass() const;
        String toString() const;
        SObject *clone() const;
        
        bool operator < (const slib::SObjPtr&obj) const;
        bool operator < (const SArray &array) const;
        bool operator == (const slib::SObjPtr&obj) const;
        bool operator == (const SArray &array) const;
    };
}

#endif
