#ifndef __RPCK2M_GETROLESUMMARYFROMMS_H__
#define __RPCK2M_GETROLESUMMARYFROMMS_H__

// generate by ProtoGen at date: 2017/2/2 22:16:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCK2M_GETROLESUMMARYFROMMS_TYPE 35

class RpcK2M_GetRoleSummaryFromMS : public CRpc
{
private:

	RpcK2M_GetRoleSummaryFromMS():CRpc(RPCK2M_GETROLESUMMARYFROMMS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcK2M_GetRoleSummaryFromMS *CreateRpc()
	{
		return new RpcK2M_GetRoleSummaryFromMS;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetRoleSummaryFromMSArg &roArg, const GetRoleSummaryFromMSRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetRoleSummaryFromMSArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetRoleSummaryFromMSArg &roArg, GetRoleSummaryFromMSRes &roRes);
	void OnDelayReplyRpc(const GetRoleSummaryFromMSArg &roArg, GetRoleSummaryFromMSRes &roRes, const CUserData &roUserData);

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
	GetRoleSummaryFromMSArg m_oArg;
	GetRoleSummaryFromMSRes m_oRes;
};

#endif
