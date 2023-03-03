#ifndef __RPCC2M_GETLISTSUBSCRIBE_H__
#define __RPCC2M_GETLISTSUBSCRIBE_H__

// generate by ProtoGen at date: 2017/1/10 14:37:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETLISTSUBSCRIBE_TYPE 1403

class RpcC2M_GetListSubscribe : public CRpc
{
private:

	RpcC2M_GetListSubscribe():CRpc(RPCC2M_GETLISTSUBSCRIBE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetListSubscribe *CreateRpc()
	{
		return new RpcC2M_GetListSubscribe;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetListSubscribeArg &roArg, const GetListSubscribeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetListSubscribeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetListSubscribeArg &roArg, GetListSubscribeRes &roRes);
	void OnDelayReplyRpc(const GetListSubscribeArg &roArg, GetListSubscribeRes &roRes, const CUserData &roUserData);

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
	GetListSubscribeArg m_oArg;
	GetListSubscribeRes m_oRes;
};

#endif
