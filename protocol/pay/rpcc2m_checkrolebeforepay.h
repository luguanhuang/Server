#ifndef __RPCC2M_CHECKROLEBEFOREPAY_H__
#define __RPCC2M_CHECKROLEBEFOREPAY_H__

// generate by ProtoGen at date: 2017/7/28 10:11:38

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CHECKROLEBEFOREPAY_TYPE 56255

class RpcC2M_CheckRoleBeforePay : public CRpc
{
private:

	RpcC2M_CheckRoleBeforePay():CRpc(RPCC2M_CHECKROLEBEFOREPAY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_CheckRoleBeforePay *CreateRpc()
	{
		return new RpcC2M_CheckRoleBeforePay;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CheckRoleBeforePayArg &roArg, const CheckRoleBeforePayRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CheckRoleBeforePayArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CheckRoleBeforePayArg &roArg, CheckRoleBeforePayRes &roRes);
	void OnDelayReplyRpc(const CheckRoleBeforePayArg &roArg, CheckRoleBeforePayRes &roRes, const CUserData &roUserData);

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
	CheckRoleBeforePayArg m_oArg;
	CheckRoleBeforePayRes m_oRes;
};

#endif
