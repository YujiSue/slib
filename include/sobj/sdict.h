#ifndef SLIB_SDICT_H
#define SLIB_SDICT_H
#include "sbasic/map.h"
#include "sobj/sobject.h"
#define D_(X,Y) slib::Pair<slib::String,slib::SObjPtr>((X),(Y))
namespace slib {
    /**
    * @class SDictionary
    * \~english @brief 
    */
    class SLIB_DLL SDictionary : public SObject, public Map<String, SObjPtr> {
    public:
        SDictionary();
        SDictionary(size_t size);
        SDictionary(std::initializer_list<Pair<String, SObjPtr>> li);
        SDictionary(const char *str, const char *sep = ",", const char *part = ":");
        SDictionary(const sindex &idx);
        SDictionary(const sattribute &attr);
        //SDictionary(const SObjPtr& obj);
		SDictionary(SDictionary &&dict) noexcept;
        SDictionary(const SDictionary &dict);
        ~SDictionary();
        
        SDictionary &operator=(const SDictionary &dic);
		SDictionary& operator=(SDictionary &&dic) noexcept;
        //void load(const char *path);
        //void save(const char *path);
        //smap_iter<String, slib::SObjPtr> search(const char *que);
		void combine(const SDictionary& dic);
		//void merge(const SDictionary& dic);
        //SArray search(const stringarray &keys);
        
        //bool operator < (const sobj &obj) const;
        bool operator < (const SDictionary &dict) const;
        //bool operator == (const sobj &obj) const;
        bool operator == (const SDictionary &dict) const;

        String getClass() const;
        String toString(const char* format = nullptr) const;
        SObject* clone() const;
    };
}
#endif
