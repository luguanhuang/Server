#ifndef __RPCC2G_SENDFLOWER_H__
#define __RPCC2G_SENDFLOWER_H__

// generate by ProtoGen at date: 2015/9/23 16:12:41

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SENDFLOWER_TYPE 16310

class RpcC2G_SendFlower : public CRpc
{
private:

	RpcC2G_SendFlower():CRpc(RPCC2G_SENDFLOWER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SendFlower *CreateRpc()
	{
		return new RpcC2G_SendFlower;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SendFlowerArg &roArg, const SendFlowerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SendFlowerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SendFlowerArg &roArg, SendFlowerRes &roRes);
	void OnDelayReplyRpc(const SendFlowerArg &roArg, SendFlowerRes &roRes, const CUserData &roUserData);

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
	SendFlowerArg m_oArg;
	SendFlowerRes m_oRes;
};

#endif
