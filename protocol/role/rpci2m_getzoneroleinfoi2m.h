#ifndef __RPCI2M_GETZONEROLEINFOI2M_H__
#define __RPCI2M_GETZONEROLEINFOI2M_H__

// generate by ProtoGen at date: 2017/10/24 15:13:26

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCI2M_GETZONEROLEINFOI2M_TYPE 53734

class RpcI2M_GetZoneRoleInfoI2M : public CRpc
{
private:

	RpcI2M_GetZoneRoleInfoI2M():CRpc(RPCI2M_GETZONEROLEINFOI2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcI2M_GetZoneRoleInfoI2M *CreateRpc()
	{
		return new RpcI2M_GetZoneRoleInfoI2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetZoneRoleInfoI2MArg &roArg, const GetZoneRoleInfoI2MRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetZoneRoleInfoI2MArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetZoneRoleInfoI2MArg &roArg, GetZoneRoleInfoI2MRes &roRes);
	void OnDelayReplyRpc(const GetZoneRoleInfoI2MArg &roArg, GetZoneRoleInfoI2MRes &roRes, const CUserData &roUserData);

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
	GetZoneRoleInfoI2MArg m_oArg;
	GetZoneRoleInfoI2MRes m_oRes;
};

#endif
