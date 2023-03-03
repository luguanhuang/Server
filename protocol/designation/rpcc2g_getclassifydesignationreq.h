#ifndef __RPCC2G_GETCLASSIFYDESIGNATIONREQ_H__
#define __RPCC2G_GETCLASSIFYDESIGNATIONREQ_H__

// generate by ProtoGen at date: 2016/5/30 12:33:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETCLASSIFYDESIGNATIONREQ_TYPE 40256

class RpcC2G_GetClassifyDesignationReq : public CRpc
{
private:

	RpcC2G_GetClassifyDesignationReq():CRpc(RPCC2G_GETCLASSIFYDESIGNATIONREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetClassifyDesignationReq *CreateRpc()
	{
		return new RpcC2G_GetClassifyDesignationReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetClassifyDesignationReq &roArg, const GetClassifyDesignationRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetClassifyDesignationReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetClassifyDesignationReq &roArg, GetClassifyDesignationRes &roRes);
	void OnDelayReplyRpc(const GetClassifyDesignationReq &roArg, GetClassifyDesignationRes &roRes, const CUserData &roUserData);

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
	GetClassifyDesignationReq m_oArg;
	GetClassifyDesignationRes m_oRes;
};

#endif
