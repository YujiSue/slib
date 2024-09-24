#ifndef SLIB_SARRAY_H
#define SLIB_SARRAY_H
#include "sbasic/array.h"
#include "sobj/sobjptr.h"
namespace slib {
    /**
    * @class SArray
    * \~english @brief Array of SObject pointers
    */
    class SLIB_DLL SArray : public SObject, public Array<SObjPtr> {
    public:
        SArray();
        SArray(const size_t size);
        SArray(const size_t size, const SObjPtr& o);
        SArray(::std::initializer_list<SObjPtr> li);
        SArray(const char *str, const char *sep = ",");
        SArray(const intarray &iarray);
        SArray(const stringarray &strarray);
        SArray(SArray&& array) noexcept;
        SArray(const SArray &array);
        ~SArray();
        SArray &operator = (const SArray &array);
        void load(const char *path);
        void save(const char *path);
        size_t search(const SObjPtr& que, size_t offset = 0) const;
        size_t findWithKey(const SObjPtr& que, const char* key) const;
        SArray arrayOfKey(const char* key) const;
        void sortByKey(const char* key, ORDER order = ORDER::ASC);
        void sortBy(const char *key, ORDER order = ORDER::ASC);
        String getClass() const;
        String toString(const char* format = nullptr) const;
        SObject *clone() const;
        bool operator < (const SArray &array) const;
        bool operator == (const SArray &array) const;
    };
}

#endif
