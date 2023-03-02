#include "pch.h"
#include "util/UtilEncode.h"
#include <stdio.h>

static const char g_hexcode[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

static char getHex(uint8_t hi, uint8_t lo)
{
	hi = hi >= 'a' ? (hi - 'a' + 10) : (hi >= 'A' ? (hi - 'A' + 10) : hi - '0');
	lo = lo >= 'a' ? (lo - 'a' + 10) : (lo >= 'A' ? (lo - 'A' + 10) : lo - '0');
	return (hi << 4) | lo;
}

std::string UtilEncode::UrlEncode(const std::string &s)
{
	std::string r;
	for (unsigned i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		if (c == ' ')
		{
			r.push_back('+');
		}
		else if ((c < '0' && c != '-' && c != '.') ||
				   (c < 'A' && c > '9') ||
				   (c > 'Z' && c < 'a' && c != '_') ||
				   (c > 'z'))
		{
			r.push_back('%');
			r.push_back(g_hexcode[(uint8_t)c >> 4]);
			r.push_back(g_hexcode[(uint8_t)c & 15]);
		}
		else
		{
			r.push_back(c);
		}
	}
	return r;
}

std::string UtilEncode::UrlDecode(const std::string &s)
{
	std::string r;
	for (unsigned i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		if (c == '+')
		{
			r.push_back(' ');
		}
		else if (c == '%'
				&& ((int)s.size() - 1 - i) >= 2
				&& ::isxdigit(s[i + 1])
				&& ::isxdigit(s[i + 2]))
		{
			r.push_back(getHex(s[i + 1], s[i + 2]));
			i += 2;
		}
		else
		{
			r.push_back(c);
		}
	}
	return r;
}

std::string UtilEncode::rawUrlEncode(const std::string &s)
{
	std::string r;
	for (unsigned i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		if ((c < '0' && c != '-' && c != '.') ||
			(c < 'A' && c > '9') ||
			(c > 'Z' && c < 'a' && c != '_') ||
			(c > 'z' && c != '~'))
		{
			r.push_back('%');
			r.push_back(g_hexcode[(uint8_t)c >> 4]);
			r.push_back(g_hexcode[(uint8_t)c & 15]);
		}
		else
		{
			r.push_back(c);
		}
	}
	return r;
}

std::string UtilEncode::rawUrlDecode(const std::string &s)
{
	std::string r;
	for (unsigned i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		if (c == '%'
				&& ((int)s.size() - 1 - i) >= 2
				&& ::isxdigit(s[i + 1])
				&& ::isxdigit(s[i + 2]))
		{
			r.push_back(getHex(s[i + 1], s[i + 2]));
			i += 2;
		}
		else
		{
			r.push_back(c);
		}
	}
	return r;
}
