#ifndef __RPCC2M_CREATEORENTERGUILD_H__
#define __RPCC2M_CREATEORENTERGUILD_H__

// generate by ProtoGen at date: 2016/8/26 16:31:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CREATEORENTERGUILD_TYPE 13871

class RpcC2M_CreateOrEnterGuild : public CRpc
{
private:

	RpcC2M_CreateOrEnterGuild():CRpc(RPCC2M_CREATEORENTERGUILD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_CreateOrEnterGuild *CreateRpc()
	{
		return new RpcC2M_CreateOrEnterGuild;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateOrJoinGuild &roArg, const CreateOrJoinGuildRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateOrJoinGuild &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateOrJoinGuild &roArg, CreateOrJoinGuildRes &roRes);
	void OnDelayReplyRpc(const CreateOrJoinGuild &roArg, CreateOrJoinGuildRes &roRes, const CUserData &roUserData);

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
	CreateOrJoinGuild m_oArg;
	CreateOrJoinGuildRes m_oRes;
};

#endif
