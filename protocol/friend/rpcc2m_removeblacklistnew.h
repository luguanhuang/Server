#ifndef __RPCC2M_REMOVEBLACKLISTNEW_H__
#define __RPCC2M_REMOVEBLACKLISTNEW_H__

// generate by ProtoGen at date: 2016/7/28 23:49:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REMOVEBLACKLISTNEW_TYPE 38702

class RpcC2M_RemoveBlackListNew : public CRpc
{
private:

	RpcC2M_RemoveBlackListNew():CRpc(RPCC2M_REMOVEBLACKLISTNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_RemoveBlackListNew *CreateRpc()
	{
		return new RpcC2M_RemoveBlackListNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RemoveBlackListArg &roArg, const RemoveBlackListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RemoveBlackListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RemoveBlackListArg &roArg, RemoveBlackListRes &roRes);
	void OnDelayReplyRpc(const RemoveBlackListArg &roArg, RemoveBlackListRes &roRes, const CUserData &roUserData);

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
	RemoveBlackListArg m_oArg;
	RemoveBlackListRes m_oRes;
};

#endif
