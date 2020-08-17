#include "sobj/sobject.h"

namespace slib {

	class SLIB_DLL SVoid : public SObject {
	protected:
		void* _ptr;

	public:
		SVoid();
		SVoid(void* p);
		SVoid(const SVoid& ptr);
		~SVoid();
		SVoid& operator = (const SVoid& ptr);
		template<class Cls>
		Cls* cast() { return static_cast<Cls*>(_ptr); }
		template<class Cls>
		const Cls* cast() const { return static_cast<const Cls*>(_ptr); }
		template<class Cls>
		Cls& ref() { return *static_cast<Cls*>(_ptr); }
		template<class Cls>
		const Cls& ref() const { return *static_cast<const Cls*>(_ptr); }
		String getClass() const;
		String toString() const;
		SObject* clone() const;

		bool operator < (const SVoid& ptr) const;
		bool operator == (const SVoid& ptr) const;
	};
}