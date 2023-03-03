#ifndef __RPCC2G_SYNCTIME_H__
#define __RPCC2G_SYNCTIME_H__

// generate by ProtoGen at date: 2016/11/1 15:21:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SYNCTIME_TYPE 30514

class RpcC2G_SyncTime : public CRpc
{
private:

	RpcC2G_SyncTime():CRpc(RPCC2G_SYNCTIME_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SyncTime *CreateRpc()
	{
		return new RpcC2G_SyncTime;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SyncTimeArg &roArg, const SyncTimeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SyncTimeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SyncTimeArg &roArg, SyncTimeRes &roRes);
	void OnDelayReplyRpc(const SyncTimeArg &roArg, SyncTimeRes &roRes, const CUserData &roUserData);

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
	SyncTimeArg m_oArg;
	SyncTimeRes m_oRes;
};

#endif
