#ifndef __RPCM2K_QUERYBIGMELEERANK2K_H__
#define __RPCM2K_QUERYBIGMELEERANK2K_H__

// generate by ProtoGen at date: 2017/6/20 16:34:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_QUERYBIGMELEERANK2K_TYPE 17912

class RpcM2K_QueryBigMeleeRank2k : public CRpc
{
private:

	RpcM2K_QueryBigMeleeRank2k():CRpc(RPCM2K_QUERYBIGMELEERANK2K_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_QueryBigMeleeRank2k *CreateRpc()
	{
		return new RpcM2K_QueryBigMeleeRank2k;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryMayhemRank2kArg &roArg, const QueryMayhemRankRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryMayhemRank2kArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryMayhemRank2kArg &roArg, QueryMayhemRankRes &roRes);
	void OnDelayReplyRpc(const QueryMayhemRank2kArg &roArg, QueryMayhemRankRes &roRes, const CUserData &roUserData);

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
	QueryMayhemRank2kArg m_oArg;
	QueryMayhemRankRes m_oRes;
};

#endif
