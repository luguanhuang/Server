#ifndef __RPCM2G_GMCONTROL_H__
#define __RPCM2G_GMCONTROL_H__

// generate by ProtoGen at date: 2015/8/21 15:20:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GMCONTROL_TYPE 16886

class RpcM2G_GMControl : public CRpc
{
private:

	RpcM2G_GMControl():CRpc(RPCM2G_GMCONTROL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GMControl *CreateRpc()
	{
		return new RpcM2G_GMControl;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GMCmdArg &roArg, const GMCmdRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GMCmdArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GMCmdArg &roArg, GMCmdRes &roRes);
	void OnDelayReplyRpc(const GMCmdArg &roArg, GMCmdRes &roRes, const CUserData &roUserData);

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
	GMCmdArg m_oArg;
	GMCmdRes m_oRes;
};

#endif
