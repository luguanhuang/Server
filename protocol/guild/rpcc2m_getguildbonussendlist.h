#ifndef __RPCC2M_GETGUILDBONUSSENDLIST_H__
#define __RPCC2M_GETGUILDBONUSSENDLIST_H__

// generate by ProtoGen at date: 2016/11/2 10:26:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETGUILDBONUSSENDLIST_TYPE 59719

class RpcC2M_GetGuildBonusSendList : public CRpc
{
private:

	RpcC2M_GetGuildBonusSendList():CRpc(RPCC2M_GETGUILDBONUSSENDLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetGuildBonusSendList *CreateRpc()
	{
		return new RpcC2M_GetGuildBonusSendList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildBonusSendListArg &roArg, const GetGuildBonusSendListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildBonusSendListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildBonusSendListArg &roArg, GetGuildBonusSendListRes &roRes);
	void OnDelayReplyRpc(const GetGuildBonusSendListArg &roArg, GetGuildBonusSendListRes &roRes, const CUserData &roUserData);

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
	GetGuildBonusSendListArg m_oArg;
	GetGuildBonusSendListRes m_oRes;
};

#endif
