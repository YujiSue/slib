#ifndef SLIB_TIME_H
#define SLIB_TIME_H

#include "sconfig.h"
#include "sbasic/string.h"
#include "sbasic/map.h"

namespace slib {
    class SOBJ_DLL Time {
    public:
        static const Map<subyte, stringarray> TIME_ZONE_NAME;
        static const Map<String, subyte> TIME_ZONE_IDX;
        
        static const stringarray TIME_STR;
        static const stringarray TIME_STR_L;
        static const stringarray TIME_STR_J;
        
        static const stringarray ERA_J;
        static const intarray ERA_J_YEAR;
        static const stringarray MONTH_LABEL;
        static const stringarray MONTH_LABEL_J;
        static const stringarray MONTH_LABEL_TRI;
        static const stringarray WEEK_LABEL;
        static const stringarray WEEK_LABEL_J;
        static const stringarray WEEK_LABEL_TRI;
        
        static const intarray DAY_OF_MONTH;
        static const intarray TOTAL_DAY_OF_MONTH;
        
        static bool isLeapYear(int y);
        static std::pair<String, int> JYear(int year);
        static int CYear(const char *s, int year);
        static int yday(int mday, int month, bool leap = false);
        static int mday(int yday, bool leap = false);
        
        sint year, month, day, hour, minute, sec, msec;
        
        Time();
        Time(const char *s);
        Time(const Time &time);
        ~Time();
        
        Time &operator=(const Time &time);
        Time &operator+=(const Time &time);
        Time &operator-=(const Time &time);
        Time operator+(const Time &time);
        Time operator-(const Time &time);
        
        String toString() const;
        String toJString() const;
        
        bool operator<(const Time &time) const;
        bool operator==(const Time &time) const;
    };
}

#endif
