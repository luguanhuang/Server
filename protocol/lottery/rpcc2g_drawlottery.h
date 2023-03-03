#ifndef __RPCC2G_DRAWLOTTERY_H__
#define __RPCC2G_DRAWLOTTERY_H__

// generate by ProtoGen at date: 2015/11/26 21:04:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_DRAWLOTTERY_TYPE 27802

class RpcC2G_DrawLottery : public CRpc
{
private:

	RpcC2G_DrawLottery():CRpc(RPCC2G_DRAWLOTTERY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_DrawLottery *CreateRpc()
	{
		return new RpcC2G_DrawLottery;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DrawLotteryArg &roArg, const DrawLotteryRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DrawLotteryArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DrawLotteryArg &roArg, DrawLotteryRes &roRes);
	void OnDelayReplyRpc(const DrawLotteryArg &roArg, DrawLotteryRes &roRes, const CUserData &roUserData);

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
	DrawLotteryArg m_oArg;
	DrawLotteryRes m_oRes;
};

#endif
