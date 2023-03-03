#ifndef __RPCM2D_READACCOUNTDATA_H__
#define __RPCM2D_READACCOUNTDATA_H__

// generate by ProtoGen at date: 2016/7/10 23:47:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_READACCOUNTDATA_TYPE 20979

class RpcM2D_ReadAccountData : public CRpc
{
private:

	RpcM2D_ReadAccountData():CRpc(RPCM2D_READACCOUNTDATA_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_ReadAccountData *CreateRpc()
	{
		return new RpcM2D_ReadAccountData;
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
