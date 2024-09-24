#ifndef SAPP_BASIC_H
#define SAPP_BASIC_H
#include "sobj/sdate.h"
#include "sobj/sdict.h"
#include "sio/sfile.h"
#include "sutil/ssys.h"

#ifdef MAC_OS
#define DEFAULT_LOCAL_APPDIR "~/Library/Application Support"
#elif defined(WIN_OS)
#define DEFAULT_LOCAL_APPDIR "~\\AppData\\Local"
#else
#define DEFAULT_LOCAL_APPDIR "~/MyApp"
#endif

namespace slib {
	namespace sapp {
		
		constexpr subyte ERROR_LOG = 0x01;
		constexpr subyte OK_LOG = 0x02;
		constexpr subyte PROGRESS_LOG = 0x04;
		constexpr subyte MSG_LOG = 0x08;
		constexpr subyte FILE_LOG = 0x10;
		constexpr subyte STDOUT_LOG = 0x20;
		constexpr subyte OS_LOG = 0x40;
		
	}
}

#endif