#ifndef __RPCG2M_OPGUILDREQDATA_H__
#define __RPCG2M_OPGUILDREQDATA_H__

// generate by ProtoGen at date: 2016/9/9 9:56:30

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_OPGUILDREQDATA_TYPE 53332

class RpcG2M_OpGuildReqData : public CRpc
{
private:

	RpcG2M_OpGuildReqData():CRpc(RPCG2M_OPGUILDREQDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_OpGuildReqData *CreateRpc()
	{
		return new RpcG2M_OpGuildReqData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const OpGuildReqDataArg &roArg, const OpGuildReqDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const OpGuildReqDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const OpGuildReqDataArg &roArg, OpGuildReqDataRes &roRes);
	void OnDelayReplyRpc(const OpGuildReqDataArg &roArg, OpGuildReqDataRes &roRes, const CUserData &roUserData);

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
	OpGuildReqDataArg m_oArg;
	OpGuildReqDataRes m_oRes;
};

#endif
