#ifndef __RPCC2G_CHANGELIVEVISIBLE_H__
#define __RPCC2G_CHANGELIVEVISIBLE_H__

// generate by ProtoGen at date: 2017/5/10 14:35:46

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHANGELIVEVISIBLE_TYPE 56831

class RpcC2G_ChangeLiveVisible : public CRpc
{
private:

	RpcC2G_ChangeLiveVisible():CRpc(RPCC2G_CHANGELIVEVISIBLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ChangeLiveVisible *CreateRpc()
	{
		return new RpcC2G_ChangeLiveVisible;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeLiveVisibleArg &roArg, const ChangeLiveVisibleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeLiveVisibleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeLiveVisibleArg &roArg, ChangeLiveVisibleRes &roRes);
	void OnDelayReplyRpc(const ChangeLiveVisibleArg &roArg, ChangeLiveVisibleRes &roRes, const CUserData &roUserData);

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
	ChangeLiveVisibleArg m_oArg;
	ChangeLiveVisibleRes m_oRes;
};

#endif
