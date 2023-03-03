#ifndef __RPCK2H_TSRELOAD_H__
#define __RPCK2H_TSRELOAD_H__

// generate by ProtoGen at date: 2017/6/23 20:43:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCK2H_TSRELOAD_TYPE 3695

class RpcK2H_TsReload : public CRpc
{
private:

	RpcK2H_TsReload():CRpc(RPCK2H_TSRELOAD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcK2H_TsReload *CreateRpc()
	{
		return new RpcK2H_TsReload;
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
