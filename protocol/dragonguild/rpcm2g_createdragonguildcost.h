#ifndef __RPCM2G_CREATEDRAGONGUILDCOST_H__
#define __RPCM2G_CREATEDRAGONGUILDCOST_H__

// generate by ProtoGen at date: 2017/9/5 11:50:46

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_CREATEDRAGONGUILDCOST_TYPE 21601

class RpcM2G_CreateDragonGuildCost : public CRpc
{
private:

	RpcM2G_CreateDragonGuildCost():CRpc(RPCM2G_CREATEDRAGONGUILDCOST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_CreateDragonGuildCost *CreateRpc()
	{
		return new RpcM2G_CreateDragonGuildCost;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateDragonGuildCostArg &roArg, const CreateDragonGuildCostRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateDragonGuildCostArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateDragonGuildCostArg &roArg, CreateDragonGuildCostRes &roRes);
	void OnDelayReplyRpc(const CreateDragonGuildCostArg &roArg, CreateDragonGuildCostRes &roRes, const CUserData &roUserData);

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
	CreateDragonGuildCostArg m_oArg;
	CreateDragonGuildCostRes m_oRes;
};

#endif
