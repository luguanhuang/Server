#ifndef __RPCG2M_MGUILDCHECKINBONUSINFO_H__
#define __RPCG2M_MGUILDCHECKINBONUSINFO_H__

// generate by ProtoGen at date: 2016/9/15 23:47:38

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_MGUILDCHECKINBONUSINFO_TYPE 60478

class RpcG2M_MGuildCheckInBonusInfo : public CRpc
{
private:

	RpcG2M_MGuildCheckInBonusInfo():CRpc(RPCG2M_MGUILDCHECKINBONUSINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_MGuildCheckInBonusInfo *CreateRpc()
	{
		return new RpcG2M_MGuildCheckInBonusInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildCheckInBonusInfoArg &roArg, const GuildCheckInBonusInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildCheckInBonusInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildCheckInBonusInfoArg &roArg, GuildCheckInBonusInfoRes &roRes);
	void OnDelayReplyRpc(const GuildCheckInBonusInfoArg &roArg, GuildCheckInBonusInfoRes &roRes, const CUserData &roUserData);

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
	GuildCheckInBonusInfoArg m_oArg;
	GuildCheckInBonusInfoRes m_oRes;
};

#endif
