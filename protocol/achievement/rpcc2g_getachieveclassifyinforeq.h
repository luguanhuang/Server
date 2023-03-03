#ifndef __RPCC2G_GETACHIEVECLASSIFYINFOREQ_H__
#define __RPCC2G_GETACHIEVECLASSIFYINFOREQ_H__

// generate by ProtoGen at date: 2016/5/30 12:45:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETACHIEVECLASSIFYINFOREQ_TYPE 14056

class RpcC2G_GetAchieveClassifyInfoReq : public CRpc
{
private:

	RpcC2G_GetAchieveClassifyInfoReq():CRpc(RPCC2G_GETACHIEVECLASSIFYINFOREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetAchieveClassifyInfoReq *CreateRpc()
	{
		return new RpcC2G_GetAchieveClassifyInfoReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetAchieveClassifyInfoReq &roArg, const GetAchieveClassifyInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetAchieveClassifyInfoReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetAchieveClassifyInfoReq &roArg, GetAchieveClassifyInfoRes &roRes);
	void OnDelayReplyRpc(const GetAchieveClassifyInfoReq &roArg, GetAchieveClassifyInfoRes &roRes, const CUserData &roUserData);

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
	GetAchieveClassifyInfoReq m_oArg;
	GetAchieveClassifyInfoRes m_oRes;
};

#endif
