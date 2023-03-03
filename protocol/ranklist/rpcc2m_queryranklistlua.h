#ifndef __RPCC2M_QUERYRANKLISTLUA_H__
#define __RPCC2M_QUERYRANKLISTLUA_H__

// generate by ProtoGen at date: 2017/3/17 15:47:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_QUERYRANKLISTLUA_TYPE 21331

class RpcC2M_QueryRankListLua : public CRpc
{
private:

	RpcC2M_QueryRankListLua():CRpc(RPCC2M_QUERYRANKLISTLUA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_QueryRankListLua *CreateRpc()
	{
		return new RpcC2M_QueryRankListLua;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ClientQueryRankListArg &roArg, const ClientQueryRankListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ClientQueryRankListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ClientQueryRankListArg &roArg, ClientQueryRankListRes &roRes);
	void OnDelayReplyRpc(const ClientQueryRankListArg &roArg, ClientQueryRankListRes &roRes, const CUserData &roUserData);

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
	ClientQueryRankListArg m_oArg;
	ClientQueryRankListRes m_oRes;
};

#endif
