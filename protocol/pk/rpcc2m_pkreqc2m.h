#ifndef __RPCC2M_PKREQC2M_H__
#define __RPCC2M_PKREQC2M_H__

// generate by ProtoGen at date: 2016/8/25 20:14:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_PKREQC2M_TYPE 41221

class RpcC2M_PkReqC2M : public CRpc
{
private:

	RpcC2M_PkReqC2M():CRpc(RPCC2M_PKREQC2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_PkReqC2M *CreateRpc()
	{
		return new RpcC2M_PkReqC2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PkReqArg &roArg, const PkReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PkReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PkReqArg &roArg, PkReqRes &roRes);
	void OnDelayReplyRpc(const PkReqArg &roArg, PkReqRes &roRes, const CUserData &roUserData);

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
	PkReqArg m_oArg;
	PkReqRes m_oRes;
};

#endif
