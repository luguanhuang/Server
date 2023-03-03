#ifndef __RPCN2G_LEAVESCENE_H__
#define __RPCN2G_LEAVESCENE_H__

// generate by ProtoGen at date: 2016/11/28 12:52:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2G_LEAVESCENE_TYPE 48300

class RpcN2G_LeaveScene : public CRpc
{
private:

	RpcN2G_LeaveScene():CRpc(RPCN2G_LEAVESCENE_TYPE)
	{
        m_dwTimeout = 60000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2G_LeaveScene *CreateRpc()
	{
		return new RpcN2G_LeaveScene;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LeaveSceneArg &roArg, const LeaveSceneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LeaveSceneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LeaveSceneArg &roArg, LeaveSceneRes &roRes);
	void OnDelayReplyRpc(const LeaveSceneArg &roArg, LeaveSceneRes &roRes, const CUserData &roUserData);

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
	LeaveSceneArg m_oArg;
	LeaveSceneRes m_oRes;
};

#endif
