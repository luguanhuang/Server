#ifndef __RPCC2G_PVPNOWALLDATA_H__
#define __RPCC2G_PVPNOWALLDATA_H__

// generate by ProtoGen at date: 2016/6/27 10:43:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PVPNOWALLDATA_TYPE 58355

class RpcC2G_PvpNowAllData : public CRpc
{
private:

	RpcC2G_PvpNowAllData():CRpc(RPCC2G_PVPNOWALLDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PvpNowAllData *CreateRpc()
	{
		return new RpcC2G_PvpNowAllData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const roArg &roArg, const PvpNowGameData &roRes, const CUserData &roUserData);
	void OnTimeout(const roArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const roArg &roArg, PvpNowGameData &roRes);
	void OnDelayReplyRpc(const roArg &roArg, PvpNowGameData &roRes, const CUserData &roUserData);

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
	roArg m_oArg;
	PvpNowGameData m_oRes;
};

#endif
