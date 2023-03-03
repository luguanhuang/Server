#ifndef __RPCM2S_INVFIGHTCROSS2CSREQ_H__
#define __RPCM2S_INVFIGHTCROSS2CSREQ_H__

// generate by ProtoGen at date: 2017/4/13 16:00:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2S_INVFIGHTCROSS2CSREQ_TYPE 60688

class RpcM2S_InvFightCross2CSReq : public CRpc
{
private:

	RpcM2S_InvFightCross2CSReq():CRpc(RPCM2S_INVFIGHTCROSS2CSREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2S_InvFightCross2CSReq *CreateRpc()
	{
		return new RpcM2S_InvFightCross2CSReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const InvFightCross2CSReqArg &roArg, const InvFightCross2CSReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const InvFightCross2CSReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const InvFightCross2CSReqArg &roArg, InvFightCross2CSReqRes &roRes);
	void OnDelayReplyRpc(const InvFightCross2CSReqArg &roArg, InvFightCross2CSReqRes &roRes, const CUserData &roUserData);

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
	InvFightCross2CSReqArg m_oArg;
	InvFightCross2CSReqRes m_oRes;
};

#endif
