#ifndef __RPCC2G_ACTIVATEPRESHOW_H__
#define __RPCC2G_ACTIVATEPRESHOW_H__

// generate by ProtoGen at date: 2017/10/9 11:48:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ACTIVATEPRESHOW_TYPE 22466

class RpcC2G_ActivatePreShow : public CRpc
{
private:

	RpcC2G_ActivatePreShow():CRpc(RPCC2G_ACTIVATEPRESHOW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ActivatePreShow *CreateRpc()
	{
		return new RpcC2G_ActivatePreShow;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ActivatePreShowArg &roArg, const ActivatePreShowRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ActivatePreShowArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ActivatePreShowArg &roArg, ActivatePreShowRes &roRes);
	void OnDelayReplyRpc(const ActivatePreShowArg &roArg, ActivatePreShowRes &roRes, const CUserData &roUserData);

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
	ActivatePreShowArg m_oArg;
	ActivatePreShowRes m_oRes;
};

#endif
