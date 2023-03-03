#ifndef __RPCC2G_GETDRAGONGUILDTASKCHEST_H__
#define __RPCC2G_GETDRAGONGUILDTASKCHEST_H__

// generate by ProtoGen at date: 2017/9/11 11:15:45

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETDRAGONGUILDTASKCHEST_TYPE 15059

class RpcC2G_GetDragonGuildTaskChest : public CRpc
{
private:

	RpcC2G_GetDragonGuildTaskChest():CRpc(RPCC2G_GETDRAGONGUILDTASKCHEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetDragonGuildTaskChest *CreateRpc()
	{
		return new RpcC2G_GetDragonGuildTaskChest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDragonGuildTaskChestArg &roArg, const GetDragonGuildTaskChestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDragonGuildTaskChestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDragonGuildTaskChestArg &roArg, GetDragonGuildTaskChestRes &roRes);
	void OnDelayReplyRpc(const GetDragonGuildTaskChestArg &roArg, GetDragonGuildTaskChestRes &roRes, const CUserData &roUserData);

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
	GetDragonGuildTaskChestArg m_oArg;
	GetDragonGuildTaskChestRes m_oRes;
};

#endif
