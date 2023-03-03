#ifndef __RPCC2M_GETSKYCRAFTTEAMINFO_H__
#define __RPCC2M_GETSKYCRAFTTEAMINFO_H__

// generate by ProtoGen at date: 2017/4/11 15:20:32

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETSKYCRAFTTEAMINFO_TYPE 25015

class RpcC2M_GetSkyCraftTeamInfo : public CRpc
{
private:

	RpcC2M_GetSkyCraftTeamInfo():CRpc(RPCC2M_GETSKYCRAFTTEAMINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetSkyCraftTeamInfo *CreateRpc()
	{
		return new RpcC2M_GetSkyCraftTeamInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetSkyCraftTeamInfoArg &roArg, const GetSkyCraftTeamInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetSkyCraftTeamInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetSkyCraftTeamInfoArg &roArg, GetSkyCraftTeamInfoRes &roRes);
	void OnDelayReplyRpc(const GetSkyCraftTeamInfoArg &roArg, GetSkyCraftTeamInfoRes &roRes, const CUserData &roUserData);

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
	GetSkyCraftTeamInfoArg m_oArg;
	GetSkyCraftTeamInfoRes m_oRes;
};

#endif
