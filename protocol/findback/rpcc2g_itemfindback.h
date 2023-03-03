#ifndef __RPCC2G_ITEMFINDBACK_H__
#define __RPCC2G_ITEMFINDBACK_H__

// generate by ProtoGen at date: 2016/8/30 16:45:47

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ITEMFINDBACK_TYPE 60242

class RpcC2G_ItemFindBack : public CRpc
{
private:

	RpcC2G_ItemFindBack():CRpc(RPCC2G_ITEMFINDBACK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ItemFindBack *CreateRpc()
	{
		return new RpcC2G_ItemFindBack;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ItemFindBackArg &roArg, const ItemFindBackRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ItemFindBackArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ItemFindBackArg &roArg, ItemFindBackRes &roRes);
	void OnDelayReplyRpc(const ItemFindBackArg &roArg, ItemFindBackRes &roRes, const CUserData &roUserData);

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
	ItemFindBackArg m_oArg;
	ItemFindBackRes m_oRes;
};

#endif
