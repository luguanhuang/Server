#ifndef __RPCC2G_SELECTCHESTREWARD_H__
#define __RPCC2G_SELECTCHESTREWARD_H__

// generate by ProtoGen at date: 2017/5/5 16:57:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SELECTCHESTREWARD_TYPE 40987

class RpcC2G_SelectChestReward : public CRpc
{
private:

	RpcC2G_SelectChestReward():CRpc(RPCC2G_SELECTCHESTREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SelectChestReward *CreateRpc()
	{
		return new RpcC2G_SelectChestReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SelectChestArg &roArg, const SelectChestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SelectChestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SelectChestArg &roArg, SelectChestRes &roRes);
	void OnDelayReplyRpc(const SelectChestArg &roArg, SelectChestRes &roRes, const CUserData &roUserData);

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
	SelectChestArg m_oArg;
	SelectChestRes m_oRes;
};

#endif
