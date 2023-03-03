#ifndef __RPCC2G_EMBLEMLEVELUP_H__
#define __RPCC2G_EMBLEMLEVELUP_H__

// generate by ProtoGen at date: 2016/10/28 22:23:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_EMBLEMLEVELUP_TYPE 9893

class RpcC2G_EmblemLevelUp : public CRpc
{
private:

	RpcC2G_EmblemLevelUp():CRpc(RPCC2G_EMBLEMLEVELUP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_EmblemLevelUp *CreateRpc()
	{
		return new RpcC2G_EmblemLevelUp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EmblemLevelUpArg &roArg, const EmblemLevelUpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EmblemLevelUpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EmblemLevelUpArg &roArg, EmblemLevelUpRes &roRes);
	void OnDelayReplyRpc(const EmblemLevelUpArg &roArg, EmblemLevelUpRes &roRes, const CUserData &roUserData);

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
	EmblemLevelUpArg m_oArg;
	EmblemLevelUpRes m_oRes;
};

#endif
