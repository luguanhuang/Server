#ifndef __RPCM2G_COSTDONATEITEM_H__
#define __RPCM2G_COSTDONATEITEM_H__

// generate by ProtoGen at date: 2016/11/16 15:21:25

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_COSTDONATEITEM_TYPE 23761

class RpcM2G_CostDonateItem : public CRpc
{
private:

	RpcM2G_CostDonateItem():CRpc(RPCM2G_COSTDONATEITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_CostDonateItem *CreateRpc()
	{
		return new RpcM2G_CostDonateItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CostDonateItemArg &roArg, const CostDonateItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CostDonateItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CostDonateItemArg &roArg, CostDonateItemRes &roRes);
	void OnDelayReplyRpc(const CostDonateItemArg &roArg, CostDonateItemRes &roRes, const CUserData &roUserData);

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
	CostDonateItemArg m_oArg;
	CostDonateItemRes m_oRes;
};

#endif
