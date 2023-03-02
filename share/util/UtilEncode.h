#ifndef _UTIL_ENCODE_
#define _UTIL_ENCODE_

#include <stdint.h>
#include <string>

class UtilEncode
{
public:
	static std::string UrlEncode(const std::string &s); // �ո��滻Ϊ+,~�滻Ϊ%7e
	static std::string UrlDecode(const std::string &s);
	static std::string rawUrlEncode(const std::string &s); // �ո��滻Ϊ%20,��ת��~
	static std::string rawUrlDecode(const std::string &s);
};

#endif
