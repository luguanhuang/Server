#ifndef __RPCC2G_QUERYLOTTERYCD_H__
#define __RPCC2G_QUERYLOTTERYCD_H__

// generate by ProtoGen at date: 2017/3/15 10:02:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYLOTTERYCD_TYPE 12242

class RpcC2G_QueryLotteryCD : public CRpc
{
private:

	RpcC2G_QueryLotteryCD():CRpc(RPCC2G_QUERYLOTTERYCD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QueryLotteryCD *CreateRpc()
	{
		return new RpcC2G_QueryLotteryCD;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryLotteryCDArg &roArg, const QueryLotteryCDRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryLotteryCDArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryLotteryCDArg &roArg, QueryLotteryCDRes &roRes);
	void OnDelayReplyRpc(const QueryLotteryCDArg &roArg, QueryLotteryCDRes &roRes, const CUserData &roUserData);

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
	QueryLotteryCDArg m_oArg;
	QueryLotteryCDRes m_oRes;
};

#endif
