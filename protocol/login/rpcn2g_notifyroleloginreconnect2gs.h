#ifndef __RPCN2G_NOTIFYROLELOGINRECONNECT2GS_H__
#define __RPCN2G_NOTIFYROLELOGINRECONNECT2GS_H__

// generate by ProtoGen at date: 2017/6/21 17:36:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2G_NOTIFYROLELOGINRECONNECT2GS_TYPE 29808

class RpcN2G_NotifyRoleLoginReconnect2Gs : public CRpc
{
private:

	RpcN2G_NotifyRoleLoginReconnect2Gs():CRpc(RPCN2G_NOTIFYROLELOGINRECONNECT2GS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2G_NotifyRoleLoginReconnect2Gs *CreateRpc()
	{
		return new RpcN2G_NotifyRoleLoginReconnect2Gs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const NotifyRoleLoginReconnect2GsArg &roArg, const NotifyRoleLoginReconnect2GsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const NotifyRoleLoginReconnect2GsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const NotifyRoleLoginReconnect2GsArg &roArg, NotifyRoleLoginReconnect2GsRes &roRes);
	void OnDelayReplyRpc(const NotifyRoleLoginReconnect2GsArg &roArg, NotifyRoleLoginReconnect2GsRes &roRes, const CUserData &roUserData);

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
	NotifyRoleLoginReconnect2GsArg m_oArg;
	NotifyRoleLoginReconnect2GsRes m_oRes;
};

#endif
