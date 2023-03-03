#ifndef __RPCC2M_FETCHPLATNOTICE_H__
#define __RPCC2M_FETCHPLATNOTICE_H__

// generate by ProtoGen at date: 2016/12/10 17:52:15

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_FETCHPLATNOTICE_TYPE 60271

class RpcC2M_FetchPlatNotice : public CRpc
{
private:

	RpcC2M_FetchPlatNotice():CRpc(RPCC2M_FETCHPLATNOTICE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_FetchPlatNotice *CreateRpc()
	{
		return new RpcC2M_FetchPlatNotice;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchPlatNoticeArg &roArg, const FetchPlatNoticeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchPlatNoticeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchPlatNoticeArg &roArg, FetchPlatNoticeRes &roRes);
	void OnDelayReplyRpc(const FetchPlatNoticeArg &roArg, FetchPlatNoticeRes &roRes, const CUserData &roUserData);

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
	FetchPlatNoticeArg m_oArg;
	FetchPlatNoticeRes m_oRes;
};

#endif
