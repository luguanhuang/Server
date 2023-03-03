#ifndef __RPCC2G_GETHEROBATTLEINFO_H__
#define __RPCC2G_GETHEROBATTLEINFO_H__

// generate by ProtoGen at date: 2016/12/13 16:36:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETHEROBATTLEINFO_TYPE 65206

class RpcC2G_GetHeroBattleInfo : public CRpc
{
private:

	RpcC2G_GetHeroBattleInfo():CRpc(RPCC2G_GETHEROBATTLEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetHeroBattleInfo *CreateRpc()
	{
		return new RpcC2G_GetHeroBattleInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetHeroBattleInfoArg &roArg, const GetHeroBattleInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetHeroBattleInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetHeroBattleInfoArg &roArg, GetHeroBattleInfoRes &roRes);
	void OnDelayReplyRpc(const GetHeroBattleInfoArg &roArg, GetHeroBattleInfoRes &roRes, const CUserData &roUserData);

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
	GetHeroBattleInfoArg m_oArg;
	GetHeroBattleInfoRes m_oRes;
};

#endif
