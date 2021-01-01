#include "sio/sfile.h"

slib::sio::SIOException::SIOException(const char* f, sint l, const char* func, sint e, const char* target, const char* note) 
	: slib::SException(f, l, func, e, target, note) { 
	prefix = "sio";
	switch (err) {
	case FILE_NOT_EXIST_ERROR:
	{
		message = "File not exist error.";
		description = TARGET_TEXT(target) + u8" does not exist.";
		break;
	}
	case FILE_OPEN_ERROR:
	{
		message = "File open error.";
		description = TARGET_TEXT(target) + u8" is unable to open.";
		break;
	}
	case DIRECTORY_LOAD_ERROR:
	{
		message = "Directory load error.";
		description = TARGET_TEXT(target) + u8" is unable to load.";
	}
	default:
		break;
	}
}
slib::sio::SIOException::~SIOException() {}