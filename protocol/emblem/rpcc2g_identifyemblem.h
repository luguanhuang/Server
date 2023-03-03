#ifndef __RPCC2G_IDENTIFYEMBLEM_H__
#define __RPCC2G_IDENTIFYEMBLEM_H__

// generate by ProtoGen at date: 2016/7/9 12:01:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_IDENTIFYEMBLEM_TYPE 43787

class RpcC2G_IdentifyEmblem : public CRpc
{
private:

	RpcC2G_IdentifyEmblem():CRpc(RPCC2G_IDENTIFYEMBLEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_IdentifyEmblem *CreateRpc()
	{
		return new RpcC2G_IdentifyEmblem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const IdentifyEmblemArg &roArg, const IdentifyEmblemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const IdentifyEmblemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const IdentifyEmblemArg &roArg, IdentifyEmblemRes &roRes);
	void OnDelayReplyRpc(const IdentifyEmblemArg &roArg, IdentifyEmblemRes &roRes, const CUserData &roUserData);

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
	IdentifyEmblemArg m_oArg;
	IdentifyEmblemRes m_oRes;
};

#endif
