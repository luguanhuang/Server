#ifndef __RPCG2M_PKRANKREQ_H__
#define __RPCG2M_PKRANKREQ_H__

// generate by ProtoGen at date: 2016/8/25 15:22:15

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_PKRANKREQ_TYPE 56003

class RpcG2M_PkRankReq : public CRpc
{
private:

	RpcG2M_PkRankReq():CRpc(RPCG2M_PKRANKREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_PkRankReq *CreateRpc()
	{
		return new RpcG2M_PkRankReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QMHRPkArg &roArg, const QMHRPkRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QMHRPkArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QMHRPkArg &roArg, QMHRPkRes &roRes);
	void OnDelayReplyRpc(const QMHRPkArg &roArg, QMHRPkRes &roRes, const CUserData &roUserData);

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
	QMHRPkArg m_oArg;
	QMHRPkRes m_oRes;
};

#endif
