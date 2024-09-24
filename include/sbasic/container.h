#ifndef SLIB_CONTAINER_H
#define SLIB_CONTAINER_H
#include "sbasic/memory.h"
namespace slib {
	/**
	* @cond
	*/
	constexpr size_t MAX_ARRAY_CAPACITY = 0x7FFFFFFF;
	constexpr int MAX_PRINT_ELEMENT_SIZE = 1000;
	constexpr int MAX_INSORT_SIZE = 0x10;
	
	template<typename T>
	class ArrayIterator;
	template<typename T>
	class ArrayCIterator;
	template<typename T>
	class CycleIterator;
	template<typename T>
	class CycleCIterator;

	template<typename T>
	class Array;
	template<typename T, size_t S>
	class FArray;
	template<typename T>
	class RArray;

	using boolarray = Array<bool>;
	using shortarray = Array<sshort>;
	using ushortarray = Array<sushort>;

	template<typename T>
	class ListIterator;
	template<typename T>
	class ListCIterator;
	template<typename T>
	class List;
	template<class Key, class Value>
	class MapIterator;
	template<class Key, class Value>
	class MapCIterator;
	template<class Key, class Value>
	class MapData;
	template<class Key, class Value>
	class Map;
	template<class Key, class Value>
	class TreeIterator;
	template<class Key, class Value>
	class TreeCIterator;
	template<class Key, class Value>
	class TreeData;
	template<class Key, class Value>
	class Tree;
	/**
	* @endcond
	*/
	template<class Ref>
	class Subset {
	protected:
		ArrayIterator<Ref> _begin, _end;
	public:
		Subset(ArrayIterator<Ref> b, ArrayIterator<Ref> e) : _begin(b), _end(e) {}
		Subset(ArrayCIterator<Ref> b, ArrayCIterator<Ref> e) : _begin(const_cast<Ref *>(b.ptr())), _end(const_cast<Ref*>(e.ptr())) {}
		~Subset() {}
		template <class Container>
		Subset& operator=(const Container& container) {
			sfor2(*this, container) { $_1 = $_2; }
			return *this;
		}
		Ref& operator[](const int i) { return *((i < 0 ?_end : _begin) + i); }
		size_t size() const { return _end - _begin; }
		ArrayIterator<Ref> begin() { return _begin; }
		ArrayIterator<Ref> end() { return _end; }
	};

}
/**
* @cond
*/
using chararray = slib::Array<char>;
using bytearray = slib::Array<int8_t>;
using ubytearray = slib::Array<uint8_t>;
using intarray = slib::Array<int32_t>;
using uintarray = slib::Array<uint32_t>;
using sizearray = slib::Array<size_t>;
using floatarray = slib::Array<float>;
using doublearray = slib::Array<double>;
using integerarray = slib::Array<int64_t>;
using uintegerarray = slib::Array<uint64_t>;
using ubytearray2d = slib::Array<ubytearray>;
using intarray2d = slib::Array<intarray>;
/**
* @endcond
*/

#endif
