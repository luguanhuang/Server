#ifndef __RPCC2A_GETAUDIOLISTREQ_H__
#define __RPCC2A_GETAUDIOLISTREQ_H__

// generate by ProtoGen at date: 2016/6/26 19:57:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2A_GETAUDIOLISTREQ_TYPE 49666

class RpcC2A_GetAudioListReq : public CRpc
{
private:

	RpcC2A_GetAudioListReq():CRpc(RPCC2A_GETAUDIOLISTREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2A_GetAudioListReq *CreateRpc()
	{
		return new RpcC2A_GetAudioListReq;
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
