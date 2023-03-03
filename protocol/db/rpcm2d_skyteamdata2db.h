#ifndef __RPCM2D_SKYTEAMDATA2DB_H__
#define __RPCM2D_SKYTEAMDATA2DB_H__

// generate by ProtoGen at date: 2017/3/30 21:49:41

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_SKYTEAMDATA2DB_TYPE 37593

class RpcM2D_SkyTeamData2DB : public CRpc
{
private:

	RpcM2D_SkyTeamData2DB():CRpc(RPCM2D_SKYTEAMDATA2DB_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_SkyTeamData2DB *CreateRpc()
	{
		return new RpcM2D_SkyTeamData2DB;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SkyTeamData2DBArg &roArg, const SkyTeamData2DBRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SkyTeamData2DBArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SkyTeamData2DBArg &roArg, SkyTeamData2DBRes &roRes);
	void OnDelayReplyRpc(const SkyTeamData2DBArg &roArg, SkyTeamData2DBRes &roRes, const CUserData &roUserData);

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
	SkyTeamData2DBArg m_oArg;
	SkyTeamData2DBRes m_oRes;
};

#endif
