#ifndef SLIB_SDATE_H
#define SLIB_SDATE_H

#include "sbasic/time.h"
#include "sobj/sobject.h"

namespace slib {
    class SOBJ_DLL SDate : public SObject, public Time {
    public:
        static const char *ISO8601;
        static const char *YMD;
        static const char *YMD_J;
        static const char *HMS;
        static const char *HMS_J;
        static const char *YMDHMS;
        static const char *YMDHMS_J;
        static const char *EMAIL;
        
    private:
		void _autoform(const char* s);
        void _parse(const String &date);
        
    protected:
        subyte _tzone;
        String _format;
        
    public:
        subyte week;
        
    public:
        SDate(const char *f = ISO8601);
        SDate(const char *s, const char *f);
        SDate(const sobj &obj);
        SDate(const SDate &date);
        ~SDate();
        
        SDate &operator = (const char *s);
        SDate &operator = (const sobj &obj);
        SDate &operator = (const SDate &date);
        SDate &operator += (const Time &time);
        SDate &operator -= (const Time &time);
        SDate operator + (const Time &time) const;
        SDate operator - (const Time &time) const;
        Time operator - (const SDate &date) const;
        
        static SDate utc(const char *f = ISO8601);
        
        const String format() const;
        void setFormat(const char *s);
        
        bool isSumTime() const;
        int lag() const;
        String lagStr() const;
        
        String getClass() const;
        String toString() const;
        SObject *clone() const;
        
        bool operator < (const sobj &obj) const;
        bool operator < (const SDate &time) const;
        bool operator == (const sobj &obj) const;
        bool operator == (const SDate &time) const;
        bool operator != (const sobj &obj) const;
        bool operator != (const SDate &time) const;
    };
}
#endif
