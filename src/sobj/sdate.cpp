#include "sobj/sdate.h"
#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sarray.h"

using namespace slib;
inline int yearday(sint y, sint m, sint d) {
	int yd = speriod::TOTAL_DAY_OF_MONTH[m - 1] + d;
	if (Time::isLeapYear(y) && 2 < m) yd++;
	return yd;
}
inline std::pair<subyte, subyte> monthday(sint y, sint d) {
	std::pair<subyte, subyte> md;
	if (31 < d) {
		d -= 31;
		if ((28 + (Time::isLeapYear(y)?1:0)) < d) {
			d -= (28 + (Time::isLeapYear(y) ? 1 : 0));
			sforin(i, 2, 12) {
				if (speriod::DAY_OF_MONTH[i] < d) d -= speriod::DAY_OF_MONTH[i];
				else return std::make_pair(i+1, d);
			}
		}
		else return std::make_pair(2, d);
	}
	else return std::make_pair(1, d);
}
inline int weekday(sint y, sint m, sint d) {
	sint y_, y_1, y_2, m_, g, w;
	if (m == 1) { y_ = y - 1; m_ = 13; }
	else if (m == 2) { y_ = y - 1; m_ = 14; }
	y_1 = y_ % 100; y_2 = y_ / 100;
	if (y_ < 1583) g = 6 * y_2 + 5;
	else g = 5 * y_2 + (y_ / 4);
	w = d + (26 * (m_ + 1) / 10) + y_1 + (y_1 / 4) + g;
	w %= 7;
	return w + 1;
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
		if (E_ == '\0' || E_.toString() == c.toString()) break;
		str << E_.toString(); NEXT_;
	}
}
const char *SDate::_autoform(const char* dt) {
	String date(dt);
	if (date.contain("/")) {
		if (date.contain(":")) return slib::sstyle::YMDHMS;
		else return slib::sstyle::YMD;
	}
	else if (date.contain(":")) {
		if (date.contain(" ")) return slib::sstyle::EMAIL;
		else return slib::sstyle::HMS;
	}
	else if (date.contain(u8"年")) {
		if (date.contain(u8"時")) return slib::sstyle::YMDHMS_J;
		else return slib::sstyle::YMD_J;
	}
	else if (date.contain(u8"時")) return slib::sstyle::HMS_J;
	else return slib::sstyle::ISO8601;
}
void SDate::_parse(const String& date, const char *form) {
	init();
	String tmp, format(form);
	bool am = false, jp = false;
	auto dit = date.ubegin();
	sforeachc(format) {
		if (dit == date.uend()) break;
		if (E_ == 'Y') {
			auto count = countRepeat(it, 'Y');
			if (count == 2) { readSize(tmp, dit, count); Time::year = tmp.intValue() + 2000; PREV_; --dit; }
			else { readTo(tmp, dit, E_); Time::year = tmp; }
			tmp.clear();
		}
		else if (E_ == 'M') {
			auto count = countRepeat(it, 'M');
			if (count == 3) { readSize(tmp, dit, count); Time::month = (sint)speriod::MONTH_INDEX[String::lower(tmp)] + 1; PREV_; --dit; }
			else if (count == 2) { readSize(tmp, dit, count); Time::month = tmp; PREV_; --dit; }
			else { readTo(tmp, dit, E_); Time::month = tmp; }
			tmp.clear();
		}
		else if (E_ == 'D') {
			auto count = countRepeat(it, 'D');
			if (count == 2) { readSize(tmp, dit, count); mday = tmp; PREV_; --dit; }
			else readTo(tmp, dit, E_); mday = tmp; tmp.clear();
		}
		else if (E_ == 'W') {
			auto count = countRepeat(it, 'W');
			if (count == 3) { readSize(tmp, dit, count); wday = (subyte)speriod::WEEK_INDEX[String::lower(tmp)]; PREV_; --dit; }
			else { readTo(tmp, dit, E_); wday = (subyte)speriod::WEEK_LABEL.find(tmp); }
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
			Time::second = tmp; tmp.clear();
		}
		else if (E_ == 'Z') {
			readSize(tmp, dit, 5); PREV_; --dit;
			if (tmp[0] == '+') tzone = 0x40; else tzone = 0x00;
			int8_t t = tmp.substring(1, 2).intValue();
			tzone |= (t << 2);
			t = tmp.substring(3).intValue() / 15;
			tzone |= t;
			tmp.clear();
		}
		else if (E_ == 'z') {
			NEXT_;
			readTo(tmp, dit, E_);
			if (!tzone) tzone = slocale::TIME_ZONE_IDX[dit->toString()];
			tmp.clear();
		}
		//Japanese
		else if (E_ == u8"号") {
			readSize(tmp, dit, 2); --dit;
			jp = true;
			year = speriod::ERA_J_YEAR[(sint)speriod::ERA_J.find(tmp)];
			tmp.clear();
		}
		else if (E_ == u8"年") {
			readTo(tmp, dit, E_);
			if (jp) {
				if (tmp == "元") Time::year += 1;
				else Time::year += tmp.transformed(sstyle::HALF_WIDTH).intValue();
			}
			else year = tmp.transformed(sstyle::HALF_WIDTH);
			tmp.clear();
		}
		else if (E_ == u8"月") {
			readTo(tmp, dit, E_);
			Time::month = tmp.transformed(sstyle::HALF_WIDTH);
			tmp.clear();
		}
		else if (E_ == u8"曜") {
			readTo(tmp, dit, E_);
			wday = (sint)speriod::WEEK_LABEL_J.find(tmp.ubegin()->toStr()) + 1;
			tmp.clear();
		}
		else if (E_ == u8"日") {
			readTo(tmp, dit, E_);
			mday = tmp.transformed(sstyle::HALF_WIDTH);
			tmp.clear();
		}
		else if (E_ == u8"時") {
			readTo(tmp, dit, E_);
			Time::hour = tmp.transformed(sstyle::HALF_WIDTH);
			tmp.clear();
		}
		else if (E_ == u8"分") {
			readTo(tmp, dit, E_);
			Time::minute = tmp.transformed(sstyle::HALF_WIDTH);
			tmp.clear();
		}
		else if (E_ == u8"秒") {
			readTo(tmp, dit, E_);
			Time::second = tmp.transformed(sstyle::HALF_WIDTH);
			tmp.clear();
		}
		++dit;
	}
	if (!Time::day) Time::day = yearday(year, month, mday);
	if(!wday) wday = weekday(year, month, mday);
}
void SDate::_dadjust() {
	if (Time::second < 0) { while (Time::second < 0) { --Time::minute; Time::second += 60; } }
	while (59 < Time::second) { ++Time::minute; Time::second -= 60; }
	if (Time::minute < 0) { while (Time::minute < 0) { --Time::hour; Time::minute += 60; } }
	while (59 < Time::minute) { ++Time::hour; Time::minute -= 60; }
	if (Time::hour < 0) { while (Time::hour < 0) { --mday; Time::hour += 24; } }
	while (23 < Time::hour) { ++mday; Time::hour -= 24; }
	if (mday < 0) {
		while (mday < 0) {
			--Time::month;
			if (Time::month < 1) { while (Time::month < 1) { --Time::year; Time::month += 12; } }
			else if (12 < Time::month) { while (12 < Time::month) { ++Time::year; Time::month -= 12; } }
			mday += speriod::DAY_OF_MONTH[Time::month - 1];
		}
	}
	if (Time::month < 1) { while (Time::month < 1) { --Time::year; Time::month += 12; }}
	else if (12 < Time::month) { while (12 < Time::month) { ++Time::year; Time::month -= 12; }}
	if (speriod::DAY_OF_MONTH[Time::month - 1] < mday) {
		while (speriod::DAY_OF_MONTH[Time::month - 1] < mday) {
			mday -= speriod::DAY_OF_MONTH[Time::month - 1]; ++Time::month;
			if (Time::month < 1) { while (Time::month < 1) { --Time::year; Time::month += 12; } }
			else if (12 < Time::month) { while (12 < Time::month) { ++Time::year; Time::month -= 12; } }
		}
	}
	Time::day = yearday(year, month, mday);
	if (!wday) wday = weekday(year, month, mday);
}
void SDate::_init() { Time::init(); tzone = 0xFF; mday = 0; wday = 0; }
SDate::SDate() : Time(), tzone(0), mday(0), wday(0), SObject() {
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
	wday = ltime->tm_wday;
	mday = ltime->tm_mday;
	Time::day = yearday(Time::year, Time::month, mday);
	Time::hour = ltime->tm_hour;
	Time::minute = ltime->tm_min;
	Time::second = ltime->tm_sec;
	if (ltime->tm_isdst) tzone |= 0x80;
	const struct tm* gmtime = std::gmtime(&now);
	sint hm_l = ltime->tm_hour * 60 + ltime->tm_min,
		hm_g = gmtime->tm_hour * 60 + gmtime->tm_min;
	sint yd_l = speriod::TOTAL_DAY_OF_MONTH[ltime->tm_mon] + ltime->tm_mday,
		yd_g = speriod::TOTAL_DAY_OF_MONTH[gmtime->tm_mon] + gmtime->tm_mday;
	if (ltime->tm_year != gmtime->tm_year) {
		if (ltime->tm_year < gmtime->tm_year) yd_g += Time::isLeapYear(ltime->tm_year) ? 366 : 365;
		else yd_l += Time::isLeapYear(ltime->tm_year) ? 366 : 365;
	}
	if (yd_l < yd_g) hm_g += 1440;
	else if (yd_g < yd_l) hm_l += 1440;
	sint lag = hm_l - hm_g;
	if (lag < 0) { tzone |= 0x40; lag = -lag; }
	tzone |= ((lag % 60) / 15) | ((lag / 60) << 2);
}
SDate::SDate(suint ui) : Time(), tzone(0xFF), mday(0), wday(0), SObject() {
	mday = (ui & 0x1F);
	month = ((ui >> 5) & 0x0F);
	year = (ui >> 9);
	day = yearday(year, month, mday);
	wday = weekday(year, month, mday);
}
SDate::SDate(suinteger ui) : Time(), tzone(0xFF), mday(0), wday(0), SObject() {
	suint ymd = (ui >> 32), hms = ui&0xFFFFFFFF;
	mday = (ymd & 0x1F);
	month = ((ymd >> 5) & 0x0F);
	year = (ymd >> 9);
	day = yearday(year, month, mday);
	wday = weekday(year, month, mday);
	second = hms & 0x3F;
	minute = (hms >> 6) & 0x3F;
	hour = (hms >> 12);
}
SDate::SDate(const char* s, const char* f) : Time(), tzone(0xFF), mday(0), wday(0), SObject() {
	if (f == nullptr) _parse(s, _autoform(s));
	else _parse(s, f);
}
SDate::SDate(const sobj& obj) : Time(), tzone(0xFF), mday(0), wday(0), SObject() {
	if (obj.isDate()) *this = obj.date();
	else if (obj.isNum()) *this = SDate(obj.uinteger());
	else {
		String str = obj.toString();
		_parse(str, _autoform(str));
	}
}
SDate::SDate(const SDate& date) : Time(date), tzone(date.tzone), mday(date.mday), wday(date.wday), SObject() {}
SDate::~SDate() {}
SDate SDate::utc() {
	SDate date;
	time_t now = std::time(nullptr);
	const struct tm* gmtime = std::gmtime(&now);
	date.tzone = 0;
	date.year = gmtime->tm_year + 1900;
	date.month = gmtime->tm_mon + 1;
	date.wday = gmtime->tm_wday + 1;
	date.mday = gmtime->tm_mday;
	date.day = yearday(date.year, date.month, date.mday);
	date.hour = gmtime->tm_hour;
	date.minute = gmtime->tm_min;
	date.second = gmtime->tm_sec;
	return date;
}
SDate& SDate::operator = (const char* s) { _parse(s, _autoform(s)); return *this; }
SDate& SDate::operator = (const SDate& date) {
	tzone = date.tzone;
	Time::year = date.year;
	Time::month = date.month;
	Time::day = date.day;
	Time::hour = date.hour;
	Time::minute = date.minute;
	Time::second = date.second;
	mday = date.mday;
	wday = date.wday;
	return (*this);
}
SDate& SDate::operator += (const slib::Time& time) {
	Time::year += time.year;
	Time::month += time.month;
	mday += time.day;
	Time::hour += time.hour;
	Time::minute += time.minute;
	Time::second += time.second;
	_dadjust();
	return *this;
}
SDate& SDate::operator -= (const slib::Time& time) {
	Time::year -= time.year;
	Time::month -= time.month;
	mday -= time.day;
	Time::hour -= time.hour;
	Time::minute -= time.minute;
	Time::second -= time.second;
	_dadjust();
	return *this;
}
SDate SDate::operator + (const slib::Time& time) const { return SDate(*this) += time; }
SDate SDate::operator - (const slib::Time& time) const { return SDate(*this) -= time; }
Time SDate::operator - (const SDate& date) const {
	if (uinteger() < date.uinteger()) return -(date - (*this));
	slib::Time dt;
	sint s1 = hour * 3600 + minute * 60 + second + (tzone < 0xFF ? lag() : 0) * 60,
		s2 = date.hour * 3600 + date.minute * 60 + date.second + (date.tzone ? date.lag() : 0) * 60,
		d = mday, m = month, y = year;
	if (s1 < s2) {
		if (d == 1) {
			if (m == 1) { m = 12; --y; d += speriod::DAY_OF_MONTH[m - 1]; }
			else m--;
			d += speriod::DAY_OF_MONTH[m - 1];
		}
		s1 += 24 * 3600; --d;
	}
	else {
		dt.second = s1 - s2;
		dt.minute = dt.second / 60;
		dt.second = dt.second % 60;
		dt.hour = dt.minute / 60;
		dt.minute = dt.minute % 60;
		if (23 < dt.hour) { dt.hour -= 24; ++d; }
	}
	if (d < date.mday) {
		while (d < date.mday) {
			if (m == 1) { m = 12; --y; }
			else m--;
			d += speriod::DAY_OF_MONTH[m - 1];
		}
	}
	dt.day = d - date.mday;
	dt.month = m - date.month;
	dt.year = y - date.year;
	while (dt.month < 0) { dt.month += 12; --dt.year; }
	return dt;
}
bool SDate::summerTime() const { return tzone & 0x80; }
suint SDate::ymd() const { suint val = 0; val |= mday | (month << 5) | (year << 9); return val; }
suint SDate::hms() const { suint val = 0; val |= second | (minute << 6) | (hour << 12); return val; }
suinteger SDate::uinteger() const { auto val = ((suinteger)ymd() << 32); val |= hms(); return val; }
int SDate::lag() const { 
	if (tzone == 0xFF) return -1;
	return (tzone & 0x40 ? -1 : 1) * ((tzone & 0x03) * 15 + ((tzone >> 2) & 0x0F)) * 60 + (tzone & 0x80 ? 60 : 0); 
}
String SDate::lagStr() const {
	if (tzone == 0xFF) return ":local";
	return String(tzone & 0x40 ? "-" : "+") << String((tzone >> 2) & 0x0F + (tzone & 0x80 ? 1 : 0)).filled(2, '0', true) << String((tzone & 0x03) * 15).filled(2, '0', true);
}
String SDate::getClass() const { return "date"; }
String SDate::toString() const { return toString(slib::sstyle::ISO8601); }
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
			if (count == 2) str << String(mday).filled(2, '0', true);
			else if (count == 3) str << Time::day;
			else str << mday;
			PREV_;
		}
		else if (E_ == 'W') {
			auto count = countRepeat(it, 'W');
			if (count == 3) str << speriod::WEEK_LABEL_TRI[wday];
			else str << speriod::WEEK_LABEL[wday];
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
			if (count == 2) str << String(second).filled(2, '0', true);
			else str << second;
			PREV_;
		}
		else if (E_ == 'Z') str << lagStr();
		else if (E_ == 'z') str << slocale::TIME_ZONE_NAME[tzone][0];
		//Japanese
		else if (E_ == u8"号") {
			auto jy = slocale::JPYear(year);
			if (*(it + 1) == u8"年") { str << jy.first << (jy.second == 1 ? "元" : String::wide(String(jy.second))) << u8"年"; NEXT_; }
			else str << jy.first;
		}
		else if (E_ == u8"年") str << String::wide(String(year)) << u8"年";
		else if (E_ == u8"月") str << String::wide(String(month)) << u8"月";
		else if (E_ == u8"陰") str << speriod::MONTH_LABEL_J[month - 1];
		else if (E_ == u8"日") str << String::wide(String(mday)) << u8"日";
		else if (E_ == u8"曜") str << speriod::WEEK_LABEL_J[wday - 1] << u8"曜";
		else if (E_ == u8"午") {
			if (*(it + 1) == u8"時") {
				str << (hour < 12 ? (u8"午前" + String::wide(String(hour))) : (u8"午後" + String::wide(String(hour - 12)))) << u8"時"; NEXT_;
			}
			else str << (hour < 12 ? u8"午前" : u8"午後");
		}
		else if (E_ == u8"時") str << String::wide(String(hour)) << u8"時";
		else if (E_ == u8"分") str << String::wide(String(minute)) << u8"分";
		else if (E_ == u8"秒") str << String::wide(String(second)) << u8"秒";
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
bool SDate::operator == (const SDate& time) const { return tzone == time.tzone && *((Time*)this) == time; }
bool SDate::operator != (const sobj& obj) const { return !((*this) == obj); }
bool SDate::operator != (const SDate& time) const { return !((*this) == time); }