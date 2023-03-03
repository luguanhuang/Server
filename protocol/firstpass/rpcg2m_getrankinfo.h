#ifndef __RPCG2M_GETRANKINFO_H__
#define __RPCG2M_GETRANKINFO_H__

// generate by ProtoGen at date: 2016/8/24 21:04:28

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GETRANKINFO_TYPE 4357

class RpcG2M_GetRankInfo : public CRpc
{
private:

	RpcG2M_GetRankInfo():CRpc(RPCG2M_GETRANKINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GetRankInfo *CreateRpc()
	{
		return new RpcG2M_GetRankInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetRankInfoArg &roArg, const GetRankInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetRankInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetRankInfoArg &roArg, GetRankInfoRes &roRes);
	void OnDelayReplyRpc(const GetRankInfoArg &roArg, GetRankInfoRes &roRes, const CUserData &roUserData);

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
	GetRankInfoArg m_oArg;
	GetRankInfoRes m_oRes;
};

#endif
