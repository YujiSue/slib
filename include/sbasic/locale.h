#ifndef SLIB_LOCALE_H
#define SLIB_LOCALE_H

#include "sbasic/map.h"

namespace slib {
	namespace slocale {
		typedef std::pair<subyte, stringarray> TZ;
		const Map<subyte, stringarray> TIME_ZONE_NAME = {
			TZ(0, { "UTC", "GMT", "WET" }), TZ(0x80, { "BST", "WEST" }),
			TZ(0x01 << 2, { "CET" }), TZ(0x80 | (0x01 << 2), { "CEST" }),
			TZ(0x02 << 2, { "EET" }), TZ(0x80 | (0x02 << 2), { "EEST" }),
			TZ(0x03 << 2, { "MSK", "FET", "SYOT" }), TZ((0x05 << 2) | 0x02, { "IST" }),
			TZ(0x08 << 2, { "CST" }), TZ(0x09 << 2, { "JST" }),
			TZ((0x09 << 2) | 0x02, { "ACST" }), TZ(0x80 | (0x0A << 2) | 0x02, { "ACDT" }),
			TZ(0x40 | (0x04 << 2), { "AST" }), TZ(0x80 | 0x40 | (0x04 << 2), { "ADT" }),
			TZ(0x40 | (0x05 << 2), { "EST" }), TZ(0x80 | 0x40 | (0x05 << 2), { "EDT" }),
			TZ(0x40 | (0x06 << 2), { "CST" }), TZ(0x80 | 0x40 | (0x06 << 2), { "CDT" }),
			TZ(0x40 | (0x07 << 2), { "MST" }), TZ(0x80 | 0x40 | (0x07 << 2), { "MDT" }),
			TZ(0x40 | (0x08 << 2), { "PST" }), TZ(0x80 | 0x40 | (0x08 << 2), { "PDT" }),
			TZ(0x40 | (0x0A << 2), { "HST" }), TZ(0x80 | 0x40 | (0x0A << 2), { "HDT" })
		};
		typedef std::pair<String, subyte> TZi;
		const Map<String, subyte> TIME_ZONE_IDX = {
			TZi("UTC", 0), TZi("GMT", 0), TZi("WET", 0),
			TZi("BST", 0x80), TZi("WEST", 0x80),
			TZi("CET", 0x01 << 2), TZi("CEST", 0x80 | (0x01 << 2)),
			TZi("EET", 0x02 << 2), TZi("EEST", 0x80 | (0x02 << 2)),
			TZi("MSK", 0x03 << 2), TZi("FET", 0x03 << 2),
			TZi("SYOT", 0x03 << 2), TZi("IST", (0x05 << 2) | 0x02),
			TZi("CST", 0x08 << 2), TZi("JST", 0x09 << 2),
			TZi("ACST", (0x09 << 2) | 0x02), TZi("ACDT", 0x80 | (0x0A << 2) | 0x02),
			TZi("AST", 0x40 | (0x04 << 2)), TZi("ADT", 0x80 | 0x40 | (0x04 << 2)),
			TZi("EST", 0x40 | (0x05 << 2)), TZi("EDT", 0x80 | 0x40 | (0x05 << 2)),
			TZi("CST", 0x40 | (0x06 << 2)), TZi("CDT", 0x80 | 0x40 | (0x06 << 2)),
			TZi("MST", 0x40 | (0x07 << 2)), TZi("MDT", 0x80 | 0x40 | (0x07 << 2)),
			TZi("PST", 0x40 | (0x08 << 2)), TZi("PDT", 0x80 | 0x40 | (0x08 << 2)),
			TZi("HST", 0x40 | (0x0A << 2)), TZi("HDT", 0x80 | 0x40 | (0x0A << 2))
		};
		extern SLIB_DLL std::pair<slib::String, int> JPYear(int year);
		extern SLIB_DLL int ADYear(const char* s, int year);
	}
	class SLIB_DLL Locale {
		static String _locale;
		static sattribute _translator;
	public:
		static void setLocale(const char* s);
		static void load(const char* s);
		static void save(const char* s);
		static void add(const char* k, const char* l);
		static void append(std::initializer_list<std::pair<slib::String, slib::String>> li);
		static void set(std::initializer_list<std::pair<slib::String, slib::String>> li);
		static void remove(const char* k);
		static String localize(const char* s);
	};
}

#endif
