#ifndef __RPCC2G_PEERBOX_H__
#define __RPCC2G_PEERBOX_H__

// generate by ProtoGen at date: 2017/9/29 11:25:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PEERBOX_TYPE 21959

class RpcC2G_PeerBox : public CRpc
{
private:

	RpcC2G_PeerBox():CRpc(RPCC2G_PEERBOX_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PeerBox *CreateRpc()
	{
		return new RpcC2G_PeerBox;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PeerBoxArg &roArg, const PeerBoxRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PeerBoxArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PeerBoxArg &roArg, PeerBoxRes &roRes);
	void OnDelayReplyRpc(const PeerBoxArg &roArg, PeerBoxRes &roRes, const CUserData &roUserData);

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
	PeerBoxArg m_oArg;
	PeerBoxRes m_oRes;
};

#endif
