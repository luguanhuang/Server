#ifndef __RPCC2M_GUILDFATIGUEOPNEW_H__
#define __RPCC2M_GUILDFATIGUEOPNEW_H__

// generate by ProtoGen at date: 2016/9/19 10:29:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GUILDFATIGUEOPNEW_TYPE 10226

class RpcC2M_GuildFatigueOPNew : public CRpc
{
private:

	RpcC2M_GuildFatigueOPNew():CRpc(RPCC2M_GUILDFATIGUEOPNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GuildFatigueOPNew *CreateRpc()
	{
		return new RpcC2M_GuildFatigueOPNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildFatigueArg &roArg, const GuildFatigueRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildFatigueArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildFatigueArg &roArg, GuildFatigueRes &roRes);
	void OnDelayReplyRpc(const GuildFatigueArg &roArg, GuildFatigueRes &roRes, const CUserData &roUserData);

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
	GuildFatigueArg m_oArg;
	GuildFatigueRes m_oRes;
};

#endif
