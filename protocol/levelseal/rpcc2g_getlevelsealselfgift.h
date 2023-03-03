#ifndef __RPCC2G_GETLEVELSEALSELFGIFT_H__
#define __RPCC2G_GETLEVELSEALSELFGIFT_H__

// generate by ProtoGen at date: 2016/10/17 15:56:06

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETLEVELSEALSELFGIFT_TYPE 61903

class RpcC2G_GetLevelSealSelfGift : public CRpc
{
private:

	RpcC2G_GetLevelSealSelfGift():CRpc(RPCC2G_GETLEVELSEALSELFGIFT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetLevelSealSelfGift *CreateRpc()
	{
		return new RpcC2G_GetLevelSealSelfGift;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetLevelSealSealGiftArg &roArg, const GetLevelSealSelfGiftRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetLevelSealSealGiftArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetLevelSealSealGiftArg &roArg, GetLevelSealSelfGiftRes &roRes);
	void OnDelayReplyRpc(const GetLevelSealSealGiftArg &roArg, GetLevelSealSelfGiftRes &roRes, const CUserData &roUserData);

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
	GetLevelSealSealGiftArg m_oArg;
	GetLevelSealSelfGiftRes m_oRes;
};

#endif
