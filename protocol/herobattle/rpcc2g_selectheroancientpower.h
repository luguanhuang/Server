#ifndef __RPCC2G_SELECTHEROANCIENTPOWER_H__
#define __RPCC2G_SELECTHEROANCIENTPOWER_H__

// generate by ProtoGen at date: 2017/7/15 15:15:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SELECTHEROANCIENTPOWER_TYPE 7667

class RpcC2G_SelectHeroAncientPower : public CRpc
{
private:

	RpcC2G_SelectHeroAncientPower():CRpc(RPCC2G_SELECTHEROANCIENTPOWER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SelectHeroAncientPower *CreateRpc()
	{
		return new RpcC2G_SelectHeroAncientPower;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SelectHeroAncientPowerArg &roArg, const SelectHeroAncientPowerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SelectHeroAncientPowerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SelectHeroAncientPowerArg &roArg, SelectHeroAncientPowerRes &roRes);
	void OnDelayReplyRpc(const SelectHeroAncientPowerArg &roArg, SelectHeroAncientPowerRes &roRes, const CUserData &roUserData);

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
	SelectHeroAncientPowerArg m_oArg;
	SelectHeroAncientPowerRes m_oRes;
};

#endif
