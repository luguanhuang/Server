#ifndef __RPCM2G_CREATEGUILDCOST_H__
#define __RPCM2G_CREATEGUILDCOST_H__

// generate by ProtoGen at date: 2016/8/26 17:30:59

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_CREATEGUILDCOST_TYPE 12393

class RpcM2G_CreateGuildCost : public CRpc
{
private:

	RpcM2G_CreateGuildCost():CRpc(RPCM2G_CREATEGUILDCOST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_CreateGuildCost *CreateRpc()
	{
		return new RpcM2G_CreateGuildCost;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateGuildCostArg &roArg, const CreateGuildCostRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateGuildCostArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateGuildCostArg &roArg, CreateGuildCostRes &roRes);
	void OnDelayReplyRpc(const CreateGuildCostArg &roArg, CreateGuildCostRes &roRes, const CUserData &roUserData);

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
	CreateGuildCostArg m_oArg;
	CreateGuildCostRes m_oRes;
};

#endif
