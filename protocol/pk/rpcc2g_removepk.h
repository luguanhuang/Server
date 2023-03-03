#ifndef __RPCC2G_REMOVEPK_H__
#define __RPCC2G_REMOVEPK_H__

// generate by ProtoGen at date: 2015/12/16 12:20:59

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REMOVEPK_TYPE 38224

class RpcC2G_RemovePk : public CRpc
{
private:

	RpcC2G_RemovePk():CRpc(RPCC2G_REMOVEPK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_RemovePk *CreateRpc()
	{
		return new RpcC2G_RemovePk;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RemovePkReq &roArg, const RemovePkRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RemovePkReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RemovePkReq &roArg, RemovePkRes &roRes);
	void OnDelayReplyRpc(const RemovePkReq &roArg, RemovePkRes &roRes, const CUserData &roUserData);

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
	RemovePkReq m_oArg;
	RemovePkRes m_oRes;
};

#endif
