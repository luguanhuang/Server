#ifndef __RPCM2G_NOTICEGUILDBOSSDOENDTOGSN_H__
#define __RPCM2G_NOTICEGUILDBOSSDOENDTOGSN_H__

// generate by ProtoGen at date: 2016/7/30 21:47:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_NOTICEGUILDBOSSDOENDTOGSN_TYPE 22920

class RpcM2G_NoticeGuildBossDoEndToGSN : public CRpc
{
private:

	RpcM2G_NoticeGuildBossDoEndToGSN():CRpc(RPCM2G_NOTICEGUILDBOSSDOENDTOGSN_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_NoticeGuildBossDoEndToGSN *CreateRpc()
	{
		return new RpcM2G_NoticeGuildBossDoEndToGSN;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const NoticeGuildBossDoEndArg &roArg, const NoticeGuildBossDoEndRes &roRes, const CUserData &roUserData);
	void OnTimeout(const NoticeGuildBossDoEndArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const NoticeGuildBossDoEndArg &roArg, NoticeGuildBossDoEndRes &roRes);
	void OnDelayReplyRpc(const NoticeGuildBossDoEndArg &roArg, NoticeGuildBossDoEndRes &roRes, const CUserData &roUserData);

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
	NoticeGuildBossDoEndArg m_oArg;
	NoticeGuildBossDoEndRes m_oRes;
};

#endif
