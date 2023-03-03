#ifndef __RPCG2M_NOTIFYCHECKINBOX_H__
#define __RPCG2M_NOTIFYCHECKINBOX_H__

// generate by ProtoGen at date: 2016/9/18 13:55:59

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_NOTIFYCHECKINBOX_TYPE 23321

class RpcG2M_NotifyCheckinBox : public CRpc
{
private:

	RpcG2M_NotifyCheckinBox():CRpc(RPCG2M_NOTIFYCHECKINBOX_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_NotifyCheckinBox *CreateRpc()
	{
		return new RpcG2M_NotifyCheckinBox;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const NotifyCheckinBoxArg &roArg, const NotifyCheckinBoxRes &roRes, const CUserData &roUserData);
	void OnTimeout(const NotifyCheckinBoxArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const NotifyCheckinBoxArg &roArg, NotifyCheckinBoxRes &roRes);
	void OnDelayReplyRpc(const NotifyCheckinBoxArg &roArg, NotifyCheckinBoxRes &roRes, const CUserData &roUserData);

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
	NotifyCheckinBoxArg m_oArg;
	NotifyCheckinBoxRes m_oRes;
};

#endif
