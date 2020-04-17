#ifndef SLIB_EXCEPTION_H
#define SLIB_EXCEPTION_H

#include "sconfig.h"
#include "sbasic/string.h"

namespace slib {
    class SOBJ_DLL String;
    
    constexpr suint SLIB_NULL_ERROR = 0x0001;
    constexpr suint SLIB_CAST_ERROR = 0x0002;
    constexpr suint SLIB_RANGE_ERROR = 0x0003;
    constexpr suint SLIB_FORMAT_ERROR = 0x0004;
    constexpr suint SLIB_NOT_FOUND_ERROR = 0x0005;
    constexpr suint SLIB_READ_ERROR = 0x0006;
    constexpr suint SLIB_WRITE_ERROR = 0x0007;
    constexpr suint SLIB_EXEC_ERROR = 0x0008;
    constexpr suint SLIB_CONFLICT_ERROR = 0x0009;
	constexpr suint SLIB_CREATE_PROCESS_ERR = 0x000A;
	constexpr suint SLIB_CREATE_PIPE_ERR = 0x000B;
	constexpr suint SLIB_SHARED_MEMORY_ALLOC_ERR = 0x000C;

    
	#define TARGET_TEXT(T) u8"'"+String(T)+u8"'"
    #define CAST_TEXT(F, T) u8"< '"+String(F)+u8"' => '"+String(T)+u8">"
    #define RANGE_TEXT(B, E) u8"["+String(B)+u8", "+String(E)+u8")"
	#define EXEC_TEXT(E) u8"Return :"+String(E)
	#define CONFLICT_TEXT(A, B) u8"'"+String(A)+u8"' vs '"+String(B)+"'"
	
	#define ERR_INFO __FILE__,__LINE__,__func__
    
    class SOBJ_DLL SException {
    public:
        sint err, line;
        String prefix, file, function, message, description;
        
    public:
        SException(const char *f, sint l,  const char *func, sint e = 0, const char *target = nullptr, const char *note = nullptr);
        virtual ~SException();
        
        sint error();
        String toString() const;
        void print() const;    
	};
}

#endif
