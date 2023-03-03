﻿#ifndef __RPCM2G_GARDENPLANTCHANGESTAGE_H__
#define __RPCM2G_GARDENPLANTCHANGESTAGE_H__

// generate by ProtoGen at date: 2016/10/18 11:34:36

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GARDENPLANTCHANGESTAGE_TYPE 55281

class RpcM2G_GardenPlantChangeStage : public CRpc
{
private:

	RpcM2G_GardenPlantChangeStage():CRpc(RPCM2G_GARDENPLANTCHANGESTAGE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GardenPlantChangeStage *CreateRpc()
	{
		return new RpcM2G_GardenPlantChangeStage;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GardenPlantChangeStageArg &roArg, const GardenPlantChangeStageRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GardenPlantChangeStageArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GardenPlantChangeStageArg &roArg, GardenPlantChangeStageRes &roRes);
	void OnDelayReplyRpc(const GardenPlantChangeStageArg &roArg, GardenPlantChangeStageRes &roRes, const CUserData &roUserData);

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
	GardenPlantChangeStageArg m_oArg;
	GardenPlantChangeStageRes m_oRes;
};

#endif
