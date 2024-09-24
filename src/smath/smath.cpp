#include "sbasic/string.h"
#include "smath/mexception.h"

slib::SMathException::SMathException() {}
slib::SMathException::SMathException(const int i, const char* m, const char* d) : slib::Exception(i, m, d) { prefix = "Math"; }
slib::SMathException::~SMathException() {}
slib::String slib::divZeroErrorText(const char* s) { return S(s) + " is zero."; }
size_t operator "" _u(unsigned long long int v) { return (size_t)v; }
