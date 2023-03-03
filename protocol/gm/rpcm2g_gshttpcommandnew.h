#ifndef __RPCM2G_GSHTTPCOMMANDNEW_H__
#define __RPCM2G_GSHTTPCOMMANDNEW_H__

// generate by ProtoGen at date: 2016/7/16 11:48:36

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GSHTTPCOMMANDNEW_TYPE 40585

class RpcM2G_GSHttpCommandNew : public CRpc
{
private:

	RpcM2G_GSHttpCommandNew():CRpc(RPCM2G_GSHTTPCOMMANDNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GSHttpCommandNew *CreateRpc()
	{
		return new RpcM2G_GSHttpCommandNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GMHttpArg &roArg, const GMHttpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GMHttpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GMHttpArg &roArg, GMHttpRes &roRes);
	void OnDelayReplyRpc(const GMHttpArg &roArg, GMHttpRes &roRes, const CUserData &roUserData);

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
	GMHttpArg m_oArg;
	GMHttpRes m_oRes;
};

#endif
