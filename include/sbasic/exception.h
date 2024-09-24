#ifndef SLIB_EXCEPTION_H
#define SLIB_EXCEPTION_H
#include "sconfig.h"
#include "sbasic/style.h"
#include "sbasic/string.h"
namespace slib {
    constexpr int NULL_ERROR = 0x0001;
    constexpr int OVERFLOW_ERROR = 0x0002;
    constexpr int RANGE_ERROR = 0x0003;
    constexpr int CAST_ERROR = 0x0004;
    constexpr int FORMAT_ERROR = 0x0005;
    constexpr int UNDEF_ERROR = 0x0006;
    constexpr int NOT_FOUND_ERROR = 0x0007;
    constexpr int NOT_COMPARABLE_ERROR = 0x0008;
    constexpr int PROC_FAILED_ERROR = 0x0009;
    constexpr int MEM_ALLOC_ERROR = 0x000A;
    constexpr int MEM_REALLOC_ERROR = 0x000B;

    /**
    * @cond
    */
	#define ERR_INFO slib::String("@")<<__func__<<" l. "<<__LINE__<<" in '"<<__FILE__<<"'."
    /**
    * @endcond
    */
    /**
    * @class Exception
    * \~english @brief Exception class 
    * \~japanese @brief 例外クラス
    */
    class SLIB_DLL Exception {
    public:
        /**
        * \~english @brief Error code.
        */
        int code;
        /**
        * \~english @brief Error code.
        */
        String prefix;
        /**
        * \~english @brief Error message.
        */
        String message;
        /**
        * \~english @brief Error description.
        */
        String description;

    public:
        Exception();
        Exception(int i, const char* msg = nullptr, const char* desc = nullptr);
        ~Exception();
        /**
        * \~english @brief Print error message.
        */
        virtual void print() const;
    };
    /**
    * @cond
    */
    extern SLIB_DLL String nullErrorText(const char* var);
    extern SLIB_DLL String overflowErrorText(const char *var, const size_t max);
    extern SLIB_DLL String outRangeErrorText(const char* var, const int current, const int min, const int max);
    extern SLIB_DLL String castErrorText(const char *ori, const char *dest);
    extern SLIB_DLL String formatErrorText(const char* var, const char* current, const char* form);
    extern SLIB_DLL String undefErrorText(const char* var, const char* current);
    extern SLIB_DLL String nofoundErrorText(const char *query, const char *src);
    extern SLIB_DLL String nocompErrorText(const char* var);
    extern SLIB_DLL String procErrorText(const char* var);


#define NullException(X) slib::Exception(slib::NULL_ERROR, "Null value error.",ERR_INFO<<slib::NL<<X)
#define OverFlowException(X) slib::Exception(slib::OVERFLOW_ERROR, "Capacity overflow error.",ERR_INFO<<slib::NL<<X)
#define RangeException(X) slib::Exception(slib::RANGE_ERROR, "Out of range error.",ERR_INFO<<slib::NL<<X)
#define CastException(X) slib::Exception(slib::CAST_ERROR, "Cast error.",ERR_INFO<<slib::NL<<X)
#define FormatException(X) slib::Exception(slib::FORMAT_ERROR, "Incorrect format error.",ERR_INFO<<slib::NL<<X)
#define UndefinedException(X) slib::Exception(slib::UNDEF_ERROR, "Not defined error.",ERR_INFO<<slib::NL<<X)
#define NotFoundException(X) slib::Exception(slib::NOT_FOUND_ERROR, "Not found error.",ERR_INFO<<slib::NL<<X)
#define ProcException(X) slib::Exception(slib::PROC_FAILED_ERROR, "Process failed error.",ERR_INFO<<slib::NL<<X)
#define NoComparableException(X) slib::Exception(slib::NOT_COMPARABLE_ERROR, "Not comparable error.",ERR_INFO<<slib::NL<<X)
#define MemAllocException() slib::Exception(slib::MEM_ALLOC_ERROR, "Memory allocation error.",ERR_INFO)
#define MemReAllocException() slib::Exception(slib::MEM_REALLOC_ERROR, "Memory reallocation error.",ERR_INFO)
    /**
    * @endcond
    */
}

#endif
