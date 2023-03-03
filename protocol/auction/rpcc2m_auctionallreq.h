#ifndef __RPCC2M_AUCTIONALLREQ_H__
#define __RPCC2M_AUCTIONALLREQ_H__

// generate by ProtoGen at date: 2016/9/10 11:59:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_AUCTIONALLREQ_TYPE 38875

class RpcC2M_AuctionAllReq : public CRpc
{
private:

	RpcC2M_AuctionAllReq():CRpc(RPCC2M_AUCTIONALLREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AuctionAllReq *CreateRpc()
	{
		return new RpcC2M_AuctionAllReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AuctionAllReqArg &roArg, const AuctionAllReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AuctionAllReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AuctionAllReqArg &roArg, AuctionAllReqRes &roRes);
	void OnDelayReplyRpc(const AuctionAllReqArg &roArg, AuctionAllReqRes &roRes, const CUserData &roUserData);

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
	AuctionAllReqArg m_oArg;
	AuctionAllReqRes m_oRes;
};

#endif
