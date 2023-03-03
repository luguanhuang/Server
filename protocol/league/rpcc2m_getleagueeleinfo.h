#ifndef __RPCC2M_GETLEAGUEELEINFO_H__
#define __RPCC2M_GETLEAGUEELEINFO_H__

// generate by ProtoGen at date: 2017/1/14 22:14:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETLEAGUEELEINFO_TYPE 40678

class RpcC2M_GetLeagueEleInfo : public CRpc
{
private:

	RpcC2M_GetLeagueEleInfo():CRpc(RPCC2M_GETLEAGUEELEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetLeagueEleInfo *CreateRpc()
	{
		return new RpcC2M_GetLeagueEleInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetLeagueEleInfoArg &roArg, const GetLeagueEleInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetLeagueEleInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetLeagueEleInfoArg &roArg, GetLeagueEleInfoRes &roRes);
	void OnDelayReplyRpc(const GetLeagueEleInfoArg &roArg, GetLeagueEleInfoRes &roRes, const CUserData &roUserData);

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
	GetLeagueEleInfoArg m_oArg;
	GetLeagueEleInfoRes m_oRes;
};

#endif
