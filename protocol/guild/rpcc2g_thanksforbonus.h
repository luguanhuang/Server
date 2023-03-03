#ifndef __RPCC2G_THANKSFORBONUS_H__
#define __RPCC2G_THANKSFORBONUS_H__

// generate by ProtoGen at date: 2016/6/16 14:25:46

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_THANKSFORBONUS_TYPE 42614

class RpcC2G_ThanksForBonus : public CRpc
{
private:

	RpcC2G_ThanksForBonus():CRpc(RPCC2G_THANKSFORBONUS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ThanksForBonus *CreateRpc()
	{
		return new RpcC2G_ThanksForBonus;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ThanksForBonusArg &roArg, const ThanksForBonusRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ThanksForBonusArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ThanksForBonusArg &roArg, ThanksForBonusRes &roRes);
	void OnDelayReplyRpc(const ThanksForBonusArg &roArg, ThanksForBonusRes &roRes, const CUserData &roUserData);

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
	ThanksForBonusArg m_oArg;
	ThanksForBonusRes m_oRes;
};

#endif
