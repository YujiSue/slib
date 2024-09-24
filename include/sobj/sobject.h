#ifndef SLIB_SOBJECT_H
#define SLIB_SOBJECT_H
#include "sbasic/string.h"
namespace slib {
    class SLIB_DLL SObjPtr;
    class SLIB_DLL SObject {
        friend SObjPtr;
    protected:
        std::atomic<int> _scope;
    public:
        SObject();
        virtual ~SObject();
        void addScope();
        void removeScope();
        bool scoped() const;
        void swapScope(SObject &obj);
        virtual String getClass() const = 0;
        virtual String toString(const char* format = nullptr) const = 0;
        virtual SObject* clone() const = 0;
    };
    //template<class Cls>
    //extern inline String toString(const SObject& cls, const char* format = nullptr) {
    //    return cls.toString(format);
    //}
}
#endif