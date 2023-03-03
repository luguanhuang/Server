#ifndef __RPCC2M_GETLEAGUEBATTLEINFO_H__
#define __RPCC2M_GETLEAGUEBATTLEINFO_H__

// generate by ProtoGen at date: 2017/2/7 21:49:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETLEAGUEBATTLEINFO_TYPE 29101

class RpcC2M_GetLeagueBattleInfo : public CRpc
{
private:

	RpcC2M_GetLeagueBattleInfo():CRpc(RPCC2M_GETLEAGUEBATTLEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetLeagueBattleInfo *CreateRpc()
	{
		return new RpcC2M_GetLeagueBattleInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetLeagueBattleInfoArg &roArg, const GetLeagueBattleInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetLeagueBattleInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetLeagueBattleInfoArg &roArg, GetLeagueBattleInfoRes &roRes);
	void OnDelayReplyRpc(const GetLeagueBattleInfoArg &roArg, GetLeagueBattleInfoRes &roRes, const CUserData &roUserData);

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
	GetLeagueBattleInfoArg m_oArg;
	GetLeagueBattleInfoRes m_oRes;
};

#endif
