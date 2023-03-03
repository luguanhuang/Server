#ifndef __RPCG2M_CREATEGUILDQAROOM_H__
#define __RPCG2M_CREATEGUILDQAROOM_H__

// generate by ProtoGen at date: 2016/10/8 16:07:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_CREATEGUILDQAROOM_TYPE 25711

class RpcG2M_CreateGuildQARoom : public CRpc
{
private:

	RpcG2M_CreateGuildQARoom():CRpc(RPCG2M_CREATEGUILDQAROOM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_CreateGuildQARoom *CreateRpc()
	{
		return new RpcG2M_CreateGuildQARoom;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateGuildQARoomReq &roArg, const CreateGuildQARoomRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateGuildQARoomReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateGuildQARoomReq &roArg, CreateGuildQARoomRes &roRes);
	void OnDelayReplyRpc(const CreateGuildQARoomReq &roArg, CreateGuildQARoomRes &roRes, const CUserData &roUserData);

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
	CreateGuildQARoomReq m_oArg;
	CreateGuildQARoomRes m_oRes;
};

#endif
