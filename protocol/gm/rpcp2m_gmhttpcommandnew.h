#ifndef __RPCP2M_GMHTTPCOMMANDNEW_H__
#define __RPCP2M_GMHTTPCOMMANDNEW_H__

// generate by ProtoGen at date: 2016/7/16 10:19:30

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCP2M_GMHTTPCOMMANDNEW_TYPE 42798

class RpcP2M_GMHttpCommandNew : public CRpc
{
private:

	RpcP2M_GMHttpCommandNew():CRpc(RPCP2M_GMHTTPCOMMANDNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcP2M_GMHttpCommandNew *CreateRpc()
	{
		return new RpcP2M_GMHttpCommandNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GMHttpArg &roArg, const GMHttpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GMHttpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GMHttpArg &roArg, GMHttpRes &roRes);
	void OnDelayReplyRpc(const GMHttpArg &roArg, GMHttpRes &roRes, const CUserData &roUserData);

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
	GMHttpArg m_oArg;
	GMHttpRes m_oRes;
};

#endif
