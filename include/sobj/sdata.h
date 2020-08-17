#ifndef SLIB_SDATA_H
#define SLIB_SDATA_H

#include "sobj/sobject.h"

namespace slib {
    class SLIB_DLL SString;
    
    class SLIB_DLL SData : public ubytearray, public SObject {
    public:
        SData();
        SData(size_t size, void *dat = nullptr);
        SData(const char *str);
        SData(const String &str);
        SData(const SString &str);
        SData(const sobj &obj);
        SData(const SData &data);
        virtual ~SData();
        SData &operator = (const sobj &obj);
        SData &operator = (const SData &dat);
        void load(const char *path);
        void save(const char *path);
        template<class Checker>
        bool check(void *ref, size_t s) { Checker::check(ptr(), size(), ref, s); }
        void compress();
        void expand();
		void trans(String &str);
		virtual String getClass() const;
        virtual String toString() const;
        virtual SObject *clone() const;
        bool operator < (const sobj &obj) const;
        bool operator < (const SData &data) const;
        bool operator == (const sobj &so) const;
        bool operator == (const SData &data) const;
    };
}
#endif
