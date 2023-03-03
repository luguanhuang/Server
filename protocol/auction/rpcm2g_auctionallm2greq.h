#ifndef __RPCM2G_AUCTIONALLM2GREQ_H__
#define __RPCM2G_AUCTIONALLM2GREQ_H__

// generate by ProtoGen at date: 2016/8/27 17:37:25

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_AUCTIONALLM2GREQ_TYPE 33345

class RpcM2G_AuctionAllM2GReq : public CRpc
{
private:

	RpcM2G_AuctionAllM2GReq():CRpc(RPCM2G_AUCTIONALLM2GREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_AuctionAllM2GReq *CreateRpc()
	{
		return new RpcM2G_AuctionAllM2GReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AuctionReqM2GArg &roArg, const AuctionReqM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AuctionReqM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AuctionReqM2GArg &roArg, AuctionReqM2GRes &roRes);
	void OnDelayReplyRpc(const AuctionReqM2GArg &roArg, AuctionReqM2GRes &roRes, const CUserData &roUserData);

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
	AuctionReqM2GArg m_oArg;
	AuctionReqM2GRes m_oRes;
};

#endif
