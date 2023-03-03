#ifndef __RPCM2G_PAYCONSUMEADDGOODS_H__
#define __RPCM2G_PAYCONSUMEADDGOODS_H__

// generate by ProtoGen at date: 2016/12/22 19:39:45

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_PAYCONSUMEADDGOODS_TYPE 49937

class RpcM2G_PayConsumeAddGoods : public CRpc
{
private:

	RpcM2G_PayConsumeAddGoods():CRpc(RPCM2G_PAYCONSUMEADDGOODS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_PayConsumeAddGoods *CreateRpc()
	{
		return new RpcM2G_PayConsumeAddGoods;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PayConsumeAddgoodsArg &roArg, const PayConsumeAddGoodsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PayConsumeAddgoodsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PayConsumeAddgoodsArg &roArg, PayConsumeAddGoodsRes &roRes);
	void OnDelayReplyRpc(const PayConsumeAddgoodsArg &roArg, PayConsumeAddGoodsRes &roRes, const CUserData &roUserData);

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
	PayConsumeAddgoodsArg m_oArg;
	PayConsumeAddGoodsRes m_oRes;
};

#endif
