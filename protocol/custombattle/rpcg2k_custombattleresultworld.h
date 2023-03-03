#ifndef __RPCG2K_CUSTOMBATTLERESULTWORLD_H__
#define __RPCG2K_CUSTOMBATTLERESULTWORLD_H__

// generate by ProtoGen at date: 2017/5/9 22:06:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2K_CUSTOMBATTLERESULTWORLD_TYPE 25314

class RpcG2K_CustomBattleResultWorld : public CRpc
{
private:

	RpcG2K_CustomBattleResultWorld():CRpc(RPCG2K_CUSTOMBATTLERESULTWORLD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2K_CustomBattleResultWorld *CreateRpc()
	{
		return new RpcG2K_CustomBattleResultWorld;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CustomBattleResultMsArg &roArg, const CustomBattleResultMsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CustomBattleResultMsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CustomBattleResultMsArg &roArg, CustomBattleResultMsRes &roRes);
	void OnDelayReplyRpc(const CustomBattleResultMsArg &roArg, CustomBattleResultMsRes &roRes, const CUserData &roUserData);

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
	CustomBattleResultMsArg m_oArg;
	CustomBattleResultMsRes m_oRes;
};

#endif
