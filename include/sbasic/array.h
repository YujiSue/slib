#ifndef SLIB_ARRAY_H
#define SLIB_ARRAY_H
#include "smath/mbasic.h"
#include "sbasic/memory.h"
#include "sbasic/range.h"
#include "sbasic/arrayiter.h"
#include "sbasic/exception.h"

namespace slib {
	/**
	* @class Array
	* \~english @brief Basic array class
	* \~japanese @brief 基本配列クラス
	*/
	template<typename T>
	class Array {
	public:
		typedef T Value; 
		typedef ArrayIterator<T> Iter;
		typedef ArrayCIterator<T> CIter;
		typedef std::function<bool(const T& t1, const T& t2)> Comparer;

	protected:
		T* _ptr, * _end;
		size_t _capacity;

	public:
		Array() : _ptr(nullptr), _end(nullptr), _capacity(0) {}
		Array(const size_t sz) : Array<T>() { resize(sz); }
		Array(const size_t sz, const T& v) : Array<T>() { resize(sz, v); }
		Array(std::initializer_list<T> li) : Array<T>(li.size()) { 
			if (li.size()) Memory<T>::copy(_ptr, li.begin(), li.size());
		}
		Array(Array<T>&& array) noexcept : Array() { swap(array); }
		Array(const Array<T>& array) : Array<T>(array.size()) { 
			if (array.size()) Memory<T>::copy(_ptr, array.data(), array.size()); 
		}
		~Array() { 
			clear(); 
			if (_ptr) { Memory<T>::dealloc(_ptr); } 
		}
		Array<T>& operator=(Array<T>&& array) noexcept { swap(array); return *this; }
		Array<T>& operator=(const Array<T>& array) {
			if (array.empty()) clear();
			else {
				resize(array.size());
				Memory<T>::copy(_ptr, array.data(), array.size());
			}
			return *this;
		}
		/**
		* \~english @brief Check whther this array is empty
		* \~english @return true if empty
		*
		* \~japanese @brief 空配列チェック
		* \~japanese @return 空配列なら true
		*/
		bool empty() const { return _end == _ptr; }

		size_t size() const { return _end - _ptr; }
		size_t capacity() const { return _capacity; }
		void resize(const size_t sz) {
			if (MAX_ARRAY_CAPACITY <= sz) 
				throw OverFlowException(overflowErrorText("Array size", MAX_ARRAY_CAPACITY - 1));
			auto s = size();
			if (sz < s) {
				sforin(i, sz, s) { Memory<T>::destroy(_end - 1); --_end; }
			}
			else if (s < sz) {
				if (_capacity <= sz) {
					if (!_capacity) reserve(sz + 1);
					else {
						auto cap = _capacity;
						while (cap <= sz) cap <<= 1;
						reserve(cap);
					}
				}
				sforin(i, s, sz) { Memory<T>::init(_end); ++_end; }
			}
		}
		void resize(const size_t sz, const T& val) {
			auto sz_ = size();
			if (sz < sz_) {
				sforin(i, sz, sz_) { Memory<T>::destroy(_end); --_end; }
			}
			else if (sz_ < sz) {
				if (_capacity <= sz) reserve(sz + 1);
				sforin(i, sz_, sz) { Memory<T>::init(_end, val); ++_end; }
			}
		}
		void reserve(const size_t sz) {
			if (MAX_ARRAY_CAPACITY <= sz) 
				throw OverFlowException(overflowErrorText("Array capacity", MAX_ARRAY_CAPACITY));
			if (sz <= _capacity) return;
			if (!_capacity) {
				_ptr = Memory<T>::alloc(sz); 
				if (!_ptr) throw MemAllocException();
				_end = _ptr; _capacity = sz;
			}
			else {
				auto s = size();
				while (_capacity <= sz) { _capacity <<= 1; }
				auto tmp = Memory<T>::alloc(_capacity);
				if (!tmp) throw MemAllocException();
				_end = tmp;
				auto p = _ptr;
				sforin(i, 0_u, s) {
					Memory<T>::init(_end, *p);
					Memory<T>::destroy(p);
					++_end; ++p;
				}
				Memory<T*>::swap(&tmp, &_ptr);
				Memory<T>::dealloc(tmp);
			}
		}
		ArrayIterator<T> iterAt(const int i) {
			auto p = (i < 0 ? _end : _ptr) + i;
			if (p < _ptr || _end < p) throw RangeException(outRangeErrorText("index", (int)(p - _ptr), 0, (int)(_end - _ptr)));
			return ArrayIterator<T>(p);
		}
		ArrayCIterator<T> iterAt(const int i) const {
			auto p = (i < 0 ? _end : _ptr) + i;
			if (p < _ptr || _end < p) throw RangeException(outRangeErrorText("index", (int)(p - _ptr), 0, (int)(_end - _ptr)));
			return ArrayCIterator<T>(p);
		}
		T* data(const int i = 0) { return iterAt(i).ptr(); }
		const T* data(const int i = 0) const { return iterAt(i).ptr(); }
		T& at(const int i) { return *iterAt(i); }
		const T& at(const int i) const { return *iterAt(i); }
		/**
		* \~english @brief Access to [i]th array element
		* \~japanese @brief i番目の要素へのアクセス
		* \~english @param[in]  i  Index to access
		* \~japanese @param[in]  i  配列要素のインデックス
		* \~english @return Reference to element
		* \~japanese @return 配列要素への参照
		*/
		T& operator[](const int i) { return at(i); }
		const T& operator[](const int i) const { return at(i); }
		T get(const int i) const { return T(at(i)); }
		ArrayIterator<T> begin() { return ArrayIterator<T>(_ptr); }
		ArrayCIterator<T> begin() const { return ArrayCIterator<T>(_ptr); }
		ArrayIterator<T> end() { return ArrayIterator<T>(_end); }
		ArrayCIterator<T> end() const { return ArrayCIterator<T>(_end); }

		//Subset<T, Iter> operator[](const Range<sinteger> range) { return Subset<T, Iter>(iterAt(range.begin), iterAt(range.end)); }
		//Subset<T, CIter> operator[](const Range<sinteger> range) const { return Subset<T, CIter>(iterAt(range.begin), iterAt(range.end)); }
		
		Array<T> subarray(const size_t off, const size_t sz = -1) const {
			Array<T> sub(sz == -1 ? size() - off : sz);
			Memory<T>::copy(sub.data(), data((int)off), sub.size());
			return sub;
		}
		Array<T> subarray(const Range<sint> range) const {
			auto beg = data(range.begin), end = data(range.end);
			auto sz = end - beg;
			Array<T> sub(sz);
			Memory<T>::copy(sub.data(), beg, sz);
			return sub;
		}
	private:
		inline void expand(const size_t sz) {
			auto c = _capacity ? _capacity : 8;
			while (c <= sz) { c <<= 1; }
			reserve(c);
		}
	public:
		void add(T&& v) noexcept { 
			if (_capacity <= size() + 1) expand(size() + 1);
			Memory<T>::init(_end, std::forward<T&&>(v));
			++_end;
		}
		void add(const T& v) { 
			if (_capacity <= size() + 1) expand(size() + 1);
			Memory<T>::init(_end, v);
			++_end;
		}
		template<typename... Args>
		void add(Args... args) { 
			if (_capacity <= size() + 1) expand(size() + 1);
			Memory<T>::init(_end, T(args...));
			++_end;
		}
		void append(std::initializer_list<T> li) {
			auto sz = li.size();
			if (_capacity <= size() + sz) expand(size() + sz);
			auto it = li.begin();
			sforin(i, 0, sz) {
				Memory<T>::init(_end, $_); ++_end; NEXT_($);
			}
		}
		void append(const Array<T>& array) {
			auto sz = array.size();
			if (_capacity <= size() + sz)  expand(size() + sz);
			auto it = array.begin();
			sforin(i, 0, (int)sz) {
				Memory<T>::init(_end, $_); ++_end; NEXT_($);
			}
		}
		void insert(const int i, T&& v) noexcept {
			auto sz = _end - data(i);
			resize(size() + 1);
			auto it = data(i);
			Memory<T>::shift($, $ + 1, sz);
			at(i) = std::forward<T&&>(v);
		}
		void insert(const int i, const T& v) {
			auto sz = _end - data(i);
			resize(size() + 1);
			auto it = data(i);
			Memory<T>::shift($, $ + 1, sz);
			at(i) = v;
		}
		template<typename... Args>
		void insert(const int i, Args... args) { insert(i, T(args...)); }
		void insert(const int i, const Array<T>& array) {
			size_t sz = _end - data(i), ins = array.size();
			resize(size() + array.size());
			Memory<T>::shift(data(i), data(i) + ins, sz);
			Memory<T>::copy(data(i), array.data(), ins);
		}
		void removeAt(const int i) {
			Memory<T>::shift(data(i + 1), data(i), size() - i - 1);
			resize(size() - 1);
		}
		void remove(const size_t off, const size_t len = -1) {
			auto sz = (len == (size_t)-1 ? size() - off : len);
			Memory<T>::shift(data((int)(off + sz)), data((int)off), size() - off - sz);
			resize(size() - sz);
		}
		void remove(const Range<int>& range) {
			auto beg = data(range.begin), end = data(range.end);
			auto sz = end - beg;
			Memory<T>::shift(end, beg, _end - end);
			resize(size() - sz);
		}
		void erase(const T v) {
			auto idx = indexOf(v);
			if (idx != NOT_FOUND) removeAt((int)idx);
		}
		void unique() {
			if (size() < 2) return;
			Array<T> tmp;
			tmp.reserve(size());
			sort();
			sfor(*this) {
				if (tmp.size() && $_ == tmp[-1]) continue;
				tmp.add($_);
			}
			swap(tmp);
		}
		void shrink() {
			Array<T> tmp(size());
			sfor2(*this, tmp) { $_2 = $_1; }
			swap(tmp);
		}
		void clear() { resize(0); }
		void swap(Array<T>& array) {
			Memory<T*>::swap(&_ptr, &array._ptr);
			Memory<T*>::swap(&_end, &array._end);
			Memory<size_t>::swap(&_capacity, &array._capacity);
		}
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
		void sort(Comparer comp = sortAsc<T>) {
			std::sort(begin(), end(), [comp](const T& v1, const T& v2) { return comp(v1, v2); });
		}
		bool operator<(const Array<T>& array) const {
			auto sz = size() < array.size() ? size() : array.size();
			auto it = std::make_pair(this->begin(), array.begin());
			sfor2(*this, array) {
				if ($_1 < $_2) return true;
			}
			return size() < array.size();
		}
		bool operator==(const Array<T>& array) const {
			if (size() != array.size()) return false;
			sfor2(*this, array) {
				if ($_1 == $_2) continue;
				else return false;
			}
			return true;
		}
	};
	template<typename T>
	extern inline String toString(const Array<T>& array, const char* sep = ",") {
		if (array.empty()) return "";
		String str;
		str << array[0];
		sforin(it, array.begin() + 1, array.end()) str << sep << $_;
		return str;
	}
}
template<typename T>
extern std::ostream& operator<<(std::ostream& os, const slib::Array<T>& arr) {
	if (arr.empty()) return os;
	os << arr[0];
	sforin(it, arr.begin() + 1, arr.end()) os << "," << $_;
	return os;
}
#endif
