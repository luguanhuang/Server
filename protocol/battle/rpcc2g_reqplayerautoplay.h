#ifndef __RPCC2G_REQPLAYERAUTOPLAY_H__
#define __RPCC2G_REQPLAYERAUTOPLAY_H__

// generate by ProtoGen at date: 2016/6/16 14:26:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REQPLAYERAUTOPLAY_TYPE 3718

class RpcC2G_ReqPlayerAutoPlay : public CRpc
{
private:

	RpcC2G_ReqPlayerAutoPlay():CRpc(RPCC2G_REQPLAYERAUTOPLAY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ReqPlayerAutoPlay *CreateRpc()
	{
		return new RpcC2G_ReqPlayerAutoPlay;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqAutoPlay &roArg, const RetAutoPlay &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqAutoPlay &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqAutoPlay &roArg, RetAutoPlay &roRes);
	void OnDelayReplyRpc(const ReqAutoPlay &roArg, RetAutoPlay &roRes, const CUserData &roUserData);

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
	ReqAutoPlay m_oArg;
	RetAutoPlay m_oRes;
};

#endif
