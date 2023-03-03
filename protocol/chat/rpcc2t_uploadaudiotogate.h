#ifndef __RPCC2T_UPLOADAUDIOTOGATE_H__
#define __RPCC2T_UPLOADAUDIOTOGATE_H__

// generate by ProtoGen at date: 2017/8/11 16:53:22

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2T_UPLOADAUDIOTOGATE_TYPE 23176

class RpcC2T_UpLoadAudioToGate : public CRpc
{
private:

	RpcC2T_UpLoadAudioToGate():CRpc(RPCC2T_UPLOADAUDIOTOGATE_TYPE)
	{
        m_dwTimeout = 20000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2T_UpLoadAudioToGate *CreateRpc()
	{
		return new RpcC2T_UpLoadAudioToGate;
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
