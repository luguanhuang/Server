#ifndef __RPCM2H_TEAMTRANMS2TS_H__
#define __RPCM2H_TEAMTRANMS2TS_H__

// generate by ProtoGen at date: 2017/4/25 21:35:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2H_TEAMTRANMS2TS_TYPE 53395

class RpcM2H_TeamTranMs2Ts : public CRpc
{
private:

	RpcM2H_TeamTranMs2Ts():CRpc(RPCM2H_TEAMTRANMS2TS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2H_TeamTranMs2Ts *CreateRpc()
	{
		return new RpcM2H_TeamTranMs2Ts;
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
