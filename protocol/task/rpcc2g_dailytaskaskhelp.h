#ifndef __RPCC2G_DAILYTASKASKHELP_H__
#define __RPCC2G_DAILYTASKASKHELP_H__

// generate by ProtoGen at date: 2016/11/16 10:01:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_DAILYTASKASKHELP_TYPE 9236

class RpcC2G_DailyTaskAskHelp : public CRpc
{
private:

	RpcC2G_DailyTaskAskHelp():CRpc(RPCC2G_DAILYTASKASKHELP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_DailyTaskAskHelp *CreateRpc()
	{
		return new RpcC2G_DailyTaskAskHelp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DailyTaskAskHelpArg &roArg, const DailyTaskAskHelpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DailyTaskAskHelpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DailyTaskAskHelpArg &roArg, DailyTaskAskHelpRes &roRes);
	void OnDelayReplyRpc(const DailyTaskAskHelpArg &roArg, DailyTaskAskHelpRes &roRes, const CUserData &roUserData);

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
	DailyTaskAskHelpArg m_oArg;
	DailyTaskAskHelpRes m_oRes;
};

#endif
