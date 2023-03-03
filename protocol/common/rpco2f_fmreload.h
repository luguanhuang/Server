#ifndef __RPCO2F_FMRELOAD_H__
#define __RPCO2F_FMRELOAD_H__

// generate by ProtoGen at date: 2017/2/14 13:51:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCO2F_FMRELOAD_TYPE 43414

class RpcO2F_FMReload : public CRpc
{
private:

	RpcO2F_FMReload():CRpc(RPCO2F_FMRELOAD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcO2F_FMReload *CreateRpc()
	{
		return new RpcO2F_FMReload;
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
