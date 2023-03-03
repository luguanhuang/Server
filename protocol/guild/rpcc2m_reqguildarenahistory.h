#ifndef __RPCC2M_REQGUILDARENAHISTORY_H__
#define __RPCC2M_REQGUILDARENAHISTORY_H__

// generate by ProtoGen at date: 2016/11/10 10:24:22

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQGUILDARENAHISTORY_TYPE 2922

class RpcC2M_ReqGuildArenaHistory : public CRpc
{
private:

	RpcC2M_ReqGuildArenaHistory():CRpc(RPCC2M_REQGUILDARENAHISTORY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqGuildArenaHistory *CreateRpc()
	{
		return new RpcC2M_ReqGuildArenaHistory;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildArenaHistoryRes &roArg, const ReqGuildArenaHistoryRse &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildArenaHistoryRes &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildArenaHistoryRes &roArg, ReqGuildArenaHistoryRse &roRes);
	void OnDelayReplyRpc(const ReqGuildArenaHistoryRes &roArg, ReqGuildArenaHistoryRse &roRes, const CUserData &roUserData);

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
	ReqGuildArenaHistoryRes m_oArg;
	ReqGuildArenaHistoryRse m_oRes;
};

#endif
