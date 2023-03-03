#ifndef __RPCM2G_TRYADDGUILDINHERIT_H__
#define __RPCM2G_TRYADDGUILDINHERIT_H__

// generate by ProtoGen at date: 2016/11/17 16:07:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_TRYADDGUILDINHERIT_TYPE 35002

class RpcM2G_tryAddGuildInherit : public CRpc
{
private:

	RpcM2G_tryAddGuildInherit():CRpc(RPCM2G_TRYADDGUILDINHERIT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_tryAddGuildInherit *CreateRpc()
	{
		return new RpcM2G_tryAddGuildInherit;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const tryAddGuildInheritArg &roArg, const tryAddGuildInheritRes &roRes, const CUserData &roUserData);
	void OnTimeout(const tryAddGuildInheritArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const tryAddGuildInheritArg &roArg, tryAddGuildInheritRes &roRes);
	void OnDelayReplyRpc(const tryAddGuildInheritArg &roArg, tryAddGuildInheritRes &roRes, const CUserData &roUserData);

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
	tryAddGuildInheritArg m_oArg;
	tryAddGuildInheritRes m_oRes;
};

#endif
