#ifndef __RPCK2G_CROSSGSRELOAD_H__
#define __RPCK2G_CROSSGSRELOAD_H__

// generate by ProtoGen at date: 2017/2/16 20:26:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCK2G_CROSSGSRELOAD_TYPE 42258

class RpcK2G_CrossGsReload : public CRpc
{
private:

	RpcK2G_CrossGsReload():CRpc(RPCK2G_CROSSGSRELOAD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcK2G_CrossGsReload *CreateRpc()
	{
		return new RpcK2G_CrossGsReload;
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
