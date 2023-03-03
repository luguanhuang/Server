#ifndef __RPCC2M_ASKGUILDARENAINFONEW_H__
#define __RPCC2M_ASKGUILDARENAINFONEW_H__

// generate by ProtoGen at date: 2016/9/22 15:41:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ASKGUILDARENAINFONEW_TYPE 24504

class RpcC2M_AskGuildArenaInfoNew : public CRpc
{
private:

	RpcC2M_AskGuildArenaInfoNew():CRpc(RPCC2M_ASKGUILDARENAINFONEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AskGuildArenaInfoNew *CreateRpc()
	{
		return new RpcC2M_AskGuildArenaInfoNew;
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
