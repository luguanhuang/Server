#ifndef __RPCM2G_CHANGENAMEGS_H__
#define __RPCM2G_CHANGENAMEGS_H__

// generate by ProtoGen at date: 2016/10/10 17:40:30

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_CHANGENAMEGS_TYPE 54716

class RpcM2G_ChangeNameGs : public CRpc
{
private:

	RpcM2G_ChangeNameGs():CRpc(RPCM2G_CHANGENAMEGS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_ChangeNameGs *CreateRpc()
	{
		return new RpcM2G_ChangeNameGs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeNameGsArg &roArg, const ChangeNameGsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeNameGsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeNameGsArg &roArg, ChangeNameGsRes &roRes);
	void OnDelayReplyRpc(const ChangeNameGsArg &roArg, ChangeNameGsRes &roRes, const CUserData &roUserData);

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
	ChangeNameGsArg m_oArg;
	ChangeNameGsRes m_oRes;
};

#endif
