#ifndef __RPCG2M_SYNCACTIVITYEND2MS_H__
#define __RPCG2M_SYNCACTIVITYEND2MS_H__

// generate by ProtoGen at date: 2017/2/23 11:36:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_SYNCACTIVITYEND2MS_TYPE 28521

class RpcG2M_SyncActivityEnd2MS : public CRpc
{
private:

	RpcG2M_SyncActivityEnd2MS():CRpc(RPCG2M_SYNCACTIVITYEND2MS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_SyncActivityEnd2MS *CreateRpc()
	{
		return new RpcG2M_SyncActivityEnd2MS;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SyncActivityEnd2MSArg &roArg, const SyncActivityEnd2MSRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SyncActivityEnd2MSArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SyncActivityEnd2MSArg &roArg, SyncActivityEnd2MSRes &roRes);
	void OnDelayReplyRpc(const SyncActivityEnd2MSArg &roArg, SyncActivityEnd2MSRes &roRes, const CUserData &roUserData);

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
	SyncActivityEnd2MSArg m_oArg;
	SyncActivityEnd2MSRes m_oRes;
};

#endif
