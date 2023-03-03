#ifndef __RPCC2M_STARTPLANT_H__
#define __RPCC2M_STARTPLANT_H__

// generate by ProtoGen at date: 2016/10/5 10:30:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_STARTPLANT_TYPE 2834

class RpcC2M_StartPlant : public CRpc
{
private:

	RpcC2M_StartPlant():CRpc(RPCC2M_STARTPLANT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_StartPlant *CreateRpc()
	{
		return new RpcC2M_StartPlant;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const StartPlantArg &roArg, const StartPlantRes &roRes, const CUserData &roUserData);
	void OnTimeout(const StartPlantArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const StartPlantArg &roArg, StartPlantRes &roRes);
	void OnDelayReplyRpc(const StartPlantArg &roArg, StartPlantRes &roRes, const CUserData &roUserData);

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
	StartPlantArg m_oArg;
	StartPlantRes m_oRes;
};

#endif
