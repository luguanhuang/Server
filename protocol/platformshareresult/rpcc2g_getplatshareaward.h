#ifndef __RPCC2G_GETPLATSHAREAWARD_H__
#define __RPCC2G_GETPLATSHAREAWARD_H__

// generate by ProtoGen at date: 2017/5/31 21:54:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETPLATSHAREAWARD_TYPE 26922

class RpcC2G_GetPlatShareAward : public CRpc
{
private:

	RpcC2G_GetPlatShareAward():CRpc(RPCC2G_GETPLATSHAREAWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetPlatShareAward *CreateRpc()
	{
		return new RpcC2G_GetPlatShareAward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPlatShareAwardArg &roArg, const GetPlatShareAwardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPlatShareAwardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPlatShareAwardArg &roArg, GetPlatShareAwardRes &roRes);
	void OnDelayReplyRpc(const GetPlatShareAwardArg &roArg, GetPlatShareAwardRes &roRes, const CUserData &roUserData);

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
	GetPlatShareAwardArg m_oArg;
	GetPlatShareAwardRes m_oRes;
};

#endif
