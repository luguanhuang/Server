#ifndef __RPCC2G_ASKGUILDARENAINFO_H__
#define __RPCC2G_ASKGUILDARENAINFO_H__

// generate by ProtoGen at date: 2016/7/14 16:51:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ASKGUILDARENAINFO_TYPE 33973

class RpcC2G_AskGuildArenaInfo : public CRpc
{
private:

	RpcC2G_AskGuildArenaInfo():CRpc(RPCC2G_ASKGUILDARENAINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_AskGuildArenaInfo *CreateRpc()
	{
		return new RpcC2G_AskGuildArenaInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AskGuildArenaInfoArg &roArg, const AskGuildArenaInfoReq &roRes, const CUserData &roUserData);
	void OnTimeout(const AskGuildArenaInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AskGuildArenaInfoArg &roArg, AskGuildArenaInfoReq &roRes);
	void OnDelayReplyRpc(const AskGuildArenaInfoArg &roArg, AskGuildArenaInfoReq &roRes, const CUserData &roUserData);

	virtual void OnReplyTimeout() 
	{ 
		OnTimeout(m_oArg, m_oUser); 
	}

	virtual void OnDelayRpc(const CUserData &userData)
	{
		OnDelayReplyRpc( m_oArg, m_oRes, userData);
	}

	virtual CRpc *NewRpc() 
	{
		return CreateRpc();
	}

public:
	AskGuildArenaInfoArg m_oArg;
	AskGuildArenaInfoReq m_oRes;
};

#endif
