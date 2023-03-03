#ifndef __RPCC2M_CHANGEMEMBERPOSITIONNEW_H__
#define __RPCC2M_CHANGEMEMBERPOSITIONNEW_H__

// generate by ProtoGen at date: 2016/9/20 17:44:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CHANGEMEMBERPOSITIONNEW_TYPE 13625

class RpcC2M_ChangeMemberPositionNew : public CRpc
{
private:

	RpcC2M_ChangeMemberPositionNew():CRpc(RPCC2M_CHANGEMEMBERPOSITIONNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ChangeMemberPositionNew *CreateRpc()
	{
		return new RpcC2M_ChangeMemberPositionNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeGuildPositionArg &roArg, const ChangeGuildPositionRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeGuildPositionArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeGuildPositionArg &roArg, ChangeGuildPositionRes &roRes);
	void OnDelayReplyRpc(const ChangeGuildPositionArg &roArg, ChangeGuildPositionRes &roRes, const CUserData &roUserData);

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
	ChangeGuildPositionArg m_oArg;
	ChangeGuildPositionRes m_oRes;
};

#endif
