#ifndef SLIB_SDATA_H
#define SLIB_SDATA_H

#include "sobj/sobject.h"

namespace slib {
    class SLIB_DLL SString;
	class SLIB_DLL SData : public ubytearray, public SObject {
	public:
		SData();
		SData(size_t size, void* dat = nullptr);
		template<typename T>
		SData(Array<T, CMemory<T>>& array) : SData(array.size()*sizeof(T), (void *)array.ptr()) {}
        SData(const char *s);
		SData(const slib::String &str);
		SData(const slib::SString& str);
        SData(const sobj &obj);
		SData(SData&& data) noexcept;
        SData(const SData &data);
        virtual ~SData();
		static SData decode(const char* b64);
		template<typename T>
		SData& operator = (const Array<T, CMemory<T>>& array) {
			ubytearray::resize(array.size() * sizeof(T));
			CMemory<subyte>::copy(ubytearray::_begin, reinterpret_cast<subyte*>(array._begin), ubytearray::size());
		}
        SData &operator = (const sobj &obj);
        SData &operator = (const SData &dat);
        void load(const char *path);
        void save(const char *path);
        void compress();
        void expand();
		void asString(String& str);
		template<typename T>
		void asArray(Array<T, CMemory<T>>& array) {
			if (ubytearray::size() % sizeof(T)) throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
			if (ubytearray::_capacity) {
				array._capacity = ubytearray::_capacity;
				array._begin = reinterpret_cast<T *>(ubytearray::_begin);
				array._end = array._begin + (ubytearray::size() / sizeof(T));
			}
			ubytearray::discard();
		}
		virtual String getClass() const;
        virtual String toString() const;
		virtual String toString(const char *f) const;
        virtual SObject *clone() const;
        bool operator < (const sobj &obj) const;
        bool operator < (const SData &data) const;
        bool operator == (const sobj &so) const;
        bool operator == (const SData &data) const;
    };
	extern inline std::istream& operator>>(std::istream& is, ubytearray& array) {
		is.seekg(0, std::ios::end);
		size_t size = is.tellg();
		is.clear();
		is.seekg(0);
		array.resize(size);
		is.read((char*)array.ptr(), size);
		return is;
	}
}
#endif
