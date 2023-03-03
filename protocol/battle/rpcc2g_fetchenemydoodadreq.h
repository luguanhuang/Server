#ifndef __RPCC2G_FETCHENEMYDOODADREQ_H__
#define __RPCC2G_FETCHENEMYDOODADREQ_H__

// generate by ProtoGen at date: 2015/11/25 15:24:21

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FETCHENEMYDOODADREQ_TYPE 56348

class RpcC2G_FetchEnemyDoodadReq : public CRpc
{
private:

	RpcC2G_FetchEnemyDoodadReq():CRpc(RPCC2G_FETCHENEMYDOODADREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FetchEnemyDoodadReq *CreateRpc()
	{
		return new RpcC2G_FetchEnemyDoodadReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnemyDoodadInfo &roArg, const RollInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnemyDoodadInfo &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnemyDoodadInfo &roArg, RollInfoRes &roRes);
	void OnDelayReplyRpc(const EnemyDoodadInfo &roArg, RollInfoRes &roRes, const CUserData &roUserData);

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
	EnemyDoodadInfo m_oArg;
	RollInfoRes m_oRes;
};

#endif
