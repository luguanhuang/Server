#ifndef __RPCC2M_CUSTOMBATTLEOP_H__
#define __RPCC2M_CUSTOMBATTLEOP_H__

// generate by ProtoGen at date: 2017/4/7 21:30:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CUSTOMBATTLEOP_TYPE 12314

class RpcC2M_CustomBattleOp : public CRpc
{
private:

	RpcC2M_CustomBattleOp():CRpc(RPCC2M_CUSTOMBATTLEOP_TYPE)
	{
        m_dwTimeout = 50000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_CustomBattleOp *CreateRpc()
	{
		return new RpcC2M_CustomBattleOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CustomBattleOpArg &roArg, const CustomBattleOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CustomBattleOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CustomBattleOpArg &roArg, CustomBattleOpRes &roRes);
	void OnDelayReplyRpc(const CustomBattleOpArg &roArg, CustomBattleOpRes &roRes, const CUserData &roUserData);

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
	CustomBattleOpArg m_oArg;
	CustomBattleOpRes m_oRes;
};

#endif
