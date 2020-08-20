#ifndef SLIB_CONFIG_H
#define SLIB_CONFIG_H

#define SLIB_VERSION 1.0.0

extern "C" {
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
}
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <queue>
#include <random>
#include <regex>
#include <utility>
#include <vector>
#ifndef _MANAGED
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#endif

#ifdef __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cmath>
#endif
#elif _WIN64
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS 1
#include <direct.h>
#include <io.h>
#include <math.h>
#include <sys/timeb.h>
#include <windows.h>
#define execv _execv
#define popen _popen
#define pclose _pclose
#define chdir _chdir
#define mkdir _mkdir
#define rmdir _rmdir
#define S_ISDIR(X) ((X&0170000)==0040000)
#define S_IRWXU 0x0700
#define S_IRWXG 0x0070
#define S_IRWXO 0x0007
#pragma warning(disable:4996)
#elif _WIN32
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS 1
#include <direct.h>
#include <io.h>
#include <math.h>
#include <sys/timeb.h>
#include <windows.h>
#define execv _execv
#define popen _popen
#define pclose _pclose
#define chdir _chdir
#define mkdir _mkdir
#define rmdir _rmdir
#define S_ISDIR(X) ((X&0170000)==0040000)
#define S_IRWXU 0x0700
#define S_IRWXG 0x0070
#define S_IRWXO 0x0007
#pragma warning(disable:4996)
#elif __linux__
#define OS_TYPE LINUX_OS
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cmath>
#elif __unix__
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cmath>
#endif

namespace slib {
#ifndef SLIB_DLL
#if defined(_WINDLL)
#define SLIB_DLL _declspec(dllexport)
#else
#define SLIB_DLL  
#endif
#endif

#ifdef __APPLE__
#if TARGET_OS_MAC
#define MAC_OS
#define LOCALIZE 
#endif
#elif _WIN64
#define WIN_OS
#define WIN64_OS
#elif _WIN32
#define WIN_OS
#define WIN32_OS
#elif __linux__
#define LINUX_OS
#define LOCALIZE 
#elif __unix__
#define UNIX_OS
#define LOCALIZE 
#else
#define UNKNOWN_OS
#endif
#define sbyte int8_t
#define subyte uint8_t
#define sshort int16_t
#define sushort uint16_t
#define sint int32_t
#define suint uint32_t
#define sinteger int64_t
#define suinteger uint64_t
#define sreal double
#define sdecimal long double
#ifdef LANG_JA
#define DEFAULT_LANG "ja"
#else 
#define	DEFAULT_LANG "en"
#endif
#define V(...) __VA_ARGS__
#define $(X) X
#define P(X,Y) std::make_pair((X), (Y))
#define R(X) #X
#define S(X) String(R(X))
#define REG(X) Regex(R(X))
#define ANY slib::Range<size_t>(0, -1)
#define L(X) slib::Locale::localize(X)
#define E_ (*it)
#define E__ (*it_)
#define E1_ (*it2.first)
#define E2_ (*it2.second)
#define E_NEXT (*(it+1))
#define AT_(X) slib::Range<size_t>((X),(X))
#define IN_(X,Y) slib::Range<size_t>((X),(Y))
#define E_PREV (*(it-1))
#define NEXT_ ++it
#define PREV_ --it
#define INDEX_(X) (it-(X).begin())
#define MAX_(X, Y) (X)<(Y)?(Y):(X)
#define MIN_(X, Y) (X)<(Y)?(X):(Y)



#define sforeach(V) for(auto it=(V).begin();it<(V).end();++it)
#define srforeach(V) for(auto it=(V).end()-1;(V).begin()<=it;--it)
#define sforeach_(I,V) for(auto (I)=(V).begin();(I)<(V).end();++(I))
#define srforeach_(I,V) for(auto (I)=(V).end()-1;(V).begin()<=(I);--(I))
#define sforeach2(X,Y) for(auto it2=std::make_pair((X).begin(), (Y).begin());it2.first<(X).end();++it2.first,++it2.second)

#define sforeachc(V) for(auto it=(V).ubegin();it<(V).uend();++it)
#define srforeachc(V) for(auto it=(V).uend()-1;(V).ubegin()<=it;--it)
#define sforeachc_(I,V) for(auto (I)=(V).ubegin();(I)<(V).uend();++(I))
#define srforeachc_(I,V) for(auto (I)=(V).uend()-1;(V).ubegin()<=(I);--(I))

#define sforeachk(V) for(auto it=(V).begin();it!=(V).end();++it)
#define sforeachk_(I,V) for(auto (I)=(V).begin();(I)!=(V).end();++(I))

#define sforeachi(V) for(auto i=0;i<(V).size();++i)
#define srforeachi(V) for(auto i=(V).size();-1<i;--i)

#define sforeachi_(I,V) for(auto (I)=0;(I)<(V).size();++(I))
#define srforeachi_(I,V) for(auto (I)=(V).size();-1<(I);--(I))

#define sforin(I,B,E) for(auto (I)=(B);(I)<(E);++(I))
#define srforin(I,B,E) for(auto (I)=(E)-1;(B)<=(I);--(I))
#define sfortill(I,B,E) for(auto (I)=(B);(I)!=(E);++(I))

	typedef enum {
		BIG = 0,
		LITTLE = 1,
	} ENDIAN;
    typedef enum {
        ASC = 0,
        DESC = 1,
    } ORDER;
	typedef enum {
		EXACT_MATCH = 0,
		BAGIN_WITH = 1,
		END_WITH = 2,
		PARTIAL_MATCH = 3,
	} QUERY_MATCH;

    constexpr size_t NOT_FOUND = -1;
    constexpr size_t SHORT_STRING_CAPACITY = 23;
	constexpr char ALPHABET_L[27] = "abcdefghijklmnopqrstuvwxyz";
	constexpr char ALPHABET_U[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	constexpr char HEX_STR[17] = "0123456789ABCDEF";

#ifdef WIN_OS
#define NEW_LINE slib::String("\r\n")
#else    
#define NEW_LINE slib::String("\n") 
#endif
#define SPACE slib::String(" ")
#define TAB slib::String("\t")
#define LF slib::String("\n")
#define CR slib::String("\r")
#define CRLF slib::String("\r\n")
#define DEL slib::String("\b")
#define BS slib::String("\\")
#define SQUOT slib::String("\'")
#define DQUOT slib::String("\"")
#define ESC slib::String(1, (char)27)

    template<typename T>
    extern inline T initVal() { return T(); }
    template<typename T>
    extern inline T unitVal() { return T(1); }
    
    template <size_t S>
    extern inline void invertEndian(void *buf) {
        size_t n = S/2;
        subyte *p = (subyte *)buf, *p_ = &p[S-1], tmp;
        sforin(i, 0, n) {
            tmp = *p; *p = *p_; *p_ = tmp; ++p; --p_;
        }
    }
    template<class C1, class C2>
    extern inline bool instanceOf(const C2 *instance) {
        if(!instance) return false;
        return dynamic_cast<const C1 *>(instance) != 0;
    };
    template<typename T>
    extern inline bool sortAsc(const T &t1, const T &t2) { return t1 < t2; }
    template<typename T>
    extern inline bool sortDesc(const T &t1, const T &t2) { return t2 < t1; }
	
}
#endif
