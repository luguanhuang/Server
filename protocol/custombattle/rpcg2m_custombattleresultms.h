#ifndef __RPCG2M_CUSTOMBATTLERESULTMS_H__
#define __RPCG2M_CUSTOMBATTLERESULTMS_H__

// generate by ProtoGen at date: 2017/5/9 22:05:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_CUSTOMBATTLERESULTMS_TYPE 44243

class RpcG2M_CustomBattleResultMs : public CRpc
{
private:

	RpcG2M_CustomBattleResultMs():CRpc(RPCG2M_CUSTOMBATTLERESULTMS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_CustomBattleResultMs *CreateRpc()
	{
		return new RpcG2M_CustomBattleResultMs;
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
