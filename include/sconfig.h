#ifndef SLIB_CONFIG_H
#define SLIB_CONFIG_H
#define SLIB_VERSION_MAJOR 1
#define SLIB_VERSION_SUB 2
#define SLIB_VERSION_MINOR 1

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
#include <limits>
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
#define MAC_OS
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
#pragma warning(disable: 4251)
#define WIN_OS
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
#pragma warning(disable: 4251)
#define WIN_OS
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
#define LINUX_OS
#include <dirent.h>
#include <dlfcn.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cmath>
#elif __unix__
#define UNIX_OS
#include <dirent.h>
#include <dlfcn.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cmath>
#endif

#ifndef SLIB_DLL
#ifdef WIN_OS
#ifdef _WINDLL
#define SLIB_DLL __declspec(dllexport)
#elif _USE_DLL
#define SLIB_DLL __declspec(dllimport)
#else
#define SLIB_DLL
#endif
#else
#define SLIB_DLL  
#endif
#endif

#define sfor(V) for(auto it=(V).begin();it<(V).end();++it)
#define srfor(V) for(auto it=(V).end()-1;((V).begin()-1)<it;--it)
#define sforeach(I,V) for(auto & I : V)
#define sfor2(X,Y) for(auto it=std::make_pair((X).begin(),(Y).begin());it.first<(X).end();++it.first,++it.second)
#define srfor2(X,Y) for(auto it=std::make_pair((X).end()-1,(Y).end()-1);((X).begin()-1)<it.first;--it.first,--it.second)
#define sforc(V) for(auto it=(V).u8begin();it<(V).u8end();++it)
#define sfori(V) for(int i=0;i<(int)(V).size();++i)
#define sforin(I,B,E) for(auto I=B;(I)<(E);++(I))
#define srforin(I,B,E) for(auto I=(E)-1;((B)-1)<(I);--(I))
#define sfortill(V) for(auto it=(V).begin();it!=(V).end();++it)

#define $ it
#define $_ (*it)
#define $_1 (*it.first)
#define $_2 (*it.second)
#define $_K (*it).key()
#define $_V (*it).value()
#define $_NEXT (*(it+1))
#define $_PREV (*(it-1))
#define $NEXT ++it
#define NEXT_(X) ++(X)
#define $PREV --it;
#define PREV_(X) --(X);
#define $INDEX(V) (it-(V).begin())
#define INDEX_(X,V) (&(X)-(V).begin().ptr())

/**
* @namespace slib
* ~english @brief Basic namespace for my library
*/
namespace slib {
    using sbyte = int8_t;
    using subyte = uint8_t;
    using sshort = int16_t;
    using sushort = uint16_t;
    using sint = int32_t;
    using suint = uint32_t;
    using sinteger = int64_t;
    using suinteger = uint64_t;
    using sreal = long double;
    constexpr size_t NOT_FOUND = -1;

    enum class ENDIAN : bool {
        BIG = 0,
        LITTLE = 1,
    };
    enum class ORDER {
        ASC = 0,
        DESC = 1,
    };
    enum class DIRECTION : subyte {
        HEAD = 0x01,
        TAIL = 0x02,
        BI = 0x03,

        LEFT = 0x01,
        RIGHT = 0x02,
        CENTER = 0x04,

        HORIZONTAL = 0x10,
        ROW = 0x10,

        VERTICAL = 0x20,
        COLUMN = 0x20,

        DEPTH = 0x30,
        FRONT = 0x31,
        BACK = 0x32,

        TIME = 0x40,
        NEXT = 0x41,
        PRV = 0x42,

        RADIAL = 0x50,
        SOURCE = 0x51,
        SINK = 0x52,

        ROTATION = 0x60,
        CW = 0x61,
        CCW = 0x62,
    };
    /**
    * @cond
    */
    template <size_t S>
    extern inline void invertEndian(void* buf) {
        size_t n = S / 2;
        subyte* start = (subyte*)buf, * end = &start[S - 1], tmp;
        sforin(i, 0, n) {
            tmp = *start; *start = *end; *end = tmp; ++start; --end;
        }
    }
    template<class C1, class C2>
    extern inline bool instanceOf(const C2* instance) {
        if (!instance) return false;
        return dynamic_cast<const C1*>(instance) != 0;
    };
    template<typename T>
    extern inline bool sortAsc(const T& t1, const T& t2) { return t1 < t2; }
    template<typename T>
    extern inline bool sortDesc(const T& t1, const T& t2) { return t2 < t1; }
    /**
    * @endcond
    */
    /**
    * @class Pair
    * ~english @brief Pair class 
    */
    template <class C1, class C2>
    class Pair {
    public:
        C1 first;
        C2 second;
    public:
        Pair() {}
        Pair(const C1& v1, const C2 &v2) : first(v1), second(v2) {}
        Pair(const Pair& p) : first(p.first), second(p.second) {}
        ~Pair() {}
        Pair& operator=(const Pair& p) { first = p.first; second = p.second; return *this; }
        bool operator==(const Pair<C1, C2>& p) const { return first == p.first && second == p.second; }
    };
}
template <class C1, class C2>
extern std::ostream& operator<<(std::ostream& os, const slib::Pair<C1, C2>& pair) { return os << pair.first << ":" << pair.second; }
#endif
