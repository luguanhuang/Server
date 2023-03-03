#ifndef __RPCC2G_ITEMCOMPOSE_H__
#define __RPCC2G_ITEMCOMPOSE_H__

// generate by ProtoGen at date: 2017/3/8 21:14:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ITEMCOMPOSE_TYPE 16118

class RpcC2G_ItemCompose : public CRpc
{
private:

	RpcC2G_ItemCompose():CRpc(RPCC2G_ITEMCOMPOSE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ItemCompose *CreateRpc()
	{
		return new RpcC2G_ItemCompose;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ItemComposeArg &roArg, const ItemComposeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ItemComposeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ItemComposeArg &roArg, ItemComposeRes &roRes);
	void OnDelayReplyRpc(const ItemComposeArg &roArg, ItemComposeRes &roRes, const CUserData &roUserData);

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
	ItemComposeArg m_oArg;
	ItemComposeRes m_oRes;
};

#endif
