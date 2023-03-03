#ifndef __RPCC2G_SETDESIGNATIONREQ_H__
#define __RPCC2G_SETDESIGNATIONREQ_H__

// generate by ProtoGen at date: 2016/5/30 12:30:23

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SETDESIGNATIONREQ_TYPE 7673

class RpcC2G_SetDesignationReq : public CRpc
{
private:

	RpcC2G_SetDesignationReq():CRpc(RPCC2G_SETDESIGNATIONREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SetDesignationReq *CreateRpc()
	{
		return new RpcC2G_SetDesignationReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SetDesignationReq &roArg, const SetDesignationRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SetDesignationReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SetDesignationReq &roArg, SetDesignationRes &roRes);
	void OnDelayReplyRpc(const SetDesignationReq &roArg, SetDesignationRes &roRes, const CUserData &roUserData);

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
	SetDesignationReq m_oArg;
	SetDesignationRes m_oRes;
};

#endif
