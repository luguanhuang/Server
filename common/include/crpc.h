#ifndef __CRPC_H__
#define __CRPC_H__

#include "define.h"
#include "netproc.h"
#include "timer.h"

#ifdef PROTO_MESSAGE_LITE
#include <google/protobuf/message_lite.h>
#else
#include <google/protobuf/message.h>
#endif

enum RpcEndType
{
	RPC_CLIENT,
	RPC_SERVER,
};


class CRpc : public ITimer
{
public:
	explicit CRpc(UINT32 dwType);
	virtual ~CRpc();

public:

	void   SetDontCompress();
	void   OnSend();
	void   SetRpcTag();
	void   SetUserData(const CUserData &userData);
	CUserData &GetUserData();
	UINT32 GetConnID() const { return m_dwConnID; }

	virtual void Process(UINT32 dwConnID) = 0;
	virtual RpcEndType GetEndType() const = 0;
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	int  Serialize(void *data, int len)const;
	bool DeSerialize(const void *data, int len);

	static bool RegistRpc(CRpc *poRpc);
	static void UnRegister();

	static CRpc *GetRpc(UINT32 dwType);
	static CRpc *GetRpcByTag(UINT32 dwTag);
	static void RemoveRpcByTag(UINT32 dwTag);
	static void ReplyDelayRpc(UINT32 dwRpcID, const CUserData &userData);

	static void SetTimeoutEnabled(bool bEnabled);
	static void Update();
protected:
	UINT32 DelayRpc();
	virtual CRpc *NewRpc() = 0;
	virtual void OnReplyTimeout() {}
	virtual void OnDelayReplyTimeout() {}
	virtual void OnDelayRpc(const CUserData &userData){}

public:
	UINT32 m_dwType;          // 协议号
	UINT32 m_dwTag;           // 等到回复的rpc号
	UINT32 m_dwDelayID;       // 需要delay的rpc用
	UINT32 m_dwConnID;        // delay的rpc在之后处理时需要知道回复那个连接
	UINT32 m_dwTimeout;       
	HTIMER m_handler;         // 等待回复的Rpc或者delay的Rpc，需要定时器处理超时，回复和delay不可能同时出现
	bool   m_isDelayed;  
	bool   m_compress;
	SessionType m_sessionID;
	CUserData m_oUser;

protected:

#ifdef PROTO_MESSAGE_LITE
	google::protobuf::MessageLite *m_ArgPtr;
	google::protobuf::MessageLite *m_ResPtr;
#else
	google::protobuf::Message *m_ArgPtr;
	google::protobuf::Message *m_ResPtr;
#endif

};


#define RPC_CLIENT_IMPLEMETION(Class, ArgType, ResType) \
	RpcEndType Class::GetEndType() const { return RPC_CLIENT; }\
	void Class::Process(UINT32 dwConnID) { m_dwConnID = dwConnID; OnReply(m_oArg, m_oRes, m_oUser); }\
	void Class::OnCall(const ArgType &roArg, ResType &roRes){}\
	void Class::OnDelayReplyRpc(const ArgType &roArg, ResType &roRes, const CUserData &roUserData){}


#define RPC_SERVER_IMPLEMETION(Class, ArgType, ResType) \
	RpcEndType Class::GetEndType() const { return RPC_SERVER; }\
	void Class::Process(UINT32 dwConnID) { \
		m_dwConnID = dwConnID;\
		m_oRes.Clear();\
		OnCall(m_oArg, m_oRes);\
		if (m_isDelayed == false)\
		{\
			CNetProcessor::Instance()->Send(dwConnID, *this);\
		}\
	}\
	void Class::OnReply(const ArgType &roArg, const ResType &roRes, const CUserData &roUserData){}\
	void Class::OnTimeout(const ArgType &roArg, const CUserData &roUserData){}

#endif
