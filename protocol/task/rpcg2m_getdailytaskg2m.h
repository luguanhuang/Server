#ifndef __RPCG2M_GETDAILYTASKG2M_H__
#define __RPCG2M_GETDAILYTASKG2M_H__

// generate by ProtoGen at date: 2017/10/12 20:31:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GETDAILYTASKG2M_TYPE 20249

class RpcG2M_GetDailyTaskG2M : public CRpc
{
private:

	RpcG2M_GetDailyTaskG2M():CRpc(RPCG2M_GETDAILYTASKG2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GetDailyTaskG2M *CreateRpc()
	{
		return new RpcG2M_GetDailyTaskG2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDailyTaskG2MArg &roArg, const GetDailyTaskG2MRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDailyTaskG2MArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDailyTaskG2MArg &roArg, GetDailyTaskG2MRes &roRes);
	void OnDelayReplyRpc(const GetDailyTaskG2MArg &roArg, GetDailyTaskG2MRes &roRes, const CUserData &roUserData);

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
	GetDailyTaskG2MArg m_oArg;
	GetDailyTaskG2MRes m_oRes;
};

#endif
