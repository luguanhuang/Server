#ifndef __RPCC2G_PLAYDICEOVER_H__
#define __RPCC2G_PLAYDICEOVER_H__

// generate by ProtoGen at date: 2016/9/8 12:30:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PLAYDICEOVER_TYPE 15035

class RpcC2G_PlayDiceOver : public CRpc
{
private:

	RpcC2G_PlayDiceOver():CRpc(RPCC2G_PLAYDICEOVER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PlayDiceOver *CreateRpc()
	{
		return new RpcC2G_PlayDiceOver;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PlayDiceOverArg &roArg, const PlayDiceOverRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PlayDiceOverArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PlayDiceOverArg &roArg, PlayDiceOverRes &roRes);
	void OnDelayReplyRpc(const PlayDiceOverArg &roArg, PlayDiceOverRes &roRes, const CUserData &roUserData);

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
	PlayDiceOverArg m_oArg;
	PlayDiceOverRes m_oRes;
};

#endif
