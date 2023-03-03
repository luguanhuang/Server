#ifndef __RPCC2M_GARDENOVERVIEW_H__
#define __RPCC2M_GARDENOVERVIEW_H__

// generate by ProtoGen at date: 2016/10/15 18:50:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GARDENOVERVIEW_TYPE 20766

class RpcC2M_GardenOverview : public CRpc
{
private:

	RpcC2M_GardenOverview():CRpc(RPCC2M_GARDENOVERVIEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GardenOverview *CreateRpc()
	{
		return new RpcC2M_GardenOverview;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GardenOverviewArg &roArg, const GardenOverviewRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GardenOverviewArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GardenOverviewArg &roArg, GardenOverviewRes &roRes);
	void OnDelayReplyRpc(const GardenOverviewArg &roArg, GardenOverviewRes &roRes, const CUserData &roUserData);

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
	GardenOverviewArg m_oArg;
	GardenOverviewRes m_oRes;
};

#endif
