#ifndef __RPCC2G_SKYCITYALLINFOREQ_H__
#define __RPCC2G_SKYCITYALLINFOREQ_H__

// generate by ProtoGen at date: 2016/10/9 19:07:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SKYCITYALLINFOREQ_TYPE 29365

class RpcC2G_SkyCityAllInfoReq : public CRpc
{
private:

	RpcC2G_SkyCityAllInfoReq():CRpc(RPCC2G_SKYCITYALLINFOREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SkyCityAllInfoReq *CreateRpc()
	{
		return new RpcC2G_SkyCityAllInfoReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SkyCityArg &roArg, const SkyCityRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SkyCityArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SkyCityArg &roArg, SkyCityRes &roRes);
	void OnDelayReplyRpc(const SkyCityArg &roArg, SkyCityRes &roRes, const CUserData &roUserData);

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
	SkyCityArg m_oArg;
	SkyCityRes m_oRes;
};

#endif
