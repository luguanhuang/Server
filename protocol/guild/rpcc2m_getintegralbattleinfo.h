#ifndef __RPCC2M_GETINTEGRALBATTLEINFO_H__
#define __RPCC2M_GETINTEGRALBATTLEINFO_H__

// generate by ProtoGen at date: 2016/11/3 20:52:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETINTEGRALBATTLEINFO_TYPE 27825

class RpcC2M_getintegralbattleInfo : public CRpc
{
private:

	RpcC2M_getintegralbattleInfo():CRpc(RPCC2M_GETINTEGRALBATTLEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_getintegralbattleInfo *CreateRpc()
	{
		return new RpcC2M_getintegralbattleInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const getintegralbattleInfoarg &roArg, const getintegralbattleInfores &roRes, const CUserData &roUserData);
	void OnTimeout(const getintegralbattleInfoarg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const getintegralbattleInfoarg &roArg, getintegralbattleInfores &roRes);
	void OnDelayReplyRpc(const getintegralbattleInfoarg &roArg, getintegralbattleInfores &roRes, const CUserData &roUserData);

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
	getintegralbattleInfoarg m_oArg;
	getintegralbattleInfores m_oRes;
};

#endif
