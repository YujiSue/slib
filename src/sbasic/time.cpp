#include "sbasic/time.h"
#include "sobj/snumber.h"

using namespace slib;

typedef std::pair<subyte, stringarray> TZ;
const Map<subyte, stringarray> Time::TIME_ZONE_NAME = {
    TZ(0, { "UTC", "GMT", "WET" }), TZ(0x80, { "BST", "WEST" }),
    TZ(0x01<<2, { "CET" }), TZ(0x80|(0x01<<2), { "CEST" }),
    TZ(0x02<<2, { "EET" }), TZ(0x80|(0x02<<2), { "EEST" }),
    TZ(0x03<<2, { "MSK", "FET", "SYOT" }), TZ((0x05<<2)|0x02, { "IST" }),
    TZ(0x08<<2, { "CST" }), TZ(0x09<<2, { "JST" }),
    TZ((0x09<<2)|0x02, { "ACST" }), TZ(0x80|(0x0A<<2)|0x02, { "ACDT" }),
    TZ(0x40|(0x04<<2), { "AST" }), TZ(0x80|0x40|(0x04<<2), { "ADT" }),
    TZ(0x40|(0x05<<2), { "EST" }), TZ(0x80|0x40|(0x05<<2), { "EDT" }),
    TZ(0x40|(0x06<<2), { "CST" }), TZ(0x80|0x40|(0x06<<2), { "CDT" }),
    TZ(0x40|(0x07<<2), { "MST" }), TZ(0x80|0x40|(0x07<<2), { "MDT" }),
    TZ(0x40|(0x08<<2), { "PST" }), TZ(0x80|0x40|(0x08<<2), { "PDT" }),
    TZ(0x40|(0x0A<<2), { "HST" }), TZ(0x80|0x40|(0x0A<<2), { "HDT" })
};
typedef std::pair<String, subyte> TZi;
const Map<String, subyte> Time::TIME_ZONE_IDX = {
    TZi("UTC", 0), TZi("GMT", 0), TZi("WET", 0),
    TZi("BST", 0x80), TZi("WEST", 0x80),
    TZi("CET", 0x01<<2), TZi("CEST", 0x80|(0x01<<2)),
    TZi("EET", 0x02<<2), TZi("EEST", 0x80|(0x02<<2)),
    TZi("MSK", 0x03<<2), TZi("FET", 0x03<<2),
    TZi("SYOT", 0x03<<2), TZi("IST", (0x05<<2)|0x02),
    TZi("CST", 0x08<<2), TZi("JST", 0x09<<2),
    TZi("ACST", (0x09<<2)|0x02), TZi("ACDT", 0x80|(0x0A<<2)|0x02),
    TZi("AST", 0x40|(0x04<<2)), TZi("ADT", 0x80|0x40|(0x04<<2)),
    TZi("EST", 0x40|(0x05<<2)), TZi("EDT", 0x80|0x40|(0x05<<2)),
    TZi("CST", 0x40|(0x06<<2)), TZi("CDT", 0x80|0x40|(0x06<<2)),
    TZi("MST", 0x40|(0x07<<2)), TZi("MDT", 0x80|0x40|(0x07<<2)),
    TZi("PST", 0x40|(0x08<<2)), TZi("PDT", 0x80|0x40|(0x08<<2)),
    TZi("HST", 0x40|(0x0A<<2)), TZi("HDT", 0x80|0x40|(0x0A<<2))
};
const stringarray Time::TIME_STR = { "ms", "s", "m", "h", "D", "W", "M", "Y" };
const stringarray Time::TIME_STR_L = { "msec", "sec", "min", "hour", "day", "week", "month", "year" };
const stringarray Time::TIME_STR_J = { u8"㍉秒", u8"秒", u8"分", u8"時間", u8"日", u8"週間", u8"ヶ月", u8"年" };
const stringarray Time::ERA_J = { u8"明治", u8"大正", u8"昭和", u8"平成", u8"令和" };
const intarray Time::ERA_J_YEAR = { 1867, 1911, 1925, 1988, 2018 };
const stringarray Time::MONTH_LABEL = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};
const stringarray Time::MONTH_LABEL_J = {
	u8"睦月", u8"如月", u8"弥生", u8"卯月", u8"皐月", u8"水無月",
	u8"文月", u8"葉月", u8"菊月", u8"神無月", u8"霜月", u8"師走"
};
const stringarray Time::MONTH_LABEL_TRI = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
const stringarray Time::WEEK_LABEL = {
    "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday", "Sunday"
};
const stringarray Time::WEEK_LABEL_J = {
	u8"月", u8"火", u8"水", u8"木", u8"金", u8"土", u8"日"
};
const stringarray Time::WEEK_LABEL_TRI = {
    "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

const intarray Time::DAY_OF_MONTH = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const intarray Time::TOTAL_DAY_OF_MONTH = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

bool Time::isLeapYear(int y) {
    if (!(y%4)) {
        if (!(y%100)) { if (!(y%400)) return true; else return false; }
        return true;
    }
    return false;
}
std::pair<String, int> Time::JYear(int year) {
    srforeach(ERA_J_YEAR) { if (E_ < year) return std::pair<String, int>(ERA_J[INDEX_(ERA_J_YEAR)], year-E_); }
    throw SException(ERR_INFO);
}
int Time::CYear(const char *s, int year) {
    auto idx = ERA_J.find(s);
    if (idx != NOT_FOUND) return ERA_J_YEAR[idx]+year;
    throw SException(ERR_INFO);
}
int Time::yday(int mday, int month, bool leap) {
    return TOTAL_DAY_OF_MONTH[month]+mday+(leap&&2<month?1:0);
}
int Time::mday(int yday, bool leap) {
    srforin(m, 12, 0) {
        if (TOTAL_DAY_OF_MONTH[m] < yday) return yday-TOTAL_DAY_OF_MONTH[m]-(leap&&1<m?1:0);
    }
    return yday;
}

Time::Time() : year(0), month(0), day(0), hour(0), minute(0), sec(0), msec(0) {}
Time::Time(const char *s) : Time() {
    String str(s);
    bool neg = false;
    int32_t val = 0;
    sforeachc(str) {
        if (E_ == '-') neg = true;
        if (Char::isNumeric(E_.cstr())) val = val*10 + Char::toNum(E_.cstr());
        else {
            if (E_ == "Y" || E_ == "y" || E_ == u8"年") { year = (neg?-1:1)*val; val = 0; }
            else if (E_ == "M" || E_ == u8"月") { month = (neg?-1:1)*val; val = 0; }
            else if (E_ == "W" || E_ == "w") { day = (neg?-1:1)*7*val; val = 0; }
            else if (E_ == "D" || E_ == "d" || E_ == u8"日") { day = (neg?-1:1)*val; val = 0; }
            else if (E_ == "H" || E_ == "h" || E_ == u8"時") { hour = (neg?-1:1)*val; val = 0; }
            else if (E_ == u8"分") { minute = (neg?-1:1)*val; val = 0; }
            else if (E_ == "s" || E_ == u8"秒") { sec = (neg?-1:1)*val; val = 0; }
            else if (E_ == "m") {
                if (it < str.uend() && (*(it+1)) == "s") { msec = (neg?-1:1)*val; val = 0; NEXT_; }
                else { minute = (neg?-1:1)*val; val = 0; }
            }
            else continue;
        }
    }
}
Time::Time(const Time &time) : year(time.year), month(time.month), day(time.day),
hour(time.hour), minute(time.minute), sec(time.sec), msec(time.msec) {}
Time::~Time() {}

Time &Time::operator=(const Time &time) {
    year = time.year; month = time.month; day = time.day; hour = time.hour;
	minute = time.minute; sec = time.sec; msec = time.msec; return *this;
}
Time &Time::operator+=(const Time &time) {
    year += time.year; month += time.month; day += time.day; hour += time.hour;
	minute += time.minute; sec += time.sec; msec += time.msec; return *this;
}
Time &Time::operator-=(const Time &time) {
    year -= time.year; month -= time.month; day -= time.day; hour -= time.hour;
	minute -= time.minute; sec -= time.sec; msec -= time.msec; return *this;
}
Time Time::operator+(const Time &time) { return Time(*this)+=time; }
Time Time::operator-(const Time &time) { return Time(*this)-=time; }

String Time::toString() const {
    String str;
    if (year) str<<year<<"Y";
    if (month) str<<month<<"M";
    if (day) str<<day<<"D";
    if (hour) str<<hour<<"h";
    if (minute) str<< minute <<"m";
    if (sec) str<<sec<<"s";
    if (msec) str<<msec<<"ms";
    return str;
}
String Time::toJString() const  {
    String str;
    if (year) str<<SNumber(year).toWideString()<<u8"年";
    if (month) str<<SNumber(month).toWideString()<<u8"ヶ月";
    if (day) str<<SNumber(day).toWideString()<<u8"日";
    if (hour) str<<SNumber(hour).toWideString()<<u8"時間";
    if (minute) str<<SNumber(minute).toWideString()<<u8"分";
    if (sec) str<<SNumber(sec).toWideString()<<u8"秒";
    if (msec) str<<SNumber(msec).toWideString()<<u8"㍉秒";
    return str;
}

bool Time::operator<(const Time &time) const {
    if (year != time.year) return year < time.year;
    if (month != time.month) return month < time.month;
    if (day != time.day) return day < time.day;
    if (hour != time.hour) return hour < time.hour;
    if (minute != time.minute) return minute < time.minute;
    if (sec != time.sec) return sec < time.sec;
    if (msec != time.msec) return msec < time.msec;
    return false;
}
bool Time::operator==(const Time &time) const {
    return year == time.year && month == time.month && day == time.day &&
    hour == time.hour && minute == time.minute && sec == time.sec && msec == time.msec;
}
