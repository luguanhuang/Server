#ifndef __RPCC2M_TRYALLIANCE_H__
#define __RPCC2M_TRYALLIANCE_H__

// generate by ProtoGen at date: 2016/12/20 11:19:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_TRYALLIANCE_TYPE 20216

class RpcC2M_TryAlliance : public CRpc
{
private:

	RpcC2M_TryAlliance():CRpc(RPCC2M_TRYALLIANCE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_TryAlliance *CreateRpc()
	{
		return new RpcC2M_TryAlliance;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TryAllianceArg &roArg, const TryAlliance &roRes, const CUserData &roUserData);
	void OnTimeout(const TryAllianceArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TryAllianceArg &roArg, TryAlliance &roRes);
	void OnDelayReplyRpc(const TryAllianceArg &roArg, TryAlliance &roRes, const CUserData &roUserData);

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
	TryAllianceArg m_oArg;
	TryAlliance m_oRes;
};

#endif
