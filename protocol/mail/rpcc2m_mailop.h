#ifndef __RPCC2M_MAILOP_H__
#define __RPCC2M_MAILOP_H__

// generate by ProtoGen at date: 2016/7/22 2:13:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_MAILOP_TYPE 50122

class RpcC2M_MailOp : public CRpc
{
private:

	RpcC2M_MailOp():CRpc(RPCC2M_MAILOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_MailOp *CreateRpc()
	{
		return new RpcC2M_MailOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MailOpArg &roArg, const MailOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MailOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MailOpArg &roArg, MailOpRes &roRes);
	void OnDelayReplyRpc(const MailOpArg &roArg, MailOpRes &roRes, const CUserData &roUserData);

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
	MailOpArg m_oArg;
	MailOpRes m_oRes;
};

#endif
