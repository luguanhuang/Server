#ifndef __RPCM2G_REQGUILDDAREREWARDREC_H__
#define __RPCM2G_REQGUILDDAREREWARDREC_H__

// generate by ProtoGen at date: 2016/9/11 18:11:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_REQGUILDDAREREWARDREC_TYPE 1896

class RpcM2G_ReqGuildDareRewardRec : public CRpc
{
private:

	RpcM2G_ReqGuildDareRewardRec():CRpc(RPCM2G_REQGUILDDAREREWARDREC_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_ReqGuildDareRewardRec *CreateRpc()
	{
		return new RpcM2G_ReqGuildDareRewardRec;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildDareRewardRecArg &roArg, const ReqGuildDareRewardRecRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildDareRewardRecArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildDareRewardRecArg &roArg, ReqGuildDareRewardRecRes &roRes);
	void OnDelayReplyRpc(const ReqGuildDareRewardRecArg &roArg, ReqGuildDareRewardRecRes &roRes, const CUserData &roUserData);

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
	ReqGuildDareRewardRecArg m_oArg;
	ReqGuildDareRewardRecRes m_oRes;
};

#endif
