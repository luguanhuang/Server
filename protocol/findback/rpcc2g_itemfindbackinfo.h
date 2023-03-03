#ifndef __RPCC2G_ITEMFINDBACKINFO_H__
#define __RPCC2G_ITEMFINDBACKINFO_H__

// generate by ProtoGen at date: 2016/8/30 16:18:36

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ITEMFINDBACKINFO_TYPE 11755

class RpcC2G_ItemFindBackInfo : public CRpc
{
private:

	RpcC2G_ItemFindBackInfo():CRpc(RPCC2G_ITEMFINDBACKINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ItemFindBackInfo *CreateRpc()
	{
		return new RpcC2G_ItemFindBackInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ItemFindBackInfoArg &roArg, const ItemFindBackInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ItemFindBackInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ItemFindBackInfoArg &roArg, ItemFindBackInfoRes &roRes);
	void OnDelayReplyRpc(const ItemFindBackInfoArg &roArg, ItemFindBackInfoRes &roRes, const CUserData &roUserData);

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
	ItemFindBackInfoArg m_oArg;
	ItemFindBackInfoRes m_oRes;
};

#endif
