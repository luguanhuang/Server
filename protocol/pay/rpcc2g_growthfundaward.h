#ifndef __RPCC2G_GROWTHFUNDAWARD_H__
#define __RPCC2G_GROWTHFUNDAWARD_H__

// generate by ProtoGen at date: 2016/8/22 21:17:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GROWTHFUNDAWARD_TYPE 43548

class RpcC2G_GrowthFundAward : public CRpc
{
private:

	RpcC2G_GrowthFundAward():CRpc(RPCC2G_GROWTHFUNDAWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GrowthFundAward *CreateRpc()
	{
		return new RpcC2G_GrowthFundAward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GrowthFundAwardArg &roArg, const GrowthFundAwardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GrowthFundAwardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GrowthFundAwardArg &roArg, GrowthFundAwardRes &roRes);
	void OnDelayReplyRpc(const GrowthFundAwardArg &roArg, GrowthFundAwardRes &roRes, const CUserData &roUserData);

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
	GrowthFundAwardArg m_oArg;
	GrowthFundAwardRes m_oRes;
};

#endif
