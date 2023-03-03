#ifndef __RPCC2M_OPENGARDENFARMLAND_H__
#define __RPCC2M_OPENGARDENFARMLAND_H__

// generate by ProtoGen at date: 2016/10/26 21:31:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_OPENGARDENFARMLAND_TYPE 42589

class RpcC2M_OPenGardenFarmland : public CRpc
{
private:

	RpcC2M_OPenGardenFarmland():CRpc(RPCC2M_OPENGARDENFARMLAND_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_OPenGardenFarmland *CreateRpc()
	{
		return new RpcC2M_OPenGardenFarmland;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const OpenGardenFarmlandArg &roArg, const OpenGardenFarmlandRes &roRes, const CUserData &roUserData);
	void OnTimeout(const OpenGardenFarmlandArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const OpenGardenFarmlandArg &roArg, OpenGardenFarmlandRes &roRes);
	void OnDelayReplyRpc(const OpenGardenFarmlandArg &roArg, OpenGardenFarmlandRes &roRes, const CUserData &roUserData);

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
	OpenGardenFarmlandArg m_oArg;
	OpenGardenFarmlandRes m_oRes;
};

#endif
