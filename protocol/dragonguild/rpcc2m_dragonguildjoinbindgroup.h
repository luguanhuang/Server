#ifndef __RPCC2M_DRAGONGUILDJOINBINDGROUP_H__
#define __RPCC2M_DRAGONGUILDJOINBINDGROUP_H__

// generate by ProtoGen at date: 2017/9/13 20:18:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_DRAGONGUILDJOINBINDGROUP_TYPE 33949

class RpcC2M_DragonGuildJoinBindGroup : public CRpc
{
private:

	RpcC2M_DragonGuildJoinBindGroup():CRpc(RPCC2M_DRAGONGUILDJOINBINDGROUP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_DragonGuildJoinBindGroup *CreateRpc()
	{
		return new RpcC2M_DragonGuildJoinBindGroup;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DragonGuildJoinBindGroupArg &roArg, const DragonGuildJoinBindGroupRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DragonGuildJoinBindGroupArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DragonGuildJoinBindGroupArg &roArg, DragonGuildJoinBindGroupRes &roRes);
	void OnDelayReplyRpc(const DragonGuildJoinBindGroupArg &roArg, DragonGuildJoinBindGroupRes &roRes, const CUserData &roUserData);

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
	DragonGuildJoinBindGroupArg m_oArg;
	DragonGuildJoinBindGroupRes m_oRes;
};

#endif
