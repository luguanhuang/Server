#ifndef __RPCC2M_CHANGEDRAGONGUILDSETTING_H__
#define __RPCC2M_CHANGEDRAGONGUILDSETTING_H__

// generate by ProtoGen at date: 2017/9/7 16:27:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CHANGEDRAGONGUILDSETTING_TYPE 52505

class RpcC2M_ChangeDragonGuildSetting : public CRpc
{
private:

	RpcC2M_ChangeDragonGuildSetting():CRpc(RPCC2M_CHANGEDRAGONGUILDSETTING_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ChangeDragonGuildSetting *CreateRpc()
	{
		return new RpcC2M_ChangeDragonGuildSetting;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeDragonGuildSettingArg &roArg, const ChangeDragonGuildSettingRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeDragonGuildSettingArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeDragonGuildSettingArg &roArg, ChangeDragonGuildSettingRes &roRes);
	void OnDelayReplyRpc(const ChangeDragonGuildSettingArg &roArg, ChangeDragonGuildSettingRes &roRes, const CUserData &roUserData);

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
	ChangeDragonGuildSettingArg m_oArg;
	ChangeDragonGuildSettingRes m_oRes;
};

#endif
