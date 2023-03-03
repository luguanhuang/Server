#ifndef __RPCC2G_PHOTOGRAPHEFFECT_H__
#define __RPCC2G_PHOTOGRAPHEFFECT_H__

// generate by ProtoGen at date: 2016/12/14 21:43:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PHOTOGRAPHEFFECT_TYPE 14666

class RpcC2G_PhotographEffect : public CRpc
{
private:

	RpcC2G_PhotographEffect():CRpc(RPCC2G_PHOTOGRAPHEFFECT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PhotographEffect *CreateRpc()
	{
		return new RpcC2G_PhotographEffect;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PhotographEffectArg &roArg, const PhotographEffect &roRes, const CUserData &roUserData);
	void OnTimeout(const PhotographEffectArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PhotographEffectArg &roArg, PhotographEffect &roRes);
	void OnDelayReplyRpc(const PhotographEffectArg &roArg, PhotographEffect &roRes, const CUserData &roUserData);

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
	PhotographEffectArg m_oArg;
	PhotographEffect m_oRes;
};

#endif
