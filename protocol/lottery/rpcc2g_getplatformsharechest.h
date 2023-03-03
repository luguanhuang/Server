#ifndef __RPCC2G_GETPLATFORMSHARECHEST_H__
#define __RPCC2G_GETPLATFORMSHARECHEST_H__

// generate by ProtoGen at date: 2017/4/11 16:26:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETPLATFORMSHARECHEST_TYPE 7875

class RpcC2G_GetPlatformShareChest : public CRpc
{
private:

	RpcC2G_GetPlatformShareChest():CRpc(RPCC2G_GETPLATFORMSHARECHEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetPlatformShareChest *CreateRpc()
	{
		return new RpcC2G_GetPlatformShareChest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPlatformShareChestArg &roArg, const GetPlatformShareChestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPlatformShareChestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPlatformShareChestArg &roArg, GetPlatformShareChestRes &roRes);
	void OnDelayReplyRpc(const GetPlatformShareChestArg &roArg, GetPlatformShareChestRes &roRes, const CUserData &roUserData);

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
	GetPlatformShareChestArg m_oArg;
	GetPlatformShareChestRes m_oRes;
};

#endif
