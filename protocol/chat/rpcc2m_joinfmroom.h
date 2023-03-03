#ifndef __RPCC2M_JOINFMROOM_H__
#define __RPCC2M_JOINFMROOM_H__

// generate by ProtoGen at date: 2016/11/23 17:44:59

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_JOINFMROOM_TYPE 25303

class RpcC2M_JoinFmRoom : public CRpc
{
private:

	RpcC2M_JoinFmRoom():CRpc(RPCC2M_JOINFMROOM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_JoinFmRoom *CreateRpc()
	{
		return new RpcC2M_JoinFmRoom;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const JoinLargeRoomArg &roArg, const JoinLargeRoomRes &roRes, const CUserData &roUserData);
	void OnTimeout(const JoinLargeRoomArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const JoinLargeRoomArg &roArg, JoinLargeRoomRes &roRes);
	void OnDelayReplyRpc(const JoinLargeRoomArg &roArg, JoinLargeRoomRes &roRes, const CUserData &roUserData);

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
	JoinLargeRoomArg m_oArg;
	JoinLargeRoomRes m_oRes;
};

#endif
