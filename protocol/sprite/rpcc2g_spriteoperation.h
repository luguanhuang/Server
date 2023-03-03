#ifndef __RPCC2G_SPRITEOPERATION_H__
#define __RPCC2G_SPRITEOPERATION_H__

// generate by ProtoGen at date: 2016/9/2 17:37:24

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SPRITEOPERATION_TYPE 62961

class RpcC2G_SpriteOperation : public CRpc
{
private:

	RpcC2G_SpriteOperation():CRpc(RPCC2G_SPRITEOPERATION_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SpriteOperation *CreateRpc()
	{
		return new RpcC2G_SpriteOperation;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SpriteOperationArg &roArg, const SpriteOperationRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SpriteOperationArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SpriteOperationArg &roArg, SpriteOperationRes &roRes);
	void OnDelayReplyRpc(const SpriteOperationArg &roArg, SpriteOperationRes &roRes, const CUserData &roUserData);

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
	SpriteOperationArg m_oArg;
	SpriteOperationRes m_oRes;
};

#endif
