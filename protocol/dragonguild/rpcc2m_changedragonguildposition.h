#ifndef __RPCC2M_CHANGEDRAGONGUILDPOSITION_H__
#define __RPCC2M_CHANGEDRAGONGUILDPOSITION_H__

// generate by ProtoGen at date: 2017/9/13 19:59:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CHANGEDRAGONGUILDPOSITION_TYPE 3888

class RpcC2M_ChangeDragonGuildPosition : public CRpc
{
private:

	RpcC2M_ChangeDragonGuildPosition():CRpc(RPCC2M_CHANGEDRAGONGUILDPOSITION_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ChangeDragonGuildPosition *CreateRpc()
	{
		return new RpcC2M_ChangeDragonGuildPosition;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeDragonGuildPositionArg &roArg, const ChangeDragonGuildPositionRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeDragonGuildPositionArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeDragonGuildPositionArg &roArg, ChangeDragonGuildPositionRes &roRes);
	void OnDelayReplyRpc(const ChangeDragonGuildPositionArg &roArg, ChangeDragonGuildPositionRes &roRes, const CUserData &roUserData);

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
	ChangeDragonGuildPositionArg m_oArg;
	ChangeDragonGuildPositionRes m_oRes;
};

#endif
