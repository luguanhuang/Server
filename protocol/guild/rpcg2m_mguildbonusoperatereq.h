#ifndef __RPCG2M_MGUILDBONUSOPERATEREQ_H__
#define __RPCG2M_MGUILDBONUSOPERATEREQ_H__

// generate by ProtoGen at date: 2016/9/16 0:27:41

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_MGUILDBONUSOPERATEREQ_TYPE 56113

class RpcG2M_MGuildBonusOperateReq : public CRpc
{
private:

	RpcG2M_MGuildBonusOperateReq():CRpc(RPCG2M_MGUILDBONUSOPERATEREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_MGuildBonusOperateReq *CreateRpc()
	{
		return new RpcG2M_MGuildBonusOperateReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MGuildBonusOperateReqArg &roArg, const MGuildBonusOperateReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MGuildBonusOperateReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MGuildBonusOperateReqArg &roArg, MGuildBonusOperateReqRes &roRes);
	void OnDelayReplyRpc(const MGuildBonusOperateReqArg &roArg, MGuildBonusOperateReqRes &roRes, const CUserData &roUserData);

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
	MGuildBonusOperateReqArg m_oArg;
	MGuildBonusOperateReqRes m_oRes;
};

#endif
