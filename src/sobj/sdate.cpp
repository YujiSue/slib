#include "sbasic/tuple.h"
#include "sobj/sobjptr.h"
#include "sobj/sdate.h"
#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sarray.h"


slib::SDate::SDate() : Time(), SObject(), mday(0), wday(0), tzone(0) {
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
	Time::day = sdt::yearday(Time::year, Time::month, mday);
	Time::hour = ltime->tm_hour;
	Time::minute = ltime->tm_min;
	Time::second = ltime->tm_sec;
	if (ltime->tm_isdst) tzone |= 0x80;
	const struct tm* gmtime = std::gmtime(&now);
	auto hm_l = ltime->tm_hour * 60 + ltime->tm_min,
		hm_g = gmtime->tm_hour * 60 + gmtime->tm_min;
	auto yd_l = sdt::TOTAL_DAY_OF_MONTH[ltime->tm_mon] + ltime->tm_mday,
		yd_g = sdt::TOTAL_DAY_OF_MONTH[gmtime->tm_mon] + gmtime->tm_mday;
	if (ltime->tm_year != gmtime->tm_year) {
		if (ltime->tm_year < gmtime->tm_year) yd_g += sdt::isLeapYear(ltime->tm_year) ? 366 : 365;
		else yd_l += sdt::isLeapYear(ltime->tm_year) ? 366 : 365;
	}
	if (yd_l < yd_g) hm_g += 1440;
	else if (yd_g < yd_l) hm_l += 1440;
	auto lag = hm_l - hm_g;
	if (lag < 0) { tzone |= 0x40; lag = -lag; }
	tzone |= ((lag % 60) / 15) | ((lag / 60) << 2);
}
inline slib::Tuple<int, slib::String, slib::String> _divider(slib::String& format) {
	if (format == slib::sstyle::YMD)
		return slib::Tuple<int, slib::String, slib::String>(3, "/(\\d{4})/(\\d{2})/(\\d{2})/", "Y4M2D");
	else if (format == slib::sstyle::HMS)
		return slib::Tuple<int, slib::String, slib::String>(3, "/(\\d{2}):(\\d{2}):(\\d{2})/", "Y4M2D");
	else if (format == slib::sstyle::YMDHMS)
		return slib::Tuple<int, slib::String, slib::String>(6, "/(\\d{4})/(\\d{2})/(\\d{2})\\s+(\\d{2}):(\\d{2}):(\\d{2})/", "Y4M2D");
	else if (format == slib::sstyle::EMAIL)
		return slib::Tuple<int, slib::String, slib::String>(8, "/([a-zA-Z]{3}),\\s+(\\d+)\\s+([a-zA-Z]{3})\\s+(\\d{4})\\s+(\\d{2}):(\\d{2}):(\\d{2})\\s+([0-9+-]{5})/", "WDM3Y4HmsL");
	else if (format == slib::sstyle::ISO8601)
		return slib::Tuple<int, slib::String, slib::String>(7, "/([\\d]{4})-(\\d+)-(\\d+)T(\\d+):(\\d+):(\\d+)([0-9+-]+)/", "Y4M2DHmsL");
	else {
		slib::String reg("/"), order;
		int count, part = 0;
		sforc(format) {
			if ($_ == "Y") {
				count = format.count("Y");
				reg << "(\\d{2,4})";
				order << "Y" << count;
				++part;
				$ += count - 1;
			}
			else if ($_ == "M") {
				count = format.count("M");
				if (count <= 2) reg << "(\\d+)";
				else reg << "([a-zA-Z]{3,})";
				order << "M" << count;
				++part;
				$ += count - 1;
			}
			else if ($_ == "D") {
				count = format.count("D");
				reg << "(\\d+)";
				order << "D";
				++part;
				$ += count - 1;
			}
			else if ($_ == "W") {
				count = format.count("W");
				reg << "([a-zA-Z]{3,})";
				order << "W";
				++part;
				$ += count - 1;
			}
			else if ($_ == "a") {
				reg << "([ap]m)";
				order << "a";
				++part;
			}
			else if ($_ == "H") {
				count = format.count("H");
				reg << "(\\d+)";
				order << "H";
				++part;
				$ += count - 1;
			}
			else if ($_ == "h") {
				count = format.count("h");
				reg << "(\\d+)";
				order << "h";
				++part;
				$ += count - 1;
			}
			else if ($_ == "m") {
				count = format.count("m");
				reg << "(\\d+)";
				order << "m";
				++part;
				$ += count - 1;
			}
			else if ($_ == "s") {
				count = format.count("s");
				reg << "(\\d+)";
				order << "s";
				++part;
				$ += count - 1;
			}
			else if ($_ == "L") {
				reg << "([+-]*[0-9]{4})";
				order << "L";
				++part;
			}
			else if ($_ == "Z") {
				reg << "([A-Z]{3,})";
				order << "Z";
				++part;
			}
			// Wide char / Japanese
			else if ($_ == u8"号" && $_NEXT == u8"Ｙ") {
				reg << "(.+)";
				order << u8"号";
				++part;
				$NEXT;
			}
			else if ($_ == u8"陰" && $_NEXT == u8"月") {
				reg << u8"(.+)";
				order << u8"陰";
				++part;
				$NEXT;
			}
			else if ($_ == u8"曜") {
				reg << u8"(.+)";
				order << u8"曜";
				++part;
			}
			else if ($_ == u8"午") {
				reg << u8"(午.+)";
				order << u8"午";
				++part;
			}
			else if ($_ == " " && reg[-1] != ' ') reg << " ";
			else if ($_ == "(" || $_ == ")" ||
				$_ == "{" || $_ == "}") reg << "[(){}]{1}";
			else reg << $_;
		}
		reg.replace(" ", "\\s+");
		reg << "/";
		return slib::Tuple<int, slib::String, slib::String>(part, reg, order);
	}
}
slib::SDate::SDate(const char* s, const char* f) : SDate() {
	String format(f ? S(f) : sdt::format(s));
	auto div = _divider(format);

	Regex reg(div.at<1>());
	auto matches = reg.search(s);
	if (matches.begin == matches.end)
		throw FormatException(formatErrorText("Date string", s, div.at<1>()));

	if (matches.begin->size() != (size_t)div.at<0>() + 1)
		throw RangeException(outRangeErrorText("Matched count", matches.begin->size() - 1, div.at<0>(), div.at<0>()));

	auto it = matches.begin->begin() + 1;
	auto order = div.at<2>().u8begin();
	bool pm = false;
	while (it < matches.begin->end()) {
		if (*order == "Y") {
			++order;
			if (*order == "2") Time::year = std::stoi($_.str()) + 2000;
			else Time::year = std::stoi($_.str());
		}
		else if (*order == "M") {
			++order;
			if (*order == "1" || *order == "2") Time::month = std::stoi($_.str());
			else Time::month = sdt::monthIndex($_.str());
		}
		else if (*order == "D") {
			mday = std::stoi($_.str());
		}
		else if (*order == "W") {
			wday = sdt::weekIndex($_.str());
		}
		else if (*order == "H") {
			Time::hour = std::stoi($_.str());
		}
		else if (*order == "a") {
			++order;
			pm = $_.str() == "pm" ? true : false;
		}
		else if (*order == "h") {
			++order;
			Time::hour = std::stoi($_.str()) + pm ? 12 : 0;
		}
		else if (*order == "m") {
			Time::minute = std::stoi($_.str());
		}
		else if (*order == "s") {
			Time::second = std::stoi($_.str());
		}
		else if (*order == "L") {
			auto lag = S($_.str());
			if (lag[0] == '-') tzone |= 0x40;
			auto h = lag.substring(1, 2).intValue(), m = lag.substring(3).intValue();
			tzone |= (m / 15) | (h << 2);
		}
		else if (*order == "Z") {
			if (!tzone) tzone = sdt::tzoneCode($_.str());
		}
		++order; $NEXT;
	}
	Time::day = sdt::yearday(year, month, mday);
}

/*
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
*/
template<class Iter>
inline size_t countRepeat(Iter& it, const char* s) {
	size_t count = 0;
	while ($_ == s) { $NEXT; ++count; }
	return count;
}
inline void readSize(slib::String& str, slib::Utf8CIterator& uit, size_t s) {
	str.clear();
	sforin(i, 0_u, s) { str << toString(*uit); ++uit; }
}
inline void readTo(slib::String& str, slib::Utf8CIterator& uit, const slib::Char& c) {
	str.clear();
	while (true) {
		if ((*uit) == "\0" || (*uit).toString() == c.toString()) break;
		str << (*uit).toString(); ++uit;
	}
}
inline slib::String autoform(const slib::String& dtf) {
	if (dtf.match("/")) {
		if (dtf.match(":")) return slib::sstyle::YMDHMS;
		else return slib::sstyle::YMD;
	}
	else if (dtf.match(":")) {
		if (dtf.match(" ")) return slib::sstyle::EMAIL;
		else return slib::sstyle::HMS;
	}
	else if (dtf.match(u8"年")) {
		if (dtf.match(u8"時")) return slib::sstyle::YMDHMS_J;
		else return slib::sstyle::YMD_J;
	}
	else if (dtf.match(u8"時")) return slib::sstyle::HMS_J;
	else return slib::sstyle::ISO8601;
}
void parse(slib::SDate& dt, const slib::String& date, const slib::String& format) {


	bool am = false, jp = false;

	slib::String buffer;
	auto uit = date.u8begin();
	int count;

	sforc(format) {
		if ($_ == "Y") {
			count = countRepeat(it, "Y");
			if (count == 2) { 
				readSize(buffer, uit, count);
				dt.year = buffer.intValue() + 2000;
				--it; --uit;
			}
			else { 
				readTo(buffer, uit, $_);
				dt.year = buffer.intValue(); 
			}
		}
		else if ($_ == "M") {
			auto count = countRepeat(it, "M");
			if (count == 3) { 
				readSize(buffer, uit, count);
				dt.month = slib::sdt::monthIndex(slib::sstr::toLower(buffer)) + 1;
				--it; --uit;
			}
			else if (count == 2) { 
				readSize(buffer, uit, count);
				dt.month = buffer.intValue(); 
				--it; --uit;
			}
			else { 
				readTo(buffer, uit, $_);
				dt.month = buffer.intValue(); 
			}
		}
		else if ($_ == "D") {
			auto count = countRepeat(it, "D");
			if (count == 2) { 
				readSize(buffer, uit, count);
				dt.mday = buffer.intValue();
				--it; --uit;
			}
			else {
				readTo(buffer, uit, $_);
				dt.mday = buffer.intValue();
			}
		}
		else if ($_ == "W") {
			auto count = countRepeat(it, "W");
			if (count == 3) { 
				readSize(buffer, uit, count);
				dt.wday = slib::sdt::weekIndex(slib::sstr::toLower(buffer));
				--it; --uit;
			}
			else { 
				readTo(buffer, uit, $_);
				dt.wday = slib::sdt::weekIndex(slib::sstr::toLower(buffer));
			}
		}
		else if ($_ == "a") {
			readSize(buffer, uit, 2); --it; --uit;
			if (buffer == "am") am = true; 
			else am = false;
		}
		else if ($_ == "h") {
			auto count = countRepeat(it, "h");
			if (count == 2) { 
				readSize(buffer, uit, count); --it; --uit;
			}
			else readTo(buffer, uit, $_);
			dt.hour = buffer.intValue() + (am ? 0 : 12);
		}
		else if ($_ == "H") {
			auto count = countRepeat(it, "H");
			if (count == 2) { readSize(buffer, uit, count); --it; --uit;
			}
			else readTo(buffer, uit, $_);
			dt.hour = buffer.intValue();
		}
		else if ($_ == "m") {
			auto count = countRepeat(it, "m");
			if (count == 2) { readSize(buffer, uit, count); --it; --uit;
			}
			else readTo(buffer, uit, $_);
			dt.minute = buffer.intValue();
		}
		else if ($_ == "s") {
			auto count = countRepeat(it, "s");
			if (count == 2) { readSize(buffer, uit, count); --it; --uit; }
			else readTo(buffer, uit, $_);
			dt.second = buffer.intValue();
		}
		else if ($_ == "Z") {
			readSize(buffer, uit, 5); --it; --uit;
			if (buffer[0] == '+') dt.tzone = 0x40; 
			else dt.tzone = 0x00;
			slib::sbyte t = buffer.substring(1, 2).intValue();
			dt.tzone |= (t << 2);
			t = buffer.substring(3).intValue() / 15;
			dt.tzone |= t;
		}
		else if ($_ == "z") {
			++it;
			readTo(buffer, uit, $_);
			if (!dt.tzone) dt.tzone = slib::sdt::tzoneCode((*uit).toString());
		}
		//Japanese
		else if ($_ == u8"号") {
			readSize(buffer, uit, 2); --uit;
			jp = true;
			dt.year = slib::sdt::adyear(buffer);
		}
		else if ($_ == u8"年") {
			readTo(buffer, uit, $_);
			if (jp) {
				if (buffer == "元") dt.year += 1;
				else dt.year += slib::sstr::toNarrow(buffer).intValue();
			}
			else dt.year = slib::sstr::toNarrow(buffer).intValue();
		}
		else if ($_ == u8"月") {
			readTo(buffer, uit, $_);
			dt.month = slib::sstr::toNarrow(buffer).intValue();
		}
		else if ($_ == u8"曜") {
			readTo(buffer, uit, $_);
			dt.wday = slib::sdt::weekJaIndex(buffer) + 1;
		}
		else if ($_ == u8"日") {
			readTo(buffer, uit, $_);
			dt.mday = slib::sstr::toNarrow(buffer).intValue();
		}
		else if ($_ == u8"時") {
			readTo(buffer, uit, $_);
			dt.hour = slib::sstr::toNarrow(buffer).intValue();
		}
		else if ($_ == u8"分") {
			readTo(buffer, uit, $_);
			dt.minute = slib::sstr::toNarrow(buffer).intValue();
		}
		else if ($_ == u8"秒") {
			readTo(buffer, uit, $_);
			dt.second = slib::sstr::toNarrow(buffer).intValue();
		}
		++uit;
	}
	if (!dt.day) dt.day = slib::sdt::yearday(dt.year, dt.month, dt.mday);
	if(!dt.wday) dt.wday = slib::sdt::weekday(dt.year, dt.month, dt.mday);
}

void slib::SDate::_dadjust() {
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
			mday += sdt::DAY_OF_MONTH[Time::month - 1];
		}
	}
	if (Time::month < 1) { while (Time::month < 1) { --Time::year; Time::month += 12; }}
	else if (12 < Time::month) { while (12 < Time::month) { ++Time::year; Time::month -= 12; }}
	if (sdt::DAY_OF_MONTH[Time::month - 1] < mday) {
		while (sdt::DAY_OF_MONTH[Time::month - 1] < mday) {
			mday -= sdt::DAY_OF_MONTH[Time::month - 1]; ++Time::month;
			if (Time::month < 1) { while (Time::month < 1) { --Time::year; Time::month += 12; } }
			else if (12 < Time::month) { while (12 < Time::month) { ++Time::year; Time::month -= 12; } }
		}
	}
	Time::day = sdt::yearday(year, month, mday);
	if (!wday) wday = sdt::weekday(year, month, mday);
}
void slib::SDate::_init() { Time::init(); tzone = 0xFF; mday = 0; wday = 0; }

slib::SDate::SDate(const sinteger ui) : SDate() {
	suint hms = ui & 0xFFFFFFFF, ymd = (ui >> 32);
	mday = (ymd & 0x1F);
	month = ((ymd >> 5) & 0x0F);
	year = (ymd >> 9);
	day = sdt::yearday(year, month, mday);
	wday = sdt::weekday(year, month, mday);
	second = hms & 0x3F;
	minute = (hms >> 6) & 0x3F;
	hour = (hms >> 12);
}
slib::SDate::SDate(const slib::SObjPtr& obj) : SDate() {
	if (obj.isDate()) *this = SDate(obj.date());
	else *this = SDate((const char*)obj);
}
slib::SDate::SDate(const slib::SElement& elem) : SDate() {
	if (elem.isDate()) *this = SDate(elem.date());
	else *this = SDate((const char*)elem);
}
slib::SDate::SDate(const slib::SDate& date) : Time(date), SObject(), mday(date.mday), wday(date.wday), tzone(date.tzone) {}
slib::SDate::~SDate() {}
slib::SDate slib::SDate::utc() {
	SDate date;
	time_t now = std::time(nullptr);
	const struct tm* gmtime = std::gmtime(&now);
	date.tzone = 0;
	date.year = gmtime->tm_year + 1900;
	date.month = gmtime->tm_mon + 1;
	date.wday = gmtime->tm_wday + 1;
	date.mday = gmtime->tm_mday;
	date.day = sdt::yearday(date.year, date.month, date.mday);
	date.hour = gmtime->tm_hour;
	date.minute = gmtime->tm_min;
	date.second = gmtime->tm_sec;
	return date;
}
//SDate& SDate::operator = (const char* s) { _parse(s, _autoform(s)); return *this; }
slib::SDate& slib::SDate::operator = (const slib::SDate& date) {
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
slib::SDate& slib::SDate::operator += (const slib::Time& time) {
	Time::year += time.year;
	Time::month += time.month;
	mday += time.day;
	Time::hour += time.hour;
	Time::minute += time.minute;
	Time::second += time.second;
	_dadjust();
	return *this;
}
slib::SDate& slib::SDate::operator -= (const slib::Time& time) {
	Time::year -= time.year;
	Time::month -= time.month;
	mday -= time.day;
	Time::hour -= time.hour;
	Time::minute -= time.minute;
	Time::second -= time.second;
	_dadjust();
	return *this;
}
slib::SDate slib::SDate::operator + (const slib::Time& time) const { return SDate(*this) += time; }
slib::SDate slib::SDate::operator - (const slib::Time& time) const { return SDate(*this) -= time; }
slib::Time slib::SDate::operator - (const SDate& date) const {
	if (integer() < date.integer()) return -(date - (*this));
	slib::Time dt;
	sint s1 = hour * 3600 + minute * 60 + second + (tzone < 0xFF ? lag() : 0) * 60,
		s2 = date.hour * 3600 + date.minute * 60 + date.second + (date.tzone < 0xFF ? date.lag() : 0) * 60,
		d = mday, m = month, y = year;
	if (s1 < s2) {
		if (d == 1) {
			if (m == 1) { m = 12; --y; d += sdt::DAY_OF_MONTH[m - 1]; }
			else m--;
			d += sdt::DAY_OF_MONTH[m - 1];
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
			d += sdt::DAY_OF_MONTH[m - 1];
		}
	}
	dt.day = d - date.mday;
	dt.month = m - date.month;
	dt.year = y - date.year;
	while (dt.month < 0) { dt.month += 12; --dt.year; }
	return dt;
}
bool slib::SDate::summerTime() const { return tzone & 0x80; }
slib::suint slib::SDate::ymd() const { suint val = 0; val |= mday | (month << 5) | (year << 9); return val; }
slib::suint slib::SDate::hms() const { suint val = 0; val |= second | (minute << 6) | (hour << 12); return val; }
slib::sinteger slib::SDate::integer() const { auto val = ((sinteger)ymd() << 32); val |= hms(); return val; }
int slib::SDate::lag() const {
	return (tzone & 0x40 ? -1 : 1) * ((tzone & 0x03) * 15 + ((tzone >> 2) & 0x0F)) * 60 + (tzone & 0x80 ? 60 : 0); 
}
slib::String slib::SDate::lagStr() const {
	auto l = lag();
	return S(l < 0 ? "-" : "+") <<
		sstr::lfill(S(abs(l) / 60), '0', 2) <<
		sstr::lfill(S(abs(l) % 60), '0', 2);
}
void slib::SDate::clear() {
	Time::year = 0;
	Time::month = 0;
	Time::day = 0;
	Time::hour = 0;
	Time::minute = 0;
	Time::second = 0;
	mday = 0;
	wday = 0;
	tzone = 0;
}
slib::String slib::SDate::getClass() const { return "date"; }
slib::String slib::SDate::toString(const char *form) const {
	String format(form ? form : sstyle::ISO8601), str;
	int count;
	sforc(format) {
		if ($_ == "Y") {
			count = format.count("Y");
			if (count == 2) str << sstr::lfill(S(Time::year - 2000), '0', 2);
			else if (count == 4) str << sstr::lfill(S(Time::year), '0', 4);
			else str << S(Time::year);
			$ += count - 1;
		}
		else if ($_ == "M") {
			count = format.count("M");
			if (count == 1) str << S(Time::month);
			else if (count == 2) str << sstr::lfill(S(Time::month), '0', 2);
			else if (count == 3) str << sdt::monthLabel(Time::month - 1, true);
			else str << sdt::monthLabel(Time::month - 1);
			$ += count - 1;
		}
		else if ($_ == "D") {
			count = format.count("D");
			if (count == 1) str << S(mday);
			else if (count == 2) str << sstr::lfill(S(mday), '0', 2);
			else str << S(Time::day);
			$ += count - 1;
		}
		else if ($_ == "W") {
			count = format.count("W");
			if (count == 3) str << sdt::weekLabel(wday, true);
			else str << sdt::weekLabel(wday);
			$ += count - 1;
		}
		else if ($_ == "H") {
			count = format.count("H");
			if (count == 2) str << sstr::lfill(S(Time::hour), '0', 2);
			else str << S(Time::hour);
			$ += count - 1;
		}
		else if ($_ == "a") {
			str << (Time::hour < 12 ? "am" : "pm");
		}
		else if ($_ == "h") {
			count = format.count("h");
			if (count == 2) str << sstr::lfill(S(Time::hour < 12 ? Time::hour : Time::hour - 12), '0', 2);
			else str << S(Time::hour < 12 ? Time::hour : Time::hour - 12);
			$ += count - 1;
		}
		else if ($_ == "m") {
			count = format.count("m");
			if (count == 2) str << sstr::lfill(S(Time::minute), '0', 2);
			else str << S(Time::minute);
			$ += count - 1;
		}
		else if ($_ == "s") {
			count = format.count("s");
			if (count == 2) str << sstr::lfill(S(Time::second), '0', 2);
			else str << S(Time::second);
			$ += count - 1;
		}
		else if ($_ == "L") { str << lagStr(); }
		else if ($_ == "Z") { str << sdt::tzone(tzone)[0]; }
		// Wide char / Japanese
		else if ($_ == u8"号") {
			auto jy = sdt::eraJa(year);
			str << jy.first;
		}
		else if ($_ == u8"Ｙ") {
			auto jy = sdt::eraJa(year);
			str << (jy.second == 1 ? u8"元" : sstr::toWide(S(jy.second)));
		}
		else if ($_ == u8"陰" && $_NEXT == u8"月") {
			str << sdt::monthJaLabel(month - 1);
			$NEXT;
		}
		else if ($_ == u8"午") { str << (hour < 12 ? u8"午前" : u8"午後"); }
		else str << $_;
	}
	return str;
}
slib::SObject* slib::SDate::clone() const { return new SDate(*this); }
/*
bool slib::SDate::operator < (const sobj& obj) const {
	if (obj.isDate()) return (*this) < obj.date();
	return getClass() < obj->getClass();
}
*/
bool slib::SDate::operator < (const slib::SDate& time) const {
	return *((slib::Time*)this) < *((const Time*)&time);
}
/*
bool slib::SDate::operator == (const sobj& obj) const {
	if (obj.isDate()) return (*this) == obj.date();
	return false;
}
*/
bool slib::SDate::operator == (const slib::SDate& time) const { return tzone == time.tzone && *((Time*)this) == time; }
//bool slib::SDate::operator != (const sobj& obj) const { return !((*this) == obj); }
bool slib::SDate::operator != (const slib::SDate& time) const { return !((*this) == time); }
std::ostream& operator<<(std::ostream& os, const slib::SDate& date) {
	return os << date.toString(slib::sstyle::ISO8601);
}