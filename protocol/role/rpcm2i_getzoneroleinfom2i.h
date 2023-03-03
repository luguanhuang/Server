#ifndef __RPCM2I_GETZONEROLEINFOM2I_H__
#define __RPCM2I_GETZONEROLEINFOM2I_H__

// generate by ProtoGen at date: 2017/10/24 15:01:59

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2I_GETZONEROLEINFOM2I_TYPE 47976

class RpcM2I_GetZoneRoleInfoM2I : public CRpc
{
private:

	RpcM2I_GetZoneRoleInfoM2I():CRpc(RPCM2I_GETZONEROLEINFOM2I_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2I_GetZoneRoleInfoM2I *CreateRpc()
	{
		return new RpcM2I_GetZoneRoleInfoM2I;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetZoneRoleInfoM2IArg &roArg, const GetZoneRoleInfoM2IRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetZoneRoleInfoM2IArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetZoneRoleInfoM2IArg &roArg, GetZoneRoleInfoM2IRes &roRes);
	void OnDelayReplyRpc(const GetZoneRoleInfoM2IArg &roArg, GetZoneRoleInfoM2IRes &roRes, const CUserData &roUserData);

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
	GetZoneRoleInfoM2IArg m_oArg;
	GetZoneRoleInfoM2IRes m_oRes;
};

#endif
