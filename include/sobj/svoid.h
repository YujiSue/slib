#include "sobj/sobject.h"

namespace slib {

	class SVoid : public SObject {
	protected:
		void* _ptr;

	public:
		SVoid();
		template<typename T>
		SVoid(T* ptr) : SVoid() { _ptr = (void*)ptr; }
		template<typename T, typename... Args>
		SVoid(Args... args) : SVoid(new T(args...)) {}
		SVoid(const SVoid& ptr);
		~SVoid();
		SVoid& operator = (const SVoid& ptr);
		template<class Cls>
		Cls* cast() { return dynamic_cast<Cls*>(_ptr); }
		template<class Cls>
		const Cls* cast() const { return dynamic_cast<const Cls*>(_ptr); }
		template<class Cls>
		Cls& ref() { return *dynamic_cast<Cls*>(_ptr); }
		template<class Cls>
		const Cls& ref() const { return *dynamic_cast<const Cls*>(_ptr); }

		String getClass() const;
		String toString() const;
		SObject* clone() const;

		bool operator < (const SVoid& ptr) const;
		bool operator == (const SVoid& ptr) const;
	};
}