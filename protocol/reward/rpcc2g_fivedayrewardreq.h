#ifndef __RPCC2G_FIVEDAYREWARDREQ_H__
#define __RPCC2G_FIVEDAYREWARDREQ_H__

// generate by ProtoGen at date: 2016/6/16 14:25:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FIVEDAYREWARDREQ_TYPE 63999

class RpcC2G_FiveDayRewardReq : public CRpc
{
private:

	RpcC2G_FiveDayRewardReq():CRpc(RPCC2G_FIVEDAYREWARDREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FiveDayRewardReq *CreateRpc()
	{
		return new RpcC2G_FiveDayRewardReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FiveRewardRes &roArg, const FiveRewardRet &roRes, const CUserData &roUserData);
	void OnTimeout(const FiveRewardRes &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FiveRewardRes &roArg, FiveRewardRet &roRes);
	void OnDelayReplyRpc(const FiveRewardRes &roArg, FiveRewardRet &roRes, const CUserData &roUserData);

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
	FiveRewardRes m_oArg;
	FiveRewardRet m_oRes;
};

#endif
