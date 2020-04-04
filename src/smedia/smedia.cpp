#include "smedia/smedia.h"

using namespace slib;
using namespace slib::smedia;

SMediaException::SMediaException(const char* f, sint l, const char* func, sint e, const char* target, const char* note)
: SException(f, l, func, e, target, note) { 
	prefix = "smedia"; 

}
SMediaException::~SMediaException() {}