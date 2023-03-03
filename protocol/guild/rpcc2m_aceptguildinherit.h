#ifndef __RPCC2M_ACEPTGUILDINHERIT_H__
#define __RPCC2M_ACEPTGUILDINHERIT_H__

// generate by ProtoGen at date: 2016/11/17 17:06:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ACEPTGUILDINHERIT_TYPE 35235

class RpcC2M_AceptGuildInherit : public CRpc
{
private:

	RpcC2M_AceptGuildInherit():CRpc(RPCC2M_ACEPTGUILDINHERIT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AceptGuildInherit *CreateRpc()
	{
		return new RpcC2M_AceptGuildInherit;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AceptGuildInheritArg &roArg, const AceptGuildInheritRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AceptGuildInheritArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AceptGuildInheritArg &roArg, AceptGuildInheritRes &roRes);
	void OnDelayReplyRpc(const AceptGuildInheritArg &roArg, AceptGuildInheritRes &roRes, const CUserData &roUserData);

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
	AceptGuildInheritArg m_oArg;
	AceptGuildInheritRes m_oRes;
};

#endif
