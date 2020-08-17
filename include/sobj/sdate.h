#ifndef SLIB_SDATE_H
#define SLIB_SDATE_H

#include "sbasic/time.h"
#include "sobj/sobject.h"

namespace slib {
    class SLIB_DLL SDate : public SObject, public Time {
    public:
    private:
		void _autoform(const char* s);
        void _parse(const String &date);
        
    protected:
		String _format;
        subyte _tzone;

    public:
        subyte week;
        
    public:
        SDate();
        SDate(const char *s, const char *f = nullptr);
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
        static SDate utc(const char *f = slib::sstyle::ISO8601);
        bool summerTime() const;
        int lag() const;
        String lagStr() const;
        String getClass() const;
        String toString() const;
		String toString(const char *form) const;
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
