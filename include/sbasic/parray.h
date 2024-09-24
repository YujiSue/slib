#ifndef SLIB_PARRAY_H
#define SLIB_PARRAY_H

#include "sbasic/ptr.h"
#include "sbasic/array.h"

namespace slib {
	/**
	* @class PArray
	* \~english @brief Pointer array class
	* \~japanese @brief ポインター配列クラス
	*/
	template<typename T>
	class PArray {
		typedef std::function<bool(const SPointer<T>& p1, const SPointer<T>& p2)> Comparer;

	protected:
		Array<SPointer<T>> _array;

	public:
		PArray() {}
		PArray(const size_t sz) : PArray<T>() { _array.resize(sz); }
		PArray(const size_t sz, const T& v) : PArray<T>() { _array.resize(sz, v); }
		PArray(PArray<T>&& array) noexcept : PArray<T>() { _array.swap(array._array); }
		PArray(const PArray<T>& array) : PArray<T>() { _array = array._array; }
		~PArray() {}
		PArray<T>& operator=(PArray<T>&& array) noexcept { swap(array); return *this; }
		PArray<T>& operator=(const PArray<T>& array) {
			_array = array._array; return *this;
		}
		bool empty() const { return _array.empty(); }
		size_t size() const { return _array.size(); }
		size_t capacity() const { return _array.capacity(); }
		void resize(const size_t sz) { _array.resize(sz); }
		void resize(const size_t sz, const T& val) { _array.resize(sz, val); }
		void reserve(const size_t sz) { _array.reserve(sz); }
		PArrayIterator<T> iterAt(const int i) {
			auto it = (i < 0 ? _array.end() : _array.begin()) + i;
			if (it < _array.begin() || _array.end() < it) throw RangeException(outRangeErrorText("index", (int)(it - _array.begin()), 0, (int)_array.size()));
			return PArrayIterator<T>(it.ptr());
		}
		PArrayCIterator<T> iterAt(const int i) const {
			auto it = (i < 0 ? _array.end() : _array.begin()) + i;
			if (it < _array.begin() || _array.end() < it) throw RangeException(outRangeErrorText("index", (int)(it - _array.begin()), 0, (int)_array.size()));
			return PArrayCIterator<T>(it.ptr());
		}
		SPointer<T> element(const int i) const { return _array[i]; }
		//T* data(const int i = 0) { return iterAt(i).ptr(); }
		//const T* data(const int i = 0) const { return iterAt(i).ptr(); }
		T& at(const int i) { return *iterAt(i); }
		const T& at(const int i) const { return *iterAt(i); }
		T& operator[](const int i) { return at(i); }
		const T& operator[](const int i) const { return at(i); }
		SPointer<T> get(const int i) const { return *iterAt(i).ptr(); }
		PArrayIterator<T> begin() { return PArrayIterator<T>(_array.begin().ptr()); }
		PArrayCIterator<T> begin() const { return PArrayCIterator<T>(_array.begin().ptr()); }
		PArrayIterator<T> end() { return PArrayIterator<T>(_array.end().ptr()); }
		PArrayCIterator<T> end() const { return PArrayCIterator<T>(_array.end().ptr()); }
		/*
		PArray<T> subarray(const size_t off, const size_t sz = -1) const {
			PArray<T> sub(sz == -1 ? size() - off : sz);
			Memory<T>::copy(sub.data(), data((int)off), sub.size());
			return sub;
		}
		PArray<T> subarray(const Range<sint> range) const {
			auto beg = data(range.begin), end = data(range.end);
			auto sz = end - beg;
			Array<T> sub(sz);
			Memory<T>::copy(sub.data(), beg, sz);
			return sub;
		}
		*/
		void add(T* v) { _array.add(SPointer<T>(v)); }
		void add(SPointer<T> v) { _array.add(v); }
		void add(T&& v) noexcept { _array.add(SPointer<T>(new T(std::forward<T&&>(v)))); }
		void add(const T& v) { _array.add(SPointer<T>(new T(v))); }
		template<typename... Args>
		void add(Args... args) { _array.add(SPointer<T>(new T(args...))); }
		void append(std::initializer_list<T> li) {
			auto sz = _array.size();
			_array.resize(sz + li.size());
			auto dest = _array.begin() + sz;
			sfor(li) { *dest = $_; ++dest; }
		}
		void append(const PArray<T>& array) { _array.append(array._array); }
		void insert(const int i, T&& v) noexcept { _array.insert(i, new T(std::forward<T&&>(v))); }
		void insert(const int i, const T& v) { _array.insert(i, new T(v)); }
		void insert(const int i, SPointer<T> v) { _array.insert(i, v); }
		template<typename... Args>
		void insert(const int i, Args... args) { _array.insert(i, new T(args...)); }
		void insert(const int i, const PArray<T>& array) { _array.insert(i, array._array); }
		void removeAt(const int i) { _array.removeAt(i); }
		void remove(const size_t off, const size_t len = -1) { _array.remove(off, len); }
		void remove(const Range<int>& range) { _array.remove(range); }

		void erase(const T v) {
			auto idx = indexOf(v);
			if (idx != NOT_FOUND) removeAt((int)idx);
		}
		void shrink() {
			Array<T> tmp(size());
			sfor2(*this, tmp) { $_2 = $_1; }
			swap(tmp);
		}
		void clear() { _array.clear(); }
		void swap(PArray<T>& array) { _array.swap(array._array); }
		bool include(const T& val) const { return indexOf(val) != NOT_FOUND; }
		size_t count(const T& val) const {
			size_t c = 0;
			sfor(*this) { if ($_ == val) ++c; }
			return c;
		}
		size_t indexOf(const T& val, const size_t off = 0) const {

			auto it = begin() + off;
			while (it < end()) {
				if ($_ == val) break;
				$NEXT;
			}
			return it == end() ? NOT_FOUND : $INDEX(*this);
		}
		void sort(Comparer comp = [](const T& p1, const T& p2) { return *p1 < *p2; }) {
			std::sort(_array.begin(), _array.end(), [comp](const SPointer<T>& v1, const SPointer<T>& v2) { return comp(*v1, *v2); });
		}
		bool operator<(const PArray<T>& array) const {
			auto sz = size() < array.size() ? size() : array.size();
			auto it = std::make_pair(this->begin(), array.begin());
			sfor2(*this, array) {
				if ($_1 < $_2) return true;
			}
			return size() < array.size();
		}
		bool operator==(const PArray<T>& array) const {
			if (size() != array.size()) return false;
			sfor2(*this, array) {
				if ($_1 == $_2) continue;
				else return false;
			}
			return true;
		}
	};
	template<typename T>
	extern inline String toString(const PArray<T>& array, const char* sep = ",") {
		if (array.empty()) return "";
		String str;
		str << array[0];
		sforin(it, array.begin() + 1, array.end()) str << sep << $_;
		return str;
	}
}
template<typename T>
extern std::ostream& operator<<(std::ostream& os, const slib::PArray<T>& array) {
	if (array.empty()) return os;
	os << array[0];
	sforin(it, array.begin() + 1, array.end()) os << "," << $_;
	return os;
}
#endif
