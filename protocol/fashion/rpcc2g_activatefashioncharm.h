#ifndef __RPCC2G_ACTIVATEFASHIONCHARM_H__
#define __RPCC2G_ACTIVATEFASHIONCHARM_H__

// generate by ProtoGen at date: 2017/4/20 16:19:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ACTIVATEFASHIONCHARM_TYPE 58036

class RpcC2G_ActivateFashionCharm : public CRpc
{
private:

	RpcC2G_ActivateFashionCharm():CRpc(RPCC2G_ACTIVATEFASHIONCHARM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ActivateFashionCharm *CreateRpc()
	{
		return new RpcC2G_ActivateFashionCharm;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ActivateFashionArg &roArg, const ActivateFashionRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ActivateFashionArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ActivateFashionArg &roArg, ActivateFashionRes &roRes);
	void OnDelayReplyRpc(const ActivateFashionArg &roArg, ActivateFashionRes &roRes, const CUserData &roUserData);

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
	ActivateFashionArg m_oArg;
	ActivateFashionRes m_oRes;
};

#endif
