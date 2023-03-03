#ifndef __RPCC2M_FRIENDGARDENPLANTLOG_H__
#define __RPCC2M_FRIENDGARDENPLANTLOG_H__

// generate by ProtoGen at date: 2016/10/9 11:03:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_FRIENDGARDENPLANTLOG_TYPE 33646

class RpcC2M_FriendGardenPlantLog : public CRpc
{
private:

	RpcC2M_FriendGardenPlantLog():CRpc(RPCC2M_FRIENDGARDENPLANTLOG_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_FriendGardenPlantLog *CreateRpc()
	{
		return new RpcC2M_FriendGardenPlantLog;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FriendGardenPlantLogArg &roArg, const FriendGardenPlantLogRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FriendGardenPlantLogArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FriendGardenPlantLogArg &roArg, FriendGardenPlantLogRes &roRes);
	void OnDelayReplyRpc(const FriendGardenPlantLogArg &roArg, FriendGardenPlantLogRes &roRes, const CUserData &roUserData);

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
	FriendGardenPlantLogArg m_oArg;
	FriendGardenPlantLogRes m_oRes;
};

#endif
