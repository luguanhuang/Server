#ifndef __RPCC2G_PAYFIRSTAWARD_H__
#define __RPCC2G_PAYFIRSTAWARD_H__

// generate by ProtoGen at date: 2016/8/22 20:34:06

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PAYFIRSTAWARD_TYPE 46058

class RpcC2G_PayFirstAward : public CRpc
{
private:

	RpcC2G_PayFirstAward():CRpc(RPCC2G_PAYFIRSTAWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PayFirstAward *CreateRpc()
	{
		return new RpcC2G_PayFirstAward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PayFirstAwardArg &roArg, const PayFirstAwardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PayFirstAwardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PayFirstAwardArg &roArg, PayFirstAwardRes &roRes);
	void OnDelayReplyRpc(const PayFirstAwardArg &roArg, PayFirstAwardRes &roRes, const CUserData &roUserData);

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
	PayFirstAwardArg m_oArg;
	PayFirstAwardRes m_oRes;
};

#endif
