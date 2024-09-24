#include "sbasic/exception.h"

slib::Exception::Exception() : code(0), prefix("slib") {}
slib::Exception::Exception(int i, const char* msg, const char* desc) : code(i), prefix("slib"), message(msg), description(desc) {}
slib::Exception::~Exception() {}
void slib::Exception::print() const {
	std::cout << stext::RED_TAG << prefix << " error. #" << code << stext::DEFAULT_COLOR << " " << message << std::endl;
	std::cout << description << std::endl;
}

slib::String slib::nullErrorText(const char* var) {
	return "'" + S(var) + "' is null object.";
}
slib::String slib::overflowErrorText(const char* var, const size_t max) {
	return "'" + S(var) + "' must be less than " + S(max) + ".";
}
slib::String slib::outRangeErrorText(const char* var, const int current, const int min, const int max) {
	return "'" + S(var) + "' must be between " + S(min) + " and " + S(max) + ", but the current value is " + S(current) + ".";
}
slib::String slib::castErrorText(const char* ori, const char* dest) {
	return "Class '" + S(ori) + "' cannot be cast to '" + S(dest) + "'.";
}
slib::String slib::formatErrorText(const char* var, const char* current, const char* form) {
	return "'" + S(var) + "' must follow the format '" + S(form) + "'. But current value '" + S(current) + "' does not match the format.";
}
slib::String slib::undefErrorText(const char* var, const char* current) {
	return "'" + S(current) + "' is not defined for '" + S(var) + "'.";
}
slib::String slib::nofoundErrorText(const char* query, const char* src) {
	return "'" + S(query) + "' is not found in '" + S(src) + "'.";
}
slib::String slib::nocompErrorText(const char* var) {
	return S(var) + " are not comparable.";
}
slib::String slib::procErrorText(const char* var) {
	return "Process '" + S(var) + "' is failed or finished with error status.";
}