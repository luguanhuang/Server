#ifndef __RPCC2M_CHANGENAMENEW_H__
#define __RPCC2M_CHANGENAMENEW_H__

// generate by ProtoGen at date: 2016/10/10 18:47:31

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CHANGENAMENEW_TYPE 46227

class RpcC2M_ChangeNameNew : public CRpc
{
private:

	RpcC2M_ChangeNameNew():CRpc(RPCC2M_CHANGENAMENEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ChangeNameNew *CreateRpc()
	{
		return new RpcC2M_ChangeNameNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeNameArg &roArg, const ChangeNameRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeNameArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeNameArg &roArg, ChangeNameRes &roRes);
	void OnDelayReplyRpc(const ChangeNameArg &roArg, ChangeNameRes &roRes, const CUserData &roUserData);

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
	ChangeNameArg m_oArg;
	ChangeNameRes m_oRes;
};

#endif
