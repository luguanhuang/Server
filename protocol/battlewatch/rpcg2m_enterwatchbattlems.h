#ifndef __RPCG2M_ENTERWATCHBATTLEMS_H__
#define __RPCG2M_ENTERWATCHBATTLEMS_H__

// generate by ProtoGen at date: 2016/11/1 22:39:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_ENTERWATCHBATTLEMS_TYPE 2371

class RpcG2M_EnterWatchBattleMs : public CRpc
{
private:

	RpcG2M_EnterWatchBattleMs():CRpc(RPCG2M_ENTERWATCHBATTLEMS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_EnterWatchBattleMs *CreateRpc()
	{
		return new RpcG2M_EnterWatchBattleMs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnterWatchBattleMsArg &roArg, const EnterWatchBattleMsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnterWatchBattleMsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnterWatchBattleMsArg &roArg, EnterWatchBattleMsRes &roRes);
	void OnDelayReplyRpc(const EnterWatchBattleMsArg &roArg, EnterWatchBattleMsRes &roRes, const CUserData &roUserData);

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
	EnterWatchBattleMsArg m_oArg;
	EnterWatchBattleMsRes m_oRes;
};

#endif
