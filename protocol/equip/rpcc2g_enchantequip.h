#ifndef __RPCC2G_ENCHANTEQUIP_H__
#define __RPCC2G_ENCHANTEQUIP_H__

// generate by ProtoGen at date: 2017/1/10 10:51:25

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ENCHANTEQUIP_TYPE 55166

class RpcC2G_EnchantEquip : public CRpc
{
private:

	RpcC2G_EnchantEquip():CRpc(RPCC2G_ENCHANTEQUIP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_EnchantEquip *CreateRpc()
	{
		return new RpcC2G_EnchantEquip;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnchantEquipArg &roArg, const EnchantEquipRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnchantEquipArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnchantEquipArg &roArg, EnchantEquipRes &roRes);
	void OnDelayReplyRpc(const EnchantEquipArg &roArg, EnchantEquipRes &roRes, const CUserData &roUserData);

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
	EnchantEquipArg m_oArg;
	EnchantEquipRes m_oRes;
};

#endif
