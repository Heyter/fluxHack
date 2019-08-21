#include "UtilsFuncs.h"
#include <Windows.h>
#include <string>

void print(std::string Message, ...)
{
	char buffer[1024];
	va_list vlist;
	va_start(vlist, Message);
	_vsnprintf(buffer, sizeof(buffer), Message.c_str(), vlist);
	va_end(vlist);

	printf("%s", buffer);
}