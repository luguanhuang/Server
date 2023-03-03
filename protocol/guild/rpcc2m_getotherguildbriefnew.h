#ifndef __RPCC2M_GETOTHERGUILDBRIEFNEW_H__
#define __RPCC2M_GETOTHERGUILDBRIEFNEW_H__

// generate by ProtoGen at date: 2016/9/22 11:43:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETOTHERGUILDBRIEFNEW_TYPE 16797

class RpcC2M_GetOtherGuildBriefNew : public CRpc
{
private:

	RpcC2M_GetOtherGuildBriefNew():CRpc(RPCC2M_GETOTHERGUILDBRIEFNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetOtherGuildBriefNew *CreateRpc()
	{
		return new RpcC2M_GetOtherGuildBriefNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetOtherGuildBriefArg &roArg, const GetOtherGuildBriefRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetOtherGuildBriefArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetOtherGuildBriefArg &roArg, GetOtherGuildBriefRes &roRes);
	void OnDelayReplyRpc(const GetOtherGuildBriefArg &roArg, GetOtherGuildBriefRes &roRes, const CUserData &roUserData);

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
	GetOtherGuildBriefArg m_oArg;
	GetOtherGuildBriefRes m_oRes;
};

#endif
