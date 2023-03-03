#ifndef __RPCG2M_SURVIVESYNPOINTG2M_H__
#define __RPCG2M_SURVIVESYNPOINTG2M_H__

// generate by ProtoGen at date: 2017/9/21 15:34:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_SURVIVESYNPOINTG2M_TYPE 11513

class RpcG2M_SurviveSynPointG2M : public CRpc
{
private:

	RpcG2M_SurviveSynPointG2M():CRpc(RPCG2M_SURVIVESYNPOINTG2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_SurviveSynPointG2M *CreateRpc()
	{
		return new RpcG2M_SurviveSynPointG2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SurviveSynPointArg &roArg, const SurviveSynPointRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SurviveSynPointArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SurviveSynPointArg &roArg, SurviveSynPointRes &roRes);
	void OnDelayReplyRpc(const SurviveSynPointArg &roArg, SurviveSynPointRes &roRes, const CUserData &roUserData);

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
	SurviveSynPointArg m_oArg;
	SurviveSynPointRes m_oRes;
};

#endif
