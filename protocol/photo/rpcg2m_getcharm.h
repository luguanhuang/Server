#ifndef __RPCG2M_GETCHARM_H__
#define __RPCG2M_GETCHARM_H__

// generate by ProtoGen at date: 2016/12/15 9:00:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GETCHARM_TYPE 6728

class RpcG2M_GetCharm : public CRpc
{
private:

	RpcG2M_GetCharm():CRpc(RPCG2M_GETCHARM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GetCharm *CreateRpc()
	{
		return new RpcG2M_GetCharm;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetCharmArg &roArg, const GetCharmRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetCharmArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetCharmArg &roArg, GetCharmRes &roRes);
	void OnDelayReplyRpc(const GetCharmArg &roArg, GetCharmRes &roRes, const CUserData &roUserData);

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
	GetCharmArg m_oArg;
	GetCharmRes m_oRes;
};

#endif
