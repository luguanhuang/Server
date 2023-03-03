#ifndef __RPCM2S_M2SREQPLATUSERINFO_H__
#define __RPCM2S_M2SREQPLATUSERINFO_H__

// generate by ProtoGen at date: 2016/10/26 15:02:26

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2S_M2SREQPLATUSERINFO_TYPE 64184

class RpcM2S_M2SReqPlatUserInfo : public CRpc
{
private:

	RpcM2S_M2SReqPlatUserInfo():CRpc(RPCM2S_M2SREQPLATUSERINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2S_M2SReqPlatUserInfo *CreateRpc()
	{
		return new RpcM2S_M2SReqPlatUserInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const M2SReqPlatUserInfoArg &roArg, const M2SReqPlatUserInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const M2SReqPlatUserInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const M2SReqPlatUserInfoArg &roArg, M2SReqPlatUserInfoRes &roRes);
	void OnDelayReplyRpc(const M2SReqPlatUserInfoArg &roArg, M2SReqPlatUserInfoRes &roRes, const CUserData &roUserData);

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
	M2SReqPlatUserInfoArg m_oArg;
	M2SReqPlatUserInfoRes m_oRes;
};

#endif
