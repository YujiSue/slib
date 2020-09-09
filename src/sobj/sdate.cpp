#include "sobj/sdate.h"
#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sarray.h"

using namespace slib;

inline int yday(sint m, sint d) {
	return speriod::TOTAL_DAY_OF_MONTH[m] + d;
}

template<class Iter>
inline size_t countRepeat(Iter& it, const char& c) {
	size_t count = 0;
	while (E_ == c) { NEXT_; ++count; }
	return count;
}
inline void readSize(String& str, SUtf8CIterator& it, size_t s) {
	sforin(i, 0, s) { str << E_.toString(); NEXT_; }
}
inline void readTo(String& str, SUtf8CIterator& it, Char& c) {
	while (true) {
		if (E_ == '\0' || E_.toStr() == c.toStr()) break;
		str << E_.toString(); NEXT_;
	}
}
void SDate::_autoform(const char* dt) {
	String date(dt);
	_format = slib::sstyle::ISO8601;
	if (date.contain("/")) {
		if (date.contain(":")) _format = slib::sstyle::YMDHMS;
		else _format = slib::sstyle::YMD;
	}
	else if (date.contain(":")) {
		if (date.contain(" ")) _format = slib::sstyle::EMAIL;
		else _format = slib::sstyle::HMS;
	}
	else if (date.contain(u8"年")) {
		if (date.contain(u8"時")) _format = slib::sstyle::YMDHMS_J;
		else _format = slib::sstyle::YMD_J;
	}
	else if (date.contain(u8"時")) _format = slib::sstyle::HMS_J;
}
void SDate::_parse(const String& date) {
	String tmp;
	bool am = false, jp = false;
	auto dit = date.ubegin();
	sforeachc(_format) {
		if (dit == date.uend()) break;
		if (E_ == 'Y') {
			auto count = countRepeat(it, 'Y');
			if (count == 2) { readSize(tmp, dit, count); Time::year = tmp.intValue() + 2000; PREV_; --dit; }
			else { readTo(tmp, dit, E_); Time::year = tmp; }
			tmp.clear();
		}
		else if (E_ == 'M') {
			auto count = countRepeat(it, 'M');
			if (count == 3) { readSize(tmp, dit, count); Time::month = speriod::MONTH_INDEX[String::lower(tmp)] + 1; PREV_; --dit; }
			else if (count == 2) { readSize(tmp, dit, count); Time::month = tmp; PREV_; --dit; }
			else { readTo(tmp, dit, E_); Time::month = tmp; }
			tmp.clear();
		}
		else if (E_ == 'D') {
			auto count = countRepeat(it, 'D');
			if (count == 2) { readSize(tmp, dit, count); Time::day = tmp; PREV_; --dit; }
			else readTo(tmp, dit, E_); Time::day = tmp; tmp.clear();
		}
		else if (E_ == 'W') {
			auto count = countRepeat(it, 'W');
			if (count == 3) { readSize(tmp, dit, count); week = speriod::WEEK_INDEX[String::lower(tmp)]; PREV_; --dit; }
			else { readTo(tmp, dit, E_); week = speriod::WEEK_LABEL.find(tmp); }
			tmp.clear();
		}
		else if (E_ == 'a') {
			readSize(tmp, dit, 2); PREV_; --dit;
			if (tmp == "am") am = true; else am = false;
		}
		else if (E_ == 'h') {
			auto count = countRepeat(it, 'h');
			if (count == 2) { readSize(tmp, dit, count); PREV_; --dit; }
			else readTo(tmp, dit, E_);
			Time::hour = (am ? tmp : tmp).intValue() + 12; tmp.clear();
		}
		else if (E_ == 'H') {
			auto count = countRepeat(it, 'H');
			if (count == 2) { readSize(tmp, dit, count); PREV_; --dit; }
			else readTo(tmp, dit, E_);
			Time::hour = tmp; tmp.clear();
		}
		else if (E_ == 'm') {
			auto count = countRepeat(it, 'm');
			if (count == 2) { readSize(tmp, dit, count); PREV_; --dit; }
			else readTo(tmp, dit, E_);
			Time::minute = tmp; tmp.clear();
		}
		else if (E_ == 's') {
			auto count = countRepeat(it, 's');
			if (count == 2) { readSize(tmp, dit, count); PREV_; --dit; }
			else readTo(tmp, dit, E_);
			Time::sec = tmp; tmp.clear();
		}
		else if (E_ == 'Z') {
			readSize(tmp, dit, 5); PREV_; --dit;
			if (tmp[0] == '+') _tzone = 0x40; else _tzone = 0x00;
			int8_t t = tmp.substring(1, 2).intValue();
			_tzone |= (t << 2);
			t = tmp.substring(3).intValue() / 15;
			_tzone |= t;
			tmp.clear();
		}
		else if (E_ == 'z') {
			NEXT_;
			readTo(tmp, dit, E_);
			if (!_tzone) _tzone = slocale::TIME_ZONE_IDX[dit->toString()];
			tmp.clear();
		}
		//Japanese
		else if (E_ == u8"号") {
			readSize(tmp, dit, 2); PREV_; --dit;
			jp = true;
			year = speriod::ERA_J_YEAR[speriod::ERA_J.find(tmp)];
		}
		else if (E_ == u8"年") {
			readTo(tmp, dit, E_);
			if (jp) {
				if (tmp == "元") Time::year += 1;
				else Time::year += tmp.transformed(sstyle::HALF_WIDTH).intValue();
			}
			else year = tmp.transformed(sstyle::HALF_WIDTH);
		}
		else if (E_ == u8"月") {
			readTo(tmp, dit, E_);
			Time::month = tmp.transformed(sstyle::HALF_WIDTH);
		}
		else if (E_ == u8"曜") {
			readTo(tmp, dit, E_);
			Time::month = speriod::WEEK_LABEL_J.find(tmp.ubegin()->toStr()) + 1;
		}
		else if (E_ == u8"日") {
			readTo(tmp, dit, E_);
			Time::day = tmp.transformed(sstyle::HALF_WIDTH);
		}
		else if (E_ == u8"時") {
			readTo(tmp, dit, E_);
			Time::hour = tmp.transformed(sstyle::HALF_WIDTH);
		}
		else if (E_ == u8"分") {
			readTo(tmp, dit, E_);
			Time::minute = tmp.transformed(sstyle::HALF_WIDTH);
		}
		else if (E_ == u8"秒") {
			readTo(tmp, dit, E_);
			Time::sec = tmp.transformed(sstyle::HALF_WIDTH);
		}
		++dit;
	}
}

SDate::SDate() : Time(), _tzone(0), week(0), _format(slib::sstyle::ISO8601), SObject() {
	time_t now = std::time(nullptr);
	const struct tm* ltime;
#if defined(MAC_OS) || defined(UNIX_OS) || defined(LINUX_OS)
	ltime = std::localtime(&now);
#else
	struct tm ltime_;
	localtime_s(&ltime_, &now);
	ltime = &ltime_;
#endif
	Time::year = ltime->tm_year + 1900;
	Time::month = ltime->tm_mon + 1;
	week = ltime->tm_wday;
	Time::day = ltime->tm_mday;
	Time::hour = ltime->tm_hour;
	Time::minute = ltime->tm_min;
	Time::sec = ltime->tm_sec;
	if (ltime->tm_isdst) _tzone |= 0x80;
	const struct tm* gmtime = std::gmtime(&now);
	auto hm_l = ltime->tm_hour * 60 + ltime->tm_min,
		hm_g = gmtime->tm_hour * 60 + gmtime->tm_min;
	auto yd_l = yday(ltime->tm_mon, ltime->tm_mday),
		yd_g = yday(gmtime->tm_mon, gmtime->tm_mday);
	if (yd_l < yd_g) hm_g += 1440;
	else if (yd_g < yd_l) hm_l += 1440;
	auto lag = hm_l - hm_g;
	if (lag < 0) { _tzone |= 0x40; lag = -lag; }
	_tzone |= ((lag % 60) / 15) | ((lag / 60) << 2);
}
SDate::SDate(const char* s, const char* f) : Time(), _tzone(0), week(0), SObject() {
	if (f == nullptr) _autoform(s);
	else _format = f;
	_parse(s);
}
SDate::SDate(const sobj& obj) : Time(), _tzone(0), week(0), SObject() {
	 if (obj.isDate()) *this = obj.date();
        else *this = SDate(obj.toString(), nullptr);
}
SDate::SDate(const SDate& date) : Time(date), _tzone(date._tzone), week(date.week), _format(date._format), SObject() {}
SDate::~SDate() {}

SDate& SDate::operator = (const char* s) {
	_parse(s); return *this;
}
SDate& SDate::operator = (const sobj& obj) {
	if (obj.isDate()) *this = obj.date();
	else *this = obj.toString().cstr();
	return *this;
}
SDate& SDate::operator = (const SDate& date) {
	_format = date._format;
	_tzone = date._tzone;
	Time::year = date.year;
	Time::month = date.month;
	Time::day = date.day;
	Time::hour = date.hour;
	Time::minute = date.minute;
	Time::sec = date.sec;
	Time::msec = date.msec;
	week = date.week;
	return (*this);
}

SDate& SDate::operator += (const slib::Time& time) {
	msec += time.msec;
	if (msec < 0) { sec -= (-msec) / 1000 + 1; msec = 1000 - ((-msec) % 1000); }
	else { sec += msec / 1000; msec %= 1000; }
	sec += time.minute;
	if (sec < 0) { minute -= (-sec) / 60 + 1; sec = 60 - ((-sec) % 60); }
	else { minute += sec / 60; sec %= 60; }
	minute += time.minute;
	if (minute < 0) { hour -= (-minute) / 60 + 1; minute = 60 - ((-minute) % 60); }
	else { hour += minute / 60; minute %= 60; }
	hour += time.hour;
	if (hour < 0) { day -= (-hour) / 24 + 1; hour = 24 - ((-hour) % 24); }
	else { day += hour / 24; hour %= 24; }
	day += time.day;
	if (day < 0) {
		while (day < 0) {
			if (month == 1) { --year; month = 12; }
			else --month;
			day += speriod::DAY_OF_MONTH[month - 1];
		}
	}
	if (speriod::DAY_OF_MONTH[month - 1] < day) {
		day -= speriod::DAY_OF_MONTH[month - 1]; ++month;
		if (12 < month) { ++year; month -= 12; }
	}
	week += time.day;
	if (week < 0) week = 7 - ((-week) % 7); else week %= 7;
	month += time.month;
	if (month < 0) { year -= (-month) / 12 + 1; month = 12 - ((-month) % 12); }
	else { year += month / 12; month %= 12; }
	year += time.year;
	return *this;
}
SDate& SDate::operator -= (const slib::Time& time) {
	msec -= time.msec;
	if (msec < 0) { sec -= (-msec) / 1000 + 1; msec = 1000 - ((-msec) % 1000); }
	else { sec += msec / 1000; msec %= 1000; }
	sec -= time.minute;
	if (sec < 0) { minute -= (-sec) / 60 + 1; sec = 60 - ((-sec) % 60); }
	else { minute += sec / 60; sec %= 60; }
	minute -= time.minute;
	if (minute < 0) { hour -= (-minute) / 60 + 1; minute = 60 - ((-minute) % 60); }
	else { hour += minute / 60; minute %= 60; }
	hour -= time.hour;
	if (hour < 0) { day -= (-hour) / 24 + 1; hour = 24 - ((-hour) % 24); }
	else { day += hour / 24; hour %= 24; }
	day -= time.day;
	if (day < 0) {
		while (day < 0) {
			if (month == 1) { month = 12; --year; }
			else --month;
			day += speriod::DAY_OF_MONTH[month - 1];
		}
	}
	if (speriod::DAY_OF_MONTH[month - 1] < day) {
		day -= speriod::DAY_OF_MONTH[month - 1]; ++month;
		if (12 < month) { ++year; month -= 12; }
	}
	week -= time.day;
	if (week < 0) week = 7 - ((-week) % 7); else week %= 7;
	month -= time.month;
	if (month < 0) { year -= (-month) / 12 + 1; month = 12 - ((-month) % 12); }
	else { year += month / 12; month %= 12; }
	year -= time.year;
	return *this;
}

SDate SDate::operator + (const slib::Time& time) const {
	return SDate(*this) += time;
}
SDate SDate::operator - (const slib::Time& time) const {
	return SDate(*this) -= time;
}
Time SDate::operator - (const SDate& date) const {
	slib::Time time;
	time.msec = msec - date.msec;
	time.sec = sec - date.sec;
	time.minute = minute - date.minute;
	time.hour = hour - date.hour;
	time.day = day - date.day;
	time.month = month - date.month;
	time.year = year - date.year;
	return time;
}

SDate SDate::utc(const char* f) {
	SDate date;
	time_t now = std::time(nullptr);
	const struct tm* gmtime = std::gmtime(&now);
	date._tzone = 0;
	date.year = gmtime->tm_year + 1900;
	date.month = gmtime->tm_mon + 1;
	date.week = gmtime->tm_wday + 1;
	date.day = gmtime->tm_mday;
	date.hour = gmtime->tm_hour;
	date.minute = gmtime->tm_min;
	date.sec = gmtime->tm_sec;
	return date;
}

bool SDate::summerTime() const { return _tzone & 0x80; }
int SDate::lag() const { return (_tzone & 0x40 ? -1 : 1) * ((_tzone & 0x03) * 15 + ((_tzone >> 2) & 0x0F)) * 60 + (_tzone & 0x80 ? 60 : 0); }
String SDate::lagStr() const {
	return String(_tzone & 0x40 ? "-" : "+") << String((_tzone >> 2) & 0x0F + (_tzone & 0x80 ? 1 : 0)).filled(2, '0', true) << String((_tzone & 0x03) * 15).filled(2, '0', true);
}

String SDate::getClass() const { return "date"; }
String SDate::toString() const { return toString(_format); }
String SDate::toString(const char *form) const {
	String str;
	String format(form);
	sforeachc(format) {
		if (E_ == 'Y') {
			auto count = countRepeat(it, 'Y');
			if (count == 2) str << (year - 2000);
			else str << year;
			PREV_;
		}
		else if (E_ == 'M') {
			auto count = countRepeat(it, 'M');
			if (count == 1) str << month;
			else if (count == 2) str << String(month).filled(2, '0', true);
			else if (count == 3) str << speriod::MONTH_LABEL_TRI[month - 1];
			else str << speriod::MONTH_LABEL[month - 1];
			PREV_;
		}
		else if (E_ == 'M') {
			auto count = countRepeat(it, 'M');
			if (count == 1) str << month;
			else if (count == 2) str << String(month).filled(2, '0', true);
			else if (count == 3) str << speriod::MONTH_LABEL_TRI[month - 1];
			else str << speriod::MONTH_LABEL[month - 1];
			PREV_;
		}
		else if (E_ == 'D') {
			auto count = countRepeat(it, 'D');
			if (count == 2) str << String(day).filled(2, '0', true);
			else if (count == 3) str << yday(month - 1, day);
			else str << day;
			PREV_;
		}
		else if (E_ == 'W') {
			auto count = countRepeat(it, 'W');
			if (count == 3) str << speriod::WEEK_LABEL_TRI[week];
			else str << speriod::WEEK_LABEL[week];
			PREV_;
		}
		else if (E_ == 'a') str << (hour < 12 ? "am" : "pm");
		else if (E_ == 'h') {
			auto count = countRepeat(it, 'h');
			if (count == 2) str << String(hour < 12 ? hour : hour - 12).filled(2, '0', true);
			else str << (hour < 12 ? hour : hour - 12);
			PREV_;
		}
		else if (E_ == 'H') {
			auto count = countRepeat(it, 'H');
			if (count == 2) str << String(hour).filled(2, '0', true);
			else str << hour;
			PREV_;
		}
		else if (E_ == 'm') {
			auto count = countRepeat(it, 'm');
			if (count == 2) str << String(minute).filled(2, '0', true);
			else str << minute;
			PREV_;
		}
		else if (E_ == 's') {
			auto count = countRepeat(it, 's');
			if (count == 2) str << String(sec).filled(2, '0', true);
			else str << sec;
			PREV_;
		}
		else if (E_ == 'Z') str << lagStr();
		else if (E_ == 'z') str << slocale::TIME_ZONE_NAME[_tzone][0];
		//Japanese
		else if (E_ == u8"号") {
			auto jy = slocale::JPYear(year);
			if (*(it + 1) == u8"年") { str << jy.first << (jy.second == 1 ? "元" : String::wide(String(jy.second))) << u8"年"; NEXT_; }
			else str << jy.first;
		}
		else if (E_ == u8"年") str << String::wide(String(year)) << u8"年";
		else if (E_ == u8"月") str << String::wide(String(month)) << u8"月";
		else if (E_ == u8"陰") str << speriod::MONTH_LABEL_J[month - 1];
		else if (E_ == u8"日") str << String::wide(String(day)) << u8"日";
		else if (E_ == u8"曜") str << speriod::WEEK_LABEL_J[week - 1] << u8"曜";
		else if (E_ == u8"午") {
			if (*(it + 1) == u8"時") {
				str << (hour < 12 ? (u8"午前" + String::wide(String(hour))) : (u8"午後" + String::wide(String(hour - 12)))) << u8"時"; NEXT_;
			}
			else str << (hour < 12 ? u8"午前" : u8"午後");
		}
		else if (E_ == u8"時") str << String::wide(String(hour)) << u8"時";
		else if (E_ == u8"分") str << String::wide(String(minute)) << u8"分";
		else if (E_ == u8"秒") str << String::wide(String(sec)) << u8"秒";
		else str << E_.toString();
	}
	return str;
}
SObject* SDate::clone() const { return new SDate(*this); }

bool SDate::operator < (const sobj& obj) const {
	if (obj.isDate()) return (*this) < obj.date();
	return getClass() < obj->getClass();
}
bool SDate::operator < (const SDate& time) const {
	return *((slib::Time*)this) < *((const Time*)&time);
}
bool SDate::operator == (const sobj& obj) const {
	if (obj.isDate()) return (*this) == obj.date();
	return false;
}
bool SDate::operator == (const SDate& time) const {
	return _format == time._format && *((Time*)this) == time;
}
bool SDate::operator != (const sobj& obj) const {
	return !((*this) == obj);
}
bool SDate::operator != (const SDate& time) const {
	return !((*this) == time);
}

