#ifndef __RPCC2G_FIRSTPASSINFOREQ_H__
#define __RPCC2G_FIRSTPASSINFOREQ_H__

// generate by ProtoGen at date: 2016/8/24 16:20:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FIRSTPASSINFOREQ_TYPE 4147

class RpcC2G_FirstPassInfoReq : public CRpc
{
private:

	RpcC2G_FirstPassInfoReq():CRpc(RPCC2G_FIRSTPASSINFOREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FirstPassInfoReq *CreateRpc()
	{
		return new RpcC2G_FirstPassInfoReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FirstPassInfoReqArg &roArg, const FirstPassInfoReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FirstPassInfoReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FirstPassInfoReqArg &roArg, FirstPassInfoReqRes &roRes);
	void OnDelayReplyRpc(const FirstPassInfoReqArg &roArg, FirstPassInfoReqRes &roRes, const CUserData &roUserData);

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
	FirstPassInfoReqArg m_oArg;
	FirstPassInfoReqRes m_oRes;
};

#endif
