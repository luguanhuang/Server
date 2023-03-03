#ifndef __RPCC2G_SELECTCHARGEBACKROLE_H__
#define __RPCC2G_SELECTCHARGEBACKROLE_H__

// generate by ProtoGen at date: 2017/10/26 12:00:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SELECTCHARGEBACKROLE_TYPE 38792

class RpcC2G_SelectChargeBackRole : public CRpc
{
private:

	RpcC2G_SelectChargeBackRole():CRpc(RPCC2G_SELECTCHARGEBACKROLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SelectChargeBackRole *CreateRpc()
	{
		return new RpcC2G_SelectChargeBackRole;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SelectChargeBackRoleArg &roArg, const SelectChargeBackRoleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SelectChargeBackRoleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SelectChargeBackRoleArg &roArg, SelectChargeBackRoleRes &roRes);
	void OnDelayReplyRpc(const SelectChargeBackRoleArg &roArg, SelectChargeBackRoleRes &roRes, const CUserData &roUserData);

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
	SelectChargeBackRoleArg m_oArg;
	SelectChargeBackRoleRes m_oRes;
};

#endif
