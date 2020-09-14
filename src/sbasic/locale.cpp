#include "sbasic/locale.h"
#include "sbasic/time.h"

using namespace slib;

std::pair<slib::String, int> slib::slocale::JPYear(int year) {
	srforeach(speriod::ERA_J_YEAR) { if (E_ < year) return std::pair<String, int>(speriod::ERA_J[(int)INDEX_(speriod::ERA_J_YEAR)], year - E_); }
	throw SException(ERR_INFO);
}
int slib::slocale::ADYear(const char* s, int year) {
	auto idx = speriod::ERA_J.find(s);
	if (idx != NOT_FOUND) return speriod::ERA_J_YEAR[(int)idx] + year;
	throw SException(ERR_INFO);
}
slib::String Locale::_locale = DEFAULT_LANG;
sattribute Locale::_translator{};
void Locale::setLocale(const char* s) { _locale = s; }
void Locale::load(const char* s) {
	std::ifstream ifs(String(s).localize(), std::ios::in);
	std::string row;
	size_t pos = 0;
	while (getline(ifs, row)) {
		if ((pos = row.find("=")) != std::string::npos) {
			_translator[row.substr(0, pos)] = row.substr(pos + 1);
		}
	}
	ifs.close();
}
void Locale::save(const char* s) {
	std::ofstream ofs(String(s).localize(), std::ios::out|std::ios::trunc);
	sforeach(_translator) ofs << E_.key.cstr() << "=" << E_.value.cstr() << LF.cstr();
	ofs.flush();
	ofs.close();
}
void Locale::add(const char* k, const char* l) { _translator[k] = l; }
void Locale::append(std::initializer_list<std::pair<slib::String, slib::String>> li) {
	sforeach(li) _translator[E_.first] = E_.second;
}
void Locale::set(std::initializer_list<std::pair<slib::String, slib::String>> li) { _translator = li; }
void Locale::remove(const char* k) { _translator.remove(k); }
 String Locale::localize(const char* s) {
	 if (_translator.hasKey(s)) return _translator[s];
	 else return s;
 }