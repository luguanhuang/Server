#ifndef __RPCC2G_BUYEXTRASKILLEMBLEMSLOT_H__
#define __RPCC2G_BUYEXTRASKILLEMBLEMSLOT_H__

// generate by ProtoGen at date: 2017/5/9 16:28:24

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BUYEXTRASKILLEMBLEMSLOT_TYPE 17851

class RpcC2G_BuyExtraSkillEmblemSlot : public CRpc
{
private:

	RpcC2G_BuyExtraSkillEmblemSlot():CRpc(RPCC2G_BUYEXTRASKILLEMBLEMSLOT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BuyExtraSkillEmblemSlot *CreateRpc()
	{
		return new RpcC2G_BuyExtraSkillEmblemSlot;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuyExtraSkillEmblemSlotArg &roArg, const BuyExtraSkillEmblemSlotRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BuyExtraSkillEmblemSlotArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuyExtraSkillEmblemSlotArg &roArg, BuyExtraSkillEmblemSlotRes &roRes);
	void OnDelayReplyRpc(const BuyExtraSkillEmblemSlotArg &roArg, BuyExtraSkillEmblemSlotRes &roRes, const CUserData &roUserData);

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
	BuyExtraSkillEmblemSlotArg m_oArg;
	BuyExtraSkillEmblemSlotRes m_oRes;
};

#endif
