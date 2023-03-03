#ifndef __RPCC2M_TRYFISH_H__
#define __RPCC2M_TRYFISH_H__

// generate by ProtoGen at date: 2016/10/5 10:36:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_TRYFISH_TYPE 7028

class RpcC2M_TryFish : public CRpc
{
private:

	RpcC2M_TryFish():CRpc(RPCC2M_TRYFISH_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_TryFish *CreateRpc()
	{
		return new RpcC2M_TryFish;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TryFishArg &roArg, const TryFishRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TryFishArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TryFishArg &roArg, TryFishRes &roRes);
	void OnDelayReplyRpc(const TryFishArg &roArg, TryFishRes &roRes, const CUserData &roUserData);

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
	TryFishArg m_oArg;
	TryFishRes m_oRes;
};

#endif
