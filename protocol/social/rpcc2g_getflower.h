#ifndef __RPCC2G_GETFLOWER_H__
#define __RPCC2G_GETFLOWER_H__

// generate by ProtoGen at date: 2016/6/25 16:14:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETFLOWER_TYPE 11473

class RpcC2G_GetFlower : public CRpc
{
private:

	RpcC2G_GetFlower():CRpc(RPCC2G_GETFLOWER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetFlower *CreateRpc()
	{
		return new RpcC2G_GetFlower;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetFlowerArg &roArg, const GetFlowerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetFlowerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetFlowerArg &roArg, GetFlowerRes &roRes);
	void OnDelayReplyRpc(const GetFlowerArg &roArg, GetFlowerRes &roRes, const CUserData &roUserData);

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
	GetFlowerArg m_oArg;
	GetFlowerRes m_oRes;
};

#endif
