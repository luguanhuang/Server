#ifndef __RPCC2G_REPORTBATTLE_H__
#define __RPCC2G_REPORTBATTLE_H__

// generate by ProtoGen at date: 2015/10/20 17:51:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REPORTBATTLE_TYPE 21292

class RpcC2G_ReportBattle : public CRpc
{
private:

	RpcC2G_ReportBattle():CRpc(RPCC2G_REPORTBATTLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ReportBattle *CreateRpc()
	{
		return new RpcC2G_ReportBattle;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReportBattleArg &roArg, const ReportBattleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReportBattleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReportBattleArg &roArg, ReportBattleRes &roRes);
	void OnDelayReplyRpc(const ReportBattleArg &roArg, ReportBattleRes &roRes, const CUserData &roUserData);

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
	ReportBattleArg m_oArg;
	ReportBattleRes m_oRes;
};

#endif
