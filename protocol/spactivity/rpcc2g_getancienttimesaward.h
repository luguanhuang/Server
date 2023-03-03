#ifndef __RPCC2G_GETANCIENTTIMESAWARD_H__
#define __RPCC2G_GETANCIENTTIMESAWARD_H__

// generate by ProtoGen at date: 2017/7/15 11:15:47

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETANCIENTTIMESAWARD_TYPE 40517

class RpcC2G_GetAncientTimesAward : public CRpc
{
private:

	RpcC2G_GetAncientTimesAward():CRpc(RPCC2G_GETANCIENTTIMESAWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetAncientTimesAward *CreateRpc()
	{
		return new RpcC2G_GetAncientTimesAward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AncientTimesArg &roArg, const AncientTimesRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AncientTimesArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AncientTimesArg &roArg, AncientTimesRes &roRes);
	void OnDelayReplyRpc(const AncientTimesArg &roArg, AncientTimesRes &roRes, const CUserData &roUserData);

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
	AncientTimesArg m_oArg;
	AncientTimesRes m_oRes;
};

#endif
