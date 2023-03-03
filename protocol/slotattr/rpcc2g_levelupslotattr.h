#ifndef __RPCC2G_LEVELUPSLOTATTR_H__
#define __RPCC2G_LEVELUPSLOTATTR_H__

// generate by ProtoGen at date: 2016/3/22 15:54:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_LEVELUPSLOTATTR_TYPE 62918

class RpcC2G_LevelUpSlotAttr : public CRpc
{
private:

	RpcC2G_LevelUpSlotAttr():CRpc(RPCC2G_LEVELUPSLOTATTR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_LevelUpSlotAttr *CreateRpc()
	{
		return new RpcC2G_LevelUpSlotAttr;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LevelUpSlotAttrArg &roArg, const LevelUpSlotAttrRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LevelUpSlotAttrArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LevelUpSlotAttrArg &roArg, LevelUpSlotAttrRes &roRes);
	void OnDelayReplyRpc(const LevelUpSlotAttrArg &roArg, LevelUpSlotAttrRes &roRes, const CUserData &roUserData);

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
	LevelUpSlotAttrArg m_oArg;
	LevelUpSlotAttrRes m_oRes;
};

#endif
