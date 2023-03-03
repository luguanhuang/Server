#ifndef __RPCC2M_MODIFYMSGUILDNAME_H__
#define __RPCC2M_MODIFYMSGUILDNAME_H__

// generate by ProtoGen at date: 2017/4/11 22:30:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_MODIFYMSGUILDNAME_TYPE 21709

class RpcC2M_ModifyMsGuildName : public CRpc
{
private:

	RpcC2M_ModifyMsGuildName():CRpc(RPCC2M_MODIFYMSGUILDNAME_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ModifyMsGuildName *CreateRpc()
	{
		return new RpcC2M_ModifyMsGuildName;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ModifyArg &roArg, const ModifyRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ModifyArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ModifyArg &roArg, ModifyRes &roRes);
	void OnDelayReplyRpc(const ModifyArg &roArg, ModifyRes &roRes, const CUserData &roUserData);

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
	ModifyArg m_oArg;
	ModifyRes m_oRes;
};

#endif
