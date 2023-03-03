#ifndef __RPCC2G_DRAGONGROUPROLELIST_H__
#define __RPCC2G_DRAGONGROUPROLELIST_H__

// generate by ProtoGen at date: 2017/7/28 13:12:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_DRAGONGROUPROLELIST_TYPE 29660

class RpcC2G_DragonGroupRoleList : public CRpc
{
private:

	RpcC2G_DragonGroupRoleList():CRpc(RPCC2G_DRAGONGROUPROLELIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_DragonGroupRoleList *CreateRpc()
	{
		return new RpcC2G_DragonGroupRoleList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DragonGroupRoleListC2S &roArg, const DragonGroupRoleListS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const DragonGroupRoleListC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DragonGroupRoleListC2S &roArg, DragonGroupRoleListS2C &roRes);
	void OnDelayReplyRpc(const DragonGroupRoleListC2S &roArg, DragonGroupRoleListS2C &roRes, const CUserData &roUserData);

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
	DragonGroupRoleListC2S m_oArg;
	DragonGroupRoleListS2C m_oRes;
};

#endif
