#ifndef __RPCC2G_GETGUILDPARTYRECEIVEINFO_H__
#define __RPCC2G_GETGUILDPARTYRECEIVEINFO_H__

// generate by ProtoGen at date: 2017/5/11 10:13:19

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDPARTYRECEIVEINFO_TYPE 58154

class RpcC2G_GetGuildPartyReceiveInfo : public CRpc
{
private:

	RpcC2G_GetGuildPartyReceiveInfo():CRpc(RPCC2G_GETGUILDPARTYRECEIVEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildPartyReceiveInfo *CreateRpc()
	{
		return new RpcC2G_GetGuildPartyReceiveInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildPartyReceiveInfoArg &roArg, const GetGuildPartyReceiveInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildPartyReceiveInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildPartyReceiveInfoArg &roArg, GetGuildPartyReceiveInfoRes &roRes);
	void OnDelayReplyRpc(const GetGuildPartyReceiveInfoArg &roArg, GetGuildPartyReceiveInfoRes &roRes, const CUserData &roUserData);

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
	GetGuildPartyReceiveInfoArg m_oArg;
	GetGuildPartyReceiveInfoRes m_oRes;
};

#endif
