#ifndef __RPCM2K_GETSCENEINFO_H__
#define __RPCM2K_GETSCENEINFO_H__

// generate by ProtoGen at date: 2016/10/29 10:49:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_GETSCENEINFO_TYPE 28161

class RpcM2K_GetSceneInfo : public CRpc
{
private:

	RpcM2K_GetSceneInfo():CRpc(RPCM2K_GETSCENEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_GetSceneInfo *CreateRpc()
	{
		return new RpcM2K_GetSceneInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetSceneInfoArg &roArg, const GetSceneInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetSceneInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetSceneInfoArg &roArg, GetSceneInfoRes &roRes);
	void OnDelayReplyRpc(const GetSceneInfoArg &roArg, GetSceneInfoRes &roRes, const CUserData &roUserData);

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
	GetSceneInfoArg m_oArg;
	GetSceneInfoRes m_oRes;
};

#endif
