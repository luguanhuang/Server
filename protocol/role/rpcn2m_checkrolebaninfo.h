#ifndef __RPCN2M_CHECKROLEBANINFO_H__
#define __RPCN2M_CHECKROLEBANINFO_H__

// generate by ProtoGen at date: 2016/11/9 10:17:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2M_CHECKROLEBANINFO_TYPE 23292

class RpcN2M_CheckRoleBanInfo : public CRpc
{
private:

	RpcN2M_CheckRoleBanInfo():CRpc(RPCN2M_CHECKROLEBANINFO_TYPE)
	{
        m_dwTimeout = 60000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2M_CheckRoleBanInfo *CreateRpc()
	{
		return new RpcN2M_CheckRoleBanInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CheckRoleBanInfoArg &roArg, const CheckRoleBanInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CheckRoleBanInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CheckRoleBanInfoArg &roArg, CheckRoleBanInfoRes &roRes);
	void OnDelayReplyRpc(const CheckRoleBanInfoArg &roArg, CheckRoleBanInfoRes &roRes, const CUserData &roUserData);

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
	CheckRoleBanInfoArg m_oArg;
	CheckRoleBanInfoRes m_oRes;
};

#endif
