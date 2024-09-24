#ifndef SLIB_SDATA_H
#define SLIB_SDATA_H
#include "sbasic/array.h"
#include "sobj/sobject.h"

namespace slib {
	/**
	* @class SData
	* \~english @brief 
	*/
	class SLIB_DLL SData : public ubytearray, public SObject {
	public:
		SData();
		SData(size_t size, void* dat = nullptr);
		template<typename T>
		SData(const Array<T>& array) : SObject() {
			ubytearray::resize(array.size() * sizeof(T));
			Memory<T>::copy(data(), reinterpret_cast<const subyte*>(array.data()), size());
		}
		template<typename T>
		SData(Array<T>&& array) noexcept : SObject() {
			ubytearray::swap(array);
		}
		SData(const ubytearray& bytes);
		//SData(const char *s);
		//SData(const slib::String &str, const char *);
		//SData(const slib::SString& str);
        //SData(const sobj &obj);
		SData(SData&& data) noexcept;
        SData(const SData &data);
        virtual ~SData();
		static SData decode(const char* b64);
		SData& operator = (const ubytearray& array);
		template<typename T>
		SData& operator = (const Array<T>& array) {
			ubytearray::resize(array.size() * sizeof(T));
			Memory<subyte>::copy(ubytearray::_ptr, reinterpret_cast<subyte*>(array._ptr), ubytearray::size());
			return *this;
		}
        //SData &operator = (const sobj &obj);
        SData &operator = (const SData &dat);
        //void load(const char *path);
        //void save(const char *path);
        //void compress();
        //void expand();
		void asString(String& str);
		template<typename T>
		T* as() { return reinterpret_cast<T*>(ubytearray::_ptr); }
		template<typename T>
		const T* as() const { return reinterpret_cast<T*>(ubytearray::_ptr); }
		//operator ubytearray& ();
		//operator const ubytearray& () const;
		bool operator < (const SData &data) const;
        bool operator == (const SData &data) const;

		String getClass() const;
		String toString(const char* format = nullptr) const;
		SObject* clone() const;
    };
	/**
	* @cond
	*/
	extern inline std::istream& operator>>(std::istream& is, ubytearray& array) {
		is.seekg(0, std::ios::end);
		size_t size = is.tellg();
		is.clear();
		is.seekg(0);
		array.resize(size);
		is.read((char*)array.data(), size);
		return is;
	}
	/**
	* @endcond
	*/
}
#endif
