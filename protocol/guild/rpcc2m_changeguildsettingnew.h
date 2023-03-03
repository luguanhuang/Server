#ifndef __RPCC2M_CHANGEGUILDSETTINGNEW_H__
#define __RPCC2M_CHANGEGUILDSETTINGNEW_H__

// generate by ProtoGen at date: 2016/9/15 22:33:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CHANGEGUILDSETTINGNEW_TYPE 55897

class RpcC2M_ChangeGuildSettingNew : public CRpc
{
private:

	RpcC2M_ChangeGuildSettingNew():CRpc(RPCC2M_CHANGEGUILDSETTINGNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ChangeGuildSettingNew *CreateRpc()
	{
		return new RpcC2M_ChangeGuildSettingNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeGuildSettingArg &roArg, const ChangeGuildSettingRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeGuildSettingArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeGuildSettingArg &roArg, ChangeGuildSettingRes &roRes);
	void OnDelayReplyRpc(const ChangeGuildSettingArg &roArg, ChangeGuildSettingRes &roRes, const CUserData &roUserData);

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
	ChangeGuildSettingArg m_oArg;
	ChangeGuildSettingRes m_oRes;
};

#endif
