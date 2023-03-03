#ifndef __RPCM2G_MODIFYGSGUILDNAME_H__
#define __RPCM2G_MODIFYGSGUILDNAME_H__

// generate by ProtoGen at date: 2017/4/11 23:41:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_MODIFYGSGUILDNAME_TYPE 12672

class RpcM2G_ModifyGsGuildName : public CRpc
{
private:

	RpcM2G_ModifyGsGuildName():CRpc(RPCM2G_MODIFYGSGUILDNAME_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_ModifyGsGuildName *CreateRpc()
	{
		return new RpcM2G_ModifyGsGuildName;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ModifyFwArg &roArg, const ModifyFwRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ModifyFwArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ModifyFwArg &roArg, ModifyFwRes &roRes);
	void OnDelayReplyRpc(const ModifyFwArg &roArg, ModifyFwRes &roRes, const CUserData &roUserData);

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
	ModifyFwArg m_oArg;
	ModifyFwRes m_oRes;
};

#endif
