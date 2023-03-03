#ifndef __RPCC2G_REVIVE_H__
#define __RPCC2G_REVIVE_H__

// generate by ProtoGen at date: 2015/7/14 15:33:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REVIVE_TYPE 29831

class RpcC2G_Revive : public CRpc
{
private:

	RpcC2G_Revive():CRpc(RPCC2G_REVIVE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_Revive *CreateRpc()
	{
		return new RpcC2G_Revive;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReviveArg &roArg, const ReviveRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReviveArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReviveArg &roArg, ReviveRes &roRes);
	void OnDelayReplyRpc(const ReviveArg &roArg, ReviveRes &roRes, const CUserData &roUserData);

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
	ReviveArg m_oArg;
	ReviveRes m_oRes;
};

#endif
