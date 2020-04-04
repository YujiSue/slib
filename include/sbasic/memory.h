#ifndef SLIB_MEMORY_H
#define SLIB_MEMORY_H

#include "sconfig.h"

namespace slib {

    template<typename T>
    struct Memory {
        static T *alloc(size_t size);
        static void dealloc(T *data);
        static void init(T * data, size_t size);
        static void release(T *data, size_t size);
		static void assign(T* data, const T &val);
        static void copy(T *dest, const T *src, size_t size);
        static void shift(T *dest, T *src, size_t size);
    };
    //classic
    template<typename T>
    struct CMemory : public Memory<T> {
        static void init(T *data, size_t size);
    };
    //not necessary to release
    template<typename T>
    struct RMemory : public Memory<T> {
        static void init(T *data, size_t size);
        static void copy(T *dest, const T *src, size_t size);
        static void shift(T *dest, T *src, size_t size);
    };
    //uncopyable
	template<typename T>
	struct UMemory : public Memory<T> {
		static void init(T* data, size_t size);
		static void release(T* data, size_t size);
		static void assign(T* data, const T& val);
		static void copy(T* dest, const T* src, size_t size);
		static void shift(T* dest, T* src, size_t size);
	};
	//general
    template<typename T>
    struct SMemory : public Memory<T> {
        static void init(T *data, size_t size);
        static void release(T *data, size_t size);
        static void copy(T *dest, const T *src, size_t size);
        static void shift(T *dest, T *src, size_t size);
    };

    /*============================================================*/
    template<typename T>
	T* Memory<T>::alloc(size_t size) { return (T *)malloc(size*sizeof(T)); }
    template<typename T>
    void Memory<T>::dealloc(T *data) { if (data) free(data); }
    template<typename T>
    void Memory<T>::init(T * data, size_t size) {}
    template<typename T>
    void Memory<T>::release(T *data, size_t size) {}
	template<typename T>
	void Memory<T>::assign(T* data, const T& val) { *data = val; }
    template<typename T>
    void Memory<T>::copy(T *dest, const T *src, size_t size) { memcpy(dest, src, size*sizeof(T)); }
    template<typename T>
    void Memory<T>::shift(T *dest, T *src, size_t size) { memmove(dest, src, size*sizeof(T)); }
    template<typename T>
    void CMemory<T>::init(T *data, size_t size) { memset(data, 0, size * sizeof(T)); }
    template<typename T>
    void RMemory<T>::init(T *data, size_t size) { sforin(i, 0, size) { new(data) T(); ++data; } }
    template<typename T>
    void RMemory<T>::copy(T *dest, const T *src, size_t size) {
        sforin(i, 0, size) { *dest = *src; ++dest; ++src; }
    }
    template<typename T>
    void RMemory<T>::shift(T *dest, T *src, size_t size) {
        if (dest < src) sforin(i, 0, size) { *dest = *src; ++dest; ++src; }
        else { dest += size-1; src += size-1; sforin(i, 0, size) { *dest = *src; --dest; --src; } }
    }
	template<typename T>
	void UMemory<T>::init(T* data, size_t size) { sforin(i, 0, size) { new(data) T(); ++data; } }
	template<typename T>
	void UMemory<T>::release(T* data, size_t size) { sforin(i, 0, size) { data->~T(); ++data; } }
	template<typename T>
	void UMemory<T>::assign(T* data, const T& val) {}
	template<typename T>
	void UMemory<T>::copy(T* dest, const T* src, size_t size) {}
	template<typename T>
	void UMemory<T>::shift(T* dest, T* src, size_t size) {}
    template<typename T>
    void SMemory<T>::init(T *data, size_t size) { sforin(i, 0, size) { new(data) T(); ++data; } }
    template<typename T>
    void SMemory<T>::release(T *data, size_t size) { sforin(i, 0, size) { data->~T(); ++data; } }
    template<typename T>
    void SMemory<T>::copy(T *dest, const T *src, size_t size) {
        sforin(i, 0, size) { new(dest) T(*src); ++dest; ++src; }
    }
    template<typename T>
    void SMemory<T>::shift(T *dest, T *src, size_t size) {
        if (dest < src) sforin(i, 0, size) {
            new(dest) T(*src); src->~T(); new(src) T(); ++dest; ++src;
        }
        else { dest += size-1; src += size-1; sforin(i, 0, size) {
            new(dest) T(*src); src->~T(); new(src) T(); --dest; --src; }
        }
    }
}

#endif
