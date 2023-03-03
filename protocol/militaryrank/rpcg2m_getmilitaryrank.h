#ifndef __RPCG2M_GETMILITARYRANK_H__
#define __RPCG2M_GETMILITARYRANK_H__

// generate by ProtoGen at date: 2017/3/10 17:34:46

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GETMILITARYRANK_TYPE 8041

class RpcG2M_GetMilitaryRank : public CRpc
{
private:

	RpcG2M_GetMilitaryRank():CRpc(RPCG2M_GETMILITARYRANK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GetMilitaryRank *CreateRpc()
	{
		return new RpcG2M_GetMilitaryRank;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMilitaryRankArg &roArg, const GetMilitaryRankRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMilitaryRankArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMilitaryRankArg &roArg, GetMilitaryRankRes &roRes);
	void OnDelayReplyRpc(const GetMilitaryRankArg &roArg, GetMilitaryRankRes &roRes, const CUserData &roUserData);

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
	GetMilitaryRankArg m_oArg;
	GetMilitaryRankRes m_oRes;
};

#endif
