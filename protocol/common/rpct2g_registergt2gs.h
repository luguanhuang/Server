#ifndef __RPCT2G_REGISTERGT2GS_H__
#define __RPCT2G_REGISTERGT2GS_H__

// generate by ProtoGen at date: 2016/7/8 18:12:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCT2G_REGISTERGT2GS_TYPE 30179

class RpcT2G_RegisterGt2Gs : public CRpc
{
private:

	RpcT2G_RegisterGt2Gs():CRpc(RPCT2G_REGISTERGT2GS_TYPE)
	{
        m_dwTimeout = 10000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2G_RegisterGt2Gs *CreateRpc()
	{
		return new RpcT2G_RegisterGt2Gs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGt2GsArg &roArg, const RegisterGt2GsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGt2GsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGt2GsArg &roArg, RegisterGt2GsRes &roRes);
	void OnDelayReplyRpc(const RegisterGt2GsArg &roArg, RegisterGt2GsRes &roRes, const CUserData &roUserData);

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
	RegisterGt2GsArg m_oArg;
	RegisterGt2GsRes m_oRes;
};

#endif
