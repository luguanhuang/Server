#ifndef __RPCC2G_SMELTITEM_H__
#define __RPCC2G_SMELTITEM_H__

// generate by ProtoGen at date: 2016/9/10 14:34:41

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SMELTITEM_TYPE 10028

class RpcC2G_SmeltItem : public CRpc
{
private:

	RpcC2G_SmeltItem():CRpc(RPCC2G_SMELTITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SmeltItem *CreateRpc()
	{
		return new RpcC2G_SmeltItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SmeltItemArg &roArg, const SmeltItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SmeltItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SmeltItemArg &roArg, SmeltItemRes &roRes);
	void OnDelayReplyRpc(const SmeltItemArg &roArg, SmeltItemRes &roRes, const CUserData &roUserData);

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
	SmeltItemArg m_oArg;
	SmeltItemRes m_oRes;
};

#endif
