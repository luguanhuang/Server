#ifndef __RPCC2M_FETCHTEAMLISTC2M_H__
#define __RPCC2M_FETCHTEAMLISTC2M_H__

// generate by ProtoGen at date: 2017/3/31 17:13:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_FETCHTEAMLISTC2M_TYPE 3930

class RpcC2M_FetchTeamListC2M : public CRpc
{
private:

	RpcC2M_FetchTeamListC2M():CRpc(RPCC2M_FETCHTEAMLISTC2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_FetchTeamListC2M *CreateRpc()
	{
		return new RpcC2M_FetchTeamListC2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchTeamListArg &roArg, const FetchTeamListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchTeamListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchTeamListArg &roArg, FetchTeamListRes &roRes);
	void OnDelayReplyRpc(const FetchTeamListArg &roArg, FetchTeamListRes &roRes, const CUserData &roUserData);

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
	FetchTeamListArg m_oArg;
	FetchTeamListRes m_oRes;
};

#endif
