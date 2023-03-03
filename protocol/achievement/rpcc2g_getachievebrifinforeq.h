#ifndef __RPCC2G_GETACHIEVEBRIFINFOREQ_H__
#define __RPCC2G_GETACHIEVEBRIFINFOREQ_H__

// generate by ProtoGen at date: 2016/5/30 12:44:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETACHIEVEBRIFINFOREQ_TYPE 25095

class RpcC2G_GetAchieveBrifInfoReq : public CRpc
{
private:

	RpcC2G_GetAchieveBrifInfoReq():CRpc(RPCC2G_GETACHIEVEBRIFINFOREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetAchieveBrifInfoReq *CreateRpc()
	{
		return new RpcC2G_GetAchieveBrifInfoReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetAchieveBrifInfoReq &roArg, const GetAchieveBrifInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetAchieveBrifInfoReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetAchieveBrifInfoReq &roArg, GetAchieveBrifInfoRes &roRes);
	void OnDelayReplyRpc(const GetAchieveBrifInfoReq &roArg, GetAchieveBrifInfoRes &roRes, const CUserData &roUserData);

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
	GetAchieveBrifInfoReq m_oArg;
	GetAchieveBrifInfoRes m_oRes;
};

#endif
