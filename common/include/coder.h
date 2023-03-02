#ifndef _Protocol_Coder_H__
#define _Protocol_Coder_H__

#include "define.h"

class CProtocol;
class CRpc;

// PtcHeadFlag 是用来填 ProtocolHead 的 dwFlag字段。
// dwFlag 字段意义
// 32个bit从高到低 31-24 23-16 15-8 7-0
//  Protocol       0     0    0   00000000
//  RPC Request    0     0    0   00000011
//  RPC Reply      0     0    0   00000001
//  Compress       CHECKSUM   0   00000100

enum PtcHeadFlag
{
	PTC_TYPE_FLAG = 0,
	RPC_TYPE_FLAG = 1,
	RPC_TYPE_REPLY = 1,
	RPC_TYPE_ISREQUEST_MASK = 2,
	RPC_TYPE_REQUEST = 3,
	PTC_TYPE_COMPRESS = 4,
	RPC_TYPE_NULL_REPLY = 8,
	PTC_TYPE_KEEP_ALIVE = 16,
};

#pragma pack (4)
struct ProtocolHead
{
	UINT32 m_dwLen;
	UINT32 m_dwType;
	UINT32 m_dwFlag;
	SessionType m_dwSessionID;
};

struct RpcHead
{
	UINT32 m_dwLen;
	UINT32 m_dwType;
	UINT32 m_dwFlag;
	SessionType m_dwSessionID;
	UINT32 m_dwTagID;
};
#pragma pack ()

#define PTC_HEAD_SIZE sizeof(ProtocolHead)
#define RPC_HEAD_SIZE sizeof(RpcHead)


class MemoryStream
{
public:
	MemoryStream(int size);
	~MemoryStream();

	char *GetBuffer();
	char *GetWritePosition();
	int   GetLength();
	int   GetCapcity();
	int   GetSpace();

	void Clear();
	void Seek(int position);
	void Advance(int len);
	bool Write(const char *data, int len);
	bool Compress(int headsize);
	bool IsCompressed() { return m_isCompressed; }

	static MemoryStream *Get(bool autoclear = true);

private:
	char *m_Buffer;
	int   m_capcity;
	int   m_length;
	int   m_position;
	bool  m_isCompressed;
};

class ZlibTool
{
public:
	ZlibTool(int size);
	~ZlibTool();

	bool Compress(const char *buf, int len);
	bool DeCompress(const char *buf, int len);

	char*	GetBuffer()	{ return m_Buffer; }
	int		GetLength()	{ return m_Len; }
	int		GetCapacity(){ return m_Capcity; }
	void	Clear()		 { m_Len = 0; }

	static ZlibTool *Get();

private:
	int   m_Capcity;
	int   m_Len;
	char *m_Buffer;
};

#endif