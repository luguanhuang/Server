#ifndef __RPCG2M_PVPALLG2MREQ_H__
#define __RPCG2M_PVPALLG2MREQ_H__

// generate by ProtoGen at date: 2016/8/19 19:50:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_PVPALLG2MREQ_TYPE 25752

class RpcG2M_PvpAllG2MReq : public CRpc
{
private:

	RpcG2M_PvpAllG2MReq():CRpc(RPCG2M_PVPALLG2MREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_PvpAllG2MReq *CreateRpc()
	{
		return new RpcG2M_PvpAllG2MReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PvpG2MArg &roArg, const PvpG2MRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PvpG2MArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PvpG2MArg &roArg, PvpG2MRes &roRes);
	void OnDelayReplyRpc(const PvpG2MArg &roArg, PvpG2MRes &roRes, const CUserData &roUserData);

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
	PvpG2MArg m_oArg;
	PvpG2MRes m_oRes;
};

#endif
