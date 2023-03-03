#ifndef __RPCG2M_GETDRAGONGUILDTASKCHESTG2M_H__
#define __RPCG2M_GETDRAGONGUILDTASKCHESTG2M_H__

// generate by ProtoGen at date: 2017/9/11 14:37:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GETDRAGONGUILDTASKCHESTG2M_TYPE 36818

class RpcG2M_GetDragonGuildTaskChestG2M : public CRpc
{
private:

	RpcG2M_GetDragonGuildTaskChestG2M():CRpc(RPCG2M_GETDRAGONGUILDTASKCHESTG2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GetDragonGuildTaskChestG2M *CreateRpc()
	{
		return new RpcG2M_GetDragonGuildTaskChestG2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDragonGuildTaskChestG2MArg &roArg, const GetDragonGuildTaskChestG2MRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDragonGuildTaskChestG2MArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDragonGuildTaskChestG2MArg &roArg, GetDragonGuildTaskChestG2MRes &roRes);
	void OnDelayReplyRpc(const GetDragonGuildTaskChestG2MArg &roArg, GetDragonGuildTaskChestG2MRes &roRes, const CUserData &roUserData);

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
	GetDragonGuildTaskChestG2MArg m_oArg;
	GetDragonGuildTaskChestG2MRes m_oRes;
};

#endif
