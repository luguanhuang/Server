#ifndef __UTIL_H__
#define __UTIL_H__

#include "zlib.h"
#include <string>
#include <vector>
#include <sstream>

std::string Trim(const std::string &s);
std::string TrimLeft(const std::string &s);
std::string TrimRight(const std::string &s);
std::vector<std::string> Split(const std::string &s, char sep);
std::vector<std::string> Split(const char *s, char sep);
std::vector<std::string> Split(const std::string &s, const char *sep);
std::vector<std::string> Split(const char *s, const char *sep);

bool ConvertUTF16ToUTF8(const std::string &from, std::string &to);

// 把字符串转换成数值
// 字符串     数值
// "30"   => 30 (char, short, int, int64)
// "3.14" => 3.14 (float, double)
// 补充 atoi atof 等的不足

template<typename T>
T convert(const char *str)
{
	std::stringstream ss(str);

	T t;
	ss >> t;
	return t;
}

template<typename T>
T convert(const std::string &str)
{
	return convert<T>(str.c_str());
}

// convert 特例化，加快速度
template<>
inline int convert(const char* str)
{
	int ret;
	sscanf(str, "%d", &ret);
	return ret;
}

template<>
inline UINT32 convert(const char* str)
{
	UINT32 ret;
	sscanf(str, "%u", &ret);
	return ret;
}

template<>
inline INT64 convert(const char* str)
{
	INT64 ret;
	sscanf(str, "%lld", &ret);
	return ret;
}

template<>
inline UINT64 convert(const char* str)
{
	UINT64 ret;
	sscanf(str, "%llu", &ret);
	return ret;
}

template<>
inline float convert(const char* str)
{
	float ret;
	sscanf(str, "%f", &ret);
	return ret;
}

template<>
inline double convert(const char* str)
{
	double ret;
	sscanf(str, "%lf", &ret);
	return ret;
}

// only use in read config
template<typename T>
T SafeConvert(const char *str)
{
	assert(strcmp(str, "") != 0);
	return convert<T>(str);
}

template<typename T>
T SafeConvert(const std::string &str)
{
	return SafeConvert<T>(str.c_str());
}

template<typename T>
std::string ToString(T t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

const char *ToString(UINT64, char *buf, UINT32 bufsize);

int Compress(const char *src, int len, char *dst, uLongf dstLen);
int DeCompress(const char *src, int len, char *dst, uLongf dstLen);

#endif


