#ifndef __RPCG2K_BMRANKLISTREQ_H__
#define __RPCG2K_BMRANKLISTREQ_H__

// generate by ProtoGen at date: 2017/9/13 14:37:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2K_BMRANKLISTREQ_TYPE 41638

class RpcG2K_BMRankListReq : public CRpc
{
private:

	RpcG2K_BMRankListReq():CRpc(RPCG2K_BMRANKLISTREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2K_BMRankListReq *CreateRpc()
	{
		return new RpcG2K_BMRankListReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BMRankListArg &roArg, const BMRankListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BMRankListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BMRankListArg &roArg, BMRankListRes &roRes);
	void OnDelayReplyRpc(const BMRankListArg &roArg, BMRankListRes &roRes, const CUserData &roUserData);

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
	BMRankListArg m_oArg;
	BMRankListRes m_oRes;
};

#endif
