#ifndef __RPCC2M_CLICKNEWNOTICE_H__
#define __RPCC2M_CLICKNEWNOTICE_H__

// generate by ProtoGen at date: 2017/2/1 15:03:32

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CLICKNEWNOTICE_TYPE 50366

class RpcC2M_ClickNewNotice : public CRpc
{
private:

	RpcC2M_ClickNewNotice():CRpc(RPCC2M_CLICKNEWNOTICE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ClickNewNotice *CreateRpc()
	{
		return new RpcC2M_ClickNewNotice;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ClickNewNoticeArg &roArg, const ClickNewNoticeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ClickNewNoticeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ClickNewNoticeArg &roArg, ClickNewNoticeRes &roRes);
	void OnDelayReplyRpc(const ClickNewNoticeArg &roArg, ClickNewNoticeRes &roRes, const CUserData &roUserData);

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
	ClickNewNoticeArg m_oArg;
	ClickNewNoticeRes m_oRes;
};

#endif
