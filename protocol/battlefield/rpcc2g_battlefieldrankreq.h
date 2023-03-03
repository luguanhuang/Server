#ifndef __RPCC2G_BATTLEFIELDRANKREQ_H__
#define __RPCC2G_BATTLEFIELDRANKREQ_H__

// generate by ProtoGen at date: 2017/9/4 15:27:16

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BATTLEFIELDRANKREQ_TYPE 4893

class RpcC2G_BattleFieldRankReq : public CRpc
{
private:

	RpcC2G_BattleFieldRankReq():CRpc(RPCC2G_BATTLEFIELDRANKREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BattleFieldRankReq *CreateRpc()
	{
		return new RpcC2G_BattleFieldRankReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BattleFieldRankArg &roArg, const BattleFieldRankRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BattleFieldRankArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BattleFieldRankArg &roArg, BattleFieldRankRes &roRes);
	void OnDelayReplyRpc(const BattleFieldRankArg &roArg, BattleFieldRankRes &roRes, const CUserData &roUserData);

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
	BattleFieldRankArg m_oArg;
	BattleFieldRankRes m_oRes;
};

#endif
