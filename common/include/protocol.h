#ifndef _Protocol_H__
#define _Protocol_H__

#include "define.h"
#include <unordered_map>

#ifdef PROTO_MESSAGE_LITE
#include <google/protobuf/message_lite.h>
#else
#include <google/protobuf/message.h>
#endif

class CProtocol
{
public:
	explicit CProtocol(UINT32 dwType);
	virtual ~CProtocol();
	virtual void Process(UINT32 dwConnID) = 0;
	
	int  Serialize(void *data, int len) const;
	bool DeSerialize(const void *data, int len);

	void SetDontCompress();

	void DebugPrint();

	bool        m_debug;
	bool        m_Compress;
	UINT32	    m_dwType;
	mutable SessionType m_sessionID;

protected:

#ifdef PROTO_MESSAGE_LITE
	google::protobuf::MessageLite *m_message;
#else
	google::protobuf::Message *m_message;
#endif
	// static method and member
public:
	typedef std::unordered_map<UINT32, CProtocol*>	CProtocolMap;

	static bool Register(CProtocol* poProtocol, bool debug=false);
	static void UnRegister();
	static CProtocol* GetProtocol(UINT32 dwType);

	static void RegistProtoName(UINT32 dwType, const char *Name);
	static const char *GetProtoName(UINT32 dwType);

private:
	static CProtocolMap	sm_oPtcMap;
};

#endif