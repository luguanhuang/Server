#ifndef __RPCC2M_ASKDRAGONGUILDMEMBERS_H__
#define __RPCC2M_ASKDRAGONGUILDMEMBERS_H__

// generate by ProtoGen at date: 2017/9/8 16:23:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ASKDRAGONGUILDMEMBERS_TYPE 26644

class RpcC2M_AskDragonGuildMembers : public CRpc
{
private:

	RpcC2M_AskDragonGuildMembers():CRpc(RPCC2M_ASKDRAGONGUILDMEMBERS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AskDragonGuildMembers *CreateRpc()
	{
		return new RpcC2M_AskDragonGuildMembers;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DragonGuildMemberArg &roArg, const DragonGuildMemberRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DragonGuildMemberArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DragonGuildMemberArg &roArg, DragonGuildMemberRes &roRes);
	void OnDelayReplyRpc(const DragonGuildMemberArg &roArg, DragonGuildMemberRes &roRes, const CUserData &roUserData);

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
	DragonGuildMemberArg m_oArg;
	DragonGuildMemberRes m_oRes;
};

#endif
