#ifndef __RPCM2N_CHANGESCENEFROMMS_H__
#define __RPCM2N_CHANGESCENEFROMMS_H__

// generate by ProtoGen at date: 2016/11/4 18:05:42

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2N_CHANGESCENEFROMMS_TYPE 8214

class RpcM2N_ChangeSceneFromMs : public CRpc
{
private:

	RpcM2N_ChangeSceneFromMs():CRpc(RPCM2N_CHANGESCENEFROMMS_TYPE)
	{
        m_dwTimeout = 50000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2N_ChangeSceneFromMs *CreateRpc()
	{
		return new RpcM2N_ChangeSceneFromMs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeSceneFromMsArg &roArg, const ChangeSceneFromMsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeSceneFromMsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeSceneFromMsArg &roArg, ChangeSceneFromMsRes &roRes);
	void OnDelayReplyRpc(const ChangeSceneFromMsArg &roArg, ChangeSceneFromMsRes &roRes, const CUserData &roUserData);

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
	ChangeSceneFromMsArg m_oArg;
	ChangeSceneFromMsRes m_oRes;
};

#endif
