#ifndef __RPCT2N_REGISTERGT2NS_H__
#define __RPCT2N_REGISTERGT2NS_H__

// generate by ProtoGen at date: 2016/11/5 15:58:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCT2N_REGISTERGT2NS_TYPE 48374

class RpcT2N_RegisterGt2Ns : public CRpc
{
private:

	RpcT2N_RegisterGt2Ns():CRpc(RPCT2N_REGISTERGT2NS_TYPE)
	{
        m_dwTimeout = 100000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2N_RegisterGt2Ns *CreateRpc()
	{
		return new RpcT2N_RegisterGt2Ns;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGt2NsArg &roArg, const RegisterGt2NsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGt2NsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGt2NsArg &roArg, RegisterGt2NsRes &roRes);
	void OnDelayReplyRpc(const RegisterGt2NsArg &roArg, RegisterGt2NsRes &roRes, const CUserData &roUserData);

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
	RegisterGt2NsArg m_oArg;
	RegisterGt2NsRes m_oRes;
};

#endif
