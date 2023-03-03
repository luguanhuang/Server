#ifndef __RPCM2D_DBRELOAD_H__
#define __RPCM2D_DBRELOAD_H__

// generate by ProtoGen at date: 2016/9/28 0:17:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_DBRELOAD_TYPE 15224

class RpcM2D_DbReload : public CRpc
{
private:

	RpcM2D_DbReload():CRpc(RPCM2D_DBRELOAD_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_DbReload *CreateRpc()
	{
		return new RpcM2D_DbReload;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GMToolCommandArg &roArg, const GMToolCommandRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GMToolCommandArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GMToolCommandArg &roArg, GMToolCommandRes &roRes);
	void OnDelayReplyRpc(const GMToolCommandArg &roArg, GMToolCommandRes &roRes, const CUserData &roUserData);

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
	GMToolCommandArg m_oArg;
	GMToolCommandRes m_oRes;
};

#endif
