#ifndef __RPCM2G_GIVEMAILPRIZE_H__
#define __RPCM2G_GIVEMAILPRIZE_H__

// generate by ProtoGen at date: 2016/11/1 20:20:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GIVEMAILPRIZE_TYPE 60134

class RpcM2G_GiveMailPrize : public CRpc
{
private:

	RpcM2G_GiveMailPrize():CRpc(RPCM2G_GIVEMAILPRIZE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GiveMailPrize *CreateRpc()
	{
		return new RpcM2G_GiveMailPrize;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GiveMailPrizeArg &roArg, const GiveMailPrizeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GiveMailPrizeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GiveMailPrizeArg &roArg, GiveMailPrizeRes &roRes);
	void OnDelayReplyRpc(const GiveMailPrizeArg &roArg, GiveMailPrizeRes &roRes, const CUserData &roUserData);

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
	GiveMailPrizeArg m_oArg;
	GiveMailPrizeRes m_oRes;
};

#endif
