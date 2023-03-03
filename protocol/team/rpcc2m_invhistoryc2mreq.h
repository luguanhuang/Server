#ifndef __RPCC2M_INVHISTORYC2MREQ_H__
#define __RPCC2M_INVHISTORYC2MREQ_H__

// generate by ProtoGen at date: 2016/8/18 14:22:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_INVHISTORYC2MREQ_TYPE 29978

class RpcC2M_InvHistoryC2MReq : public CRpc
{
private:

	RpcC2M_InvHistoryC2MReq():CRpc(RPCC2M_INVHISTORYC2MREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_InvHistoryC2MReq *CreateRpc()
	{
		return new RpcC2M_InvHistoryC2MReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const InvHistoryArg &roArg, const InvHistoryRes &roRes, const CUserData &roUserData);
	void OnTimeout(const InvHistoryArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const InvHistoryArg &roArg, InvHistoryRes &roRes);
	void OnDelayReplyRpc(const InvHistoryArg &roArg, InvHistoryRes &roRes, const CUserData &roUserData);

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
	InvHistoryArg m_oArg;
	InvHistoryRes m_oRes;
};

#endif
