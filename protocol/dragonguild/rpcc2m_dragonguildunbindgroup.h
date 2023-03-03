#ifndef __RPCC2M_DRAGONGUILDUNBINDGROUP_H__
#define __RPCC2M_DRAGONGUILDUNBINDGROUP_H__

// generate by ProtoGen at date: 2017/9/13 20:19:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_DRAGONGUILDUNBINDGROUP_TYPE 56553

class RpcC2M_DragonGuildUnBindGroup : public CRpc
{
private:

	RpcC2M_DragonGuildUnBindGroup():CRpc(RPCC2M_DRAGONGUILDUNBINDGROUP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_DragonGuildUnBindGroup *CreateRpc()
	{
		return new RpcC2M_DragonGuildUnBindGroup;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DragonGuildUnBindGroupArg &roArg, const DragonGuildUnBindGroupRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DragonGuildUnBindGroupArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DragonGuildUnBindGroupArg &roArg, DragonGuildUnBindGroupRes &roRes);
	void OnDelayReplyRpc(const DragonGuildUnBindGroupArg &roArg, DragonGuildUnBindGroupRes &roRes, const CUserData &roUserData);

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
	DragonGuildUnBindGroupArg m_oArg;
	DragonGuildUnBindGroupRes m_oRes;
};

#endif
