#ifndef __RPCC2G_GETGUILDBONUSLIST_H__
#define __RPCC2G_GETGUILDBONUSLIST_H__

// generate by ProtoGen at date: 2015/10/8 19:39:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDBONUSLIST_TYPE 43440

class RpcC2G_GetGuildBonusList : public CRpc
{
private:

	RpcC2G_GetGuildBonusList():CRpc(RPCC2G_GETGUILDBONUSLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildBonusList *CreateRpc()
	{
		return new RpcC2G_GetGuildBonusList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildBonusListArg &roArg, const GetGuildBonusListResult &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildBonusListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildBonusListArg &roArg, GetGuildBonusListResult &roRes);
	void OnDelayReplyRpc(const GetGuildBonusListArg &roArg, GetGuildBonusListResult &roRes, const CUserData &roUserData);

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
	GetGuildBonusListArg m_oArg;
	GetGuildBonusListResult m_oRes;
};

#endif
