#ifndef __RPCO2M_GMTOOLCOMMAND2MS_H__
#define __RPCO2M_GMTOOLCOMMAND2MS_H__

// generate by ProtoGen at date: 2016/11/26 22:01:21

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCO2M_GMTOOLCOMMAND2MS_TYPE 20856

class RpcO2M_GMToolCommand2Ms : public CRpc
{
private:

	RpcO2M_GMToolCommand2Ms():CRpc(RPCO2M_GMTOOLCOMMAND2MS_TYPE)
	{
        m_dwTimeout = 20000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcO2M_GMToolCommand2Ms *CreateRpc()
	{
		return new RpcO2M_GMToolCommand2Ms;
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
