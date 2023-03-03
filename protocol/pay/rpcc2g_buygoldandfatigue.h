#ifndef __RPCC2G_BUYGOLDANDFATIGUE_H__
#define __RPCC2G_BUYGOLDANDFATIGUE_H__

// generate by ProtoGen at date: 2015/7/20 10:15:31

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BUYGOLDANDFATIGUE_TYPE 31095

class RpcC2G_BuyGoldAndFatigue : public CRpc
{
private:

	RpcC2G_BuyGoldAndFatigue():CRpc(RPCC2G_BUYGOLDANDFATIGUE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BuyGoldAndFatigue *CreateRpc()
	{
		return new RpcC2G_BuyGoldAndFatigue;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuyGoldAndFatigueArg &roArg, const BuyGoldAndFatigueRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BuyGoldAndFatigueArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuyGoldAndFatigueArg &roArg, BuyGoldAndFatigueRes &roRes);
	void OnDelayReplyRpc(const BuyGoldAndFatigueArg &roArg, BuyGoldAndFatigueRes &roRes, const CUserData &roUserData);

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
	BuyGoldAndFatigueArg m_oArg;
	BuyGoldAndFatigueRes m_oRes;
};

#endif
