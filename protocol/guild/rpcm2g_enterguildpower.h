#ifndef __RPCM2G_ENTERGUILDPOWER_H__
#define __RPCM2G_ENTERGUILDPOWER_H__

// generate by ProtoGen at date: 2016/8/29 17:36:36

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_ENTERGUILDPOWER_TYPE 63349

class RpcM2G_EnterGuildPower : public CRpc
{
private:

	RpcM2G_EnterGuildPower():CRpc(RPCM2G_ENTERGUILDPOWER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_EnterGuildPower *CreateRpc()
	{
		return new RpcM2G_EnterGuildPower;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnterGuildPowerArg &roArg, const EnterGuildPowerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnterGuildPowerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnterGuildPowerArg &roArg, EnterGuildPowerRes &roRes);
	void OnDelayReplyRpc(const EnterGuildPowerArg &roArg, EnterGuildPowerRes &roRes, const CUserData &roUserData);

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
	EnterGuildPowerArg m_oArg;
	EnterGuildPowerRes m_oRes;
};

#endif
