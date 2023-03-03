#ifndef __RPCC2G_GETGROWTHFUNDTIME_H__
#define __RPCC2G_GETGROWTHFUNDTIME_H__

// generate by ProtoGen at date: 2017/5/2 9:42:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGROWTHFUNDTIME_TYPE 38694

class RpcC2G_GetGrowthFundTime : public CRpc
{
private:

	RpcC2G_GetGrowthFundTime():CRpc(RPCC2G_GETGROWTHFUNDTIME_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGrowthFundTime *CreateRpc()
	{
		return new RpcC2G_GetGrowthFundTime;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGrowthFundTimeArg &roArg, const GetGrowthFundTimeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGrowthFundTimeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGrowthFundTimeArg &roArg, GetGrowthFundTimeRes &roRes);
	void OnDelayReplyRpc(const GetGrowthFundTimeArg &roArg, GetGrowthFundTimeRes &roRes, const CUserData &roUserData);

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
	GetGrowthFundTimeArg m_oArg;
	GetGrowthFundTimeRes m_oRes;
};

#endif
