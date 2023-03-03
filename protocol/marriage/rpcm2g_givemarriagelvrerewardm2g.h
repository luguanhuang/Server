#ifndef __RPCM2G_GIVEMARRIAGELVREREWARDM2G_H__
#define __RPCM2G_GIVEMARRIAGELVREREWARDM2G_H__

// generate by ProtoGen at date: 2017/7/28 23:41:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GIVEMARRIAGELVREREWARDM2G_TYPE 7964

class RpcM2G_GiveMarriageLvRerewardM2G : public CRpc
{
private:

	RpcM2G_GiveMarriageLvRerewardM2G():CRpc(RPCM2G_GIVEMARRIAGELVREREWARDM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GiveMarriageLvRerewardM2G *CreateRpc()
	{
		return new RpcM2G_GiveMarriageLvRerewardM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GiveMarriageLvRewardM2GArg &roArg, const GiveMarriageLvRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GiveMarriageLvRewardM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GiveMarriageLvRewardM2GArg &roArg, GiveMarriageLvRewardRes &roRes);
	void OnDelayReplyRpc(const GiveMarriageLvRewardM2GArg &roArg, GiveMarriageLvRewardRes &roRes, const CUserData &roUserData);

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
	GiveMarriageLvRewardM2GArg m_oArg;
	GiveMarriageLvRewardRes m_oRes;
};

#endif
