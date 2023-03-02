#ifndef _UTIL_STRING_H_
#define _UTIL_STRING_H_

#include <cstring>
#include <string.h>

namespace UtilString
{
	inline void SafeStringCopy(char* dest, const char* src, UINT32 len)
	{
		strncpy(dest, src, len);
		dest[len - 1] = '\0';
	}
}

#endif
