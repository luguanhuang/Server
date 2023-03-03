#ifndef __RPCC2M_GETDRAGONGUILDSHOPRECORD_H__
#define __RPCC2M_GETDRAGONGUILDSHOPRECORD_H__

// generate by ProtoGen at date: 2017/9/12 20:02:15

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETDRAGONGUILDSHOPRECORD_TYPE 3114

class RpcC2M_GetDragonGuildShopRecord : public CRpc
{
private:

	RpcC2M_GetDragonGuildShopRecord():CRpc(RPCC2M_GETDRAGONGUILDSHOPRECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetDragonGuildShopRecord *CreateRpc()
	{
		return new RpcC2M_GetDragonGuildShopRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDragonGuildShopRecordArg &roArg, const GetDragonGuildShopRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDragonGuildShopRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDragonGuildShopRecordArg &roArg, GetDragonGuildShopRecordRes &roRes);
	void OnDelayReplyRpc(const GetDragonGuildShopRecordArg &roArg, GetDragonGuildShopRecordRes &roRes, const CUserData &roUserData);

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
	GetDragonGuildShopRecordArg m_oArg;
	GetDragonGuildShopRecordRes m_oRes;
};

#endif
