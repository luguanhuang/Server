#ifndef __RPCC2G_REPORTBADPLAYER_H__
#define __RPCC2G_REPORTBADPLAYER_H__

// generate by ProtoGen at date: 2017/5/10 16:10:32

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REPORTBADPLAYER_TYPE 32807

class RpcC2G_ReportBadPlayer : public CRpc
{
private:

	RpcC2G_ReportBadPlayer():CRpc(RPCC2G_REPORTBADPLAYER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ReportBadPlayer *CreateRpc()
	{
		return new RpcC2G_ReportBadPlayer;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReportBadPlayerArg &roArg, const ReportBadPlayerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReportBadPlayerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReportBadPlayerArg &roArg, ReportBadPlayerRes &roRes);
	void OnDelayReplyRpc(const ReportBadPlayerArg &roArg, ReportBadPlayerRes &roRes, const CUserData &roUserData);

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
	ReportBadPlayerArg m_oArg;
	ReportBadPlayerRes m_oRes;
};

#endif
