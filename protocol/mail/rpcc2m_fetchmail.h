#ifndef __RPCC2M_FETCHMAIL_H__
#define __RPCC2M_FETCHMAIL_H__

// generate by ProtoGen at date: 2016/7/22 10:18:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_FETCHMAIL_TYPE 12373

class RpcC2M_FetchMail : public CRpc
{
private:

	RpcC2M_FetchMail():CRpc(RPCC2M_FETCHMAIL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_FetchMail *CreateRpc()
	{
		return new RpcC2M_FetchMail;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchMailArg &roArg, const FetchMailRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchMailArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchMailArg &roArg, FetchMailRes &roRes);
	void OnDelayReplyRpc(const FetchMailArg &roArg, FetchMailRes &roRes, const CUserData &roUserData);

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
	FetchMailArg m_oArg;
	FetchMailRes m_oRes;
};

#endif
