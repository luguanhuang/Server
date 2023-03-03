#ifndef __RPCM2G_CHECKNOTICE_H__
#define __RPCM2G_CHECKNOTICE_H__

// generate by ProtoGen at date: 2017/2/3 20:12:41

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_CHECKNOTICE_TYPE 21107

class RpcM2G_CheckNotice : public CRpc
{
private:

	RpcM2G_CheckNotice():CRpc(RPCM2G_CHECKNOTICE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_CheckNotice *CreateRpc()
	{
		return new RpcM2G_CheckNotice;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CheckNoticeArg &roArg, const CheckNoticeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CheckNoticeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CheckNoticeArg &roArg, CheckNoticeRes &roRes);
	void OnDelayReplyRpc(const CheckNoticeArg &roArg, CheckNoticeRes &roRes, const CUserData &roUserData);

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
	CheckNoticeArg m_oArg;
	CheckNoticeRes m_oRes;
};

#endif
