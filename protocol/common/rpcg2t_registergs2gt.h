#ifndef __RPCG2T_REGISTERGS2GT_H__
#define __RPCG2T_REGISTERGS2GT_H__

// generate by ProtoGen at date: 2016/7/8 18:08:47

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2T_REGISTERGS2GT_TYPE 9408

class RpcG2T_RegisterGs2Gt : public CRpc
{
private:

	RpcG2T_RegisterGs2Gt():CRpc(RPCG2T_REGISTERGS2GT_TYPE)
	{
        m_dwTimeout = 10000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2T_RegisterGs2Gt *CreateRpc()
	{
		return new RpcG2T_RegisterGs2Gt;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGs2GtArg &roArg, const RegisterGs2GtRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGs2GtArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGs2GtArg &roArg, RegisterGs2GtRes &roRes);
	void OnDelayReplyRpc(const RegisterGs2GtArg &roArg, RegisterGs2GtRes &roRes, const CUserData &roUserData);

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
	RegisterGs2GtArg m_oArg;
	RegisterGs2GtRes m_oRes;
};

#endif
