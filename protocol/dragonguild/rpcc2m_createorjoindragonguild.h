#ifndef __RPCC2M_CREATEORJOINDRAGONGUILD_H__
#define __RPCC2M_CREATEORJOINDRAGONGUILD_H__

// generate by ProtoGen at date: 2017/9/5 17:28:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CREATEORJOINDRAGONGUILD_TYPE 8623

class RpcC2M_CreateOrJoinDragonGuild : public CRpc
{
private:

	RpcC2M_CreateOrJoinDragonGuild():CRpc(RPCC2M_CREATEORJOINDRAGONGUILD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_CreateOrJoinDragonGuild *CreateRpc()
	{
		return new RpcC2M_CreateOrJoinDragonGuild;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateOrJoinDragonGuildArg &roArg, const CreateOrJoinDragonGuildRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateOrJoinDragonGuildArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateOrJoinDragonGuildArg &roArg, CreateOrJoinDragonGuildRes &roRes);
	void OnDelayReplyRpc(const CreateOrJoinDragonGuildArg &roArg, CreateOrJoinDragonGuildRes &roRes, const CUserData &roUserData);

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
	CreateOrJoinDragonGuildArg m_oArg;
	CreateOrJoinDragonGuildRes m_oRes;
};

#endif
