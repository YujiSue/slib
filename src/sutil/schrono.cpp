#include "sutil/schrono.h"

using namespace slib;

SClock::SClock() : _total(0) {}
SClock::~SClock() {}
void SClock::start() { _start = clock(); }
void SClock::stop() { _end = clock(); _total += _end-_start; }
void SClock::reset() { _total = 0; }
double SClock::time() { return (double)_total/CLOCKS_PER_SEC; }