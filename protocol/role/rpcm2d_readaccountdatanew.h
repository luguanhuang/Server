#ifndef __RPCM2D_READACCOUNTDATANEW_H__
#define __RPCM2D_READACCOUNTDATANEW_H__

// generate by ProtoGen at date: 2017/10/25 15:48:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_READACCOUNTDATANEW_TYPE 11357

class RpcM2D_ReadAccountDataNew : public CRpc
{
private:

	RpcM2D_ReadAccountDataNew():CRpc(RPCM2D_READACCOUNTDATANEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_ReadAccountDataNew *CreateRpc()
	{
		return new RpcM2D_ReadAccountDataNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReadAccountDataNewArg &roArg, const ReadAccountDataNewRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReadAccountDataNewArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReadAccountDataNewArg &roArg, ReadAccountDataNewRes &roRes);
	void OnDelayReplyRpc(const ReadAccountDataNewArg &roArg, ReadAccountDataNewRes &roRes, const CUserData &roUserData);

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
	ReadAccountDataNewArg m_oArg;
	ReadAccountDataNewRes m_oRes;
};

#endif
