#ifndef _Define_H__
#define _Define_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>

#define PROTO_MESSAGE_LITE

// define basic data types

typedef signed char  		INT8;
typedef signed short 		INT16;
typedef signed int	  		INT32;
typedef signed long long 	INT64;



typedef unsigned char 		BYTE;
typedef unsigned char 		UINT8;
typedef unsigned short 		UINT16;
typedef unsigned int 		UINT32;
typedef unsigned long long 	UINT64;

typedef UINT64 SessionType;

#define INVALID_SESSION_ID 0
#define SessionGateIDShift ((sizeof(SessionType)-2)*8)

#ifdef WIN32
typedef __int64 int64_t;
#endif

#define ASSERT(cond) assert(cond)
#define ARRAY_SIZE(Array) (sizeof(Array)/sizeof(Array[0]))
#define STRUCT_OFFSET(struct_name, elem_name) (unsigned long)(&((struct struct_name*)0)->elem_name)
#define STRUCT_HEADOF(elem_ptr, struct_name, elem_name) (struct_name*)((char*)elem_ptr - STRUCT_OFFSET(struct_name, elem_name)) 


struct CUserData
{
	CUserData()
		:m_dwUserData(0), m_pUserPtr(0)
	{ 
	}

	CUserData(UINT32 dwData, void* pPtr)
		:m_dwUserData(dwData), m_pUserPtr(pPtr)
	{ 
	}

	UINT32	m_dwUserData;
	void*	m_pUserPtr;
};

struct IPAddress
{
	IPAddress() : m_port(0) {}
	std::string  m_ip;
	unsigned int m_port;
};

inline UINT16 MakeUINT16(UINT8 HiByte, UINT8 LoByte)
{
	return ((UINT16)HiByte << 8 | LoByte);
}

inline UINT32 MakeUINT32(UINT16 HiWord, UINT16 LoWord)
{
	return ((UINT32)HiWord << 16 | LoWord);
}

inline UINT64 MakeUINT64(UINT32 HiDWord, UINT32 LoDWord)
{
	return ((UINT64)HiDWord << 32 | LoDWord);
}

inline UINT8 GetHiByte(UINT16 wVal)
{
	return (UINT8)(wVal >> 8 & 0xFF);
}
 
inline UINT8 GetLoByte(UINT16 wVal)
{
	return (UINT8)(wVal & 0xFF);
}

inline UINT16 GetHiWord(UINT32 dwVal)
{
	return (UINT16)(dwVal >> 16 & 0xFFFF);
}

inline UINT16 GetLoWord(UINT32 dwVal)
{ 
	return (UINT16)(dwVal & 0xFFFF);
}

inline UINT32 GetHiDword(UINT64 qwVal)
{
	return (UINT32)(qwVal >> 32 & 0xFFFFFFFF);
}

inline UINT32 GetLoDword(UINT64 qwVal)
{
	return (UINT32)(qwVal & 0xFFFFFFFF); 
}



const char *str(UINT64 qwValue, char *buf, UINT32 bufsize);
const char *strDec(UINT64 qwValue, char *buf, UINT32 bufsize);

// not thread safe
const char *str(UINT64 qwValue);
const char *strHex(UINT64 qwValue);
const char *strDec(UINT64 qwValue);

inline UINT32 GetGateLine(SessionType session)
{
	return (session >> (SessionGateIDShift)) & 0xFFFF;
}

inline UINT32 GetServerIdBySession(SessionType session)
{
	return (session >> 32) & 0xFFFF;
}

// 仅对伪造的session有用
inline UINT32 GetGsLine(SessionType session)
{
	return (session >> 16) & 0xFFFF;
}

#endif
