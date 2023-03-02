#include "pch.h"
#include "md5.h"


std::string Md5Hash(const std::string& str)
{
	const char HexDigit[] = "0123456789abcdef";
	unsigned char hexdigit[33];

	unsigned char md5[16];
	li_MD5_CTX ctx;
	li_MD5_Init(&ctx);
	li_MD5_Update(&ctx, str.data(), str.size());
	li_MD5_Final(md5, &ctx);
	for (int i = 0; i < 16; ++i)
	{
		hexdigit[i*2] = HexDigit[(md5[i] >> 4) & 0xF];
		hexdigit[i*2+1] = HexDigit[md5[i] & 0xF];
	}

	hexdigit[32] = 0;
	return std::string((char*)hexdigit);
}