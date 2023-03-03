#ifndef __RPCC2G_PVPALLREQ_H__
#define __RPCC2G_PVPALLREQ_H__

// generate by ProtoGen at date: 2016/6/16 14:25:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PVPALLREQ_TYPE 57262

class RpcC2G_PvpAllReq : public CRpc
{
private:

	RpcC2G_PvpAllReq():CRpc(RPCC2G_PVPALLREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PvpAllReq *CreateRpc()
	{
		return new RpcC2G_PvpAllReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PvpArg &roArg, const PvpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PvpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PvpArg &roArg, PvpRes &roRes);
	void OnDelayReplyRpc(const PvpArg &roArg, PvpRes &roRes, const CUserData &roUserData);

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
	PvpArg m_oArg;
	PvpRes m_oRes;
};

#endif
