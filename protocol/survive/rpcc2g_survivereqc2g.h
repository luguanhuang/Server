#ifndef __RPCC2G_SURVIVEREQC2G_H__
#define __RPCC2G_SURVIVEREQC2G_H__

// generate by ProtoGen at date: 2017/9/20 17:49:28

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SURVIVEREQC2G_TYPE 19408

class RpcC2G_SurviveReqC2G : public CRpc
{
private:

	RpcC2G_SurviveReqC2G():CRpc(RPCC2G_SURVIVEREQC2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SurviveReqC2G *CreateRpc()
	{
		return new RpcC2G_SurviveReqC2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SurviveReqArg &roArg, const SurviveReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SurviveReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SurviveReqArg &roArg, SurviveReqRes &roRes);
	void OnDelayReplyRpc(const SurviveReqArg &roArg, SurviveReqRes &roRes, const CUserData &roUserData);

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
	SurviveReqArg m_oArg;
	SurviveReqRes m_oRes;
};

#endif
