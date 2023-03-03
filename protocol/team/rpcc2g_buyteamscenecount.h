#ifndef __RPCC2G_BUYTEAMSCENECOUNT_H__
#define __RPCC2G_BUYTEAMSCENECOUNT_H__

// generate by ProtoGen at date: 2016/5/3 15:18:40

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BUYTEAMSCENECOUNT_TYPE 9226

class RpcC2G_BuyTeamSceneCount : public CRpc
{
private:

	RpcC2G_BuyTeamSceneCount():CRpc(RPCC2G_BUYTEAMSCENECOUNT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BuyTeamSceneCount *CreateRpc()
	{
		return new RpcC2G_BuyTeamSceneCount;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuyTeamSceneCountP &roArg, const BuyTeamSceneCountRet &roRes, const CUserData &roUserData);
	void OnTimeout(const BuyTeamSceneCountP &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuyTeamSceneCountP &roArg, BuyTeamSceneCountRet &roRes);
	void OnDelayReplyRpc(const BuyTeamSceneCountP &roArg, BuyTeamSceneCountRet &roRes, const CUserData &roUserData);

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
	BuyTeamSceneCountP m_oArg;
	BuyTeamSceneCountRet m_oRes;
};

#endif
