#ifndef __RPCC2G_BUYSPRITEEGG_H__
#define __RPCC2G_BUYSPRITEEGG_H__

// generate by ProtoGen at date: 2017/6/26 20:59:19

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BUYSPRITEEGG_TYPE 34552

class RpcC2G_BuySpriteEgg : public CRpc
{
private:

	RpcC2G_BuySpriteEgg():CRpc(RPCC2G_BUYSPRITEEGG_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BuySpriteEgg *CreateRpc()
	{
		return new RpcC2G_BuySpriteEgg;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuySpriteEggArg &roArg, const BuySpriteEggRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BuySpriteEggArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuySpriteEggArg &roArg, BuySpriteEggRes &roRes);
	void OnDelayReplyRpc(const BuySpriteEggArg &roArg, BuySpriteEggRes &roRes, const CUserData &roUserData);

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
	BuySpriteEggArg m_oArg;
	BuySpriteEggRes m_oRes;
};

#endif
