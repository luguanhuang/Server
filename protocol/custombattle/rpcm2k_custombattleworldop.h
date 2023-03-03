#ifndef __RPCM2K_CUSTOMBATTLEWORLDOP_H__
#define __RPCM2K_CUSTOMBATTLEWORLDOP_H__

// generate by ProtoGen at date: 2017/4/7 21:32:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_CUSTOMBATTLEWORLDOP_TYPE 18992

class RpcM2K_CustomBattleWorldOp : public CRpc
{
private:

	RpcM2K_CustomBattleWorldOp():CRpc(RPCM2K_CUSTOMBATTLEWORLDOP_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_CustomBattleWorldOp *CreateRpc()
	{
		return new RpcM2K_CustomBattleWorldOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CustomBattleWorldOpArg &roArg, const CustomBattleWorldOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CustomBattleWorldOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CustomBattleWorldOpArg &roArg, CustomBattleWorldOpRes &roRes);
	void OnDelayReplyRpc(const CustomBattleWorldOpArg &roArg, CustomBattleWorldOpRes &roRes, const CUserData &roUserData);

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
	CustomBattleWorldOpArg m_oArg;
	CustomBattleWorldOpRes m_oRes;
};

#endif
