#ifndef __RPCC2G_GETFLOWERREWARDLIST_H__
#define __RPCC2G_GETFLOWERREWARDLIST_H__

// generate by ProtoGen at date: 2016/6/25 18:04:23

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETFLOWERREWARDLIST_TYPE 26656

class RpcC2G_GetFlowerRewardList : public CRpc
{
private:

	RpcC2G_GetFlowerRewardList():CRpc(RPCC2G_GETFLOWERREWARDLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetFlowerRewardList *CreateRpc()
	{
		return new RpcC2G_GetFlowerRewardList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetFlowerRewardListArg &roArg, const GetFlowerRewardListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetFlowerRewardListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetFlowerRewardListArg &roArg, GetFlowerRewardListRes &roRes);
	void OnDelayReplyRpc(const GetFlowerRewardListArg &roArg, GetFlowerRewardListRes &roRes, const CUserData &roUserData);

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
	GetFlowerRewardListArg m_oArg;
	GetFlowerRewardListRes m_oRes;
};

#endif
