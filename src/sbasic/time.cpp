#include "sbasic/time.h"
#include "sobj/snumber.h"

using namespace slib;

bool slib::Time::isLeapYear(int y) {
    if (!(y%4)) {
        if (!(y%100)) { if (!(y%400)) return true; else return false; }
        return true;
    }
    return false;
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
Time::Time() : year(0), month(0), day(0), hour(0), minute(0), second(0) {}
Time::Time(const char *s) : Time() {
    String str(s), sub;
    bool neg = false;
    sforeachc(str) {
		if (E_ == '-') neg = true;
		else if (E_ == "Y") { year = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if (E_ == "M") { month = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if (E_ == "W") { day = (neg ? -1 : 1) * 7 * sub.intValue(); sub.clear(); }
		else if (E_ == "D") { day = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if (E_ == "h") { hour = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if (E_ == "s") { second = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if (E_ == "m") { minute = (neg ? -1 : 1) * sub.intValue(); sub.clear(); }
		else if (E_ == u8"年") { year = (neg ? -1 : 1) * String::narrow(sub).intValue(); sub.clear(); }
		else if (E_ == u8"月") {
			if (sub.endWith(u8"ヶ")) sub.clip(0, sub.find(u8"ヶ"));
			month = (neg ? -1 : 1) * String::narrow(sub).intValue(); sub.clear();
		}
		else if (E_ == u8"週") { day = (neg ? -1 : 1) * 7 * String::narrow(sub).intValue(); sub.clear(); }
		else if (E_ == u8"日") { day = (neg ? -1 : 1) * String::narrow(sub).intValue(); sub.clear(); }
		else if (E_ == u8"時") { hour = (neg ? -1 : 1) * String::narrow(sub).intValue(); sub.clear(); }
		else if (E_ == u8"分") { minute = (neg ? -1 : 1) * String::narrow(sub).intValue(); sub.clear(); }
		else if (E_ == u8"秒") { second = (neg ? -1 : 1) * String::narrow(sub).intValue(); sub.clear(); }
		else if (E_ == u8"間") continue;
		else sub += E_;
    }
}
Time::Time(const Time& time) : year(time.year), month(time.month), day(time.day),
hour(time.hour), minute(time.minute), second(time.second) {}
Time::~Time() {}
Time &Time::operator=(const Time &time) {
    year = time.year; month = time.month; day = time.day;
	hour = time.hour; minute = time.minute; second = time.second;
	return *this;
}
Time &Time::operator+=(const Time &time) {
    year += time.year; month += time.month; day += time.day; 
	hour += time.hour; minute += time.minute; second += time.second;
	_tadjust(); return *this;
}
Time &Time::operator-=(const Time &time) {
    year -= time.year; month -= time.month; day -= time.day; 
	hour -= time.hour; minute -= time.minute; second -= time.second;
	_tadjust(); return *this;
}
Time Time::operator+(const Time &time) { return Time(*this)+=time; }
Time Time::operator-(const Time &time) { return Time(*this)-=time; }
Time Time::operator-() {
	year *= -1; month *= -1; day *= -1; hour *= -1; minute *= -1; second *= -1;
}
void Time::init() {
	year = 0;  month = 0; day = 0; hour = 0; minute = 0; second = 0;
}
String Time::toString() const {
	String str;
	if (year) str << year << "Y";
	if (month) str << month << "M";
	if (day) str << day << "D";
	if (hour) str << hour << "h";
	if (minute) str << minute << "m";
	if (second) str << second << "s";
	return str;
}
String Time::toJString() const  {
    String str;
	if (year) str << String::wide(String(year)) << u8"年";
	if (month) str << String::wide(String(month)) << u8"ヶ月";
	if (day) str << String::wide(String(day)) << u8"日";
	if (hour) str << String::wide(String(hour)) << u8"時間";
	if (minute) str << String::wide(String(minute)) << u8"分";
	if (second) str << String::wide(String(second)) << u8"秒";
    return str;
}
bool Time::operator<(const Time &time) const {
    if (year != time.year) return year < time.year;
    if (month != time.month) return month < time.month;
    if (day != time.day) return day < time.day;
    if (hour != time.hour) return hour < time.hour;
    if (minute != time.minute) return minute < time.minute;
    if (second != time.second) return second < time.second;
    return false;
}
bool Time::operator==(const Time &time) const {
    return year == time.year && month == time.month && day == time.day &&
    hour == time.hour && minute == time.minute && second == time.second;
}
