#ifndef __RPCG2M_ASKGUILDHELP_H__
#define __RPCG2M_ASKGUILDHELP_H__

// generate by ProtoGen at date: 2016/11/16 11:19:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_ASKGUILDHELP_TYPE 57320

class RpcG2M_AskGuildHelp : public CRpc
{
private:

	RpcG2M_AskGuildHelp():CRpc(RPCG2M_ASKGUILDHELP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_AskGuildHelp *CreateRpc()
	{
		return new RpcG2M_AskGuildHelp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AskGuildHelpReq &roArg, const AskGuildHelpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AskGuildHelpReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AskGuildHelpReq &roArg, AskGuildHelpRes &roRes);
	void OnDelayReplyRpc(const AskGuildHelpReq &roArg, AskGuildHelpRes &roRes, const CUserData &roUserData);

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
	AskGuildHelpReq m_oArg;
	AskGuildHelpRes m_oRes;
};

#endif
