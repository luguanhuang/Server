#ifndef __RPCN2D_NSREADACCOUNTDATA_H__
#define __RPCN2D_NSREADACCOUNTDATA_H__

// generate by ProtoGen at date: 2016/11/3 1:33:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2D_NSREADACCOUNTDATA_TYPE 17926

class RpcN2D_NsReadAccountData : public CRpc
{
private:

	RpcN2D_NsReadAccountData():CRpc(RPCN2D_NSREADACCOUNTDATA_TYPE)
	{
        m_dwTimeout = 50000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2D_NsReadAccountData *CreateRpc()
	{
		return new RpcN2D_NsReadAccountData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReadAccountDataArg &roArg, const ReadAccountDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReadAccountDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReadAccountDataArg &roArg, ReadAccountDataRes &roRes);
	void OnDelayReplyRpc(const ReadAccountDataArg &roArg, ReadAccountDataRes &roRes, const CUserData &roUserData);

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
	ReadAccountDataArg m_oArg;
	ReadAccountDataRes m_oRes;
};

#endif
