#ifndef __RPCC2G_ENHANCETRANSTER_H__
#define __RPCC2G_ENHANCETRANSTER_H__

// generate by ProtoGen at date: 2016/6/27 0:04:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ENHANCETRANSTER_TYPE 25778

class RpcC2G_EnhanceTranster : public CRpc
{
private:

	RpcC2G_EnhanceTranster():CRpc(RPCC2G_ENHANCETRANSTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_EnhanceTranster *CreateRpc()
	{
		return new RpcC2G_EnhanceTranster;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnhanceTransterArg &roArg, const EnhanceTransterRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnhanceTransterArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnhanceTransterArg &roArg, EnhanceTransterRes &roRes);
	void OnDelayReplyRpc(const EnhanceTransterArg &roArg, EnhanceTransterRes &roRes, const CUserData &roUserData);

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
	EnhanceTransterArg m_oArg;
	EnhanceTransterRes m_oRes;
};

#endif
