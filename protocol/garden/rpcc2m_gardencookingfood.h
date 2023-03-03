#ifndef __RPCC2M_GARDENCOOKINGFOOD_H__
#define __RPCC2M_GARDENCOOKINGFOOD_H__

// generate by ProtoGen at date: 2016/10/10 14:15:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GARDENCOOKINGFOOD_TYPE 31406

class RpcC2M_GardenCookingFood : public CRpc
{
private:

	RpcC2M_GardenCookingFood():CRpc(RPCC2M_GARDENCOOKINGFOOD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GardenCookingFood *CreateRpc()
	{
		return new RpcC2M_GardenCookingFood;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GardenCookingFoodArg &roArg, const GardenCookingFoodRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GardenCookingFoodArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GardenCookingFoodArg &roArg, GardenCookingFoodRes &roRes);
	void OnDelayReplyRpc(const GardenCookingFoodArg &roArg, GardenCookingFoodRes &roRes, const CUserData &roUserData);

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
	GardenCookingFoodArg m_oArg;
	GardenCookingFoodRes m_oRes;
};

#endif
