#ifndef __RPCC2A_UPLOADAUDIOREQ_H__
#define __RPCC2A_UPLOADAUDIOREQ_H__

// generate by ProtoGen at date: 2016/6/26 20:08:21

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2A_UPLOADAUDIOREQ_TYPE 3069

class RpcC2A_UpLoadAudioReq : public CRpc
{
private:

	RpcC2A_UpLoadAudioReq():CRpc(RPCC2A_UPLOADAUDIOREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2A_UpLoadAudioReq *CreateRpc()
	{
		return new RpcC2A_UpLoadAudioReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpLoadAudioReq &roArg, const UpLoadAudioRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpLoadAudioReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpLoadAudioReq &roArg, UpLoadAudioRes &roRes);
	void OnDelayReplyRpc(const UpLoadAudioReq &roArg, UpLoadAudioRes &roRes, const CUserData &roUserData);

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
	UpLoadAudioReq m_oArg;
	UpLoadAudioRes m_oRes;
};

#endif
