#ifndef __RPCC2M_CLEARGUILDTERRALLIANCE_H__
#define __RPCC2M_CLEARGUILDTERRALLIANCE_H__

// generate by ProtoGen at date: 2016/12/21 10:43:26

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CLEARGUILDTERRALLIANCE_TYPE 38312

class RpcC2M_ClearGuildTerrAlliance : public CRpc
{
private:

	RpcC2M_ClearGuildTerrAlliance():CRpc(RPCC2M_CLEARGUILDTERRALLIANCE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ClearGuildTerrAlliance *CreateRpc()
	{
		return new RpcC2M_ClearGuildTerrAlliance;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ClearGuildTerrAllianceArg &roArg, const ClearGuildTerrAllianceRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ClearGuildTerrAllianceArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ClearGuildTerrAllianceArg &roArg, ClearGuildTerrAllianceRes &roRes);
	void OnDelayReplyRpc(const ClearGuildTerrAllianceArg &roArg, ClearGuildTerrAllianceRes &roRes, const CUserData &roUserData);

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
	ClearGuildTerrAllianceArg m_oArg;
	ClearGuildTerrAllianceRes m_oRes;
};

#endif
