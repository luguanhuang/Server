#ifndef __RPCC2M_SETSUBSCRIBE_H__
#define __RPCC2M_SETSUBSCRIBE_H__

// generate by ProtoGen at date: 2017/1/10 14:44:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_SETSUBSCRIBE_TYPE 40540

class RpcC2M_SetSubscribe : public CRpc
{
private:

	RpcC2M_SetSubscribe():CRpc(RPCC2M_SETSUBSCRIBE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_SetSubscribe *CreateRpc()
	{
		return new RpcC2M_SetSubscribe;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SetSubscirbeArg &roArg, const SetSubscribeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SetSubscirbeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SetSubscirbeArg &roArg, SetSubscribeRes &roRes);
	void OnDelayReplyRpc(const SetSubscirbeArg &roArg, SetSubscribeRes &roRes, const CUserData &roUserData);

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
	SetSubscirbeArg m_oArg;
	SetSubscribeRes m_oRes;
};

#endif
