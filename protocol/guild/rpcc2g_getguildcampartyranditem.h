#ifndef __RPCC2G_GETGUILDCAMPARTYRANDITEM_H__
#define __RPCC2G_GETGUILDCAMPARTYRANDITEM_H__

// generate by ProtoGen at date: 2017/4/24 17:12:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDCAMPARTYRANDITEM_TYPE 53025

class RpcC2G_GetGuildCamPartyRandItem : public CRpc
{
private:

	RpcC2G_GetGuildCamPartyRandItem():CRpc(RPCC2G_GETGUILDCAMPARTYRANDITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildCamPartyRandItem *CreateRpc()
	{
		return new RpcC2G_GetGuildCamPartyRandItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildCamPartyRandItemArg &roArg, const GetGuildCamPartyRandItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildCamPartyRandItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildCamPartyRandItemArg &roArg, GetGuildCamPartyRandItemRes &roRes);
	void OnDelayReplyRpc(const GetGuildCamPartyRandItemArg &roArg, GetGuildCamPartyRandItemRes &roRes, const CUserData &roUserData);

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
	GetGuildCamPartyRandItemArg m_oArg;
	GetGuildCamPartyRandItemRes m_oRes;
};

#endif
