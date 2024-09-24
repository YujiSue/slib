#ifndef SLIB_SCHRONO_H
#define SLIB_SCHRONO_H
#include "sbasic/time.h"
#include "sutil/sthread.h"
namespace slib {
    class SLIB_DLL SClock {
    private:
        clock_t _start, _end, _total;
    public:
        SClock();
        ~SClock();
        void start();
        void stop();
        void reset();
        double time();
    };
}
#endif
