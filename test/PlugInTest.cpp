#include "sobj.h"
#include "sapp/sapp.h"

using namespace slib;
extern "C" {
	splugin dlltest(int i, double d, String s) {
		return i * d * s.intValue();
	}
}