#ifndef __RPCC2M_IBGIFTHISTREQ_H__
#define __RPCC2M_IBGIFTHISTREQ_H__

// generate by ProtoGen at date: 2017/4/17 17:25:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_IBGIFTHISTREQ_TYPE 27050

class RpcC2M_IbGiftHistReq : public CRpc
{
private:

	RpcC2M_IbGiftHistReq():CRpc(RPCC2M_IBGIFTHISTREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_IbGiftHistReq *CreateRpc()
	{
		return new RpcC2M_IbGiftHistReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const IBGiftHistAllItemArg &roArg, const IBGiftHistAllItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const IBGiftHistAllItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const IBGiftHistAllItemArg &roArg, IBGiftHistAllItemRes &roRes);
	void OnDelayReplyRpc(const IBGiftHistAllItemArg &roArg, IBGiftHistAllItemRes &roRes, const CUserData &roUserData);

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
	IBGiftHistAllItemArg m_oArg;
	IBGiftHistAllItemRes m_oRes;
};

#endif
