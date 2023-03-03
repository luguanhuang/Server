#ifndef __RPCC2G_DECOMPOSEEQUIPMENT_H__
#define __RPCC2G_DECOMPOSEEQUIPMENT_H__

// generate by ProtoGen at date: 2015/6/4 15:41:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_DECOMPOSEEQUIPMENT_TYPE 6556

class RpcC2G_DecomposeEquipment : public CRpc
{
private:

	RpcC2G_DecomposeEquipment():CRpc(RPCC2G_DECOMPOSEEQUIPMENT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_DecomposeEquipment *CreateRpc()
	{
		return new RpcC2G_DecomposeEquipment;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DecomposeEquipmentArg &roArg, const DecomposeEquipmentRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DecomposeEquipmentArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DecomposeEquipmentArg &roArg, DecomposeEquipmentRes &roRes);
	void OnDelayReplyRpc(const DecomposeEquipmentArg &roArg, DecomposeEquipmentRes &roRes, const CUserData &roUserData);

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
	DecomposeEquipmentArg m_oArg;
	DecomposeEquipmentRes m_oRes;
};

#endif
