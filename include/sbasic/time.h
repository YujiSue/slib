#ifndef SLIB_TIME_H
#define SLIB_TIME_H

#include "sconfig.h"
#include "sbasic/string.h"
#include "sbasic/map.h"

namespace slib {
	namespace speriod {
		const stringarray ERA_J = { u8"明治", u8"大正", u8"昭和", u8"平成", u8"令和" };
		const intarray ERA_J_YEAR = { 1867, 1911, 1925, 1988, 2018 };
		const stringarray MONTH_LABEL = {
			"January", "February", "March", "April", "May", "June",
			"July", "August", "September", "October", "November", "December"
		};
		const stringarray MONTH_LABEL_J = {
			u8"睦月", u8"如月", u8"弥生", u8"卯月", u8"皐月", u8"水無月",
			u8"文月", u8"葉月", u8"菊月", u8"神無月", u8"霜月", u8"師走"
		};
		const stringarray MONTH_LABEL_TRI = {
			"Jan", "Feb", "Mar", "Apr", "May", "Jun",
			"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
		};
		const sindex MONTH_INDEX = {
			ki("jan", 1), ki("feb", 2), ki("mar", 3), ki("apr", 4), ki("may", 5), ki("jun", 6),
			ki("jul", 7), ki("aug", 8), ki("sep", 9), ki("oct", 10), ki("nov", 11), ki("dec", 12)
		};
		const stringarray WEEK_LABEL = {
			"Monday", "Tuesday", "Wednesday",
			"Thursday", "Friday", "Saturday", "Sunday"
		};
		const stringarray WEEK_LABEL_J = {
			u8"月", u8"火", u8"水", u8"木", u8"金", u8"土", u8"日"
		};
		const stringarray WEEK_LABEL_TRI = {
			"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
		}; 
		const sindex WEEK_INDEX = {
			ki("mon", 0), ki("tue", 1), ki("wed", 2), ki("thu", 3), ki("fri", 4), ki("sat", 5), ki("sun", 6)
		};
		const intarray DAY_OF_MONTH = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		const intarray TOTAL_DAY_OF_MONTH = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
	}

    struct SLIB_DLL Time {
	private:
		sinteger _totalsec();
		sinteger _totaldays();
		void _tadjust();

	public:
		static bool isLeapYear(int y);

    public:
		sint year, month, day, hour, minute, second;
        
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
        String toString() const;
        String toJString() const;        
        bool operator<(const Time &time) const;
        bool operator==(const Time &time) const;
    };
}

#endif
