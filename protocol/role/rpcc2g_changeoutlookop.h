#ifndef __RPCC2G_CHANGEOUTLOOKOP_H__
#define __RPCC2G_CHANGEOUTLOOKOP_H__

// generate by ProtoGen at date: 2016/8/30 12:07:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHANGEOUTLOOKOP_TYPE 56978

class RpcC2G_ChangeOutLookOp : public CRpc
{
private:

	RpcC2G_ChangeOutLookOp():CRpc(RPCC2G_CHANGEOUTLOOKOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ChangeOutLookOp *CreateRpc()
	{
		return new RpcC2G_ChangeOutLookOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeOutLookOpArg &roArg, const ChangeOutLookOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeOutLookOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeOutLookOpArg &roArg, ChangeOutLookOpRes &roRes);
	void OnDelayReplyRpc(const ChangeOutLookOpArg &roArg, ChangeOutLookOpRes &roRes, const CUserData &roUserData);

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
	ChangeOutLookOpArg m_oArg;
	ChangeOutLookOpRes m_oRes;
};

#endif
