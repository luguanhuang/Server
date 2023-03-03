#ifndef __RPCC2G_SWEEPTOWER_H__
#define __RPCC2G_SWEEPTOWER_H__

// generate by ProtoGen at date: 2016/7/15 20:07:40

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SWEEPTOWER_TYPE 39381

class RpcC2G_SweepTower : public CRpc
{
private:

	RpcC2G_SweepTower():CRpc(RPCC2G_SWEEPTOWER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SweepTower *CreateRpc()
	{
		return new RpcC2G_SweepTower;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SweepTowerArg &roArg, const SweepTowerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SweepTowerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SweepTowerArg &roArg, SweepTowerRes &roRes);
	void OnDelayReplyRpc(const SweepTowerArg &roArg, SweepTowerRes &roRes, const CUserData &roUserData);

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
	SweepTowerArg m_oArg;
	SweepTowerRes m_oRes;
};

#endif
