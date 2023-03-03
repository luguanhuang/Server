#ifndef __RPCC2M_GARDENSTEAL_H__
#define __RPCC2M_GARDENSTEAL_H__

// generate by ProtoGen at date: 2016/10/24 12:28:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GARDENSTEAL_TYPE 12696

class RpcC2M_GardenSteal : public CRpc
{
private:

	RpcC2M_GardenSteal():CRpc(RPCC2M_GARDENSTEAL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GardenSteal *CreateRpc()
	{
		return new RpcC2M_GardenSteal;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GardenStealArg &roArg, const GardenStealRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GardenStealArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GardenStealArg &roArg, GardenStealRes &roRes);
	void OnDelayReplyRpc(const GardenStealArg &roArg, GardenStealRes &roRes, const CUserData &roUserData);

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
	GardenStealArg m_oArg;
	GardenStealRes m_oRes;
};

#endif
