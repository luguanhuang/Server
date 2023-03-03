#ifndef __RPCC2M_FETCHGUILDAPP_H__
#define __RPCC2M_FETCHGUILDAPP_H__

// generate by ProtoGen at date: 2016/9/15 21:01:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_FETCHGUILDAPP_TYPE 3668

class RpcC2M_FetchGuildApp : public CRpc
{
private:

	RpcC2M_FetchGuildApp():CRpc(RPCC2M_FETCHGUILDAPP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_FetchGuildApp *CreateRpc()
	{
		return new RpcC2M_FetchGuildApp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchGAPPArg &roArg, const FetchGAPPRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchGAPPArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchGAPPArg &roArg, FetchGAPPRes &roRes);
	void OnDelayReplyRpc(const FetchGAPPArg &roArg, FetchGAPPRes &roRes, const CUserData &roUserData);

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
	FetchGAPPArg m_oArg;
	FetchGAPPRes m_oRes;
};

#endif
