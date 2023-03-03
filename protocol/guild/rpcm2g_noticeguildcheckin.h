#ifndef __RPCM2G_NOTICEGUILDCHECKIN_H__
#define __RPCM2G_NOTICEGUILDCHECKIN_H__

// generate by ProtoGen at date: 2016/9/18 11:17:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_NOTICEGUILDCHECKIN_TYPE 12454

class RpcM2G_NoticeGuildCheckin : public CRpc
{
private:

	RpcM2G_NoticeGuildCheckin():CRpc(RPCM2G_NOTICEGUILDCHECKIN_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_NoticeGuildCheckin *CreateRpc()
	{
		return new RpcM2G_NoticeGuildCheckin;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const NoticeGuildCheckinArg &roArg, const NoticeGuildCheckinRes &roRes, const CUserData &roUserData);
	void OnTimeout(const NoticeGuildCheckinArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const NoticeGuildCheckinArg &roArg, NoticeGuildCheckinRes &roRes);
	void OnDelayReplyRpc(const NoticeGuildCheckinArg &roArg, NoticeGuildCheckinRes &roRes, const CUserData &roUserData);

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
	NoticeGuildCheckinArg m_oArg;
	NoticeGuildCheckinRes m_oRes;
};

#endif
