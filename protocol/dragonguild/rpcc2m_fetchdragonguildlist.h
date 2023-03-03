#ifndef __RPCC2M_FETCHDRAGONGUILDLIST_H__
#define __RPCC2M_FETCHDRAGONGUILDLIST_H__

// generate by ProtoGen at date: 2017/9/8 16:40:23

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_FETCHDRAGONGUILDLIST_TYPE 23518

class RpcC2M_FetchDragonGuildList : public CRpc
{
private:

	RpcC2M_FetchDragonGuildList():CRpc(RPCC2M_FETCHDRAGONGUILDLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_FetchDragonGuildList *CreateRpc()
	{
		return new RpcC2M_FetchDragonGuildList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchDragonGuildListArg &roArg, const FetchDragonGuildRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchDragonGuildListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchDragonGuildListArg &roArg, FetchDragonGuildRes &roRes);
	void OnDelayReplyRpc(const FetchDragonGuildListArg &roArg, FetchDragonGuildRes &roRes, const CUserData &roUserData);

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
	FetchDragonGuildListArg m_oArg;
	FetchDragonGuildRes m_oRes;
};

#endif
