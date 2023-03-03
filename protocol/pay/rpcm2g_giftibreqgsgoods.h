#ifndef __RPCM2G_GIFTIBREQGSGOODS_H__
#define __RPCM2G_GIFTIBREQGSGOODS_H__

// generate by ProtoGen at date: 2017/5/5 13:37:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GIFTIBREQGSGOODS_TYPE 3057

class RpcM2G_GiftIbReqGSGoods : public CRpc
{
private:

	RpcM2G_GiftIbReqGSGoods():CRpc(RPCM2G_GIFTIBREQGSGOODS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GiftIbReqGSGoods *CreateRpc()
	{
		return new RpcM2G_GiftIbReqGSGoods;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GiftIbReqGSGoodsArg &roArg, const GiftIbReqGSGoodsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GiftIbReqGSGoodsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GiftIbReqGSGoodsArg &roArg, GiftIbReqGSGoodsRes &roRes);
	void OnDelayReplyRpc(const GiftIbReqGSGoodsArg &roArg, GiftIbReqGSGoodsRes &roRes, const CUserData &roUserData);

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
	GiftIbReqGSGoodsArg m_oArg;
	GiftIbReqGSGoodsRes m_oRes;
};

#endif
