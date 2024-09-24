#ifndef SLIB_TIME_H
#define SLIB_TIME_H

#include "sconfig.h"
#include "sbasic/string.h"
#include "sbasic/map.h"

namespace slib {
	namespace sdt {
		constexpr int DAY_OF_MONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		constexpr int TOTAL_DAY_OF_MONTH[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
		extern SLIB_DLL const stringarray MONTH_LABEL;
		extern SLIB_DLL const stringarray WEEK_LABEL;
		extern SLIB_DLL String monthLabel(const size_t s, bool tri = false);
		extern SLIB_DLL size_t monthIndex(const String& mon);
		extern SLIB_DLL String weekLabel(const size_t s, bool tri = false);
		extern SLIB_DLL size_t weekIndex(const String& wk);
		extern SLIB_DLL bool isLeapYear(int y);
		extern SLIB_DLL int yearday(int y, int m, int d);
		extern SLIB_DLL int weekday(int y, int m, int d);
		extern SLIB_DLL String tzone(subyte tz, int op = 0);
		extern SLIB_DLL subyte tzoneCode(const String& tz);
		extern SLIB_DLL String format(const char *s);
	
		extern SLIB_DLL String monthJaLabel(const size_t s);
		extern SLIB_DLL size_t monthJaIndex(const String& mon);
		extern SLIB_DLL String weekJaLabel(const size_t s);
		extern SLIB_DLL size_t weekJaIndex(const String& wk);
		extern SLIB_DLL Pair<String, size_t> eraJa(const size_t ad_year);
		extern SLIB_DLL int adyear(const char* ja_era, const size_t year = 1);
	}

    class SLIB_DLL Time {
	private:
		sinteger _totalsec();
		sinteger _totaldays();
		void _tadjust();

    public:
		sint year, month, day, hour, minute, second;

	public:
        Time();
        Time(const char *s);
        Time(const Time &time);
        ~Time();
        Time &operator=(const Time &time);
        Time &operator+=(const Time &time);
        Time &operator-=(const Time &time);
        Time operator+(const Time &time);
        Time operator-(const Time &time);
		Time operator-();
		void init();
        String toString(const char *format = nullptr) const;
        bool operator<(const Time &time) const;
        bool operator==(const Time &time) const;
    };
	extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::Time& tm);
}
#endif
