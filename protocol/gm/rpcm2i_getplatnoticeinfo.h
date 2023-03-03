#ifndef __RPCM2I_GETPLATNOTICEINFO_H__
#define __RPCM2I_GETPLATNOTICEINFO_H__

// generate by ProtoGen at date: 2016/11/19 14:54:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2I_GETPLATNOTICEINFO_TYPE 47300

class RpcM2I_GetPlatNoticeInfo : public CRpc
{
private:

	RpcM2I_GetPlatNoticeInfo():CRpc(RPCM2I_GETPLATNOTICEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2I_GetPlatNoticeInfo *CreateRpc()
	{
		return new RpcM2I_GetPlatNoticeInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPlatNoticeArg &roArg, const GetPlatNoticeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPlatNoticeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPlatNoticeArg &roArg, GetPlatNoticeRes &roRes);
	void OnDelayReplyRpc(const GetPlatNoticeArg &roArg, GetPlatNoticeRes &roRes, const CUserData &roUserData);

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
	GetPlatNoticeArg m_oArg;
	GetPlatNoticeRes m_oRes;
};

#endif
