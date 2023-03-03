#ifndef __RPCC2G_PANDORALOTTERY_H__
#define __RPCC2G_PANDORALOTTERY_H__

// generate by ProtoGen at date: 2016/12/17 12:18:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PANDORALOTTERY_TYPE 12575

class RpcC2G_PandoraLottery : public CRpc
{
private:

	RpcC2G_PandoraLottery():CRpc(RPCC2G_PANDORALOTTERY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PandoraLottery *CreateRpc()
	{
		return new RpcC2G_PandoraLottery;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PandoraLotteryArg &roArg, const PandoraLotteryRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PandoraLotteryArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PandoraLotteryArg &roArg, PandoraLotteryRes &roRes);
	void OnDelayReplyRpc(const PandoraLotteryArg &roArg, PandoraLotteryRes &roRes, const CUserData &roUserData);

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
	PandoraLotteryArg m_oArg;
	PandoraLotteryRes m_oRes;
};

#endif
