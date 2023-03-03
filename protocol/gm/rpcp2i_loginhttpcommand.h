#ifndef __RPCP2I_LOGINHTTPCOMMAND_H__
#define __RPCP2I_LOGINHTTPCOMMAND_H__

// generate by ProtoGen at date: 2016/7/4 14:32:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCP2I_LOGINHTTPCOMMAND_TYPE 32926

class RpcP2I_LoginHttpCommand : public CRpc
{
private:

	RpcP2I_LoginHttpCommand():CRpc(RPCP2I_LOGINHTTPCOMMAND_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcP2I_LoginHttpCommand *CreateRpc()
	{
		return new RpcP2I_LoginHttpCommand;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GMHttpArg &roArg, const GMHttpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GMHttpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GMHttpArg &roArg, GMHttpRes &roRes);
	void OnDelayReplyRpc(const GMHttpArg &roArg, GMHttpRes &roRes, const CUserData &roUserData);

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
	GMHttpArg m_oArg;
	GMHttpRes m_oRes;
};

#endif
