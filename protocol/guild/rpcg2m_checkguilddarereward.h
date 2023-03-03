#ifndef __RPCG2M_CHECKGUILDDAREREWARD_H__
#define __RPCG2M_CHECKGUILDDAREREWARD_H__

// generate by ProtoGen at date: 2016/9/11 18:34:28

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_CHECKGUILDDAREREWARD_TYPE 65431

class RpcG2M_CheckGuildDareReward : public CRpc
{
private:

	RpcG2M_CheckGuildDareReward():CRpc(RPCG2M_CHECKGUILDDAREREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_CheckGuildDareReward *CreateRpc()
	{
		return new RpcG2M_CheckGuildDareReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CheckGuildDareRewardArg &roArg, const CheckGuildDareRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CheckGuildDareRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CheckGuildDareRewardArg &roArg, CheckGuildDareRewardRes &roRes);
	void OnDelayReplyRpc(const CheckGuildDareRewardArg &roArg, CheckGuildDareRewardRes &roRes, const CUserData &roUserData);

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
	CheckGuildDareRewardArg m_oArg;
	CheckGuildDareRewardRes m_oRes;
};

#endif
