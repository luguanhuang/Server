#ifndef __RPCC2M_GIFTIBREQGOODS_H__
#define __RPCC2M_GIFTIBREQGOODS_H__

// generate by ProtoGen at date: 2017/5/5 13:34:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GIFTIBREQGOODS_TYPE 18140

class RpcC2M_GiftIbReqGoods : public CRpc
{
private:

	RpcC2M_GiftIbReqGoods():CRpc(RPCC2M_GIFTIBREQGOODS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GiftIbReqGoods *CreateRpc()
	{
		return new RpcC2M_GiftIbReqGoods;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GiftIbReqGoodsArg &roArg, const GiftIbReqGoodsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GiftIbReqGoodsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GiftIbReqGoodsArg &roArg, GiftIbReqGoodsRes &roRes);
	void OnDelayReplyRpc(const GiftIbReqGoodsArg &roArg, GiftIbReqGoodsRes &roRes, const CUserData &roUserData);

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
	GiftIbReqGoodsArg m_oArg;
	GiftIbReqGoodsRes m_oRes;
};

#endif
