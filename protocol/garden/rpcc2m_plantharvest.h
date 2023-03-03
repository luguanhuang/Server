#ifndef __RPCC2M_PLANTHARVEST_H__
#define __RPCC2M_PLANTHARVEST_H__

// generate by ProtoGen at date: 2016/10/5 10:34:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_PLANTHARVEST_TYPE 39568

class RpcC2M_PlantHarvest : public CRpc
{
private:

	RpcC2M_PlantHarvest():CRpc(RPCC2M_PLANTHARVEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_PlantHarvest *CreateRpc()
	{
		return new RpcC2M_PlantHarvest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PlantHarvestArg &roArg, const PlantHarvestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PlantHarvestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PlantHarvestArg &roArg, PlantHarvestRes &roRes);
	void OnDelayReplyRpc(const PlantHarvestArg &roArg, PlantHarvestRes &roRes, const CUserData &roUserData);

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
	PlantHarvestArg m_oArg;
	PlantHarvestRes m_oRes;
};

#endif
