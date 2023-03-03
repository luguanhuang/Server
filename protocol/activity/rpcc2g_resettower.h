#ifndef __RPCC2G_RESETTOWER_H__
#define __RPCC2G_RESETTOWER_H__

// generate by ProtoGen at date: 2016/7/15 20:17:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_RESETTOWER_TYPE 8570

class RpcC2G_ResetTower : public CRpc
{
private:

	RpcC2G_ResetTower():CRpc(RPCC2G_RESETTOWER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ResetTower *CreateRpc()
	{
		return new RpcC2G_ResetTower;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ResetTowerArg &roArg, const ResetTowerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ResetTowerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ResetTowerArg &roArg, ResetTowerRes &roRes);
	void OnDelayReplyRpc(const ResetTowerArg &roArg, ResetTowerRes &roRes, const CUserData &roUserData);

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
	ResetTowerArg m_oArg;
	ResetTowerRes m_oRes;
};

#endif
