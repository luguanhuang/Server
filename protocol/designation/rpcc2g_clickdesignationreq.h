#ifndef __RPCC2G_CLICKDESIGNATIONREQ_H__
#define __RPCC2G_CLICKDESIGNATIONREQ_H__

// generate by ProtoGen at date: 2016/5/30 12:36:47

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CLICKDESIGNATIONREQ_TYPE 8916

class RpcC2G_ClickDesignationReq : public CRpc
{
private:

	RpcC2G_ClickDesignationReq():CRpc(RPCC2G_CLICKDESIGNATIONREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ClickDesignationReq *CreateRpc()
	{
		return new RpcC2G_ClickDesignationReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ClickDesignationReq &roArg, const ClickDesignationRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ClickDesignationReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ClickDesignationReq &roArg, ClickDesignationRes &roRes);
	void OnDelayReplyRpc(const ClickDesignationReq &roArg, ClickDesignationRes &roRes, const CUserData &roUserData);

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
	ClickDesignationReq m_oArg;
	ClickDesignationRes m_oRes;
};

#endif
