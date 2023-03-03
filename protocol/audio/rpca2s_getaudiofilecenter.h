#ifndef __RPCA2S_GETAUDIOFILECENTER_H__
#define __RPCA2S_GETAUDIOFILECENTER_H__

// generate by ProtoGen at date: 2017/6/1 11:43:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCA2S_GETAUDIOFILECENTER_TYPE 16548

class RpcA2S_GetAudioFileCenter : public CRpc
{
private:

	RpcA2S_GetAudioFileCenter():CRpc(RPCA2S_GETAUDIOFILECENTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcA2S_GetAudioFileCenter *CreateRpc()
	{
		return new RpcA2S_GetAudioFileCenter;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetAudioListReq &roArg, const GetAudioListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetAudioListReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetAudioListReq &roArg, GetAudioListRes &roRes);
	void OnDelayReplyRpc(const GetAudioListReq &roArg, GetAudioListRes &roRes, const CUserData &roUserData);

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
	GetAudioListReq m_oArg;
	GetAudioListRes m_oRes;
};

#endif
