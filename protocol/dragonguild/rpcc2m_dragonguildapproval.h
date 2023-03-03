#ifndef __RPCC2M_DRAGONGUILDAPPROVAL_H__
#define __RPCC2M_DRAGONGUILDAPPROVAL_H__

// generate by ProtoGen at date: 2017/9/7 17:22:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_DRAGONGUILDAPPROVAL_TYPE 4753

class RpcC2M_DragonGuildApproval : public CRpc
{
private:

	RpcC2M_DragonGuildApproval():CRpc(RPCC2M_DRAGONGUILDAPPROVAL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_DragonGuildApproval *CreateRpc()
	{
		return new RpcC2M_DragonGuildApproval;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DragonGuildApprovalArg &roArg, const DragonGuildApprovalRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DragonGuildApprovalArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DragonGuildApprovalArg &roArg, DragonGuildApprovalRes &roRes);
	void OnDelayReplyRpc(const DragonGuildApprovalArg &roArg, DragonGuildApprovalRes &roRes, const CUserData &roUserData);

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
	DragonGuildApprovalArg m_oArg;
	DragonGuildApprovalRes m_oRes;
};

#endif
