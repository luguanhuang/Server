#ifndef __RPCC2G_BATTLEFIELDAWARDNUMREQ_H__
#define __RPCC2G_BATTLEFIELDAWARDNUMREQ_H__

// generate by ProtoGen at date: 2017/9/4 14:06:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BATTLEFIELDAWARDNUMREQ_TYPE 59171

class RpcC2G_BattleFieldAwardNumReq : public CRpc
{
private:

	RpcC2G_BattleFieldAwardNumReq():CRpc(RPCC2G_BATTLEFIELDAWARDNUMREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BattleFieldAwardNumReq *CreateRpc()
	{
		return new RpcC2G_BattleFieldAwardNumReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BattleFieldAwardNumArg &roArg, const BattleFieldAwardNumRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BattleFieldAwardNumArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BattleFieldAwardNumArg &roArg, BattleFieldAwardNumRes &roRes);
	void OnDelayReplyRpc(const BattleFieldAwardNumArg &roArg, BattleFieldAwardNumRes &roRes, const CUserData &roUserData);

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
	BattleFieldAwardNumArg m_oArg;
	BattleFieldAwardNumRes m_oRes;
};

#endif
