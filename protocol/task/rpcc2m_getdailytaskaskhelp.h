#ifndef __RPCC2M_GETDAILYTASKASKHELP_H__
#define __RPCC2M_GETDAILYTASKASKHELP_H__

// generate by ProtoGen at date: 2017/9/18 1:36:22

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETDAILYTASKASKHELP_TYPE 46394

class RpcC2M_GetDailyTaskAskHelp : public CRpc
{
private:

	RpcC2M_GetDailyTaskAskHelp():CRpc(RPCC2M_GETDAILYTASKASKHELP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetDailyTaskAskHelp *CreateRpc()
	{
		return new RpcC2M_GetDailyTaskAskHelp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDailyTaskAskHelpArg &roArg, const GetDailyTaskAskHelpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDailyTaskAskHelpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDailyTaskAskHelpArg &roArg, GetDailyTaskAskHelpRes &roRes);
	void OnDelayReplyRpc(const GetDailyTaskAskHelpArg &roArg, GetDailyTaskAskHelpRes &roRes, const CUserData &roUserData);

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
	GetDailyTaskAskHelpArg m_oArg;
	GetDailyTaskAskHelpRes m_oRes;
};

#endif
