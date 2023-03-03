#ifndef __RPCG2M_CHECKDRAGONGUILDREDPOINTG2M_H__
#define __RPCG2M_CHECKDRAGONGUILDREDPOINTG2M_H__

// generate by ProtoGen at date: 2017/9/14 17:41:46

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_CHECKDRAGONGUILDREDPOINTG2M_TYPE 13810

class RpcG2M_CheckDragonGuildRedPointG2M : public CRpc
{
private:

	RpcG2M_CheckDragonGuildRedPointG2M():CRpc(RPCG2M_CHECKDRAGONGUILDREDPOINTG2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_CheckDragonGuildRedPointG2M *CreateRpc()
	{
		return new RpcG2M_CheckDragonGuildRedPointG2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CheckDragonGuildRedPointG2MArg &roArg, const CheckDragonGuildRedPointG2MRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CheckDragonGuildRedPointG2MArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CheckDragonGuildRedPointG2MArg &roArg, CheckDragonGuildRedPointG2MRes &roRes);
	void OnDelayReplyRpc(const CheckDragonGuildRedPointG2MArg &roArg, CheckDragonGuildRedPointG2MRes &roRes, const CUserData &roUserData);

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
	CheckDragonGuildRedPointG2MArg m_oArg;
	CheckDragonGuildRedPointG2MRes m_oRes;
};

#endif
