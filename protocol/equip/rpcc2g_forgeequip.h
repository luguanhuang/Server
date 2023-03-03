#ifndef __RPCC2G_FORGEEQUIP_H__
#define __RPCC2G_FORGEEQUIP_H__

// generate by ProtoGen at date: 2017/1/10 10:36:59

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FORGEEQUIP_TYPE 58244

class RpcC2G_ForgeEquip : public CRpc
{
private:

	RpcC2G_ForgeEquip():CRpc(RPCC2G_FORGEEQUIP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ForgeEquip *CreateRpc()
	{
		return new RpcC2G_ForgeEquip;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ForgeEquipArg &roArg, const ForgeEquipRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ForgeEquipArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ForgeEquipArg &roArg, ForgeEquipRes &roRes);
	void OnDelayReplyRpc(const ForgeEquipArg &roArg, ForgeEquipRes &roRes, const CUserData &roUserData);

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
	ForgeEquipArg m_oArg;
	ForgeEquipRes m_oRes;
};

#endif
