#include "sbasic/exception.h"
#include "sobj/stext.h"
#include "sio/sio.h"

using namespace slib;
#ifdef WIN_OS
extern "C" {
	auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode = 0;
	auto console_get = GetConsoleMode(handle, &mode);
	auto console_set = SetConsoleMode(handle, mode|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif
SException::SException(const char *f, sint l, const char *func, sint e, const char *target, const char *note) :
prefix("slib"), file(f), line(l), function(func), err(e) {
    switch (err) {
        case SLIB_NULL_ERROR:
		{
#ifdef LANG_JA
			message = u8"Null値エラー";
			description << target << u8" は null です。";
#else
			message = "Null value error.";
			description << target << " is null value.";

#endif
			break;
		}
		case SLIB_CAST_ERROR:
		{
#ifdef LANG_JA
			message = u8"値の型変換エラー";
			description << target << u8" は型変換 " + String(note ? note : "") + u8" に対応していません。";
#else
			message = "Cast error.";
			description << target << u8" is unable to cast " + String(note ? note : "")+u8".";
#endif
			break;
		}
		case SLIB_RANGE_ERROR:
		{
#ifdef LANG_JA
			message = u8"範囲外値指定エラー";
			description << target << u8" は適正な範囲 " + String(note ? note : "") << " から外れた値です。";
#else
			message = "Out of range error.";
			description  << target << u8" is out of range " + String(note ? note : "");
#endif
			break;
		}
		case SLIB_FORMAT_ERROR:
		{
#ifdef LANG_JA
			message = u8"フォーマットエラー";
			description << "'" << target << u8"' は形式 '" + String(note ? note : "") + u8"' に適合しません。";
#else
			message = "Format error.";
			description << "'" << target << u8"' does not match format '" + String(note ? note : "")+u8"'";
#endif
			break;
		}
		case SLIB_NOT_FOUND_ERROR:
		{
#ifdef LANG_JA
			message = u8"不一致エラー";
			description << target << u8" は '" + String(note ? note : "") + u8"' の中には見つかりませんでした。";
#else
			message = "Not found error.";
			description << target << u8" was not found in '" + String(note ? note : "") + u8"'";
#endif
			break;
		}
		case SLIB_READ_ERROR:
		{
			message = "Read failed.";
			description = TARGET_TEXT(target) + u8" was unable to read to '" + String(note ? note : "") + u8"'";
			break;
		}
		case SLIB_WRITE_ERROR:
		{
			message = "Write failed.";
			description = TARGET_TEXT(target) + u8" was unable to write to '" + String(note ? note : "") + u8"'";
			break;
		}
		case SLIB_EXEC_ERROR:
		{
			message = "Process error.";
			description = TARGET_TEXT(target) + u8" was not executed successfully (" + String(note ? note : "") + u8")";
			break;
		}
		case SLIB_CONFLICT_ERROR:
		{
			message = "Conflict has occurred.";
			description = TARGET_TEXT(target) + u8" conflicts (" + String(note ? note : "") + u8").";
			break;
		}
		case SLIB_CREATE_PROCESS_ERR:
		{
			message = "Process creation error.";
			description = TARGET_TEXT(target) + u8" was failed.";
			break;
		}
		case SLIB_CREATE_PIPE_ERR:
		{
			message = "Pipe creation error.";
			description = TARGET_TEXT(target) + u8" was failed (" + String(note ? note : "") + u8").";
			break;
		}
		case SLIB_SHARED_MEMORY_ALLOC_ERR:
		{
			message = "Shared memory error.";
			description = TARGET_TEXT(target) + u8" was failed.";
			break;
		}
        default:
            break;
    }
}
SException::~SException() {}

sint SException::error() { return err; }
String SException::toString() const {
	String err_str = String(prefix) << " error! [#" << err << "]: " <<
		message << NEW_LINE << "> " << description << NEW_LINE << 
		" @ '" << function << "'" << NEW_LINE << " in { '" << file << "' l." << line << " }";
	return err_str;
}
void SException::print() const {
	std::cout << SColorText("red", toString()) << std::endl;
}