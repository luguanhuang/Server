#ifndef __RPCC2G_UPGRADEEQUIP_H__
#define __RPCC2G_UPGRADEEQUIP_H__

// generate by ProtoGen at date: 2017/8/29 16:48:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_UPGRADEEQUIP_TYPE 32424

class RpcC2G_UpgradeEquip : public CRpc
{
private:

	RpcC2G_UpgradeEquip():CRpc(RPCC2G_UPGRADEEQUIP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_UpgradeEquip *CreateRpc()
	{
		return new RpcC2G_UpgradeEquip;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpgradeEquipArg &roArg, const UpgradeEquipRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpgradeEquipArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpgradeEquipArg &roArg, UpgradeEquipRes &roRes);
	void OnDelayReplyRpc(const UpgradeEquipArg &roArg, UpgradeEquipRes &roRes, const CUserData &roUserData);

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
	UpgradeEquipArg m_oArg;
	UpgradeEquipRes m_oRes;
};

#endif
