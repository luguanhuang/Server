#ifndef __RPCM2N_GMDOWHITELIST_H__
#define __RPCM2N_GMDOWHITELIST_H__

// generate by ProtoGen at date: 2017/8/22 15:21:06

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2N_GMDOWHITELIST_TYPE 5271

class RpcM2N_GMDoWhiteList : public CRpc
{
private:

	RpcM2N_GMDoWhiteList():CRpc(RPCM2N_GMDOWHITELIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2N_GMDoWhiteList *CreateRpc()
	{
		return new RpcM2N_GMDoWhiteList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GMDoWhiteListArg &roArg, const GMDoWhiteListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GMDoWhiteListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GMDoWhiteListArg &roArg, GMDoWhiteListRes &roRes);
	void OnDelayReplyRpc(const GMDoWhiteListArg &roArg, GMDoWhiteListRes &roRes, const CUserData &roUserData);

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
	GMDoWhiteListArg m_oArg;
	GMDoWhiteListRes m_oRes;
};

#endif
