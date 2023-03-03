#ifndef __RPCM2G_GARDENPLAYEVENT_H__
#define __RPCM2G_GARDENPLAYEVENT_H__

// generate by ProtoGen at date: 2016/10/14 15:11:36

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GARDENPLAYEVENT_TYPE 18525

class RpcM2G_GardenPlayEvent : public CRpc
{
private:

	RpcM2G_GardenPlayEvent():CRpc(RPCM2G_GARDENPLAYEVENT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GardenPlayEvent *CreateRpc()
	{
		return new RpcM2G_GardenPlayEvent;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GardenPlayEventArg &roArg, const GardenPlayEventRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GardenPlayEventArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GardenPlayEventArg &roArg, GardenPlayEventRes &roRes);
	void OnDelayReplyRpc(const GardenPlayEventArg &roArg, GardenPlayEventRes &roRes, const CUserData &roUserData);

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
	GardenPlayEventArg m_oArg;
	GardenPlayEventRes m_oRes;
};

#endif
