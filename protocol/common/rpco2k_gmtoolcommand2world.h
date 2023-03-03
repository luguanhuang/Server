#ifndef __RPCO2K_GMTOOLCOMMAND2WORLD_H__
#define __RPCO2K_GMTOOLCOMMAND2WORLD_H__

// generate by ProtoGen at date: 2017/2/16 20:20:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCO2K_GMTOOLCOMMAND2WORLD_TYPE 48620

class RpcO2K_GMToolCommand2World : public CRpc
{
private:

	RpcO2K_GMToolCommand2World():CRpc(RPCO2K_GMTOOLCOMMAND2WORLD_TYPE)
	{
        m_dwTimeout = 20000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcO2K_GMToolCommand2World *CreateRpc()
	{
		return new RpcO2K_GMToolCommand2World;
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
