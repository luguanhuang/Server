#ifndef __RPCG2M_ENTERQAROOM_H__
#define __RPCG2M_ENTERQAROOM_H__

// generate by ProtoGen at date: 2016/10/31 16:28:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_ENTERQAROOM_TYPE 15015

class RpcG2M_EnterQARoom : public CRpc
{
private:

	RpcG2M_EnterQARoom():CRpc(RPCG2M_ENTERQAROOM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_EnterQARoom *CreateRpc()
	{
		return new RpcG2M_EnterQARoom;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnterQARoomReq &roArg, const EnterQARoomRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnterQARoomReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnterQARoomReq &roArg, EnterQARoomRes &roRes);
	void OnDelayReplyRpc(const EnterQARoomReq &roArg, EnterQARoomRes &roRes, const CUserData &roUserData);

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
	EnterQARoomReq m_oArg;
	EnterQARoomRes m_oRes;
};

#endif
