#ifndef __RPCM2K_GMCOMMANDM2W_H__
#define __RPCM2K_GMCOMMANDM2W_H__

// generate by ProtoGen at date: 2017/2/4 21:15:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_GMCOMMANDM2W_TYPE 3038

class RpcM2K_GMCommandM2W : public CRpc
{
private:

	RpcM2K_GMCommandM2W():CRpc(RPCM2K_GMCOMMANDM2W_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_GMCommandM2W *CreateRpc()
	{
		return new RpcM2K_GMCommandM2W;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GMCmdg2mArg &roArg, const GMCmdg2mRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GMCmdg2mArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GMCmdg2mArg &roArg, GMCmdg2mRes &roRes);
	void OnDelayReplyRpc(const GMCmdg2mArg &roArg, GMCmdg2mRes &roRes, const CUserData &roUserData);

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
	GMCmdg2mArg m_oArg;
	GMCmdg2mRes m_oRes;
};

#endif
