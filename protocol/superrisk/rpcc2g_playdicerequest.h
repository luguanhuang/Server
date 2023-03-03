#ifndef __RPCC2G_PLAYDICEREQUEST_H__
#define __RPCC2G_PLAYDICEREQUEST_H__

// generate by ProtoGen at date: 2016/9/5 16:11:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PLAYDICEREQUEST_TYPE 51246

class RpcC2G_PlayDiceRequest : public CRpc
{
private:

	RpcC2G_PlayDiceRequest():CRpc(RPCC2G_PLAYDICEREQUEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PlayDiceRequest *CreateRpc()
	{
		return new RpcC2G_PlayDiceRequest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PlayDiceRequestArg &roArg, const PlayDiceRequestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PlayDiceRequestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PlayDiceRequestArg &roArg, PlayDiceRequestRes &roRes);
	void OnDelayReplyRpc(const PlayDiceRequestArg &roArg, PlayDiceRequestRes &roRes, const CUserData &roUserData);

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
	PlayDiceRequestArg m_oArg;
	PlayDiceRequestRes m_oRes;
};

#endif
