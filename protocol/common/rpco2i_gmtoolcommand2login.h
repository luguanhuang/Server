#ifndef __RPCO2I_GMTOOLCOMMAND2LOGIN_H__
#define __RPCO2I_GMTOOLCOMMAND2LOGIN_H__

// generate by ProtoGen at date: 2016/11/26 22:00:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCO2I_GMTOOLCOMMAND2LOGIN_TYPE 63875

class RpcO2I_GMToolCommand2Login : public CRpc
{
private:

	RpcO2I_GMToolCommand2Login():CRpc(RPCO2I_GMTOOLCOMMAND2LOGIN_TYPE)
	{
        m_dwTimeout = 120000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcO2I_GMToolCommand2Login *CreateRpc()
	{
		return new RpcO2I_GMToolCommand2Login;
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
