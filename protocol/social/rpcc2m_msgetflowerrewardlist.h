#ifndef __RPCC2M_MSGETFLOWERREWARDLIST_H__
#define __RPCC2M_MSGETFLOWERREWARDLIST_H__

// generate by ProtoGen at date: 2016/8/19 9:55:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_MSGETFLOWERREWARDLIST_TYPE 16271

class RpcC2M_MSGetFlowerRewardList : public CRpc
{
private:

	RpcC2M_MSGetFlowerRewardList():CRpc(RPCC2M_MSGETFLOWERREWARDLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_MSGetFlowerRewardList *CreateRpc()
	{
		return new RpcC2M_MSGetFlowerRewardList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const NewGetFlowerRewardListArg &roArg, const NewGetFlowerRewardListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const NewGetFlowerRewardListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const NewGetFlowerRewardListArg &roArg, NewGetFlowerRewardListRes &roRes);
	void OnDelayReplyRpc(const NewGetFlowerRewardListArg &roArg, NewGetFlowerRewardListRes &roRes, const CUserData &roUserData);

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
	NewGetFlowerRewardListArg m_oArg;
	NewGetFlowerRewardListRes m_oRes;
};

#endif
