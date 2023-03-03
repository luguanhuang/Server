#ifndef __RPCC2M_USEGUILDBUFF_H__
#define __RPCC2M_USEGUILDBUFF_H__

// generate by ProtoGen at date: 2016/11/2 20:24:36

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_USEGUILDBUFF_TYPE 15817

class RpcC2M_UseGuildBuff : public CRpc
{
private:

	RpcC2M_UseGuildBuff():CRpc(RPCC2M_USEGUILDBUFF_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_UseGuildBuff *CreateRpc()
	{
		return new RpcC2M_UseGuildBuff;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UseGuildBuffArg &roArg, const UseGuildBuffRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UseGuildBuffArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UseGuildBuffArg &roArg, UseGuildBuffRes &roRes);
	void OnDelayReplyRpc(const UseGuildBuffArg &roArg, UseGuildBuffRes &roRes, const CUserData &roUserData);

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
	UseGuildBuffArg m_oArg;
	UseGuildBuffRes m_oRes;
};

#endif
