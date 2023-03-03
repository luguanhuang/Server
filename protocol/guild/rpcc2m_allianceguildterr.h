#ifndef __RPCC2M_ALLIANCEGUILDTERR_H__
#define __RPCC2M_ALLIANCEGUILDTERR_H__

// generate by ProtoGen at date: 2016/12/18 10:40:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ALLIANCEGUILDTERR_TYPE 10041

class RpcC2M_AllianceGuildTerr : public CRpc
{
private:

	RpcC2M_AllianceGuildTerr():CRpc(RPCC2M_ALLIANCEGUILDTERR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AllianceGuildTerr *CreateRpc()
	{
		return new RpcC2M_AllianceGuildTerr;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AllianceGuildTerrArg &roArg, const AllianceGuildTerrRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AllianceGuildTerrArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AllianceGuildTerrArg &roArg, AllianceGuildTerrRes &roRes);
	void OnDelayReplyRpc(const AllianceGuildTerrArg &roArg, AllianceGuildTerrRes &roRes, const CUserData &roUserData);

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
	AllianceGuildTerrArg m_oArg;
	AllianceGuildTerrRes m_oRes;
};

#endif
