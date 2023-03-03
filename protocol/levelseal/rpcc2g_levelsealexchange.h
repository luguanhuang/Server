#ifndef __RPCC2G_LEVELSEALEXCHANGE_H__
#define __RPCC2G_LEVELSEALEXCHANGE_H__

// generate by ProtoGen at date: 2016/9/7 20:10:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_LEVELSEALEXCHANGE_TYPE 65467

class RpcC2G_LevelSealExchange : public CRpc
{
private:

	RpcC2G_LevelSealExchange():CRpc(RPCC2G_LEVELSEALEXCHANGE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_LevelSealExchange *CreateRpc()
	{
		return new RpcC2G_LevelSealExchange;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LevelSealExchangeArg &roArg, const LevelSealExchangeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LevelSealExchangeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LevelSealExchangeArg &roArg, LevelSealExchangeRes &roRes);
	void OnDelayReplyRpc(const LevelSealExchangeArg &roArg, LevelSealExchangeRes &roRes, const CUserData &roUserData);

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
	LevelSealExchangeArg m_oArg;
	LevelSealExchangeRes m_oRes;
};

#endif
