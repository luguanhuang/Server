#ifndef __RPCM2K_CUSTOMBATTLECHECKVALID_H__
#define __RPCM2K_CUSTOMBATTLECHECKVALID_H__

// generate by ProtoGen at date: 2017/5/8 11:55:59

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_CUSTOMBATTLECHECKVALID_TYPE 20197

class RpcM2K_CustomBattleCheckValid : public CRpc
{
private:

	RpcM2K_CustomBattleCheckValid():CRpc(RPCM2K_CUSTOMBATTLECHECKVALID_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_CustomBattleCheckValid *CreateRpc()
	{
		return new RpcM2K_CustomBattleCheckValid;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CustomBattleCheckValidArg &roArg, const CustomBattleCheckValidRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CustomBattleCheckValidArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CustomBattleCheckValidArg &roArg, CustomBattleCheckValidRes &roRes);
	void OnDelayReplyRpc(const CustomBattleCheckValidArg &roArg, CustomBattleCheckValidRes &roRes, const CUserData &roUserData);

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
	CustomBattleCheckValidArg m_oArg;
	CustomBattleCheckValidRes m_oRes;
};

#endif
