#include "sapp/sapp.h"

using namespace slib;
using namespace slib::sapp;

SAppException::SAppException(const char* f, sint l, const char* func, sint e, const char* target, const char* note) 
	: SException(f, l, func, e, target, note) { 
	prefix = "sapp"; 
	switch (e) {
	case INSUFFICIENT_ARGS_ERROR:
	{
		message = "Insufficient argment error.";
		description = TARGET_TEXT(target) + u8" requires " + String(note);
		break;
	}
	case INSUFFICIENT_OPT_ERROR:
	{
		message = "Insufficient option error.";
		description = TARGET_TEXT(target) + u8" requires " + String(note);
		break;
	}
	case COMMAND_NOT_EXIST_ERROR:
	{
		message = "Command error.";
		description = TARGET_TEXT(target) + u8" is not defined. ";
		break;
	}
	case OPTION_NOT_EXIST_ERROR:
	{
		message = "Option error.";
		description = TARGET_TEXT(target) + u8" is not defined. ";
		break;
	}
	default:
		break;
	}

}
SAppException::~SAppException() {}

SApp::SApp() {}
SApp::SApp(const char *path) : SApp() {
    try { profile.load(path); }
    catch (sio::SIOException ie) {}
}
SApp::SApp(SDictionary &&prof) : SApp() {
    profile.swap(prof);
}
SApp::~SApp() {}
