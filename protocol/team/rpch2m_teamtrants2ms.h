#ifndef __RPCH2M_TEAMTRANTS2MS_H__
#define __RPCH2M_TEAMTRANTS2MS_H__

// generate by ProtoGen at date: 2017/4/25 21:36:22

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCH2M_TEAMTRANTS2MS_TYPE 27234

class RpcH2M_TeamTranTs2Ms : public CRpc
{
private:

	RpcH2M_TeamTranTs2Ms():CRpc(RPCH2M_TEAMTRANTS2MS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2M_TeamTranTs2Ms *CreateRpc()
	{
		return new RpcH2M_TeamTranTs2Ms;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TeamTransData &roArg, const TeamTransRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TeamTransData &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TeamTransData &roArg, TeamTransRes &roRes);
	void OnDelayReplyRpc(const TeamTransData &roArg, TeamTransRes &roRes, const CUserData &roUserData);

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
	TeamTransData m_oArg;
	TeamTransRes m_oRes;
};

#endif
