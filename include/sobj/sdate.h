#ifndef SLIB_SDATE_H
#define SLIB_SDATE_H
#include "sbasic/time.h"
#include "sobj/sobject.h"
namespace slib {
	class SLIB_DLL SObjPtr;
	class SLIB_DLL SElement;
	/**
	* @class SDate
	* \~english @brief Date class
	* \~english @brief 日時クラス
	*/
	class SLIB_DLL SDate : public Time, public SObject {
	public:
	private:
		const char *_autoform(const char* s);
		void _parse(const String& date, const char *form);
		void _dadjust();
		void _init();

	public:
		sbyte mday, wday;
		subyte tzone;

	public:
		SDate();
		SDate(const sinteger i);
		SDate(const char* s, const char* f = nullptr);
		SDate(const SObjPtr& obj);
		SDate(const SElement& elem);
		SDate(const SDate& date);
		~SDate();
		static SDate utc();
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
		sinteger integer() const;
		int lag() const;
		String lagStr() const;
		//bool operator < (const sobj& obj) const;
		bool operator < (const SDate& time) const;
		//bool operator == (const sobj& obj) const;
		bool operator == (const SDate& time) const;
		//bool operator != (const sobj& obj) const;
		bool operator != (const SDate& time) const;
		
		void clear();
		String getClass() const;
		String toString(const char* format = sstyle::ISO8601) const;
		SObject* clone() const;
	};
}
#endif