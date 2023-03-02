#ifndef _UTIL_ENCODE_
#define _UTIL_ENCODE_

#include <stdint.h>
#include <string>

class UtilEncode
{
public:
	static std::string UrlEncode(const std::string &s); // 空格替换为+,~替换为%7e
	static std::string UrlDecode(const std::string &s);
	static std::string rawUrlEncode(const std::string &s); // 空格替换为%20,不转义~
	static std::string rawUrlDecode(const std::string &s);
};

#endif
