#ifndef __RPCM2G_CUSTOMBATTLEGSOP_H__
#define __RPCM2G_CUSTOMBATTLEGSOP_H__

// generate by ProtoGen at date: 2017/5/2 17:00:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_CUSTOMBATTLEGSOP_TYPE 20452

class RpcM2G_CustomBattleGsOp : public CRpc
{
private:

	RpcM2G_CustomBattleGsOp():CRpc(RPCM2G_CUSTOMBATTLEGSOP_TYPE)
	{
        m_dwTimeout = 20000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_CustomBattleGsOp *CreateRpc()
	{
		return new RpcM2G_CustomBattleGsOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CustomBattleGsOpArg &roArg, const CustomBattleGsOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CustomBattleGsOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CustomBattleGsOpArg &roArg, CustomBattleGsOpRes &roRes);
	void OnDelayReplyRpc(const CustomBattleGsOpArg &roArg, CustomBattleGsOpRes &roRes, const CUserData &roUserData);

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
	CustomBattleGsOpArg m_oArg;
	CustomBattleGsOpRes m_oRes;
};

#endif
