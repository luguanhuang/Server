#ifndef __RPCC2M_TAKEMARRIAGECHEST_H__
#define __RPCC2M_TAKEMARRIAGECHEST_H__

// generate by ProtoGen at date: 2017/7/28 17:09:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_TAKEMARRIAGECHEST_TYPE 38713

class RpcC2M_TakeMarriageChest : public CRpc
{
private:

	RpcC2M_TakeMarriageChest():CRpc(RPCC2M_TAKEMARRIAGECHEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_TakeMarriageChest *CreateRpc()
	{
		return new RpcC2M_TakeMarriageChest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TakeMarriageChestArg &roArg, const TakeMarriageChestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TakeMarriageChestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TakeMarriageChestArg &roArg, TakeMarriageChestRes &roRes);
	void OnDelayReplyRpc(const TakeMarriageChestArg &roArg, TakeMarriageChestRes &roRes, const CUserData &roUserData);

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
	TakeMarriageChestArg m_oArg;
	TakeMarriageChestRes m_oRes;
};

#endif
