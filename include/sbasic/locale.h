#ifndef SLIB_LOCALE_H
#define SLIB_LOCALE_H

#include "sbasic/array.h"
#include "sbasic/map.h"
#include "sbasic/string.h"

namespace slib {
	class SOBJ_DLL Locale : public Map<String, stringarray> {
	protected:
		sindex _lang;

	public:
		Locale(const char **lang);
		~Locale();


		void addWord(const char* key, const char** loc);
		String localize(String& str);

	};
}
#endif