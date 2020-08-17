#ifndef SLIB_SDICT_H
#define SLIB_SDICT_H

#include "sbasic/map.h"
#include "sobj/sarray.h"
#include "sobj/sobject.h"

namespace slib {
    class SLIB_DLL SString;
    class SLIB_DLL SDictionary;
    
    class SLIB_DLL SPair : public SObject, public kvpair<String, slib::SObjPtr> {
    public:
        SPair();
        SPair(const kvpair<String, slib::SObjPtr> &p);
        SPair(const SPair &p);
        ~SPair();
        
        SPair &operator=(const SPair &p);
        String getClass() const;
        String toString() const;
        SObject *clone() const;
    };
    
    class SLIB_DLL SDictionary : public SObject, public Map<String, slib::SObjPtr> {
    public:
        SDictionary();
        SDictionary(size_t size);
        SDictionary(std::initializer_list<std::pair<String, slib::SObjPtr>> li);
        SDictionary(std::initializer_list<kvpair<String, slib::SObjPtr>> li);
        SDictionary(const char *str, const char *sep = ",", const char *part = ":");
        SDictionary(const sindex &idx);
        SDictionary(const sattribute &attr);
        SDictionary(const sobj &obj);
		SDictionary(SDictionary &&dict);
        SDictionary(const SDictionary &dict);
        ~SDictionary();
        
        SDictionary &operator=(const SDictionary &dic);
		SDictionary& operator=(SDictionary &&dic);
        void load(const char *path);
        void save(const char *path);
        smap_iter<String, slib::SObjPtr> search(const char *que);
        SArray lump(const stringarray &keys);
        String getClass() const;
        String toString() const;
        SObject *clone() const;
        
        bool operator < (const sobj &obj) const;
        bool operator < (const SDictionary &dict) const;
        bool operator == (const sobj &obj) const;
        bool operator == (const SDictionary &dict) const;
    };
}

#endif
