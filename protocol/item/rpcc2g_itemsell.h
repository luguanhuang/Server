#ifndef __RPCC2G_ITEMSELL_H__
#define __RPCC2G_ITEMSELL_H__

// generate by ProtoGen at date: 2017/1/13 17:13:46

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ITEMSELL_TYPE 34826

class RpcC2G_ItemSell : public CRpc
{
private:

	RpcC2G_ItemSell():CRpc(RPCC2G_ITEMSELL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ItemSell *CreateRpc()
	{
		return new RpcC2G_ItemSell;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ItemSellArg &roArg, const ItemSellRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ItemSellArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ItemSellArg &roArg, ItemSellRes &roRes);
	void OnDelayReplyRpc(const ItemSellArg &roArg, ItemSellRes &roRes, const CUserData &roUserData);

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
	ItemSellArg m_oArg;
	ItemSellRes m_oRes;
};

#endif
