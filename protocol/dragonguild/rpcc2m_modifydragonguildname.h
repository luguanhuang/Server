#ifndef __RPCC2M_MODIFYDRAGONGUILDNAME_H__
#define __RPCC2M_MODIFYDRAGONGUILDNAME_H__

// generate by ProtoGen at date: 2017/9/8 10:48:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_MODIFYDRAGONGUILDNAME_TYPE 10624

class RpcC2M_ModifyDragonGuildName : public CRpc
{
private:

	RpcC2M_ModifyDragonGuildName():CRpc(RPCC2M_MODIFYDRAGONGUILDNAME_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ModifyDragonGuildName *CreateRpc()
	{
		return new RpcC2M_ModifyDragonGuildName;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ModifyDragonGuildNameArg &roArg, const ModifyDragonGuildNameRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ModifyDragonGuildNameArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ModifyDragonGuildNameArg &roArg, ModifyDragonGuildNameRes &roRes);
	void OnDelayReplyRpc(const ModifyDragonGuildNameArg &roArg, ModifyDragonGuildNameRes &roRes, const CUserData &roUserData);

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
	ModifyDragonGuildNameArg m_oArg;
	ModifyDragonGuildNameRes m_oRes;
};

#endif
