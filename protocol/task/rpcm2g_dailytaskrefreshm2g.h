﻿#ifndef __RPCM2G_DAILYTASKREFRESHM2G_H__
#define __RPCM2G_DAILYTASKREFRESHM2G_H__

// generate by ProtoGen at date: 2017/9/19 15:53:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_DAILYTASKREFRESHM2G_TYPE 7069

class RpcM2G_DailyTaskRefreshM2G : public CRpc
{
private:

	RpcM2G_DailyTaskRefreshM2G():CRpc(RPCM2G_DAILYTASKREFRESHM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_DailyTaskRefreshM2G *CreateRpc()
	{
		return new RpcM2G_DailyTaskRefreshM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DailyTaskRefreshM2GArg &roArg, const DailyTaskRefreshM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DailyTaskRefreshM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DailyTaskRefreshM2GArg &roArg, DailyTaskRefreshM2GRes &roRes);
	void OnDelayReplyRpc(const DailyTaskRefreshM2GArg &roArg, DailyTaskRefreshM2GRes &roRes, const CUserData &roUserData);

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
	DailyTaskRefreshM2GArg m_oArg;
	DailyTaskRefreshM2GRes m_oRes;
};

#endif
