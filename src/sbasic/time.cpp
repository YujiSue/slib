#include "sbasic/time.h"
const stringarray slib::sdt::MONTH_LABEL = {
	"january", "february", "march", "april", "may", "june",
	"july", "august", "september", "october", "november", "december"
};
const stringarray slib::sdt::WEEK_LABEL = {
	"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"
};
slib::String slib::sdt::monthLabel(const size_t s, bool tri) {
	auto lbl = String(MONTH_LABEL[s]);
	lbl[0] = (char)std::toupper((int)lbl[0]);
	return tri ? lbl.substring(0, 3) : lbl;
}
size_t slib::sdt::monthIndex(const slib::String& mon) {
	sfor(MONTH_LABEL) {
		if (sstr::toLower($_).match(sstr::toLower(mon))) return $INDEX(MONTH_LABEL) + 1;
	}
	return NOT_FOUND;
}
slib::String slib::sdt::weekLabel(const size_t s, bool tri) {
	auto lbl = String(WEEK_LABEL[s]);
	lbl[0] = (char)std::toupper((int)lbl[0]);
	return tri ? lbl.substring(0, 3) : lbl;
}
size_t slib::sdt::weekIndex(const slib::String& wk) {
	sfor(WEEK_LABEL) {
		if (sstr::toLower($_).match(sstr::toLower(wk))) return $INDEX(WEEK_LABEL);
	}
	return NOT_FOUND;
}
slib::Pair<slib::String, size_t> slib::sdt::eraJa(const size_t ad_year) {
	if (2018 < ad_year) return Pair<slib::String, size_t>(u8"令和", ad_year - 2018);
	else if (1988 < ad_year) return Pair<slib::String, size_t>(u8"平成", ad_year - 1988);
	else if (1925 < ad_year) return Pair<slib::String, size_t>(u8"昭和", ad_year - 1925);
	else if (1925 < ad_year) return Pair<slib::String, size_t>(u8"大正", ad_year - 1911);
	else if (1925 < ad_year) return Pair<slib::String, size_t>(u8"明治", ad_year - 1867);
	throw Exception();
}
int slib::sdt::adyear(const char* ja_era, const size_t year) {
	String je(ja_era);
	if (je == u8"令和") return 2018 + year;
	else if (je == u8"平成") return 1988 + year;
	else if (je == u8"昭和") return 1925 + year;
	else if (je == u8"大正") return 1911 + year;
	else if (je == u8"明治") return 1867 + year;
	throw Exception();
}
slib::String slib::sdt::monthJaLabel(const size_t s) {
	switch (s) {
	case 0:
		return u8"睦月";
	case 1:
		return u8"如月";
	case 2:
		return u8"弥生";
	case 3:
		return u8"卯月";
	case 4:
		return u8"皐月";
	case 5:
		return u8"水無月";
	case 6:
		return u8"文月";
	case 7:
		return u8"葉月";
	case 8:
		return u8"菊月";
	case 9:
		return u8"神無月";
	case 10:
		return u8"霜月";
	case 11:
		return u8"師走";
	default:
		throw RangeException(outRangeErrorText("month index", s, 0, 11));
	}
}
size_t slib::sdt::monthJaIndex(const slib::String& mon) {
	if (mon.match(u8"睦月")) return 0;
	else if (mon.match(u8"如月")) return 1;
	else if (mon.match(u8"弥生")) return 2;
	else if (mon.match(u8"卯月")) return 3;
	else if (mon.match(u8"皐月")) return 4;
	else if (mon.match(u8"水無月")) return 5;
	else if (mon.match(u8"文月")) return 6;
	else if (mon.match(u8"葉月")) return 7;
	else if (mon.match(u8"菊月")) return 8;
	else if (mon.match(u8"神無月")) return 9;
	else if (mon.match(u8"霜月")) return 10;
	else if (mon.match(u8"師走")) return 11;
	return -1;
}
slib::String slib::sdt::weekJaLabel(const size_t s) {
	switch (s) {
	case 0:
		return u8"月";
	case 1:
		return u8"火";
	case 2:
		return u8"水";
	case 3:
		return u8"木";
	case 4:
		return u8"金";
	case 5:
		return u8"土";
	case 6:
		return u8"日";
	default:
		throw RangeException(outRangeErrorText("week index", s, 0, 6));
	}
}
size_t slib::sdt::weekJaIndex(const slib::String& wk) {
	if (wk.match(u8"月")) return 0;
	else if (wk.match(u8"火")) return 1;
	else if (wk.match(u8"水")) return 2;
	else if (wk.match(u8"木")) return 3;
	else if (wk.match(u8"金")) return 4;
	else if (wk.match(u8"土")) return 5;
	else if (wk.match(u8"日")) return 6;
	return -1;
}
bool slib::sdt::isLeapYear(const int y) {
    if (!(y%4)) {
        if (!(y%100)) { if (!(y%400)) return true; else return false; }
        return true;
    }
    return false;
}
int slib::sdt::yearday(int y, int m, int d) {
	int yd = sdt::TOTAL_DAY_OF_MONTH[m - 1] + d;
	if (sdt::isLeapYear(y) && 2 < m) yd++;
	return yd;
}
int slib::sdt::weekday(int y, int m, int d) {
	int y_, y_1, y_2, m_, g, w;
	if (m == 1) { y_ = y - 1; m_ = 13; }
	else if (m == 2) { y_ = y - 1; m_ = 14; }
	else { y_ = 0; m_ = 0; }
	y_1 = y_ % 100; y_2 = y_ / 100;
	if (y_ < 1583) g = 6 * y_2 + 5;
	else g = 5 * y_2 + (y_ / 4);
	w = d + (26 * (m_ + 1) / 10) + y_1 + (y_1 / 4) + g;
	w %= 7;
	return w + 1;
}
slib::String slib::sdt::tzone(subyte tz, int op) {
	if (tz & 0x80) {
		tz -= 0x80;
		switch (tz) {
		case 4:
			return "CEST";
		case 8:
			return "EEST";
		case 42:
			return "ACDT";
		case 80:
			return "ADT";
		case 84:
			return "EDT";
		case 88:
			return "CDT";
		case 92:
			return "MDT";
		case 96:
			return "PDT";
		case 104:
			return "HDT";
		default:
		{
			if (op == 0) return "BST";
			else return "WEST";
		}
		}	
	}
	else {
		switch (tz) {
		case 4:
			return "CET";
		case 8:
			return "EET";
		case 12:
		{
			if (op == 0) return "MSK";
			else if (op == 1) return "FET";
			else return "SYOT";
		}
		case 22:
			return "IST";
		case 32:
			return "CST";
		case 36:
			return "JST";
		case 38:
			return "ACST";
		case 80:
			return "AST";
		case 84:
			return "EST";
		case 88:
			return "CST";
		case 92:
			return "MST";
		case 96:
			return "PST";
		case 104:
			return "HST";
		default:
		{
			if (op == 0) return "UTC";
			else if (op == 1) return "GMT";
			else return "WET";
		}
		}
	}
	return "";
}
slib::subyte slib::sdt::tzoneCode(const String& tz) {
	if (tz == "UTC" || tz == "GMT" || tz == "WET") return 0;
	else if (tz == "BST" || tz == "WEST") return 0x80;
	else if (tz == "CET") return (0x01 << 2);
	else if (tz == "CEST") return (0x80 | (0x01 << 2));
	else if (tz == "EET") return (0x02 << 2);
	else if (tz == "EEST") return (0x80 | (0x02 << 2));
	else if (tz == "MSK" || tz == "FET" || tz == "SYOT") return (0x03 << 2);
	else if (tz == "IST") return ((0x05 << 2) | 0x02);
	else if (tz == "CST") return (0x08 << 2);
	else if (tz == "JST") return (0x09 << 2);
	else if (tz == "ACST") return ((0x09 << 2) | 0x02);
	else if (tz == "ACDT") return (0x80 | (0x0A << 2) | 0x02);
	else if (tz == "AST") return (0x40 | (0x04 << 2));
	else if (tz == "ADT") return (0x80 | 0x40 | (0x04 << 2));
	else if (tz == "EST") return (0x40 | (0x05 << 2));
	else if (tz == "EDT") return (0x80 | 0x40 | (0x05 << 2));
	else if (tz == "CST") return (0x40 | (0x06 << 2));
	else if (tz == "CDT") return (0x80 | 0x40 | (0x06 << 2));
	else if (tz == "MST") return (0x40 | (0x07 << 2));
	else if (tz == "MDT") return (0x80 | 0x40 | (0x07 << 2));
	else if (tz == "PST") return (0x40 | (0x08 << 2));
	else if (tz == "PDT") return (0x80 | 0x40 | (0x08 << 2));
	else if (tz == "HST") return (0x40 | (0x0A << 2));
	else if (tz == "HDT") return (0x80 | 0x40 | (0x0A << 2));
	return 0;
}
slib::String slib::sdt::format(const char* s) {
	String str(s);
	if (str.match("/")) {
		if (str.match(":")) return slib::sstyle::YMDHMS;
		else return slib::sstyle::YMD;
	}
	else if (str.match(":")) {
		if (str.match(", ")) return slib::sstyle::EMAIL;
		else return slib::sstyle::HMS;
	}
	else if (str.match(u8"年")) {
		if (str.match(u8"時")) return slib::sstyle::YMDHMS_J;
		else return slib::sstyle::YMD_J;
	}
	else if (str.match(u8"時")) return slib::sstyle::HMS_J;
	else return slib::sstyle::ISO8601;
}
void slib::Time::_tadjust() {
	while (second < 0) { second += 60; --minute; }
	while (59 < second) { second -= 60; ++minute; }
	while (minute < 0) { minute += 60; --hour; }
	while (59 < minute) { minute -= 60; ++hour; }
	while (hour < 0) { hour += 24; --day; }
	while (23 < hour) { hour -= 24; ++day; }
	while (day < 0) { day += 365; --year; }
	while (364 < day) { day -= 365; ++year; }
	while (month < 0) { month += 12; --year; }
	while (11 < month) { month -= 12; ++year; }
}

slib::Time::Time() : year(0), month(0), day(0), hour(0), minute(0), second(0) {}
slib::Time::Time(const char *s) : Time() {
    String str(s), sub;
    bool neg = false;
    sforc(str) {
		if ($_ == "-") neg = true;
		else if ($_ == "Y") { year = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if ($_ == "M") { month = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if ($_ == "D") { day = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if ($_ == "h" || $_ == "H") { hour = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if ($_ == "m") { minute = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if ($_ == "s") { second = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if ($_ == u8"年") { year = (neg ? -1 : 1) * sstr::toNarrow(sub).intValue(); sub.clear(); }
		else if ($_ == u8"月") {
			if (sub.endWith(u8"ヶ")) sub.clip(0, sub.find(u8"ヶ"));
			month = (neg ? -1 : 1) * sstr::toNarrow(sub).intValue(); sub.clear();
		}
		else if ($_ == u8"日") { day = (neg ? -1 : 1) * sstr::toNarrow(sub).intValue(); sub.clear(); }
		else if ($_ == u8"時") { hour = (neg ? -1 : 1) * sstr::toNarrow(sub).intValue(); sub.clear(); }
		else if ($_ == u8"分") { minute = (neg ? -1 : 1) * sstr::toNarrow(sub).intValue(); sub.clear(); }
		else if ($_ == u8"秒") { second = (neg ? -1 : 1) * sstr::toNarrow(sub).intValue(); sub.clear(); }
		else if ($_ == u8"間") continue;
		else sub << $_;
    }
}
slib::Time::Time(const Time& time) 
	: year(time.year), month(time.month), day(time.day), hour(time.hour), minute(time.minute), second(time.second) {}
slib::Time::~Time() {}
slib::Time & slib::Time::operator=(const Time &time) {
    year = time.year; month = time.month; day = time.day;
	hour = time.hour; minute = time.minute; second = time.second;
	return *this;
}
slib::Time & slib::Time::operator+=(const Time &time) {
    year += time.year; month += time.month; day += time.day; 
	hour += time.hour; minute += time.minute; second += time.second;
	_tadjust(); return *this;
}
slib::Time & slib::Time::operator-=(const Time &time) {
    year -= time.year; month -= time.month; day -= time.day; 
	hour -= time.hour; minute -= time.minute; second -= time.second;
	_tadjust(); return *this;
}
slib::Time slib::Time::operator+(const Time &time) { return Time(*this)+=time; }
slib::Time slib::Time::operator-(const Time &time) { return Time(*this)-=time; }
slib::Time slib::Time::operator-() {
	Time t;
	t.year = -year; t.month = -month; t.day = -day; 
	t.hour = -hour; t.minute = -minute; t.second = -second;
	return t;
}
void slib::Time::init() {
	year = 0;  month = 0; day = 0; hour = 0; minute = 0; second = 0;
}
slib::String slib::Time::toString(const char *format) const {
	String str, f(format);
	if (year) str << year << "Y";
	if (month) str << month << "m";
	if (day) str << day << "d";
	if (hour) str << hour << "H";
	if (minute) str << minute << "M";
	if (second) str << second << "S";
	return str;
}
bool slib::Time::operator<(const Time &time) const {
    if (year != time.year) return year < time.year;
    if (month != time.month) return month < time.month;
    if (day != time.day) return day < time.day;
    if (hour != time.hour) return hour < time.hour;
    if (minute != time.minute) return minute < time.minute;
    if (second != time.second) return second < time.second;
    return false;
}
bool slib::Time::operator==(const Time &time) const {
    return year == time.year && month == time.month && day == time.day &&
    hour == time.hour && minute == time.minute && second == time.second;
}
std::ostream& slib::operator<<(std::ostream& os, const slib::Time& tm) {
	return os << tm.year << "Y" << tm.month << "m" << tm.day << "d" << tm.hour << "H" << tm.minute << "M" << tm.second << "S";
}