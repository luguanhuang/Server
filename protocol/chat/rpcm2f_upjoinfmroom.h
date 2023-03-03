#ifndef __RPCM2F_UPJOINFMROOM_H__
#define __RPCM2F_UPJOINFMROOM_H__

// generate by ProtoGen at date: 2016/11/23 17:51:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2F_UPJOINFMROOM_TYPE 21052

class RpcM2F_UPJoinFmRoom : public CRpc
{
private:

	RpcM2F_UPJoinFmRoom():CRpc(RPCM2F_UPJOINFMROOM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2F_UPJoinFmRoom *CreateRpc()
	{
		return new RpcM2F_UPJoinFmRoom;
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
