#include "sbasic/exception.h"
#include "sbasic/map.h"
#include "sobj/stext.h"
#include "sio/sio.h"

using namespace slib;

SException::SException(const char *f, sint l, const char *func, sint e, const char *target, const char *note) :
prefix("slib"), file(f), line(l), function(func), err(e) {
    switch (err) {
        case SLIB_NULL_ERROR:
		{
			message = "Null value error.";
			description = TARGET_TEXT(target) + u8" is null value.";
			break;

		}
		case SLIB_CAST_ERROR:
		{
			message = "Cast error.";
			description = TARGET_TEXT(target) + u8" is unable to cast (" + String(note ? note : "")+u8")";
			break;
		}
		case SLIB_RANGE_ERROR:
		{
			message = "Out of range error.";
			description = TARGET_TEXT(target) + u8" is out of range " + String(note ? note : "");
			break;
		}
		case SLIB_FORMAT_ERROR:
		{
			message = "Format error.";
			description = TARGET_TEXT(target) + u8" does not match format '" + String(note ? note : "")+u8"'";
			break;
		}
		case SLIB_NOT_FOUND_ERROR:
		{
			message = "Not found error.";
			description = TARGET_TEXT(target) + u8" was not found in '" + String(note ? note : "") + u8"'";
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
    String err_str =  String(prefix) << " error! [#" << err << "]: " <<
		message << NEW_LINE << "> "<< description;
#ifdef _DEBUG
	err_str << NEW_LINE << " in {" << function << "}" << NEW_LINE << "@" << file << " (l." << line << ")";
#endif
	return err_str;
}
void SException::print() const {
	std::cout << SColorText("red", toString()) << std::endl;
}