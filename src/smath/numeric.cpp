#include "smath/numeric.h"
slib::String slib::toString(const bool val, const char* f, ENDIAN en) {
	if (!f || f[0] == 0) return val ? "true" : "false";
	slib::String form = f;
	if (form == "c") return val ? "true" : "false";
	else if (form == "int") return val ? "1" : "0";
	else if (form == "objc") return val ? "YES" : "NO";
	else if (form == "py") return val ? "True" : "False";
	else if (form == "xls") return val ? "TRUE" : "FALSE";
	return numToString(val, f, en);
}
slib::String slib::toString(const slib::sbyte val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
slib::String slib::toString(const slib::subyte val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
slib::String slib::toString(const slib::sshort val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
slib::String slib::toString(const slib::sushort val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
slib::String slib::toString(const int val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
slib::String slib::toString(const unsigned int val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
slib::String slib::toString(const size_t val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
//slib::String slib::toString(const slib::sint val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
//slib::String slib::toString(const slib::suint val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
slib::String slib::toString(const slib::sinteger val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
//slib::String slib::toString(const slib::suinteger val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
slib::String slib::toString(const float val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }
slib::String slib::toString(const double val, const char* f, ENDIAN en) { return slib::numToString(val, f, en); }