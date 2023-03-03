#ifndef __RPCT2M_REGISTERGT2MS_H__
#define __RPCT2M_REGISTERGT2MS_H__

// generate by ProtoGen at date: 2017/5/18 14:54:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCT2M_REGISTERGT2MS_TYPE 8141

class RpcT2M_RegisterGt2Ms : public CRpc
{
private:

	RpcT2M_RegisterGt2Ms():CRpc(RPCT2M_REGISTERGT2MS_TYPE)
	{
        m_dwTimeout = 2000000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2M_RegisterGt2Ms *CreateRpc()
	{
		return new RpcT2M_RegisterGt2Ms;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGt2MsArg &roArg, const RegisterGt2MsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGt2MsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGt2MsArg &roArg, RegisterGt2MsRes &roRes);
	void OnDelayReplyRpc(const RegisterGt2MsArg &roArg, RegisterGt2MsRes &roRes, const CUserData &roUserData);

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
	RegisterGt2MsArg m_oArg;
	RegisterGt2MsRes m_oRes;
};

#endif
