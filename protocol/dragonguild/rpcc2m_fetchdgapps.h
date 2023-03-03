#ifndef __RPCC2M_FETCHDGAPPS_H__
#define __RPCC2M_FETCHDGAPPS_H__

// generate by ProtoGen at date: 2017/9/7 15:15:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_FETCHDGAPPS_TYPE 48732

class RpcC2M_FetchDGApps : public CRpc
{
private:

	RpcC2M_FetchDGApps():CRpc(RPCC2M_FETCHDGAPPS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_FetchDGApps *CreateRpc()
	{
		return new RpcC2M_FetchDGApps;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchDGAppArg &roArg, const FetchDGAppRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchDGAppArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchDGAppArg &roArg, FetchDGAppRes &roRes);
	void OnDelayReplyRpc(const FetchDGAppArg &roArg, FetchDGAppRes &roRes, const CUserData &roUserData);

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
	FetchDGAppArg m_oArg;
	FetchDGAppRes m_oRes;
};

#endif
