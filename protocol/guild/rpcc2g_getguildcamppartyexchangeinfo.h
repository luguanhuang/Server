#ifndef __RPCC2G_GETGUILDCAMPPARTYEXCHANGEINFO_H__
#define __RPCC2G_GETGUILDCAMPPARTYEXCHANGEINFO_H__

// generate by ProtoGen at date: 2017/5/2 14:02:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDCAMPPARTYEXCHANGEINFO_TYPE 44443

class RpcC2G_GetGuildCampPartyExchangeInfo : public CRpc
{
private:

	RpcC2G_GetGuildCampPartyExchangeInfo():CRpc(RPCC2G_GETGUILDCAMPPARTYEXCHANGEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildCampPartyExchangeInfo *CreateRpc()
	{
		return new RpcC2G_GetGuildCampPartyExchangeInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildCampPartyExchangeInfoArg &roArg, const GetGuildCampPartyExchangeInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildCampPartyExchangeInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildCampPartyExchangeInfoArg &roArg, GetGuildCampPartyExchangeInfoRes &roRes);
	void OnDelayReplyRpc(const GetGuildCampPartyExchangeInfoArg &roArg, GetGuildCampPartyExchangeInfoRes &roRes, const CUserData &roUserData);

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
	GetGuildCampPartyExchangeInfoArg m_oArg;
	GetGuildCampPartyExchangeInfoRes m_oRes;
};

#endif
