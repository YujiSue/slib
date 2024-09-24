#ifndef SLIB_MEMORY_H
#define SLIB_MEMORY_H
#include "sconfig.h"
namespace slib {
	/**
	* @class Memory
	*\~english @brief Set of static functions for memory handling.
	*\~japanese @brief  メモリ操作に関わる静的関数群をまとめたクラス
	*/
	template<typename T>
	class Memory {
	public:
		static inline T* alloc(const size_t sz) { return (T*)malloc(sz * sizeof(T)); }
		static inline T* reallocate(T *ptr, const size_t sz) { return (T*)realloc((void*)ptr, sz * sizeof(T)); }
		static inline void dealloc(T* ptr) { free(ptr); }
		static inline void init(T* ptr) { new(ptr) T(); }
		static inline void init(T* ptr, const T& val) { new(ptr) T(val); }
		static inline void init(T* ptr, T&& val) noexcept { new(ptr) T(std::forward<T&&>(val)); }
		template<class... Args>
		static inline void init(T* ptr, Args... args) { new(ptr) T(args...); }
		static inline void assign(T* ptr, const T& val) { *ptr = val; }
		static inline void assign(T* ptr, T&& val) noexcept { *ptr = std::forward<T&&>(val); }
		static inline void destroy(T* ptr) { ptr->~T(); }
		static inline void clear(T* ptr) { memset(ptr, 0, sizeof(T)); }
		static inline void copy(T* dest, const T* src, const size_t sz) {
			for(size_t i = 0; i < sz; ++i) { *dest = *src; ++dest; ++src; }
		}
		static inline void move(T** dest, T** src) { *dest = *src; *src = nullptr; }
		static inline void shift(T* from, T* to, const size_t sz) {
			if (from < to) {
				auto src = from + sz - 1, dest = to + sz - 1;
				for(size_t i = 0; i < sz; ++i) { *dest = *src; --dest; --src; }
			}
			else if (to < from) {
				for(size_t i = 0; i < sz; ++i) { *to = *from; ++to; ++from; }
			}
		}
		static inline void swap(T* from, T* to) {
			auto tmp = *from;
			*from = *to;
			*to = tmp;
		}
	};
}
#endif