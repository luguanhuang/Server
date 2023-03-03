#ifndef __RPCC2G_GETFLOWERLEFTTIME_H__
#define __RPCC2G_GETFLOWERLEFTTIME_H__

// generate by ProtoGen at date: 2016/6/25 15:46:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETFLOWERLEFTTIME_TYPE 26834

class RpcC2G_GetFlowerLeftTime : public CRpc
{
private:

	RpcC2G_GetFlowerLeftTime():CRpc(RPCC2G_GETFLOWERLEFTTIME_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetFlowerLeftTime *CreateRpc()
	{
		return new RpcC2G_GetFlowerLeftTime;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetFlowerLeftTimeArg &roArg, const GetFlowerLeftTimeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetFlowerLeftTimeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetFlowerLeftTimeArg &roArg, GetFlowerLeftTimeRes &roRes);
	void OnDelayReplyRpc(const GetFlowerLeftTimeArg &roArg, GetFlowerLeftTimeRes &roRes, const CUserData &roUserData);

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
	GetFlowerLeftTimeArg m_oArg;
	GetFlowerLeftTimeRes m_oRes;
};

#endif
