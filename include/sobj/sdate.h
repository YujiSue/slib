#ifndef SLIB_SDATE_H
#define SLIB_SDATE_H

#include "sbasic/time.h"
#include "sobj/sobject.h"

namespace slib {
	class SLIB_DLL SDate : public SObject, public Time {
	public:
	private:
		const char *_autoform(const char* s);
		void _parse(const String& date, const char *form);
		void _adjust();

	public:
		subyte mday, wday, tzone;

	public:
		SDate();
		SDate(suint ui);
		SDate(suinteger ui);
		SDate(const char* s, const char* f = nullptr);
		SDate(const sobj& obj);
		SDate(const SDate& date);
		~SDate();
		static SDate utc();
		void init();
		SDate& operator = (const char* s);
		SDate& operator = (const SDate& date);
		SDate& operator += (const Time& time);
		SDate& operator -= (const Time& time);
		SDate operator + (const Time& time) const;
		SDate operator - (const Time& time) const;
		Time operator - (const SDate& date) const;
		bool summerTime() const;
		suint ymd() const;
		suint hms() const;
		sinteger intValue() const;
		int lag() const;
		String lagStr() const;
		String getClass() const;
		String toString() const;
		String toString(const char* form) const;
		SObject* clone() const;
		bool operator < (const sobj& obj) const;
		bool operator < (const SDate& time) const;
		bool operator == (const sobj& obj) const;
		bool operator == (const SDate& time) const;
		bool operator != (const sobj& obj) const;
		bool operator != (const SDate& time) const;
	};
}
#endif