#ifndef __RPCC2G_GETACTIVITYCHEST_H__
#define __RPCC2G_GETACTIVITYCHEST_H__

// generate by ProtoGen at date: 2015/6/27 12:23:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETACTIVITYCHEST_TYPE 34363

class RpcC2G_GetActivityChest : public CRpc
{
private:

	RpcC2G_GetActivityChest():CRpc(RPCC2G_GETACTIVITYCHEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetActivityChest *CreateRpc()
	{
		return new RpcC2G_GetActivityChest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetActivityChestArg &roArg, const GetActivityChestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetActivityChestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetActivityChestArg &roArg, GetActivityChestRes &roRes);
	void OnDelayReplyRpc(const GetActivityChestArg &roArg, GetActivityChestRes &roRes, const CUserData &roUserData);

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
	GetActivityChestArg m_oArg;
	GetActivityChestRes m_oRes;
};

#endif
