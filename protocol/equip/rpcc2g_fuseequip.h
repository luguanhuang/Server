#ifndef __RPCC2G_FUSEEQUIP_H__
#define __RPCC2G_FUSEEQUIP_H__

// generate by ProtoGen at date: 2017/9/7 16:59:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FUSEEQUIP_TYPE 56006

class RpcC2G_FuseEquip : public CRpc
{
private:

	RpcC2G_FuseEquip():CRpc(RPCC2G_FUSEEQUIP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FuseEquip *CreateRpc()
	{
		return new RpcC2G_FuseEquip;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FuseEquipArg &roArg, const FuseEquipRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FuseEquipArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FuseEquipArg &roArg, FuseEquipRes &roRes);
	void OnDelayReplyRpc(const FuseEquipArg &roArg, FuseEquipRes &roRes, const CUserData &roUserData);

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
	FuseEquipArg m_oArg;
	FuseEquipRes m_oRes;
};

#endif
