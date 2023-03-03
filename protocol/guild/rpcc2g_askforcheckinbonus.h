﻿#ifndef __RPCC2G_ASKFORCHECKINBONUS_H__
#define __RPCC2G_ASKFORCHECKINBONUS_H__

// generate by ProtoGen at date: 2016/6/16 14:25:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ASKFORCHECKINBONUS_TYPE 32843

class RpcC2G_AskForCheckInBonus : public CRpc
{
private:

	RpcC2G_AskForCheckInBonus():CRpc(RPCC2G_ASKFORCHECKINBONUS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_AskForCheckInBonus *CreateRpc()
	{
		return new RpcC2G_AskForCheckInBonus;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AskForCheckInBonusArg &roArg, const AskForCheckInBonusRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AskForCheckInBonusArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AskForCheckInBonusArg &roArg, AskForCheckInBonusRes &roRes);
	void OnDelayReplyRpc(const AskForCheckInBonusArg &roArg, AskForCheckInBonusRes &roRes, const CUserData &roUserData);

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
	AskForCheckInBonusArg m_oArg;
	AskForCheckInBonusRes m_oRes;
};

#endif
