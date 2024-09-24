#include "sutil/schrono.h"

slib::SClock::SClock() : _start(0), _end(0), _total(0) {}
slib::SClock::~SClock() {}
void slib::SClock::start() { _start = clock(); }
void slib::SClock::stop() { _end = clock(); _total += _end - _start; _start = _end; }
void slib::SClock::reset() { _start = 0; _end = 0; _total = 0; }
double slib::SClock::time() { return (double)_total/CLOCKS_PER_SEC; }