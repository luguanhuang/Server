#ifndef __RPCC2M_PAYNOTIFY_H__
#define __RPCC2M_PAYNOTIFY_H__

// generate by ProtoGen at date: 2016/10/24 11:50:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_PAYNOTIFY_TYPE 32125

class RpcC2M_PayNotify : public CRpc
{
private:

	RpcC2M_PayNotify():CRpc(RPCC2M_PAYNOTIFY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_PayNotify *CreateRpc()
	{
		return new RpcC2M_PayNotify;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PayNotifyArg &roArg, const PayNotifyRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PayNotifyArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PayNotifyArg &roArg, PayNotifyRes &roRes);
	void OnDelayReplyRpc(const PayNotifyArg &roArg, PayNotifyRes &roRes, const CUserData &roUserData);

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
	PayNotifyArg m_oArg;
	PayNotifyRes m_oRes;
};

#endif
