#ifndef __RPCC2G_GETGUILDBONUSLEFT_H__
#define __RPCC2G_GETGUILDBONUSLEFT_H__

// generate by ProtoGen at date: 2016/12/24 11:57:15

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDBONUSLEFT_TYPE 9967

class RpcC2G_GetGuildBonusLeft : public CRpc
{
private:

	RpcC2G_GetGuildBonusLeft():CRpc(RPCC2G_GETGUILDBONUSLEFT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildBonusLeft *CreateRpc()
	{
		return new RpcC2G_GetGuildBonusLeft;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildBonusLeftArg &roArg, const GetGuildBonusLeftRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildBonusLeftArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildBonusLeftArg &roArg, GetGuildBonusLeftRes &roRes);
	void OnDelayReplyRpc(const GetGuildBonusLeftArg &roArg, GetGuildBonusLeftRes &roRes, const CUserData &roUserData);

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
	GetGuildBonusLeftArg m_oArg;
	GetGuildBonusLeftRes m_oRes;
};

#endif
