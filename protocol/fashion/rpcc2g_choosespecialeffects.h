#ifndef __RPCC2G_CHOOSESPECIALEFFECTS_H__
#define __RPCC2G_CHOOSESPECIALEFFECTS_H__

// generate by ProtoGen at date: 2017/10/16 11:31:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHOOSESPECIALEFFECTS_TYPE 55040

class RpcC2G_ChooseSpecialEffects : public CRpc
{
private:

	RpcC2G_ChooseSpecialEffects():CRpc(RPCC2G_CHOOSESPECIALEFFECTS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ChooseSpecialEffects *CreateRpc()
	{
		return new RpcC2G_ChooseSpecialEffects;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChooseSpecialEffectsArg &roArg, const ChooseSpecialEffectsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChooseSpecialEffectsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChooseSpecialEffectsArg &roArg, ChooseSpecialEffectsRes &roRes);
	void OnDelayReplyRpc(const ChooseSpecialEffectsArg &roArg, ChooseSpecialEffectsRes &roRes, const CUserData &roUserData);

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
	ChooseSpecialEffectsArg m_oArg;
	ChooseSpecialEffectsRes m_oRes;
};

#endif
