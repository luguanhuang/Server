#ifndef __RPCM2G_REQGUILDINHERITTIMES_H__
#define __RPCM2G_REQGUILDINHERITTIMES_H__

// generate by ProtoGen at date: 2016/12/5 16:59:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_REQGUILDINHERITTIMES_TYPE 27870

class RpcM2G_ReqGuildInheritTimes : public CRpc
{
private:

	RpcM2G_ReqGuildInheritTimes():CRpc(RPCM2G_REQGUILDINHERITTIMES_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_ReqGuildInheritTimes *CreateRpc()
	{
		return new RpcM2G_ReqGuildInheritTimes;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildInheritTimesArg &roArg, const ReqGuildInheritTimesRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildInheritTimesArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildInheritTimesArg &roArg, ReqGuildInheritTimesRes &roRes);
	void OnDelayReplyRpc(const ReqGuildInheritTimesArg &roArg, ReqGuildInheritTimesRes &roRes, const CUserData &roUserData);

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
	ReqGuildInheritTimesArg m_oArg;
	ReqGuildInheritTimesRes m_oRes;
};

#endif
