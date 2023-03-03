#ifndef __RPCK2G_CREATEBATTLECROSS_H__
#define __RPCK2G_CREATEBATTLECROSS_H__

// generate by ProtoGen at date: 2016/10/29 14:35:19

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCK2G_CREATEBATTLECROSS_TYPE 53349

class RpcK2G_CreateBattleCross : public CRpc
{
private:

	RpcK2G_CreateBattleCross():CRpc(RPCK2G_CREATEBATTLECROSS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcK2G_CreateBattleCross *CreateRpc()
	{
		return new RpcK2G_CreateBattleCross;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateBattleCrossArg &roArg, const CreateBattleCrossRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateBattleCrossArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateBattleCrossArg &roArg, CreateBattleCrossRes &roRes);
	void OnDelayReplyRpc(const CreateBattleCrossArg &roArg, CreateBattleCrossRes &roRes, const CUserData &roUserData);

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
	CreateBattleCrossArg m_oArg;
	CreateBattleCrossRes m_oRes;
};

#endif
